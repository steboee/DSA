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

        *head = -size -remainig_memory;
        foot = (char*)(head)+size+4+remainig_memory;
        *foot = *head;

    }
    else{                                                   //alokovanie pamata vo velkom volnom bloku --> splitnem ho na 2 bloky  (1 bude moj alokovany a druhy bude free)


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
    int *header;
    int *header_before;
    int *header_after;
    int *footer;
    int *footer_before;
    int *footer_after;
    p = (char*)valid_ptr;
    int velkost;

    header = (int*)valid_ptr;
    footer_before = (int*)valid_ptr - 1;

    if(footer_before == memory){                            //Odsraňujem prvý blok v pamati

        velkost = abs(*header);
        header_after = p +velkost + VLK_HLAV + VLK_PATY;

        if(*header_after >0){                                // Spájam 2 po sebe idúce free bloky
            velkost = velkost + *header_after;
            p = header_after;
            p = p + *header_after;
            footer_after = p;
            *footer_after = velkost;
            *header_after =0;
            header_after = NULL;
            return 0;

        }
        else{                                                 //Nespájam nič... je to len prvý blok  a za nim je další alokovaný
            *header = abs(*header);
            p = p+velkost+VLK_HLAV;
            footer = p;
            *footer = *header;
            return 0;
        }

    }

    else{                                                                                                           //Nie je to prvý blok pamate
        velkost = abs(*header);
        header_after = p +velkost + VLK_HLAV + VLK_PATY;
        footer = p + abs(*p)+VLK_HLAV;

        if(*header_after >0 && *footer_before >0 ) {                                                                       // Uvolnujem blok medzi 2 free blokmi
            velkost = velkost + *footer_before + *header_after +2*VLK_HLAV + 2* VLK_PATY;                 //velkost noveho free bloku musim pripočítať 1 hlavicku a 1 paticku bloku ktorý
                                                                                                         // uvolnujem ....  + paticku free bloku pred nim     a hlavicku free bloku za nim
            p = header_after;
            p = p + abs(*header_after) + VLK_HLAV;
            footer_after = p;

            p=footer_before;
            p = p - *p- VLK_PATY;
            header_before = p;

            *header_before = velkost;
            *footer_after = velkost;

            *footer =0;
            footer = NULL;
            *header_after = 0;
            header_after = NULL;
            *footer_before = 0;
            footer = NULL;
            *header = 0;
            header = NULL;
            return 0;
        }



    }

    velkost = abs(*(int*)valid_ptr);
    *(int*)valid_ptr = abs(*(int*)valid_ptr);
    p = p+velkost+VLK_HLAV;
    footer =p;
    *footer = *(int*)valid_ptr;
    valid_ptr = NULL;


}

int memory_check(void* ptr) {
    int * KONIEC = memory + *memory-4;
    int*head;
    head=(int*)memory+1;
    char*test;
    test = (char*)head;

    while((int*)test <KONIEC){
        if(ptr == test) {
            if(*(int*)ptr < 0){
                return 1;
            }
            else{
                return 0;
            }

        }
        else{
            int ofsetik = abs(*(int*)test)+  VLK_HLAV + VLK_PATY;
            test = test+ofsetik;
        }
    }
    return 0;

}

void memory_init(void* ptr, unsigned int size) {
    memory = (char*)ptr;
    memset(memory,0,size);
    char *mem_size = (char*)memory;
    *(mem_size) = size;                                                                                   //Zaciatok HEAP-u

    *(int*)(mem_size+4) = size - SIZE_OF_START_OF_HEAP - SIZE_OF_END_OF_HEAP-VLK_HLAV-VLK_PATY;           //Prvá hlavička
    *(int*)(mem_size+size-8) = size - SIZE_OF_START_OF_HEAP - SIZE_OF_END_OF_HEAP-VLK_HLAV-VLK_PATY;      //Prvá patička
    *(int*)(mem_size+size-4) = size;                                                                      //Koniec Heap-u

}

int main(){
    char region[100];
    memory_init(region, 100);   // Initialization of my memory of 100bytes
    char *pointer1 = (char *) memory_alloc(13);
    char *pointer2 = (char *) memory_alloc(7);
    char *pointer3 = (char *) memory_alloc(2);
    memory_free(pointer2);
    memory_free(pointer3);

    memset(region, 0, 50);
    return 0;
}


