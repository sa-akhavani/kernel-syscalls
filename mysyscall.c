#include "mysyscall.h"

unsigned int		gb_table_size;
struct file_entity	**gb_file_array;

unsigned int hash_function(char* file_addr)
{
	unsigned int ans = 0;
	int i;
	for (i = 0; i < (int)(strlen(file_addr)); i++)
		ans = ans * 33 + file_addr[i];
	ans = ans % gb_table_size;
	return ans;
}

asmlinkage long sys_free_hash_table(void) //System call number is: 338
{
	printk("free hash table\n");
	int i;

	for(i = 0; i < gb_table_size; i++) {
	       	struct list_head *p;
		struct file_entity *f;

		list_for_each(p, &(gb_file_array[i]->list)) {
			f = list_entry(p, struct file_entity, list);
			kfree(f);
		}
		kfree(gb_file_array[i]);
	}
	printk("EZ\n");
	return 0;
}

void file_entity_init(long pid, int fd, char* path, struct file_entity* new_node ) {
	new_node->pid = pid;
	new_node->fd = fd;
	strcpy(new_node->path, path);
}

asmlinkage long sys_init_hash_table(unsigned int table_size) // System call number is: 339
{
	int j;
	printk("init hash table\n");
	gb_table_size = table_size;
	gb_file_array = kmalloc((sizeof(struct file_entity *))*table_size, GFP_KERNEL);

        struct task_struct *task;
        struct files_struct *current_files;
        struct fdtable *files_table;
        int i = 0;
        struct path files_path;
        char *cwd;
        char *buf = (char *)kmalloc(GFP_KERNEL,100*sizeof(char));

	for(j=0; j < table_size; j++) {
		gb_file_array[j] = kmalloc(sizeof(struct file_entity), GFP_KERNEL);
		INIT_LIST_HEAD(&(gb_file_array[j]->list));
	}


        for_each_process(task) {
                if (task->state < 0)
                        continue;
                current_files = task->files;
                files_table = files_fdtable(current_files);

                for(i = 0;files_table->fd[i] != NULL; i++) {
                        files_path = files_table->fd[i]->f_path;
                        cwd = d_path(&files_path,buf,100*sizeof(char));
			struct file_entity *new_node = kmalloc(sizeof(struct file_entity), GFP_KERNEL);
			file_entity_init(task->pid, i, cwd, new_node);
			int hash_key = hash_function(cwd);
			struct list_head* cur_head = &(gb_file_array[hash_key]->list);
			list_add(&new_node->list, cur_head);

			printk("pid: [%ld]  -  fd: [%d]  -  path: [%s]\n", new_node->pid, new_node->fd, new_node->path);
		}			
        }
	return 0;
}


asmlinkage long sys_show_pid_fd(char *path) // System call number is: 337
{
	printk("start show pid fd\n");
	unsigned int hash_key;
	hash_key = hash_function(path);

       	struct list_head *p;
        struct file_entity *f;

	list_for_each(p, &(gb_file_array[hash_key]->list)) {
		f = list_entry(p, struct file_entity, list);
		if (strcmp(f->path, path) == 0)
			printk("pid: [%ld]  -  fd: [%d]  -  path: [%s]\n", f->pid, f->fd, f->path);
		else
			continue;
	}
		
	return 0;
}
