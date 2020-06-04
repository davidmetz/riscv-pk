// See LICENSE for license details.

#include "pk.h"
#include "atomic.h"
#include "frontend.h"
#include "syscall.h"
#include "htif.h"
#include "boot.h"
#include <stdint.h>

//static int frontend_ctr = 0;
//static size_t frontend_call_times[1024];
//
//void print_frontend_times(){
//    int fc = frontend_ctr;
//    printk("frontend times\n");
//    for(int i=0; i<fc; i++){
//        printk("%d\n", frontend_call_times[i]-current.ctrs.cycle0);
//    }
//}

long frontend_syscall(long n, uint64_t a0, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6)
{
  static volatile uint64_t magic_mem[8];
  counter_data dt;
  if(current.collect_counters){
    read_csrs(&dt);
  }

//  frontend_call_times[frontend_ctr] = rdcycle64();
//  frontend_ctr++;
  static spinlock_t lock = SPINLOCK_INIT;
  spinlock_lock(&lock);

  magic_mem[0] = n;
  magic_mem[1] = a0;
  magic_mem[2] = a1;
  magic_mem[3] = a2;
  magic_mem[4] = a3;
  magic_mem[5] = a4;
  magic_mem[6] = a5;
  magic_mem[7] = a6;

  htif_syscall((uintptr_t)magic_mem);

  long ret = magic_mem[0];

  spinlock_unlock(&lock);

  if(current.collect_counters){
      current.frontend_syscall_cnt++;
      counter_data dt2;
      read_csrs(&dt2);
      subtract_counter(&dt2, &dt);
      add_counter(&current.frontend_ctrs, &dt2);
  }
  return ret;
}

void shutdown(int code)
{
  frontend_syscall(SYS_exit, code, 0, 0, 0, 0, 0, 0);
  while (1);
}

void copy_stat(struct stat* dest, struct frontend_stat* src)
{
  dest->st_dev = src->dev;
  dest->st_ino = src->ino;
  dest->st_mode = src->mode;
  dest->st_nlink = src->nlink;
  dest->st_uid = src->uid;
  dest->st_gid = src->gid;
  dest->st_rdev = src->rdev;
  dest->st_size = src->size;
  dest->st_blksize = src->blksize;
  dest->st_blocks = src->blocks;
  dest->st_atime = src->atime;
  dest->st_mtime = src->mtime;
  dest->st_ctime = src->ctime;
}
