#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LineSize 1024

int main(int argc, char *argv[])
{
    //check if argv is right 
    if (argc != 2)
    {
        printf("Please enter File path as the second argument\n");
        exit(1);
    }

    //open cvs file for reading only
    FILE *file = fopen("/Users/zhihaixiang/Documents/ECS160/stream/cl-tweets-short.csv", "r");

    //check if its successfully read
    if(file == NULL)
    {
        printf("Fail to open\n");
        exit(1);
    }

    //read file
    char line[LineSize];
    while (fgets(line, LineSize, file))
    {
        char *tmp = strdup(line);
        printf("%s\n", tmp);
        free(tmp);
    }

    return 0;
}