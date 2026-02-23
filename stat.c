//Copyright © Advanced Micro Devices, Inc., or its affiliates.
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	for(int i=1; i<argc; i++)
		if(!strncmp(argv[i],"/opt/rocm",strlen("/opt/rocm")))
		{
			fprintf(stderr,"stat: cannot statx '%s': No such file or directory\n",argv[i]);
			return 1;
		}

	execv("/usr/bin/stat",argv);
}
