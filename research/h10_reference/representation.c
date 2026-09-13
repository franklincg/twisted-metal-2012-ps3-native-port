/* Reconstructed published representation contracts, adapted to real C17/Windows
 * ABI and inline VM helpers. SRW lock synchronizes THIS model's participants.
 * Existing runtime MFC/DMA/signal locks do not share it; integration is pending. */
#include "representation.h"
#include <windows.h>
#include <string.h>
typedef struct {
 int used; uint64_t generation; uint32_t taskset;
 uint64_t task_generation[128];
} H10Slot;
typedef struct { uint32_t owner,capacity; H10Slot slots[32]; } H10Owner;
static H10Owner owners[8];
static SRWLOCK lock=SRWLOCK_INIT;
static uint64_t next_generation=1;
static size_t memory_size;
void h10_set_memory_size(size_t size) { memory_size=size; }
static int range(uint32_t ea,size_t n) { return vm_base && ea && (size_t)ea<=memory_size && n<=memory_size-(size_t)ea; }
static H10Owner* owner_find(uint32_t ea) {
 for(unsigned i=0;i<8;i++) if(owners[i].owner==ea && ea) return &owners[i];
 return NULL;
}
static H10Slot* slot_find(uint32_t caller,H10Handle h) {
 H10Owner* o=owner_find(h.owner);
 if(caller!=h.owner || !o || h.wid>=o->capacity) return NULL;
 H10Slot* s=&o->slots[h.wid];
 return s->used && s->generation==h.generation ? s : NULL;
}
static H10Slot* capture_find(uint32_t caller,H10Capture c) {
 H10Slot* s=slot_find(caller,c.workload);
 if(!s || s->taskset!=c.taskset || c.task>=128 || !range(c.taskset,CSTS_STRUCT_END)) return NULL;
 if(s->task_generation[c.task]!=c.task_generation || !c.task_generation) return NULL;
 if(vm_read64(c.taskset+CSTS_SPURS)!=caller || vm_read32(c.taskset+CSTS_WID)!=c.workload.wid) return NULL;
 return s;
}
static unsigned count_unlocked(uint32_t ts) {
 unsigned n=0;for(unsigned i=0;i<4;i++){uint32_t w=vm_read32(ts+CSTS_ENABLED+i*4);while(w){w&=w-1;n++;}}return n;
}
int h10_owner_init(uint32_t owner,uint32_t capacity) {
 if(!range(owner,128) || (owner&127) || (capacity!=16 && capacity!=32)) return H10_INVAL;
 AcquireSRWLockExclusive(&lock);
 int rc=H10_FULL;
 if(owner_find(owner)) rc=H10_BUSY;
 else for(unsigned i=0;i<8;i++) if(!owners[i].owner){owners[i].owner=owner;owners[i].capacity=capacity;rc=H10_OK;break;}
 ReleaseSRWLockExclusive(&lock);return rc;
}
int h10_allocate(uint32_t owner,uint32_t output_ea,H10Handle* handle) {
 if(!handle || !range(output_ea,4) || (output_ea&3)) return H10_INVAL;
 AcquireSRWLockExclusive(&lock);H10Owner* o=owner_find(owner);int rc=H10_INVAL;
 if(o){rc=H10_FULL;for(uint32_t i=0;i<o->capacity;i++)if(!o->slots[i].used){
  /* Fail closed on generation exhaustion; never allow ABA through wrap. */
  if(next_generation==UINT64_MAX){rc=H10_FULL;break;}
  H10Slot* s=&o->slots[i];memset(s,0,sizeof(*s));s->used=1;s->generation=next_generation++;
  *handle=(H10Handle){owner,i,s->generation};vm_write32(output_ea,i);rc=H10_OK;break;
 }}ReleaseSRWLockExclusive(&lock);return rc;
}
int h10_exists(uint32_t caller,H10Handle h) {
 AcquireSRWLockExclusive(&lock);int yes=slot_find(caller,h)!=NULL;ReleaseSRWLockExclusive(&lock);return yes;
}
int h10_remove(uint32_t caller,H10Handle h) {
 AcquireSRWLockExclusive(&lock);H10Slot* s=slot_find(caller,h);int rc=H10_STALE;
 if(s){if(s->taskset && count_unlocked(s->taskset)) rc=H10_BUSY;else{s->used=0;rc=H10_OK;}}
 ReleaseSRWLockExclusive(&lock);return rc;
}
int h10_attach(uint32_t caller,H10Handle h,uint32_t ts,uint64_t args) {
 if(!range(ts,CSTS_STRUCT_END) || (ts&127)) return H10_INVAL;
 AcquireSRWLockExclusive(&lock);H10Slot* s=slot_find(caller,h);int rc=H10_STALE;
 if(s){rc=H10_BUSY;int overlap=s->taskset!=0;
  for(unsigned a=0;a<8;a++)for(unsigned b=0;b<32;b++){
   H10Slot* t=&owners[a].slots[b];if(t->used && t->taskset && (uint64_t)ts<t->taskset+(uint64_t)CSTS_STRUCT_END && (uint64_t)t->taskset<ts+(uint64_t)CSTS_STRUCT_END)overlap=1;
  }
  if(!overlap){s->taskset=ts;vm_memset(ts,0,CSTS_STRUCT_END);
   spurs_taskset_init(ts,caller,args,h.wid,CSTS_STRUCT_END,0,0);
   vm_write8(ts+CSTS_WKL_FLAG_WAIT,0x80);rc=H10_OK;
  }
 }ReleaseSRWLockExclusive(&lock);return rc;
}
int h10_create_at(uint32_t caller,H10Handle h,uint32_t ts,uint32_t task,uint32_t out,uint64_t elf,uint64_t context,H10Capture* capture) {
 if(task>=128 || !capture || !range(out,4) || (out&3) || !range(ts,CSTS_STRUCT_END) || (ts&127) || !elf) return H10_INVAL;
 if((uint64_t)out<ts+(uint64_t)CSTS_STRUCT_END && (uint64_t)out+4>ts) return H10_INVAL;
 AcquireSRWLockExclusive(&lock);H10Slot* s=slot_find(caller,h);int rc=H10_STALE;
 if(s && s->taskset==ts && vm_read64(ts+CSTS_SPURS)==caller && vm_read32(ts+CSTS_WID)==h.wid){
  rc=H10_BUSY;
  if(!spurs_bitset_test(ts+CSTS_ENABLED,task) && next_generation!=UINT64_MAX){
   spurs_taskset_add_task(ts,task,elf,context,NULL,NULL);
   s->task_generation[task]=next_generation++;
   vm_write32(out,task);*capture=(H10Capture){h,ts,task,s->task_generation[task],elf};rc=H10_OK;
  }
 }ReleaseSRWLockExclusive(&lock);return rc;
}
int h10_count(uint32_t caller,H10Handle h,uint32_t ts,uint32_t* count) {
 if(!count)return H10_INVAL;
 AcquireSRWLockExclusive(&lock);H10Slot* s=slot_find(caller,h);int rc=H10_STALE;
 if(s && s->taskset==ts && ts){*count=count_unlocked(ts);rc=H10_OK;}
 ReleaseSRWLockExclusive(&lock);return rc;
}
int h10_claim_build(uint32_t caller,H10Capture c,int loaded,uint8_t* ls,size_t ls_size) {
 if(!loaded || !ls || ls_size<0x40000)return H10_INVAL;
 AcquireSRWLockExclusive(&lock);H10Slot* s=capture_find(caller,c);int rc=H10_STALE;
 if(s && vm_read64(spurs_taskset_taskinfo_ea(c.taskset,c.task)+TI_ELF)==c.elf){
  rc=H10_BUSY;
  if(spurs_bitset_test(c.taskset+CSTS_ENABLED,c.task) && spurs_bitset_test(c.taskset+CSTS_READY,c.task) && !spurs_bitset_test(c.taskset+CSTS_RUNNING,c.task)){
   spurs_pm_mark_running(c.taskset,c.task);
   spurs_pm_build_context(ls,c.taskset,c.task,0,0);rc=H10_OK;
  }
 }ReleaseSRWLockExclusive(&lock);return rc;
}
int h10_signal(uint32_t caller,H10Capture c) {
 AcquireSRWLockExclusive(&lock);H10Slot* s=capture_find(caller,c);int rc=H10_STALE;
 if(s){spurs_bitset_set(c.taskset+CSTS_SIGNALLED,c.task);rc=H10_OK;}
 ReleaseSRWLockExclusive(&lock);return rc;
}
int h10_take_signal(uint32_t caller,H10Capture c) {
 AcquireSRWLockExclusive(&lock);H10Slot* s=capture_find(caller,c);int rc=H10_STALE;
 if(s){rc=H10_BUSY;if(spurs_bitset_test(c.taskset+CSTS_SIGNALLED,c.task)){spurs_bitset_clear(c.taskset+CSTS_SIGNALLED,c.task);rc=H10_OK;}}
 ReleaseSRWLockExclusive(&lock);return rc;
}
