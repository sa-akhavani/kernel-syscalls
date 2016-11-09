#ifndef MYSYSCALL_H
#define MYSYSCALL_H

#include <linux/syscalls.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rcupdate.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>


struct file_entity {
long    pid;
int     fd;
char    path[4096];
struct  list_head list;
};

unsigned int hash_function(char* file_addr);
asmlinkage long sys_show_pid_fd(char *path);
asmlinkage long sys_free_hash_table(void);
asmlinkage long sys_init_hash_table(unsigned int table_size);

#endif
