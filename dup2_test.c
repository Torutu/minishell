
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int fd = open("file1", O_RDWR | O_CREAT, 0644);
	if (fd < 0)
		return (1);
	
	int out_copy = dup(STDOUT_FILENO);

	if (dup2(fd, STDOUT_FILENO) == -1 )
		return (1);
	close(fd);
	write(1, "Hello world!!1\n", 16);

	if(dup2(out_copy, STDOUT_FILENO) == -1)
		return (1);
	close(out_copy);

	write(1, "Hello world!!2\n", 16);

	
	return (0);
}
