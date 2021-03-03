#include <stdio.h>
#include <math.h>
#include <string.h>
#define VELKOST 100


int region[VELKOST];

typedef struct header{
    int size;
    struct header *next;
}Header;

typedef struct footer{
    int size;
    struct footer *previous;
}Footer;


Header *list= (int*)region;




void* memory_alloc(unsigned int size) {
    int zbytek;
    zbytek = VELKOST;

    Header *current;
    current = list;

    while (current->size < 0){
        zbytek = zbytek + current->size;
        current = current->next;

    }



    int *test1;

    test1 = current;


    current->size = -size;
    zbytek = zbytek - size;
    current->next = test1+size;
    current = current->next;
    current->size = zbytek;
    return current;


}

int memory_free(void* valid_ptr) {
    return 1;

}
int memory_check(void* ptr) {
    return 1;

}



void memory_init(void* ptr, unsigned int size) {
    for (int i = 0 ; i<size;i++){
        region[i] = 0;
    }

    list->next = NULL;

    region[0] = size;
    region[4] = list->next;



}

int main(){
    memory_init(region, 100);   // Initialization of my memory of 100bytes
    char *pointer1 = (char *) memory_alloc(10);
    char *pointer2 = (char *) memory_alloc(12);
    char *pointer3 = (char *) memory_alloc(30);
    memset(region, 0, 10);
    return 0;
}

