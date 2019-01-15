#include "classification.h"
#include <linux/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <linux/fs.h>	//filesystem
//#include <linux/unistd.h>
#include <sys/stat.h>	//stat systemcall

int set_classification(const char *filename, int new_class)
{
	//systemcall# = 333
	printf("Invoking 'set_classification' system call number 333 FOR: %s.\n", filename);
	int fd = open(filename, O_RDONLY);
	long temp = syscall(333, fd, new_class);
	int ret_status = (int)temp;
	return ret_status;
}
int get_classification(const char *filename)
{
	//systemcall# = 332
	int fd = open(filename, O_RDONLY);
	printf("Invoking 'get_classification' system call number 332 FOR: %s.\n", filename);
	long temp = syscall(332, fd);
	int ret_status = (int)temp;
	return ret_status;
}
