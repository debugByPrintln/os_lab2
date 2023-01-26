#include <stdio.h>

int main(){

  char buffer[128];
  char path[64];
  int request;
  int iter;
  int pid;

  printf("%s\n", "Hello, select the structure type\n1: thread_struct\n2: dentry");
  scanf("%d", &request);

  if(request == 1){
    printf("%s\n", "Enter the pid");
    scanf("%d", &pid);
    snprintf(buffer, 64, "echo %d %d >> /proc/request", request, pid);
  }
  if (request == 2){
    printf("%s\n", "Enter path to file");
    scanf("%s", &path);
    snprintf(buffer, 128, "echo %d %s >> /proc/request", request, path);
  }

  iter = system(buffer);
  
  int r = system("cat /proc/request");
  
  return r;
}
