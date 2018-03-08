#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LineSize 1024

static int NameSlot;


int setNameSlot(char *line) 
{
    NameSlot = 0;
    
    //separate by ,
    char *token = strtok(line, ",");
    int flag = strcmp(token, "\"name\"");

    //exit if all token traversed or name is found
    while ((token = strtok(NULL, ",")) != NULL && flag)
    {
        flag = strcmp(token, "\"name\"");
        NameSlot++;
    }

    //name not found
    if (flag) 
        return -1; 
    return 0;
}
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

    //read first line and get NameSlot
    char line[LineSize];
    fgets(line, LineSize, file);
    setNameSlot(line);
    
    //iterate remain lines to get name
    while (fgets(line, LineSize, file))
    {
        char *tmp = strdup(line);
        tmp = strtok(tmp, ",");

        //go to name 
        for (int i = 0; i < NameSlot; i++)
            tmp = strtok(NULL, ",");
        printf("%s\n", tmp);
    }

    fclose(file);
    return 0;
}