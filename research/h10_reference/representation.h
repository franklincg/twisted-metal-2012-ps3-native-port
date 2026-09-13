/* H10 host-only reconstruction. No scheduler or native runtime replacement. */
#ifndef H10_REPRESENTATION_H
#define H10_REPRESENTATION_H
#include <stdint.h>
#include <stddef.h>
#include "cellSpurs.h"
#include "spurs_taskset.h"
typedef struct { uint32_t owner,wid; uint64_t generation; } H10Handle;
typedef struct { H10Handle workload; uint32_t taskset,task; uint64_t task_generation,elf; } H10Capture;
enum { H10_OK=0,H10_INVAL=1,H10_BUSY=2,H10_STALE=3,H10_FULL=4 };
void h10_set_memory_size(size_t size); /* setup only, before concurrent access */
int h10_owner_init(uint32_t owner,uint32_t capacity);
int h10_allocate(uint32_t owner,uint32_t output_ea,H10Handle* handle);
int h10_remove(uint32_t caller,H10Handle handle);
int h10_exists(uint32_t caller,H10Handle handle);
int h10_attach(uint32_t caller,H10Handle handle,uint32_t taskset,uint64_t args);
int h10_create_at(uint32_t caller,H10Handle handle,uint32_t taskset,uint32_t task,
                  uint32_t output_ea,uint64_t elf,uint64_t context,H10Capture* capture);
int h10_count(uint32_t caller,H10Handle handle,uint32_t taskset,uint32_t* count);
int h10_claim_build(uint32_t caller,H10Capture capture,int load_succeeded,uint8_t* ls,size_t ls_size);
int h10_signal(uint32_t caller,H10Capture capture);
int h10_take_signal(uint32_t caller,H10Capture capture);
void h10_check(int ok,const char* id);
void h10_legacy_tests(void);
#endif
