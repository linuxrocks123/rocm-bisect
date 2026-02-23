//Copyright © Advanced Micro Devices, Inc., or its affiliates.
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
