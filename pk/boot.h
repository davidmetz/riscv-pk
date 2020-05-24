// See LICENSE for license details.

#ifndef _BOOT_H
#define _BOOT_H

#ifndef __ASSEMBLER__

#include <stddef.h>

typedef struct {
  uint64_t cycle0;
  uint64_t time0;
  uint64_t instret0;
  uint64_t branch_misp_0;
  uint64_t branch_res_0;
  uint64_t q0_0_0;
  uint64_t q1_0_0;
  uint64_t q2_0_0;
  uint64_t q0_1_0;
  uint64_t q1_1_0;
  uint64_t q2_1_0;
} counter_data;

typedef struct {
  int phent;
  int phnum;
  int is_supervisor;
  size_t phdr;
  size_t phdr_size;
  size_t bias;
  size_t entry;
  size_t brk_min;
  size_t brk;
  size_t brk_max;
  size_t mmap_max;
  size_t stack_top;
  int collect_counters;
  counter_data ctrs;
  uint64_t syscall_cnt;
  uint64_t frontend_syscall_cnt;
  counter_data frontend_ctrs;
} elf_info;

extern elf_info current;

void load_elf(const char* fn, elf_info* info);

static void read_csrs(counter_data * ptr){
  // Read the initial value of the CSR regs attached to the counters
  ptr -> cycle0 = rdcycle64();
  ptr -> time0 = rdtime64();
  ptr -> instret0 = rdinstret64();
  ptr -> branch_misp_0 = read_csr(hpmcounter3);
  ptr -> branch_res_0 = read_csr(hpmcounter4);
  ptr -> q0_0_0 = read_csr(hpmcounter5);
  ptr -> q1_0_0 = read_csr(hpmcounter6);
  ptr -> q2_0_0 = read_csr(hpmcounter7);
  ptr -> q0_1_0 = read_csr(hpmcounter8);
  ptr -> q1_1_0 = read_csr(hpmcounter9);
  ptr -> q2_1_0 = read_csr(hpmcounter10);
}

static void subtract_counter(counter_data * a, counter_data * b){
  a->cycle0 -= b->cycle0;
  a->time0 -= b->time0;
  a->instret0 -= b->instret0;
  a->branch_misp_0 -= b->branch_misp_0;
  a->branch_res_0 -= b->branch_res_0;
  a->q0_0_0 -= b->q0_0_0;
  a->q1_0_0 -= b->q1_0_0;
  a->q2_0_0 -= b->q2_0_0;
  a->q0_1_0 -= b->q0_1_0;
  a->q1_1_0 -= b->q1_1_0;
  a->q2_1_0 -= b->q2_1_0;
}

static void add_counter(counter_data * a, counter_data * b){
  a->cycle0 += b->cycle0;
  a->time0 += b->time0;
  a->instret0 += b->instret0;
  a->branch_misp_0 += b->branch_misp_0;
  a->branch_res_0 += b->branch_res_0;
  a->q0_0_0 += b->q0_0_0;
  a->q1_0_0 += b->q1_0_0;
  a->q2_0_0 += b->q2_0_0;
  a->q0_1_0 += b->q0_1_0;
  a->q1_1_0 += b->q1_1_0;
  a->q2_1_0 += b->q2_1_0;
}

static void zero_counter(counter_data * a){
  a->cycle0 =0;
  a->time0 =0;
  a->instret0 =0;
  a->branch_misp_0 =0;
  a->branch_res_0 =0;
  a->q0_0_0 =0;
  a->q1_0_0 =0;
  a->q2_0_0 =0;
  a->q0_1_0 =0;
  a->q1_1_0 =0;
  a->q2_1_0 =0;
}

#endif // !__ASSEMBLER__

#endif
