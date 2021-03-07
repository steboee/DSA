#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <stdbool.h>

#define VLK_HLAV 4                          // Velkost hlavičky bloku
#define VLK_PATY 4                          // Velkost päty bloku
#define SIZE_OF_START_OF_HEAP 4             // Velkost "hlavicky" inicializovanej pamäte
#define SIZE_OF_END_OF_HEAP 4               // Velkost "päty" inicializovanej pamäte

char*memory = NULL;                         // pointer ktorý bude ukazovať na začiatok pamäte


void* memory_alloc(unsigned int size) {
    int*head;
    int*foot;
    int*nexthead;
    int*nextfoot;
    int prev_memory;
    int *KONIEC;

    int merge=0;
    int remainig_memory;
    int* pomocka_memory;

    char*kon;
    kon = memory-1 + *(int*)memory;
    pomocka_memory = (int*)memory;
    KONIEC = kon;
    *KONIEC = *pomocka_memory;
    head=(int*)memory+1;

    char*test;
    test = (char*)head;
    while((int*)test <KONIEC){                 //prechádzaj cez alokované bloky az kým nenatrafíš na voľný
        if (*(int*)(test) > 0){                       // Našla sa voľná pamäť

            int free_block_size = *(int*)test;         //free_block_size = velkosť !!dostupnej pamäťe pre používateľa!! (už čistá pamäť bez velkosti hlavičký a pamate)

            if(size <= free_block_size){
                remainig_memory = free_block_size - (int)size;

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



    char*pt= test+size+4+4;

    if (pt > KONIEC){                           // Kontrola ak by uživateľ chcel alokovať viac Bytov ako je vôbec jeho zvyšná voľná pamäť veľká
        //printf("ATTEMPTING TO ENTER OUTSIDE YOUR MEMORY\n");              //Moja kontrola ...
        return NULL;                            // V tomto prípade sa použivateľovi vráti NULL pointer pretože chce alokovať pamäť o veľkosti väčšej než je dostupná pamäť V mojej HEAP
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
    int *header;
    int *header_before;
    int *header_after;
    int *footer;
    int *footer_before;
    int *footer_after;
    int * KONIEC = memory + *(int*)memory-4;
    int velkost;

    char*p;                 // Pomocný pointer


    if (valid_ptr == NULL){                                 //Ak do funnkcie memory free vstúpi pointer ktorý neukazuje na nič napr(z memory_alloc keď nebolo dostatok pamäte)
        return 1;
    }

    header = (int*)valid_ptr;                               // pointer na Hlavičku uvoľnovaného bloku
    footer_before = (int*)valid_ptr - 1;                    // pointer na Pätu bloku pred uvoľnovaným blokom

    p = (char*)header;
    p= p + abs(*header) + VLK_HLAV + VLK_PATY;
    header_after = (int*)p;                                 // pointer na Hlavičku v poradí ďalšíeho bloku po uvoľnovanom bloku

    p = (char*)header;
    p = p + abs(*header) + VLK_HLAV;
    footer = (int*)p;                                       // pointer na Pätu uvoľnovaného bloku

    p = (char*)header_after;
    p = p + abs(*header_after) + VLK_HLAV;
    footer_after = (int*)p;                                 // pointer na Pätu v poradí ďalšieho bloku po uvoľnovanom bloku

    p = (char*)footer_before;
    p = p - abs(*footer_before)- VLK_PATY;
    header_before = (int*)p;                                // pointer na Hlavičku bloku pred uvoľnovaným blokom


    if (footer_before == (int*)memory){                            //Ak idem dealokovať prvý blok v pamäti
        velkost = abs(*header);                                    // (*header) = Veľkosť alokovanej pamäťe --> absolútna hodnota pretože pri alokovanom bloku je velkosť v záporných číslach

        if(*header_after >0){                                           // |START OF HEAP|----|WANT_TO_DEALLOCATE|----|FREE|
            velkost = velkost + *header_after + VLK_HLAV + VLK_PATY;
            *footer_after = velkost;
            *header_after =0;
            header_after = NULL;
            *footer = 0;
            footer = NULL;
            *header = velkost;
            return 0;
        }
        else{                                                 // |START OF HEAP|----|WANT_TO_DEALLOCATE|----|ALLOCATED|
            *header = abs(*header);
            *footer = *header;
            return 0;
        }

    }
    else if(header_after == KONIEC){                           // Odstranujem posledný blok pamete

        if(*header_before >0) {                                               // |FREE|----|WANT_TO_DEALLOCATE|----|END OF HEAP|
            velkost = *header_before + abs(*header);
            *header_before = velkost;
            *footer = velkost;
            return 0;


        }


        else{                                                  // |ALLOCATED|----|WANT_TO_DEALLOCATE|----|END OF HEAP|
            *header = abs(*header);
            *footer = *header;
            return 0;
        }
    }

    else{                                                                                                           //Nie je to prvý blok ani posledný blok pamate



        //|FREE|----|WANT_TO_DEALLOCATE|----|FREE|
        if(*footer_before >0 && *header_after >0) {                                                        //|FREE|----|WANT_TO_DEALLOCATE|----|FREE|
            velkost = abs(*header) + *footer_before + *header_after +2*VLK_HLAV + 2* VLK_PATY;                 //velkost noveho free bloku musim pripočítať 1 hlavicku a 1 paticku bloku ktorý
                                                                                                         // uvolnujem ....  + paticku free bloku pred nim     a hlavicku free bloku za nim
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

        // |ALLOCATED|----|WANT_TO_DEALLOCATE|----|ALLOCATED|
        else if(*footer_before <0 && *header_after <0)  {                                    // |ALLOCATED|----|WANT_TO_DEALLOCATE|----|ALLOCATED|
            velkost = abs(*header);
            *header = velkost;
            *footer = velkost;

            return 0;
        }

        // |ALLOCATED|----|WANT_TO_DEALLOCATE|----|FREE|
        else if(*footer_before <0 && *header_after >0){                                      // |ALLOCATED|----|WANT_TO_DEALLOCATE|----|FREE|
            velkost = abs(*header)+ *header_after + VLK_HLAV +VLK_PATY;
            *header = velkost;
            *footer_after = velkost;
            *footer = 0;
            *header_after = 0;
            footer = NULL;
            header_after = NULL;

            return 0;

        }


        //  |FREE|----|WANT_TO_DEALLOCATE|----|ALLOCATED|
        else if(*footer_before >0 && *header_after <0  ){                               //  |FREE|----|WANT_TO_DEALLOCATE|----|ALLOCATED|
            velkost = abs(*header) + *footer_before + VLK_HLAV + VLK_PATY;
            *header = 0;
            *footer_before =0;
            *footer = velkost;
            *header_before = velkost;


            return 0;
        }

    }

}


int memory_check(void* ptr) {
    char*al;
    int*pointera;
    pointera = ptr;
    al = memory + *(int*)memory-1;
    int *KONIEC;
    KONIEC = al;                                // KONIEC - > ukazovateľ na koniec inicializovanej pamäte
    int*head;
    head=(int*)memory+1;                        // head -> prvá hlavička inicializovanej pamäte
    char*test;
    test = (char*)head;                         // pomocný pointer na posúvanie

    while((int*)test < KONIEC){                    // Prechádzam hlavičky až kým nedojdem na koniec
        if(ptr == test) {                           // ak sa pointer ktorý vstupil do funkcie memory_check == nejakej mojej hlavičke
            if(*pointera < 0){                      // ak je ten blok alokovaný tak memory_check prebehla úspešne a daný pointer je validný
                return 1;
            }
            else{                                   // ak je ten blok voľný tak daný pointer nie je validný
                return 0;
            }
        }
        else{
            int ofsetik = abs(*head)+  VLK_HLAV + VLK_PATY;                 // offsetik -> počet Bytov o koľko sa má posunúť pomocný pointer test
            test = test+ofsetik;                                            // posunutie na ďalšiu hlavičku
            head = test;
        }
    }
    return 0;

}

void memory_init(void* ptr, unsigned int size) {
    int*head;
    int*foot;
    int*heap_start;
    int*heap_end;
    memory = (char*)ptr;
    memset(memory,0,size);
    heap_start = (int*)ptr;
    head = (int*)ptr+1;
    heap_end = (int*)(memory+size-1);
    foot = heap_end-1;




    *heap_start = size;                                                                                     //Zaciatok HEAP-u

    *head = (int)size - SIZE_OF_START_OF_HEAP - SIZE_OF_END_OF_HEAP-VLK_HLAV-VLK_PATY;           //Prvá hlavička
    *foot = (int)size - SIZE_OF_START_OF_HEAP - SIZE_OF_END_OF_HEAP-VLK_HLAV-VLK_PATY;           //Prvá patička
    *heap_end = (int)size;                                                                       //Koniec Heap-u

}


//MOJ TESTER
void tester(char*region,int min_block_size,int max_block_size,int memory_size,int test_free) {

    char* pointers[1000];

    region[memory_size];
    memory_init(region,memory_size);
    int user_allocated =0;
    int allocated__plus_header_footer=0;
    int user_allocated_blocks =0;

    int free_memory_size;
    free_memory_size = memory_size - SIZE_OF_END_OF_HEAP - SIZE_OF_START_OF_HEAP - VLK_PATY - VLK_HLAV;             // zostatok voľnej pamate pre používatela


    time_t seconds;             // náhodný generátor
    seconds = time(NULL);       //
    srand(seconds);             //




    puts("----------------------------------------------------------------------------");

    printf("min block size: %d B\nmax_block_size: %d B\nmemory_size: %d B\n",min_block_size,max_block_size,memory_size);



    int ran_block_size;
    int i=0;
    while(free_memory_size >= min_block_size ){

        ran_block_size = (rand() % (max_block_size + 1 - min_block_size)) + min_block_size;                                         // náhodná veľkosť uživateľskej pamate


        pointers[i] = memory_alloc(ran_block_size);                                                                                   // alokácia náhodnej veľkosti uživateľskej pamäte



        if (pointers[i]){                                                                                               // Ak alokácia prebehla úspešne

            user_allocated  = user_allocated + abs(*(int*)pointers[i]);
            allocated__plus_header_footer = allocated__plus_header_footer + VLK_HLAV+VLK_PATY;
            user_allocated_blocks++;
            if(memory_check(pointers[i])){
                i++;
                free_memory_size = free_memory_size - ran_block_size - VLK_HLAV - VLK_PATY;
            }
            else{
                printf("Memory Check  --------  PROBLEM!!!!");
            }


        }

    }


    int user_deallocated=0;
    int size_of_dealocated;
    bool PROBLEM = false;

    if (test_free == 1){                                // Ak idem kontrolovať uvoĽnovanie blokov
        for (int j = 0; j<i;j++){
            size_of_dealocated = abs(*pointers[j]);             // program si zapamatá velkosť uzivatelskej pamate ktorá sa ide uvolniť
            memory_free(pointers[j]);
            if(memory_check(pointers[j])==1) {

                printf("PROBLEM! memory_free invalid");
                PROBLEM = true;

            }
            else {
                user_deallocated = user_deallocated + size_of_dealocated;               // po uspešnom dealokovaní pamäte ...

            }
        }

    }






    float percentage;
    percentage = (float)((float)user_allocated/(float)memory_size)*100; // Percentá Koľko Bytov bolo alokovaných oproti idealnemu riešeniu

    //printf("User allocated : %d\nMemory size",user_allocated);
    printf("SUCCESFULLY ALLOCATED : %d B  divided into (%d block/s) in memory of size: %d B , what is %.2f%% of IDEAL ALLOCATION\n",user_allocated,user_allocated_blocks,memory_size,percentage);
    if (test_free == 1){
        printf("SUCCESFULLY FREED:      %d B\n",user_deallocated);
    }
    if (PROBLEM == true){
        printf("PROBLEM IN MEMORY CHECK ..........\n");
    }

    puts("----------------------------------------------------------------------------");
    puts("");
    memset(memory,0,memory_size);



}


int main(){
    char region [100000];


/*
#pragma region Moj_maly_tester
    //MALY TESTER LEN NA SKÚŠANIE UVOĽNOVANIA a SPÁJANIA BLOKOV
    memory_init(region, 120);   // Initialization of my memory of 100bytes

    char *pointer1 = (char *) memory_alloc(18);
    char *pointer2 = (char *) memory_alloc(40);

    if (memory_check(pointer1)==1){
        memory_free(pointer1);
    }
    else{
        printf("pointer1 nie je platny ");
    }

    char *pointer3 = (char *) memory_alloc(2);
    char *pointer4 = (char *) memory_alloc(80);
    char *pointer5 = (char *) memory_alloc(2);
    char *pointer6 = (char *) memory_alloc(2);


    if (memory_check(pointer3) == 1){
        memory_free(pointer3);
    }
    else{
        printf("pointer3 nie je platny");
    }

    if (memory_check(pointer4) == 1){                       // pointer4 sa nepodarilo alokovať čiže ani nepojde do memory free pretože je neplatný
        memory_free(pointer4);
    }
    else{
        printf("pointer4 nie je platny");
    }

    if (memory_check(pointer2) == 1){
        memory_free(pointer2);
    }
    else{
        printf("pointer2 nie je platny");
    }

    if (memory_check(pointer5) == 1){
        memory_free(pointer5);
    }
    else{
        printf("pointer5 nie je platny");
    }

    if (memory_check(pointer6) == 1){
        memory_free(pointer6);
    }
    else{
        printf("pointer6 nie je platny");
    }
    memset(region,0,200);
    //MALY TESTER LEN NA SKÚŠANIE UVOĽNOVANIA a SPÁJANIA BLOKOV
#pragma endregion Moj_maly_tester
*/





    printf("TEST 1 ++++++++++++++++++++++++++++++++++++++\n");
    tester(region , 8 , 8 , 50 , 1);
    tester(region , 16 , 16 , 50 , 1);
    tester(region , 24 , 24 , 50 , 1);
    tester(region , 16 , 16 , 100 , 1);
    tester(region , 10 , 10 , 100 , 1);
    tester(region , 24 , 24 , 200 , 1);
    tester(region , 8 , 8 , 200 , 1);

    printf("TEST 2 ++++++++++++++++++++++++++++++++++++++\n");
    tester(region , 8 , 24 , 50 , 1 );
    tester(region , 8 , 24 , 100 , 1 );
    tester(region , 8 , 24 , 200 , 1 );

    printf("TEST 3 ++++++++++++++++++++++++++++++++++++++\n");
    tester(region , 100 , 800 , 10000 , 0 );
    tester(region , 500 , 1000 , 10000 , 0 );
    tester(region , 1000, 5000 , 10000 , 0 );

    printf("TEST 4 ++++++++++++++++++++++++++++++++++++++\n");
    tester(region , 8 , 50000 , 1000 , 0 );
    tester(region , 8 , 50000 , 25000 , 0 );
    tester(region , 8 , 50000 , 34780 , 0 );


    return 0;
}


