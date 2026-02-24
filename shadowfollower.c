//Copyright © Advanced Micro Devices, Inc., or its affiliates.

#define _GNU_SOURCE

#include <sys/stat.h>

#include <dlfcn.h>
#include <errno.h>
#include <string.h>

int stat(const char *restrict path, struct stat *restrict statbuf)
{
     int (*real_stat)(const char *restrict path, struct stat *restrict statbuf) = dlsym(((void *) -1l),"stat");
     if(!strncmp(path,"/opt/rocm",strlen("/opt/rocm")))
       return -1;
     return real_stat(path,statbuf);
}

int lstat(const char *restrict path, struct stat *restrict statbuf)
{
     int (*real_lstat)(const char *restrict path, struct stat *restrict statbuf) = dlsym(((void *) -1l),"lstat");
     if(!strncmp(path,"/opt/rocm",strlen("/opt/rocm")))
       return -1;
     return real_lstat(path,statbuf);
}

int fstatat(int dirfd, const char *restrict path, struct stat *restrict statbuf, int flags)
{
	int (*real_fstatat)(int dirfd, const char *restrict path, struct stat *restrict statbuf, int flags) = dlsym(((void *) -1l),"fstatat");
	if(!strncmp(path,"/opt/rocm",strlen("/opt/rocm")))
		return -1;
	return real_fstatat(dirfd,path,statbuf,flags);
}

int statx(int dirfd, const char * restrict path, int flags, unsigned int mask, struct statx *restrict statxbuf)
{
	int (*real_statx)(int dirfd, const char * restrict path, int flags, unsigned int mask, struct statx *restrict statxbuf) = dlsym(((void *) -1l),"statx");
	if(!strncmp(path,"/opt/rocm",strlen("/opt/rocm")))
	       return -1;
     	return real_statx(dirfd,path,flags,mask,statxbuf);
}
