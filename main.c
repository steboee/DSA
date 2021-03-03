#include <stdio.h>
#include <math.h>
#include <string.h>


#define VELKOST 1000


char region[VELKOST];
typedef struct block
{
    int size;
    char free;
    struct block *next;

}BLOCK;


BLOCK *list= region;








void* memory_alloc(unsigned int size) {

    if( size <= 0 ){
        return NULL;
    }

    int memory_used=0;
    while(list->free != 1){
        list = list->next;
        memory_used = memory_used + list->size;
    }

    list->size = size;
    list->free = 0;
    list->next++;
    list = list->next;
    list->size = VELKOST - memory_used - size;
    list->free = 1;


}

int memory_free(void* valid_ptr) {
    return 1;

}
int memory_check(void* ptr) {
    return 1;

}



void memory_init(void* ptr, unsigned int size) {

    list->size = size;
    list->free = 1;
    list->next = ptr + size;


}

int main(){
    memory_init(region, 100);   // Initialization of my memory of 100bytes
    char *pointer1 = (char *) memory_alloc(10);
    char *pointer2 = (char *) memory_alloc(12);
    char *pointer3 = (char *) memory_alloc(30);
    memset(region, 0, 10);
    return 0;
}

