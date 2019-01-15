#ifndef SYSCLASSIFICATION
#define SYSCLASSIFICATION

asmlinkage long sys_set_classification(int fd, int new_class);
asmlinkage long sys_get_classification(int fd);

#endif
