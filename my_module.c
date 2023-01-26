#include "my_module.h"
 
DEFINE_MUTEX(lock);
 
ssize_t write_response(char __user *user_buff, size_t user_buffer_length, loff_t *offset){
    ssize_t ret = buffer_size;
 
    if (*offset >= buffer_size || copy_to_user(user_buff, buffer, buffer_size)) {           
            ret = 0;
    } else {         
            *offset += buffer_size;
    }
    mutex_unlock(&lock);
    return ret;
}
 
ssize_t get_thread_struct(char __user *user_buff, size_t user_buffer_length, loff_t *offset){
 
  task = get_pid_task(find_get_pid(pid), PIDTYPE_PID);
  if (task == NULL){
    pr_err("Fail to find pid task");
    mutex_unlock(&lock);
    return -EFAULT;
  }
 
  pr_info("%lu\n", task->thread.fsbase);
  pr_info("%lu\n", task->thread.sp);
  pr_info("%lu\n", task->thread.gsbase);
  pr_info("%hu\n", task->thread.gsindex);
 
  buffer_size = sprintf(buffer, "The thread_struct structure was obtained successfully!\n{\n\
     thread.es: %hu \n \
     thread.ds: %hu \n \
     thread.fsindex: %hu \n \
     thread.gsindex: %hu \n \
     thread.fsbase: %lu \n \
     thread.gsbase: %lu \n \
     thread.sp: %lu \n}\n",
     task->thread.es, task->thread.ds, task->thread.fsindex, task->thread.gsindex, task->thread.fsbase, task->thread.gsbase, task->thread.sp);
 
  return write_response(user_buff, user_buffer_length, offset);
}
 
ssize_t get_dentry(char __user *user_buff, size_t user_buffer_length, loff_t *offset){
 
  struct path* my_path = kmalloc(sizeof(struct page), GFP_ATOMIC);
  if(my_path == NULL){
    pr_err("Fail to get path");
    mutex_unlock(&lock);
    return -EFAULT;
  }
  int status = kern_path(user_path, LOOKUP_FOLLOW, my_path);
  if (status){
        pr_err("My_module: Cant find file from user path");
        kfree(my_path);
        mutex_unlock(&lock);
      return -EFAULT;
  }
 
  buffer_size = sprintf(buffer, "The dentry structure was obtained successfully!\n{\n\
     dentry.d_name.name: %s \n \
     dentry.d_parent.name: %s \n \
     dentry.d_inode.i_uid: %d \n \
     dentry.d_inode.i_gid: %d \n \
     dentry.d_inode.flags: %d \n}\n",
     my_path->dentry->d_name.name, my_path->dentry->d_parent->d_name.name, my_path->dentry->d_inode->i_uid, my_path->dentry->d_inode->i_gid, my_path->dentry->d_inode->i_flags);
 
  pr_info("name %s\n", my_path->dentry->d_name.name);
  kfree(my_path);
  return write_response(user_buff, user_buffer_length, offset);
}
 
ssize_t file_read(struct file *filePointer, char __user *user_buff,
                             size_t user_buffer_length, loff_t *offset){
  if (struct_num == NULL){
      pr_err("My_module: No input data, please enter them");
      mutex_unlock(&lock);
      return -EFAULT;
  }
 
  if (struct_num == THREAD_STRUCT ){
      return get_thread_struct(user_buff, user_buffer_length, offset);
  } 
  else if (struct_num == DENTRY){
      return get_dentry(user_buff, user_buffer_length, offset);
  }
  mutex_unlock(&lock);
  return -EFAULT;
}
 
ssize_t file_write(struct file *file, const char __user *user_buff,
                              size_t len, loff_t *off){
    if(mutex_trylock(&lock)){
      buffer_size = len;
      if (buffer_size > BUFFER_MAX_SIZE)
          buffer_size = BUFFER_MAX_SIZE;
   
      if (copy_from_user(buffer, user_buff, buffer_size))
          return -EFAULT;
   
          int count_of_args = sscanf(buffer, "%d", &struct_num);
   
      if (struct_num == THREAD_STRUCT){
        count_of_args = sscanf(buffer, "%d %d", &struct_num, &pid);
      }
   
      if (struct_num == DENTRY){
        count_of_args = sscanf(buffer, "%d %s", &struct_num, &user_path);
      }
   
      return buffer_size;
    }
    else{
    	return -EFAULT;
    }
}



struct proc_ops add_ops = {
    .proc_read = file_read,
    .proc_write = file_write,
};
 
int __init my_module_init(void) {
    pr_info("My_module start init\n");
    request = proc_create(MODULE_NAME, 0777, NULL, &add_ops);
    if (!request) {
            pr_err("procfs: failed to create file \n");
            return -1;
    }
    return 0;
}
 
void __exit my_module_exit(void) {
    proc_remove(request);
    pr_info("my_module unloaded\n");
}
 
module_init(my_module_init);
module_exit(my_module_exit);

