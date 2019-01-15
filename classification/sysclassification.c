#include <linux/types.h>
#include <linux/stat.h>
#include <linux/fs.h>	//filesystem
#include "sysclassification.h"
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/cred.h>
#include <linux/unistd.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/fcntl.h>
#include <linux/syscalls.h>
#include <linux/file.h>

asmlinkage long sys_set_classification(int fd, int new_class)
{
	//1) This function checks if the user has permissions to WRITE to a file
	//2) Then this function will change the requested file's i_classification # to new_class
	if(fd < 0)	//check if fd is valid
	{
		return -1;
	}
	//use task_struct to get the uid and gid of the user running the process
	struct task_struct *task = current;
	int userid = task->cred->uid.val;
	int groupid = task->cred->gid.val;

	//fd 'f' is used to point to the file
	//the 'inode' will point to the file's inode
	struct fd f = fdget_pos(fd);
	struct inode *inode;
	inode = file_inode(f.file);

	//Permissions
	if(userid == 0)	//superuser can do what he/she pleases (check your pronouncs boys and girls)
	{
		inode->i_classification = new_class;
		return inode->i_classification;
	}

	else if(inode->i_uid.val==userid)	//file owner  can write too
	{
		char rwPerm = ((inode->i_mode & S_IRWXU) ? 'w' : '-');	//this will return w on success and - on failure
		char wPerm = ((inode->i_mode & S_IWUSR) ? 'w' : '-');
		if(rwPerm == 'w' || wPerm == 'w')
		{
			inode->i_classification = new_class;	//set class to new_class
			return inode->i_classification;		//return and done
		}
	}

	else if(inode->i_gid.val == groupid)	//group
	{
		char rwPerm = ((inode->i_mode & S_IRWXG) ? 'w' : '-');
		char wPerm = ((inode->i_mode & S_IWGRP) ? 'w' : '-');
		if(rwPerm == 'w' || wPerm == 'w')
		{
			inode->i_classification = new_class;
			return inode->i_classification;
		}
	} 	//other users
		char readwrite = ((inode->i_mode & S_IRWXO) ? 'w' : '-');
		char write = ((inode->i_mode & S_IWOTH) ? 'w' : '-');
		if(readwrite == 'w' || write == 'w')
		{
			inode->i_classification = new_class;
			return inode->i_classification;
		}
	 return -1;	//feelsbadman no permissions
}

asmlinkage long sys_get_classification(int fd)
{
	if(fd<0)
	{
		return -1;
	}

	struct task_struct *task = current;
	int userid = task->cred->uid.val;
	int groupid = task->cred->gid.val;

	struct inode *inode;
	struct fd f = fdget_pos(fd);
	inode = file_inode(f.file);
	//check file permissions
	//logic behind comparing inode->i_mode to stat->st_mode
	//st_mode = stat->mode
	//stat->mode = i->mode
	//stat->mode = inode->i_mode

	//Permissions
	if(userid == 0)
	{
		return inode->i_classification;
	}
	else if(inode->i_uid.val == userid)	//owner
	{
		char rwPerm = ((inode->i_mode & S_IRWXU) ? 'r' : '-');
		char rPerm = ((inode->i_mode & S_IRUSR) ? 'r' : '-');
		if(rwPerm == 'r' || rPerm == 'r')
		{
			return inode->i_classification;
		}
	}
	else if(inode->i_gid.val == groupid)	//group
	{
		char rwPerm = ((inode->i_mode & S_IRWXG) ? 'r' : '-');
		char rPerm = ((inode->i_mode & S_IRGRP) ? 'r' : '-');
		if(rPerm == 'r' || rwPerm == 'r')
		{
			return inode->i_classification;
		}
	}
		char rwPerm = ((inode->i_mode & S_IRWXO) ? 'r' : '-');
		char rPerm = ((inode->i_mode & S_IROTH) ? 'r' : '-');
		if(rwPerm == 'r' || rPerm == 'r')
		{
			return inode->i_classification;
		}
	  return -1;
}

















