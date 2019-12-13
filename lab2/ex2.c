#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/syscall.h>

int main(){

	long ret = syscall(333, "a.out", "test.out");
//	long ret = syscall(333);
	printf("this syscall return number is: %ld\n", ret);
	return 0;
}
