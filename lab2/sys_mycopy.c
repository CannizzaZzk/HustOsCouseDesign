//223

asmlinkage int sys_mycopy( char *oFileName, char *wFileName)
{
    long fout, fin;
    char ptemp[100];
    ssize_t rSize;
    mm_segment_t old_fs;

    fout = sys_open( oFileName, O_RDONLY, 0);
    if ( fout == -1) {
        printk("Open file failed!\n");
        return 1;
    }
    fin = sys_open( wFileName, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
    if ( fin == -1) {
        printk("Create file failed!\n");
        sys_close(fout);
        return 1;
    }
    old_fs = get_fs();
    set_fs(KERNEL_DS);
    while(1) {
        rSize = sys_read( fout, ptemp, 100);
        if (rSize == 0)
            break;
        else if ( rSize < 0) {
            printk("Error when read()!\n");
	    set_fs(old_fs);
            sys_close(fout);
            sys_close(fin);
            return 1;
        }
        sys_write( fin, ptemp, rSize);

    }
    set_fs(old_fs);
    sys_close(fout);
    sys_close(fin);
    printk("sys_mycopy: Finish copy file!\n");
    return 0;
}
