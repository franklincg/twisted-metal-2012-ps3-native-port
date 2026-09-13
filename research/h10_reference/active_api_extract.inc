typedef struct {
    int         in_use;
    const void* pm;
    u32         sizePm;
    u64         data;
    u8          priority[CELL_SPURS_MAX_SPU];
    u32         minContention;
    u32         maxContention;
    u32         readyCount;
} SpursWorkload;

typedef struct {
    int         in_use;
    u32         id;
    int         active;
    int         completed;
    s32         exitCode;
    void*       entryPoint;
    u64         argA;
} SpursTask;

/* Global workload table (per-SPURS instance in a real system, simplified) */
static SpursWorkload s_workloads[CELL_SPURS_MAX_WORKLOAD];
static SpursTask     s_tasks[CELL_SPURS_MAX_TASK];
static u32           s_next_task_id = 0;
#define GUEST_PTR(p, T) ((T)((p) ? (void*)(vm_base + (uint32_t)(uintptr_t)(p)) : (void*)0))
s32 cellSpursInitializeWithAttribute(CellSpurs* spurs,
                                     const CellSpursAttribute* attr)
{
    if (!spurs || !attr)
        return CELL_SPURS_CORE_ERROR_NULL_POINTER;

    spurs = GUEST_PTR(spurs, CellSpurs*);
    attr  = GUEST_PTR(attr, const CellSpursAttribute*);
    printf("[cellSpurs] InitializeWithAttribute(prefix=\"%.15s\")\n", attr->prefix);

    memset(spurs, 0, sizeof(CellSpurs));
    spurs->initialized = 1;
    spurs->nSpus = attr->nSpus;
    spurs->flags = attr->flags;
    memcpy(spurs->prefix, attr->prefix, sizeof(spurs->prefix));

    memset(s_workloads, 0, sizeof(s_workloads));
    return CELL_OK;
}
s32 cellSpursCreateTaskset(CellSpurs* spurs, CellSpursTaskset* taskset,
                           u64 args, const u8* priority, u32 maxContention)
{
    (void)args; (void)priority; (void)maxContention;

    /* Capture the GUEST EAs (raw register values) BEFORE host translation -- the real
     * BE taskset builder writes to guest memory at these EAs. */
    uint32_t taskset_ea = (uint32_t)(uintptr_t)taskset;
    uint32_t spurs_ea   = (uint32_t)(uintptr_t)spurs;

    /* Args arrive as guest effective addresses (ps3_hle_call passes raw guest
     * register values); translate to host before dereferencing. */
    spurs   = GUEST_PTR(spurs, CellSpurs*);
    taskset = GUEST_PTR(taskset, CellSpursTaskset*);

    if (!spurs || !taskset)
        return CELL_SPURS_TASK_ERROR_NULL_POINTER;

    if (!spurs->initialized)
        return CELL_SPURS_CORE_ERROR_STAT;

    memset(taskset, 0, sizeof(CellSpursTaskset));
    taskset->initialized = 1;
    taskset->spurs = spurs;

    /* Write the REAL big-endian CellSpursTaskset layout (fork Option-B) so the lifted
     * SPU leaf + spurs_pm_build_context read valid data (the native writes above are
     * little-endian = garbage to the SPU). Overwrites 0x00-0x80 with BE fields. */
    spurs_taskset_init(taskset_ea, spurs_ea, args, /*wid*/0,
                       (uint32_t)sizeof(CellSpursTaskset), /*evf1*/0, /*evf2*/0);
    g_ydkj_real_taskset_ea = taskset_ea;

    g_ydkj_real_spurs_ea = spurs_ea;   /* capture for the taskset-policy handoff (LS[0x1C0]) */
    printf("[cellSpurs] CreateTaskset() ea=0x%08X spurs=0x%08X (real BE layout)\n", taskset_ea, spurs_ea);
    return CELL_OK;
}
s32 cellSpursCreateTask(CellSpursTaskset* taskset, CellSpursTaskId* taskId,
                        void* elf, void* context, u32 sizeContext,
                        CellSpursTaskAttribute* attr)
{
    (void)context; (void)sizeContext; (void)attr;

    /* Capture guest EAs BEFORE host translation (the real BE taskset builder + the
     * SPU DMA use guest EAs). */
    uint32_t taskset_ea = (uint32_t)(uintptr_t)taskset;
    uint32_t elf_ea     = (uint32_t)(uintptr_t)elf;
    uint32_t context_ea = (uint32_t)(uintptr_t)context;

    /* taskId/taskset are guest EAs; translate before deref. elf/context stay
     * guest EAs (handled below — elf is translated for load, context kept EA). */
    taskset = GUEST_PTR(taskset, CellSpursTaskset*);
    CellSpursTaskId* taskId_h = GUEST_PTR(taskId, CellSpursTaskId*);

    if (!taskset)
        return CELL_SPURS_TASK_ERROR_NULL_POINTER;

    if (!g_ydkj_real_taskset_ea)   /* real-BE init flag (native ->initialized clobbered by BE layout) */
        return CELL_SPURS_TASK_ERROR_STAT;

    /* Find a free task slot */
    for (u32 i = 0; i < CELL_SPURS_MAX_TASK; i++) {
        if (!s_tasks[i].in_use) {
            s_tasks[i].in_use = 1;
            s_tasks[i].id = s_next_task_id++;
            s_tasks[i].active = 1;
            s_tasks[i].completed = 0;
            s_tasks[i].exitCode = 0;
            s_tasks[i].entryPoint = elf;

            if (taskId_h) *taskId_h = s_tasks[i].id;
            taskset->taskCount++;

            /* Register the task in the REAL BE taskset: writes task_info[slot]
             * (args/elf/context/ls_pattern) + sets enabled+ready bits so the PM's
             * SELECT_TASK picks it. Slot index i = the SPURS taskId (bitset bit). */
            spurs_taskset_add_task(taskset_ea, i, (uint64_t)elf_ea,
                                   (uint64_t)context_ea, /*arg*/NULL, /*ls_pattern*/NULL);
            /* Bridge to the image-22 dispatch so build_context uses this taskset+task. */
            g_ydkj_real_taskset_ea = taskset_ea;
            g_ydkj_real_taskid     = i;

            printf("[cellSpurs] CreateTask(id=%u, entry=%p) - task logged\n",
                   s_tasks[i].id, elf);

            /* Run the task's SPU program if a lifted build is registered for it.
             * The registry maps the task ELF (by content fingerprint) to its
             * pre-lifted native entry; dispatch loads the ELF into a local store
             * and runs it with the task arg in r3. INERT until the title
             * registers its lifted SPU set: an unregistered image MISSes and
             * returns 0, preserving the prior "track only" behaviour.
             *
             * NOTE: dispatch is synchronous (runs to completion inline). That
             * suits create+join task patterns; a workload/taskset whose SPU job
             * waits on concurrent PPU-side signals will want the async lv2
             * SPU-thread path instead — wired when a title exercises it. */
            if (elf) {
                /* elf/context are guest effective addresses; translate the image
                 * pointer to host memory for fingerprint+load, but keep context
                 * as the guest EA (the SPU job's DMA uses guest EAs / r3). */
                const uint8_t* host_elf = GUEST_PTR(elf, const uint8_t*);
                size_t sz = spu_elf_image_size(host_elf, 2u * 1024 * 1024);
                if (sz)
                    /* Async: SPURS tasks are persistent workers — running them
                     * inline would block this PPU thread forever (deadlock). */
                    spu_workload_dispatch_async(host_elf, (uint32_t)sz,
                                                (uint32_t)(uintptr_t)context);
            }
            return CELL_OK;
        }
    }

    return CELL_SPURS_TASK_ERROR_NOMEM;
}
s32 cellSpursAddWorkload(CellSpurs* spurs, CellSpursWorkloadId* wid,
                         const void* pm, u32 sizePm, u64 data,
                         const u8* priority, u32 minContention,
                         u32 maxContention)
{
    if (!spurs || !wid)
        return CELL_SPURS_CORE_ERROR_NULL_POINTER;
    /* spurs/wid/priority are guest EAs; pm stays a guest EA (it's the SPU
     * program address consumed later by the workload dispatch). */
    spurs = GUEST_PTR(spurs, CellSpurs*);
    CellSpursWorkloadId* wid_h = GUEST_PTR(wid, CellSpursWorkloadId*);
    const u8* priority_h = GUEST_PTR(priority, const u8*);

    if (!spurs->initialized)
        return CELL_SPURS_CORE_ERROR_STAT;

    for (u32 i = 0; i < CELL_SPURS_MAX_WORKLOAD; i++) {
        if (!s_workloads[i].in_use) {
            s_workloads[i].in_use = 1;
            s_workloads[i].pm = pm;
            s_workloads[i].sizePm = sizePm;
            s_workloads[i].data = data;
            s_workloads[i].minContention = minContention;
            s_workloads[i].maxContention = maxContention;
            s_workloads[i].readyCount = 0;

            if (priority_h)
                memcpy(s_workloads[i].priority, priority_h, CELL_SPURS_MAX_SPU);
            else
                memset(s_workloads[i].priority, 0, CELL_SPURS_MAX_SPU);

            *wid_h = i;
            printf("[cellSpurs] AddWorkload(wid=%u, pm=%p, size=%u)\n",
                   i, pm, sizePm);
            return CELL_OK;
        }
    }

    return CELL_SPURS_CORE_ERROR_NOMEM;
}
s32 cellSpursRemoveWorkload(CellSpurs* spurs, CellSpursWorkloadId wid)
{
    if (!spurs) return CELL_SPURS_CORE_ERROR_NULL_POINTER;
    if (wid >= CELL_SPURS_MAX_WORKLOAD) return CELL_SPURS_CORE_ERROR_INVAL;
    if (!s_workloads[wid].in_use) return CELL_SPURS_CORE_ERROR_SRCH;

    s_workloads[wid].in_use = 0;
    printf("[cellSpurs] RemoveWorkload(wid=%u)\n", wid);
    return CELL_OK;
}
