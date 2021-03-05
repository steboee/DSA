#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define VELKOST 100
#define VLK_HLAV 4
#define VLK_PATY 4
#define SIZE_OF_START_OF_HEAP 4
#define SIZE_OF_END_OF_HEAP 4

char*memory = NULL;


void* memory_alloc(unsigned int size) {
    int*head;
    int*foot;
    int*nexthead;
    int*nextfoot;
    int prev_memory=0;
    int * KONIEC = memory + *memory-4;

    int merge=0;
    int remainig_memory;

    *KONIEC = *memory;
    head=(int*)memory+1;

    char*test;
    test = (char*)head;
    while((int*)test <KONIEC){                 //prechádzaj cez alokované bloky az kým nenatrafíš na voľný
        if (*(test) > 0){                       // Našla sa voľná pamäť

            int free_block_size = *test;         //free_block_size = velkosť !!dostupnej pamäťe pre používateľa!! (už čistá pamäť bez velkosti hlavičký a pamate)

            if(size <= free_block_size){
                remainig_memory = free_block_size -size;

                if (remainig_memory <=8){        //Ak by mi po alokácií mal ostat voľný blok o veľkosti menej alebo rovný ako 8B. -> tak by bol nepoužiteľný a tým padom ho spojím s alokovaným poľom pamate
                    merge = 1;
                }
                break;
            }

            else{
                int ofsetik = abs(*(int*)test)+  VLK_HLAV + VLK_PATY;
                test = test+ofsetik;
            }
        }
        else{
            int ofsetik = abs(*(int*)test)+  VLK_HLAV + VLK_PATY;
            test = test+ofsetik;
        }
    }

    int test_size = test+size+4+4;
    int mem_size = memory + *memory-4;
    if (test+size+4+4 > mem_size){                           // Kontrola ak by uživateľ chcel alokovať viac Bytov ako je vôbec jeho voľná pamäť veľká
        printf("ATTEMPTING TO ENTER OUTSIDE YOUR MEMORY");
        exit(1);
    }


    head = (int*)test;
    char*ptr;
    ptr = head;


    if (merge ==1){                                     // Spájanie zvyšného free bloku s alokovaným ( free blok by bol nevyužitelný -> (VELKOST <=8 )  zmestila by sa iba hlavicka a pata)



        prev_memory = *head;
        *head = -size;
        foot = ((char*)(head)+size+4+remainig_memory);
        *foot = *head;
        /*nexthead = foot+1;
        *nexthead = prev_memory -size-VLK_HLAV-VLK_PATY;
        nextfoot = ((char*)nexthead + *nexthead+4);
        *nextfoot = *nexthead;*/

    }
    else{


        prev_memory = *head;
        *head = -size;
        foot = ((char*)(head)+size+4);
        *foot = *head;
        nexthead = foot+1;
        *nexthead = prev_memory -size-VLK_HLAV-VLK_PATY;
        nextfoot = ((char*)nexthead + *nexthead+4);
        *nextfoot = *nexthead;
    }








    return ptr;


}

int memory_free(void* valid_ptr) {
    char*p;
    p=valid_ptr;
    while(*p>0);

}

int memory_check(void* ptr) {


}



void memory_init(void* ptr, unsigned int size) {
    memory = (char*)ptr;
    memset(memory,0,size);
    char *mem_size = (char*)memory;
    *(mem_size) = size;

    *(int*)(mem_size+4) = size - SIZE_OF_START_OF_HEAP - SIZE_OF_END_OF_HEAP-VLK_HLAV-VLK_PATY;           //Prvá hlavička
    *(int*)(mem_size+size-8) = size - SIZE_OF_START_OF_HEAP - SIZE_OF_END_OF_HEAP-VLK_HLAV-VLK_PATY;     //Prvá patička

    *(int*)(mem_size+size-4) = size-SIZE_OF_START_OF_HEAP - SIZE_OF_END_OF_HEAP;




}

int main(){
    char region[100];
    memory_init(region, 100);   // Initialization of my memory of 100bytes
    char *pointer1 = (char *) memory_alloc(13);
    char *pointer2 = (char *) memory_alloc(7);
    char *pointer3 = (char *) memory_alloc(5);
    char *pointer4 = (char *) memory_alloc(8);
    char *pointer5 = (char *) memory_alloc(12);
    char *pointer6 = (char *) memory_alloc(12);
    char *ptrs;
    memory_check(pointer1);

    memory_free(pointer1);
    memset(region, 0, 10);
    return 0;
}

