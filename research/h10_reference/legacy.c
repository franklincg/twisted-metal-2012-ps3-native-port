#include "representation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
uint32_t g_ydkj_real_taskset_ea=0,g_ydkj_real_taskid=0,g_ydkj_real_spurs_ea=0;
static size_t spu_elf_image_size(const uint8_t* p,size_t cap){(void)p;(void)cap;return 0;}
static int spu_workload_dispatch_async(const uint8_t*p,uint32_t n,uint32_t args){(void)p;(void)n;(void)args;abort();return 0;}
#include "active_api_extract.inc"
#define P(T,a) ((T*)(uintptr_t)(a))
void h10_legacy_tests(void) {
 const uint32_t a=0x10000,b=0x12000,ts=0x14000,attr=0x18000,out=0x19000;
 CellSpursAttribute* at=(CellSpursAttribute*)(vm_base+attr);memset(at,0,sizeof(*at));at->nSpus=2;
 h10_check(cellSpursInitializeWithAttribute(P(CellSpurs,a),P(CellSpursAttribute,attr))==0,"legacy.init_A");
 h10_check(cellSpursCreateTaskset(P(CellSpurs,a),P(CellSpursTaskset,ts),0,NULL,1)==0,"legacy.create_taskset_exact_source");
 h10_check(sizeof(CellSpursTaskset)==152 && _Alignof(CellSpursTaskset)==8,"legacy.windows_size152_align8");
 h10_check(offsetof(CellSpursTaskset,taskCount)==4,"legacy.taskCount_offset4");
 h10_check(vm_read32(ts+CSTS_SIZE_FIELD)==152,"legacy.wrong_guest_size152");
 h10_check(vm_read8(ts+CSTS_WKL_FLAG_WAIT)==0,"legacy.original_sentinel_zero");
 h10_check(cellSpursCreateTask(P(CellSpursTaskset,ts),P(CellSpursTaskId,out),P(void,0x20000),NULL,0,NULL)==0,"legacy.create_task_exact_source");
 h10_check(vm_read32(ts+4)==0x01000000u && vm_base[ts+4]==1 && !vm_base[ts+5] && !vm_base[ts+6] && !vm_base[ts+7],"legacy.alias_bytes_01000000");
 h10_check(spurs_bitset_test(ts+CSTS_RUNNING,39) && !spurs_bitset_test(ts+CSTS_ENABLED,39),"legacy.phantom_task39");
 h10_check(spurs_bitset_test(ts+CSTS_READY,0) && !spurs_bitset_test(ts+CSTS_RUNNING,0),"legacy.task0_ready_not_running");
 h10_check(cellSpursCreateTask(P(CellSpursTaskset,ts),P(CellSpursTaskId,out),P(void,0x20000),NULL,0,NULL)==0 && vm_read32(out)==0x01000000u,"legacy.task_id1_wrong_endian");
 h10_check(cellSpursAddWorkload(P(CellSpurs,a),P(CellSpursWorkloadId,out),NULL,0,0,NULL,1,1)==0,"legacy.workload0");
 h10_check(cellSpursAddWorkload(P(CellSpurs,a),P(CellSpursWorkloadId,out),NULL,0,0,NULL,1,1)==0 && vm_read32(out)==0x01000000u,"legacy.wid1_wrong_endian");
 h10_check(cellSpursRemoveWorkload(P(CellSpurs,b),0)==0 && !s_workloads[0].in_use,"legacy.foreign_remove_interference");
 h10_check(cellSpursInitializeWithAttribute(P(CellSpurs,b),P(CellSpursAttribute,attr))==0 && !s_workloads[1].in_use,"legacy.init_B_erases_A");
 printf("ABI pointer=%zu long=%zu taskset=%zu alignment=%zu count_offset=%zu spurs_offset=%zu\n",sizeof(void*),sizeof(long),sizeof(CellSpursTaskset),(size_t)_Alignof(CellSpursTaskset),offsetof(CellSpursTaskset,taskCount),offsetof(CellSpursTaskset,spurs));
}
