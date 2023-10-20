#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define BUF_LEN 100

typedef struct File{
    unsigned int lines;
    unsigned int arrLen;
    unsigned long long lastPos;
    unsigned long long *stringPositions;
} File;

void addToArr(unsigned long long *arr, unsigned int *pos, unsigned int *len, unsigned long long n){
    if(*pos >= *len){
        *len += 20;
        arr = (unsigned long long*) realloc(arr, sizeof(unsigned long long) * (*len));
    }
    arr[*pos] = n;
    *pos += 1;
}

void findLines(const char *buf, File *file, int maxLen){
    for(int i = 0; i < maxLen; i++)
        if(buf[i] == '\n' && buf[i] != '\"') {
//            char tmp = buf[i];
//            printf("%c%c%c%c%c\n", buf[i-5],buf[i-4],buf[i-3],buf[i-2],buf[i-1]);
            addToArr(file->stringPositions, &file->lines, &file->arrLen, i + file->lastPos);
        }
    file->lastPos += maxLen + 1;
}


int main(int argc, char *argv[]) {
    char inp[50];
    printf("Enter input file name below:");
    scanf("%s", inp);
    File file;
    file.lines = 0;
    file.arrLen = 20;
    file.lastPos = 0;
    file.stringPositions = (unsigned long long*) calloc(20, sizeof(unsigned long long));
    int fd = open(inp, O_RDONLY);
    if(fd == -1) {
        fprintf(stderr, "fsync failed\n");
        fd = open(inp, O_RDONLY);
        if (fd == -1) exit(1);
    }
    char *buf = (char*) calloc(BUF_LEN, sizeof(char));
    int ret = read(fd, buf, BUF_LEN);
    while(ret > 0){
        findLines(buf, &file, ret < BUF_LEN ? ret : BUF_LEN);
        ret = read(fd, buf, BUF_LEN);
    }
    int strNum = 1;
    while(strNum != 0){
        printf("\nEnter number of string, you want to see\n");
        scanf("%d", &strNum);
        if(strNum < 0 || strNum > file.lines){
            fprintf(stderr, "String index out of range");
            continue;
        }
        int offset = strNum == 1 ? 0 : file.stringPositions[strNum - 2];
        lseek(fd, offset - 1, SEEK_SET);
        int resLen = strNum == 1 ? file.stringPositions[0]: file.stringPositions[strNum - 1] - file.stringPositions[strNum - 2] - 1;
        free(buf);
        buf = (char*) calloc(resLen + 1, sizeof(char));
        read(fd, buf, resLen);
        buf[resLen] = '\000';
        printf("%d'th string is:%s", strNum, buf);

    }
    return 0;
}
