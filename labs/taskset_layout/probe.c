/* SPDX-License-Identifier: MIT
 * Original public synthetic diagnostic. It does not execute game code and is
 * NOT a runtime replacement, H10 integration proof or PS3 conformance suite. */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

enum { EXTENT=0x1900, RUNNING=0x00, READY=0x10, ENABLED=0x30, WID=0x74, TASKS=128 };
static unsigned checks, failures;
static void check(int ok,const char* name) {
    ++checks;
    if(!ok){++failures;fprintf(stderr,"FAIL %s\n",name);}
}
static uint32_t read_be32(const uint8_t* p) {
    return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];
}
static void write_be32(uint8_t* p,uint32_t x) {
    p[0]=(uint8_t)(x>>24);p[1]=(uint8_t)(x>>16);p[2]=(uint8_t)(x>>8);p[3]=(uint8_t)x;
}
static int bit(const uint8_t* mem,size_t field,unsigned task) {
    if(task>=TASKS || field>EXTENT-16)return -1;
    return (read_be32(mem+field+4*(task/32)) & (UINT32_C(0x80000000)>>(task%32)))!=0;
}
static int set_bit(uint8_t* mem,size_t field,unsigned task) {
    if(task>=TASKS || field>EXTENT-16)return 0;
    uint8_t* p=mem+field+4*(task/32);
    write_be32(p,read_be32(p)|(UINT32_C(0x80000000)>>(task%32)));return 1;
}
static unsigned population(const uint8_t* mem,size_t field) {
    unsigned n=0;
    for(unsigned i=0;i<4;++i){uint32_t w=read_be32(mem+field+4*i);while(w){w&=w-1;++n;}}
    return n;
}
int main(void) {
    uint8_t mem[EXTENT]={0},before[16],snapshot[EXTENT];
    /* Simulate precisely the bytes of a 32-bit little-endian increment at +4,
       without depending on host endianness or overlaying a host struct. */
    mem[4]=1;
    check(read_be32(mem+4)==UINT32_C(0x01000000),"legacy byte order");
    check(bit(mem,RUNNING,39)==1,"legacy task39 phantom reproduced");
    check(bit(mem,ENABLED,39)==0,"phantom not enabled");
    /* A BE scalar at the same offset is STILL an invalid guest counter. */
    memset(mem,0,sizeof mem);write_be32(mem+4,1);
    check(population(mem,RUNNING)==1,"endian-only change still corrupts RUNNING");
    memset(mem,0,sizeof mem);memcpy(before,mem+RUNNING,sizeof before);
    for(unsigned task=0;task<TASKS;++task) {
        check(set_bit(mem,ENABLED,task),"enabled bounds");
        check(set_bit(mem,READY,task),"ready bounds");
        check(bit(mem,ENABLED,task)==1,"enabled state");
        check(memcmp(mem+RUNNING,before,sizeof before)==0,"running untouched by creation fixture");
    }
    check(population(mem,ENABLED)==128,"count derived from enabled bits");
    check(population(mem,READY)==128,"ready populated");
    check(population(mem,RUNNING)==0,"no phantom running tasks");
    write_be32(mem+WID,1);
    check(mem[WID]==0 && mem[WID+1]==0 && mem[WID+2]==0 && mem[WID+3]==1,"WID big endian bytes");
    check(read_be32(mem+WID)==1,"WID round trip");
    memcpy(snapshot,mem,sizeof mem);
    check(set_bit(mem,ENABLED,128)==0,"invalid task rejected");
    check(set_bit(mem,EXTENT-8,0)==0,"invalid range rejected");
    check(memcmp(mem,snapshot,sizeof mem)==0,"rejected operations preserve data");
    printf("{\"suite\":\"public_synthetic_layout_probe\",\"assertions\":%u,\"failed\":%u,\"game_executed\":false,\"production_integration_proved\":false}\n",checks,failures);
    return failures ? 1 : 0;
}
