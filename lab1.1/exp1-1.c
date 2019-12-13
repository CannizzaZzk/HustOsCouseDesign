#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 32

int main(int argc,char* argv[])
{
    char read_buf[BUF_SIZE];

    //check the cmd line
    if(argc != 3)
    {
        printf("argc = %d \n",argc);
        printf("cmd line error!\n");
        return 0;
    }

    //open two files to get the file pointer
    //check if the file is opened successfully
    int fp_src,fp_targ;
    fp_src = open(argv[1],O_RDONLY);
    if(fp_src == -1)
    {
        printf("Open source file error!\n");
        return 0;
    }
    fp_targ = open(argv[2],O_RDWR | O_CREAT,S_IRUSR | S_IWGRP);
    if(fp_targ == -1)
    {
        printf("Open target file error!\n");
        return 0;
    }

    int last_num = 0;
   
    //copy file
    while(1)
    {
       
        //check = read(fp_src,read_buf,32);
        //if(read(fp_src,read_buf,32) == 0)
          //  break;

        last_num = read(fp_src,read_buf,32);
        if(last_num != 32)
            break;

        write(fp_targ,read_buf,32);

       // memset(read_buf,0,sizeof(char)*32);

       // if(check != 32)
         //   break;
        memset(read_buf,0,sizeof(char)*32);

    }

    int i;
    for(i=0;i<last_num;i++)
        write(fp_targ,read_buf+i,1);

    //write(fp_targ,read_buf,32);

    close(fp_src);
    close(fp_targ);

    return 0;
}