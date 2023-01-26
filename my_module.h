#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/sched/task_stack.h>
#include <linux/path.h>
#include <linux/namei.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/file.h>

MODULE_LICENSE("GPL");

#define MODULE_NAME "request"
#define BUFFER_MAX_SIZE 1024
#define THREAD_STRUCT 1
#define DENTRY 2

struct proc_dir_entry* request = NULL;
u64 pid = NULL;
u64 struct_num = NULL;
char user_path[64];

char buffer [BUFFER_MAX_SIZE];
size_t buffer_size = 0;

struct task_struct* task;
