#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>    /* BSD and Linux */
#include <stropts.h>

int main()
{
    char str_1[1024],str_2[1024];
    int fp, stop;
    int ret,rd_num,num_out;
    char choice;
    int ofst_rcd = 0;
    int last_pos = 0;

    fp = open("/dev/mydev",O_RDWR | O_TRUNC,S_IRUSR | S_IWUSR);
    
    if(fp == -1)
    {
        printf("Open device failed.\n");
    }
    else
    {   
        while(1){
            memset(str_2,0,sizeof(char)*1024);


            // if(ofst_rcd != 0){
            //     printf("Continue add strings? 'Y' to continue.\n");
            //     scanf("%c",&choice);
            //     getchar();
            //     if(choice == 'Y')
            //     {
            //         printf("Input the string: ");
            //         scanf("%s",str_1);
            //         ioctl(fp,1,NULL);

            //         ret = write(fp,str_1,strlen(str_1));
            //         printf("The length of input string is %d.\n",ret);

                    
            //     }
            // }

            printf("Read or write, input 'r' or 'w'.\n");
            scanf("%c",&choice);
            getchar();
            while(choice != 'w' && choice != 'r')
            {
                printf("Please input 'r' or 'w'.\n");
                scanf("%c",&choice);
                getchar();
            }


            if(choice == 'w')
            {
                printf("Input the string: ");
                scanf("%s",str_1);
                getchar();
                ioctl(fp,1,NULL);

                ret = write(fp,str_1,strlen(str_1));
                printf("The length of input string is %d.\n",ret);
                last_pos += ret;
                lseek(fp,ofst_rcd,SEEK_SET);
            }
            else
            {
                printf("The length of data is %d.\n",last_pos-ofst_rcd);
                if(last_pos == 0 || last_pos - ofst_rcd == 0 )
                {
                    printf("Buffer is empty.\n");
                }
                else 
                {
                    printf("How many numbers do you want?\n");
                    scanf("%d",&num_out);
                    getchar();

                    rd_num = read(fp,str_2,num_out);
                    ofst_rcd += rd_num;
                    printf("Output string is %s.\n",str_2);
                }
            }

            
            
            //lseek(fp,0,SEEK_CUR);
            // if(rd_num == 0)
            //     printf("The buffer is empty.\n");
            // else
            // {
            //      ofst_rcd += rd_num;
            //      printf("Output string is %s.\n",str_2);
            // }

            printf("Input 'q' to quit:\n");
           
            scanf("%c",&choice);

            getchar();
            if(choice == 'q')
                break;
        }
        
        close(fp);
    }

    return 0;
}