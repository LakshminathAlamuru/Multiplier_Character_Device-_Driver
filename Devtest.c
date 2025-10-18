#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main() {
    unsigned int result;
    int fd;
    int i, j, readi, readj;
    char input = 0;
    char drvbuf[8];
    char resultbuf[12];
    int intarray[3];
    /* Open device files for reading and writing*/
    fd = open("/dev/multiplier", O_RDWR);

    /* Handle error opening file*/
    if(fd == -1) {
        printf("Failed to open device file\n");
        return -1;
    }

    while(input != 'q') // continues unless user entered 'q'
    {
        for(i = 0; i <= 16; i++)
        {
            for (j = 0; j <= 16; j++)
            {
                intarray[0] = i;
                intarray[0] = j;
                memcpy(drvbuf, intarray, 8);
                write(fd, drvbuf, 8);
                read(fd, resultbuf, 12);
                memcpy(intarray, resultbuf, 12);
                readi = intarray[0];
                readj = intarray[1];
                result = intarray[2];
                printf("%u * %u = %u\n", readi, readj, result);
                input = getchar();
            }
        }

    }
    /* Close the file descriptor */
    close(fd);
    return 0;
}