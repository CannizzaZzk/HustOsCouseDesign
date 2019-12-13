#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main()
{
    char str_1[1024],str_2[1024];
    int fp, stop;
    int ret;

    fp = open("/dev/mydev",O_RDWR,S_IRUSR | S_IWUSR);
    
    if(fp == -1)
    {
        printf("Open device failed.\n");
    }
    else
    {
        printf("Input the string: ");
        scanf("%s",str_1);
        ioctl(fp,1,NULL);

        ret = write(fp,str_1,strlen(str_1));
        printf("The length of input string is %d.\n",ret);

        lseek(fp,0,SEEK_SET);
        read(fp,str_2,100);

        printf("Output string is %s.\n",str_2);

        close(fp);
    }

    return 0;
}