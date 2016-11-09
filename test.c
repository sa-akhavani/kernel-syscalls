#include <stdio.h>
#include <linux/unistd.h>
#include <sys/syscall.h>

#define sys_init_hash_table 	339
#define sys_show_pid_fd		337
#define sys_free_hash_table	338

int main (void)
{
	unsigned int size = 1000;
	char* sag = "/home/ali/.xsession-errors";
	syscall(sys_init_hash_table, size);
	syscall(sys_show_pid_fd, sag);
	syscall(sys_free_hash_table);
//	scanf();
	return 0;
}
