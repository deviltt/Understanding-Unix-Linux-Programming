#include "stdio.h"
#include "fcntl.h"

int main()
{
	int fd1, fd2, fd3;
	char buf[20];
	int amt;

	if((fd1 = open("/home/book/test.txt", O_RDONLY)) == -1)	//以读的方式打开文件
		exit(1);
	
	if((fd2 = open("/home/book/test.txt", O_WRONLY)) == -1)	//以写的方式打开文件
		exit(1);
	
	if((fd3 = open("/home/book/test.txt", O_RDONLY)) == -1)	//以读的方式打开文件
		exit(1);

	if(read(fd1, buf, 20) != 20)	//从文件中读20个字节的字符串存放到buf数组中
		exit(1);
	printf("%s\n",  buf);

	if(lseek(fd2, 4, SEEK_END) == -1)
		exit(1);	

	if(write(fd2, "bc", 25) !=25)	//写25个字符到该文件中 可以使用lseek移动该文件的偏移量
		exit(1);
	
	if(read(fd3, buf, 20) != 20)	//从文件中读20个字符
		exit(1);
	printf("%s\n",  buf);
	
	return 0;
}
