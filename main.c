#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define VELKOST 100


char*memory = NULL;


void* memory_alloc(unsigned int size) {
    int*head;
    int*foot;
    int*nexthead;
    int*nextfoot;
    int prev_memory=0;

    head=(int*)memory+1;

    char*test;
    test = (char*)head;
    while(*(int*)test < 0){
        int ofsetik = abs(*head)+4+4;

        test = test+ofsetik;

    }
    if (size + test>memory+ *memory){                           // Kontrola ak by uživateľ chcel alokovať viac Bytov ako je vôbec jeho pamäť veľká
        printf("ATTEMPTING TO ENTER OUTSIDE YOUR MEMORY");
        exit(1);
    }
    head = (int*)test;
    char*ptr;
    ptr = head;
    prev_memory = *head;
    *head = -size;
    foot = ((char*)(head)+size+4);
    *foot = *head;
    nexthead = foot+1;


    return ptr;


}

int memory_free(void* valid_ptr) {

}

int memory_check(void* ptr) {


}



void memory_init(void* ptr, unsigned int size) {
    memory = (char*)ptr;
    memset(memory,0,size);
    char *mem_size = (char*)memory;
    *(mem_size) = size;

    *(int*)(mem_size+4) = size - 4;

    *(int*)(mem_size+size-4) = size-4;




}

int main(){
    char region[50];
    memory_init(region, 50);   // Initialization of my memory of 100bytes
    char *pointer1 = (char *) memory_alloc(10);
    char *pointer2 = (char *) memory_alloc(10);
    char *ptrs;
    memory_check(pointer1);
    char *pointer3 = (char *) memory_alloc(10);
    memory_free(pointer1);
    memset(region, 0, 10);
    return 0;
}

