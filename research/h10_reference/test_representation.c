#include "representation.h"
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
__declspec(align(128)) static uint8_t guest[0x100000];
uint8_t* vm_base=guest;
static unsigned checks,failures;
void h10_check(int ok,const char* id){checks++;failures+=!ok;printf("%s %s\n",ok?"PASS":"FAIL",id);fflush(stdout);}
#define CHECK(x,id) h10_check((x),(id))
static uint32_t be32(const uint8_t* p){return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];}
static int zeros(uint32_t ea,unsigned n){for(unsigned i=0;i<n;i++)if(guest[ea+i])return 0;return 1;}
typedef struct { unsigned action,index;uint32_t owner,ts;H10Handle h;H10Capture c;int rc;HANDLE start; } Job;
static DWORD WINAPI worker(void* p){
 Job* j=(Job*)p;WaitForSingleObject(j->start,INFINITE);
 if(j->action==0)j->rc=h10_allocate(j->owner,0xA0000+j->index*4,&j->h);
 else if(j->action==1)j->rc=h10_remove(j->owner,j->h);
 else if(j->action==2)j->rc=h10_create_at(j->owner,j->h,j->ts,j->index,0xA1000+j->index*4,0x80000+j->index*8,0,&j->c);
 else if(j->action==3){uint8_t* ls=(uint8_t*)calloc(1,0x40000);if(!ls)j->rc=-1;else{j->rc=h10_claim_build(j->owner,j->c,1,ls,0x40000);free(ls);}}
 else if(j->action==4)j->rc=h10_take_signal(j->owner,j->c);
 return 0;
}
static int parallel(Job* jobs,unsigned n){
 HANDLE threads[128];HANDLE start=CreateEventW(NULL,TRUE,FALSE,NULL);if(!start)return 0;
 unsigned made=0;for(;made<n;made++){jobs[made].start=start;threads[made]=CreateThread(NULL,0,worker,&jobs[made],0,NULL);if(!threads[made])break;}
 SetEvent(start);int ok=made==n;for(unsigned i=0;i<made;i++){ok&=WaitForSingleObject(threads[i],30000)==WAIT_OBJECT_0;CloseHandle(threads[i]);}CloseHandle(start);return ok;
}
int main(void){
 h10_set_memory_size(sizeof(guest));h10_legacy_tests();memset(guest,0,sizeof(guest));
 const uint32_t A=0x20000,B=0x21000,C=0x22000,D=0x23000,E=0x24000,F=0x25000;
 const uint32_t ta=0x30000,tb=0x34000,tc=0x38000,td=0x3C000,out=0x90000;
 H10Handle a={0},a1={0},b={0},c={0},d={0},e={0},fresh={0},bad={0};uint32_t count=999;
 CHECK(h10_owner_init(A,16)==H10_OK,"owner.init_A_16");
 CHECK(h10_allocate(A,out,&a)==0 && a.wid==0,"wid.A_lowest_free0");
 CHECK(h10_owner_init(B,32)==0 && h10_exists(A,a),"owner.init_B_preserves_A");
 CHECK(h10_owner_init(A,16)==H10_BUSY && h10_exists(A,a),"owner.reinit_A_BUSY_preserves_records");
 CHECK(h10_allocate(A,out+4,&a1)==0 && a1.wid==1,"wid.A_next_free1");
 CHECK(guest[out+4]==0 && guest[out+5]==0 && guest[out+6]==0 && guest[out+7]==1,"wid.WID1_BE_bytes");
 CHECK(h10_allocate(B,out+8,&b)==0 && b.wid==0,"owner.same_numeric_WID_separate_B");
 CHECK(h10_remove(B,a)==H10_STALE && h10_exists(A,a),"owner.foreign_remove_rejected");
 CHECK(h10_attach(B,a,ta,0)==H10_STALE && zeros(ta,CSTS_STRUCT_END),"owner.foreign_attach_no_writes");
 CHECK(h10_remove(B,b)==0 && h10_exists(A,a),"owner.remove_B_own0_preserves_A0");
 CHECK(h10_allocate(B,out+8,&fresh)==0 && fresh.wid==b.wid && fresh.generation!=b.generation,"owner.generation_changes_on_reuse");
 CHECK(h10_remove(B,b)==H10_STALE && h10_exists(B,fresh),"owner.stale_generation_cannot_remove_new");b=fresh;
 CHECK(h10_attach(A,a,ta,0x1122334455667788ull)==0,"init.fresh_taskset_A");
 CHECK(h10_attach(B,b,tb,0)==0,"init.fresh_taskset_B");
 CHECK(vm_read64(ta+CSTS_SPURS)==A && vm_read32(ta+CSTS_WID)==0,"init.guest_owner_BE64_wid_BE32");
 CHECK(vm_read64(ta+CSTS_ARGS)==0x1122334455667788ull,"init.guest_args_BE64");
 CHECK(vm_read32(ta+CSTS_SIZE_FIELD)==0x1900,"init.guest_extent_6400");
 CHECK(guest[ta+CSTS_WKL_FLAG_WAIT]==0x80 && guest[ta+0x71]==0 && guest[ta+0x73]==0,"init.sentinel_u8_offset72_neighbors_preserved");
 CHECK(zeros(ta,0x60) && zeros(ta+CSTS_TASKINFO,128*TI_SIZE),"init.all_bitsets_TaskInfo_zero");
 CHECK(h10_attach(A,a,ta,0)==H10_BUSY,"init.no_reset_attached_taskset");
 CHECK(h10_attach(A,a1,ta+128,0)==H10_BUSY,"init.reject_overlapping_taskset");
 CHECK(h10_attach(A,a1,0xFFFFF000,0)==H10_INVAL,"bounds.reject_EA_overflow");
 CHECK(h10_attach(A,a1,ta+1,0)==H10_INVAL,"bounds.require_guest_alignment128");
 CHECK(h10_attach(A,a1,tc,0)==0 && vm_read32(tc+CSTS_WID)==1,"init.taskset_WID1_BE");
 uint8_t snapshot[CSTS_STRUCT_END];memcpy(snapshot,guest+ta,sizeof(snapshot));
 CHECK(h10_count(A,a,ta,&count)==0 && count==0 && !memcmp(snapshot,guest+ta,sizeof(snapshot)),"count.zero_read_only");
 H10Capture cap[128]={0};const uint32_t cases[]={0,1,31,32,39,63,127};
 for(unsigned i=0;i<7;i++){
  unsigned t=cases[i];char id[100];
  int rc=h10_create_at(A,a,ta,t,out+0x100+t*4,0x80000+t*8,0x82000,&cap[t]);
  snprintf(id,sizeof(id),"create.task%u_enabled_ready_correct",t);CHECK(rc==0 && spurs_bitset_test(ta+CSTS_ENABLED,t) && spurs_bitset_test(ta+CSTS_READY,t),id);
  snprintf(id,sizeof(id),"create.task%u_running_intact",t);CHECK(zeros(ta+CSTS_RUNNING,16),id);
  snprintf(id,sizeof(id),"create.task%u_output_BE_slot",t);CHECK(vm_read32(out+0x100+t*4)==t,id);
  snprintf(id,sizeof(id),"create.task%u_TaskInfo_BE",t);CHECK(vm_read64(spurs_taskset_taskinfo_ea(ta,t)+TI_ELF)==0x80000+t*8 && vm_read64(spurs_taskset_taskinfo_ea(ta,t)+TI_CONTEXT)==0x82000,id);
 }
 CHECK(!spurs_bitset_test(ta+CSTS_RUNNING,39),"create.no_phantom_task39");
 memcpy(snapshot,guest+ta,sizeof(snapshot));
 CHECK(h10_count(A,a,ta,&count)==0 && count==7 && !memcmp(snapshot,guest+ta,sizeof(snapshot)),"count.ENABLED_popcount_preserves_READY_RUNNING_all_bytes");
 CHECK(h10_create_at(A,a,ta,128,out,0x80000,0,&cap[2])==H10_INVAL && !memcmp(snapshot,guest+ta,sizeof(snapshot)),"bounds.task128_rejected_no_mutation");
 CHECK(h10_create_at(A,a,ta,UINT32_MAX,out,0x80000,0,&cap[2])==H10_INVAL,"bounds.task_max_rejected");
 CHECK(h10_create_at(A,a,ta,2,ta+4,0x80000,0,&cap[2])==H10_INVAL && !memcmp(snapshot,guest+ta,sizeof(snapshot)),"bounds.output_cannot_alias_taskset");
 CHECK(h10_create_at(A,a,ta,0,out,0x80000,0,&cap[2])==H10_BUSY,"create.duplicate_slot_rejected");
 CHECK(h10_remove(A,a)==H10_BUSY,"lifecycle.live_tasks_remove_BUSY");
 CHECK(h10_create_at(B,a,ta,2,out,0x80000,0,&cap[2])==H10_STALE,"create.foreign_owner_rejected");
 CHECK(h10_create_at(B,b,tb,0,out,0x85000,0,&cap[2])==0,"create.same_slot0_second_instance");
 CHECK(h10_count(B,b,tb,&count)==0 && count==1,"count.second_instance_independent");
 CHECK(h10_owner_init(C,32)==0 && h10_allocate(C,out,&c)==0 && h10_attach(C,c,td,0)==0,"concurrent.setup_128_slots");
 Job jobs[128]={0};for(unsigned i=0;i<128;i++){jobs[i].action=2;jobs[i].index=i;jobs[i].owner=C;jobs[i].ts=td;jobs[i].h=c;}
 CHECK(parallel(jobs,128),"concurrent.128_Windows_threads_joined");
 int all=1;for(unsigned i=0;i<128;i++)all&=jobs[i].rc==0 && vm_read32(0xA1000+i*4)==i;
 CHECK(all,"concurrent.128_distinct_creations_outputs_correct");
 CHECK(h10_count(C,c,td,&count)==0 && count==128,"concurrent.128_ENABLED_no_lost_update");
 all=1;for(unsigned i=0;i<4;i++)all&=vm_read32(td+CSTS_READY+i*4)==UINT32_MAX;
 CHECK(all && zeros(td+CSTS_RUNNING,16),"concurrent.READY_all128_RUNNING_intact");
 CHECK(h10_owner_init(D,32)==0,"concurrent.setup_32_workloads");
 memset(jobs,0,sizeof(jobs));for(unsigned i=0;i<32;i++){jobs[i].owner=D;jobs[i].index=i;}
 CHECK(parallel(jobs,32),"concurrent.32_allocation_threads_joined");
 uint32_t mask=0;all=1;for(unsigned i=0;i<32;i++){all&=jobs[i].rc==0 && jobs[i].h.wid<32;if(jobs[i].h.wid<32)mask|=1u<<jobs[i].h.wid;}
 CHECK(all && mask==UINT32_MAX,"concurrent.32_unique_WIDs");
 vm_write32(out,0xAABBCCDD);CHECK(h10_allocate(D,out,&bad)==H10_FULL && vm_read32(out)==0xAABBCCDD,"wid.capacity32_preserves_output_on_full");
 bad=jobs[0].h;bad.wid=32;CHECK(!h10_exists(D,bad) && h10_remove(D,bad)==H10_STALE,"wid.reject_system_service32");
 bad.wid=UINT32_MAX;CHECK(!h10_exists(D,bad),"wid.reject_uint32max");
 d=jobs[0].h;for(unsigned i=0;i<32;i++)jobs[i].action=1;
 CHECK(parallel(jobs,32),"concurrent.32_removal_threads_joined");all=1;for(unsigned i=0;i<32;i++)all&=jobs[i].rc==0 && !h10_exists(D,jobs[i].h);
 CHECK(all,"concurrent.32_own_removals");
 CHECK(h10_allocate(D,out,&fresh)==0 && fresh.wid==0,"wid.deterministic_lowest_reuse");
 CHECK(h10_remove(D,d)==H10_STALE && h10_exists(D,fresh),"concurrent.old_handle_cannot_delete_reused_record");
 CHECK(h10_owner_init(E,16)==0,"wid.capacity16_setup");
 all=1;for(unsigned i=0;i<16;i++){all&=h10_allocate(E,out,&e)==0 && e.wid==i;}
 CHECK(all && e.wid==15,"wid.capacity16_boundary15");vm_write32(out,0x88776655);
 CHECK(h10_allocate(E,out,&e)==H10_FULL && vm_read32(out)==0x88776655,"wid.capacity16_full_output_preserved");
 CHECK(h10_owner_init(F,17)==H10_INVAL,"wid.invalid_capacity17");
 static uint8_t ls[0x40000],other[CSTS_STRUCT_END];memcpy(other,guest+tb,sizeof(other));
 spurs_bitset_set(ta+CSTS_PENDING_READY,65);spurs_bitset_set(ta+CSTS_SIGNALLED,0);
 vm_write8(ta+0x70,0x56);vm_write8(ta+0x71,0x78);memcpy(snapshot,guest+ta,sizeof(snapshot));
 CHECK(h10_claim_build(A,cap[0],0,ls,sizeof(ls))==H10_INVAL && !memcmp(snapshot,guest+ta,sizeof(snapshot)),"claim.failed_load_no_mutation");
 H10Capture invalid=cap[0];invalid.task_generation++;
 CHECK(h10_claim_build(A,invalid,1,ls,sizeof(ls))==H10_STALE,"claim.stale_task_generation_rejected");
 invalid=cap[0];invalid.elf+=8;
 CHECK(h10_claim_build(A,invalid,1,ls,sizeof(ls))==H10_STALE,"claim.wrong_captured_ELF_rejected");
 CHECK(h10_claim_build(B,cap[0],1,ls,sizeof(ls))==H10_STALE,"claim.foreign_owner_rejected");
 CHECK(h10_claim_build(A,cap[0],1,ls,sizeof(ls))==0,"claim.mark_running_then_context");
 CHECK(!spurs_bitset_test(ta+CSTS_READY,0) && spurs_bitset_test(ta+CSTS_RUNNING,0),"claim.READY0_to_RUNNING0");
 CHECK(h10_claim_build(A,cap[0],1,ls,sizeof(ls))==H10_BUSY,"claim.duplicate_start_rejected");
 CHECK(spurs_bitset_test(ta+CSTS_READY,39) && !spurs_bitset_test(ta+CSTS_RUNNING,39),"claim.other_task39_preserved");
 CHECK(spurs_bitset_test(ta+CSTS_PENDING_READY,65),"claim.pending_ready_preserved");
 CHECK(spurs_bitset_test(ta+CSTS_SIGNALLED,0),"claim.signal_preserved");
 CHECK(!memcmp(other,guest+tb,sizeof(other)),"claim.other_taskset_byte_identical");
 CHECK(guest[ta+0x70]==0x56 && guest[ta+0x71]==0x78 && guest[ta+0x72]==0x80 && guest[ta+0x73]==0,"claim.last_scheduled_only_control_byte_changed");
 CHECK(be32(ls+STC_TASK_ID)==0 && be32(ls+STC_TASKSET_PTR)==0 && be32(ls+STC_TASKSET_PTR+4)==ta,"context.captured_identity_BE");
 CHECK(be32(ls+STC_SYSCALL_ADDR)==0xA70 && be32(ls+STC_BASE+CSTS_WID)==a.wid,"context.A70_and_assigned_WID_preserved");
 CHECK(be32(ls+STC_TEMP_TASKINFO+TI_ELF+4)==0x80000,"context.TaskInfo_correct");
 CHECK(!memcmp(ls+STC_BASE,guest+ta,0x80),"context.header_copied_after_transition");
 all=1;for(unsigned i=0x100;i<0x200;i++)all&=ls[i]==0;CHECK(all,"scope.kernel_selection_region_unconstructed");
 memset(jobs,0,sizeof(jobs));for(unsigned i=0;i<8;i++){jobs[i].action=3;jobs[i].owner=A;jobs[i].c=cap[1];}
 CHECK(parallel(jobs,8),"claim.eight_concurrent_attempts_joined");unsigned wins=0;for(unsigned i=0;i<8;i++)wins+=jobs[i].rc==0;
 CHECK(wins==1,"claim.exactly_one_concurrent_winner");
 CHECK(h10_signal(A,cap[1])==0,"signal.owner_scoped_set");
 memset(jobs,0,sizeof(jobs));for(unsigned i=0;i<8;i++){jobs[i].action=4;jobs[i].owner=A;jobs[i].c=cap[1];}
 CHECK(parallel(jobs,8),"signal.eight_consumers_joined");wins=0;for(unsigned i=0;i<8;i++)wins+=jobs[i].rc==0;
 CHECK(wins==1 && !spurs_bitset_test(ta+CSTS_SIGNALLED,1),"signal.exactly_one_consumer");
 CHECK(spurs_bitset_test(ta+CSTS_SIGNALLED,0),"signal.other_task_signal_preserved");
 CHECK(h10_count(A,a,ta,&count)==0 && count==7,"claim.ENABLED_count_unchanged");
 printf("HOST_H10 checks=%u failures=%u native_build=0 native_link=0 native_run=0 scheduler=0\n",checks,failures);
 return failures?1:0;
}
