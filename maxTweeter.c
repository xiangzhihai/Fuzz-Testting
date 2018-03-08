#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define LineSize 1024

/*
 * hash table
 */

typedef struct entry_s
{
    char *key;
    int value;
    struct entry_s *next;
} entry_t;

typedef struct hashtable_s
{
    int size;
    struct entry_s **table;
} hashtable_t;

//create hashtable
hashtable_t *create(int size) 
{
    hashtable_t *ht = NULL;

    //allocate hashtable
    if ((ht = malloc(sizeof(hashtable_t))) == NULL)
        return NULL;

    //set table size
    ht->size = size;

    //allocate entries to table
    if ((ht->table = malloc(size * sizeof(entry_t))) == NULL )
        return NULL;

    //make each entry to NULL pointer
    for (int i = 0; i < size; i++)
        ht->table[i] = NULL;

    return ht;
}

//hash function
int hashcode(hashtable_t *ht, char* key)
{
    unsigned long int hashval;

    //convert key to int as entry
    int i = 0;
    while (i < strlen(key) && hashval < ULONG_MAX)
    {
        hashval <<= 8;
        hashval += key[i++];   
    }

    return hashval % ht->size;
}

//create new Entry
entry_t *newEntry(char *key)
{
    entry_t *e;

    //set all value to the entry
    if ((e = malloc(sizeof(entry_t))) == NULL)
        return NULL;

    if ((e->key = strdup(key)) == NULL)
        return NULL;

    //first time
    e->value = 1;

    return e;
}

void set(hashtable_t *ht, char* key)
{
     entry_t *last;

     int bin = hashcode(ht, key);

     entry_t *next = ht->table[bin];

     //go to the end of the linked list in case of collision 
     while(next != NULL && next->key != NULL && strcmp(key, next->key) > 0)
     {
         last = next;
         next = next->next;
     }

     //check of the entry is already exited
     if (next != NULL && next->key != NULL && !strcmp(key, next->key))
     {
         next->value++; //increase its occurence
     } 
     else //new entry 
     {
         entry_t *new = newEntry(key);

         //at the start of the bin
         if (next == ht->table[bin])
         {
             new->next = next;
             ht->table[bin] = new;
         }
         else if (next == NULL)//at the end of the linked list
         {
             last->next = new; //put it at the end
         }
         else //in the middle
         {
             new->next = next;
             last->next = new;
         }
     }
}

//retrieve count 
int get(hashtable_t *ht, char *key)
{
    int bin = hashcode(ht, key);
    entry_t *e = ht->table[bin];

    while (e != NULL && e->key != NULL && strcmp(key, e->key) > 0)
        e = e->next;

    //see if the entry exists
    if (e == NULL || e->key == NULL || strcmp(key, e->key))
        return -1; //not found
    else
        return e->value;
}

/*
 * end of hash table
 */

static int NameSlot;

void HashTableIter(hashtable_t *ht)
{
    for (int i = 0; i < ht->size; i++) 
    {   
        //enpty entry
        if (ht->table[i] == NULL)
            continue;

        entry_t *e = ht->table[i];
        while (e != NULL) {
            printf("entry: %s times: %d\n", e->key, e->value);
            e = e->next;
        }
    }
}

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
    if (file == NULL)
    {
        printf("Fail to open\n");
        exit(1);
    }

    //read first line and get NameSlot
    char line[LineSize];
    fgets(line, LineSize, file);
    if (setNameSlot(line))
    {
        printf("there is no name\n");
        exit(1);
    }

    hashtable_t *ht = create(65536);
    /*set(ht, "zero");
    set(ht, "neva");
    set(ht, "rick");
    set(ht, "zero");
    set(ht, "zero");

    printf("%d\n", get(ht, "neva"));
    printf("%d\n", get(ht, "rick"));
    printf("%d\n", get(ht, "zero"));*/
    

    
    //iterate remain lines to get name
    /*while (fgets(line, LineSize, file))
    {
        char *tmp = strdup(line);
        tmp = strtok(tmp, ",");

        //go to name
        for (int i = 0; i < NameSlot; i++)
            tmp = strtok(NULL, ",");
        printf("%s\n", tmp);
    }*/
    for (int j = 0; j < 10; j++)
    {
        fgets(line, LineSize, file);
        char *tmp = strdup(line);
        tmp = strtok(tmp, ",");

        //go to name
        for (int i = 0; i < NameSlot; i++)
            tmp = strtok(NULL, ",");
        set(ht, tmp);
    }
    HashTableIter(ht);

    fclose(file);
    return 0;
}