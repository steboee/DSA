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


Header *list= (char*)region;




void* memory_alloc(unsigned int size) {
    int zbytek;
    zbytek = VELKOST;

    Header *current;
    current = list;

    while (current->size < 0 || current->size < size){
        zbytek = zbytek + current->size;
        current = current->next;

    }
    char *ptr;
    ptr = current;
    int *test1;
    test1 = current;


    current->size = -size;
    zbytek = zbytek - size;
    current->next = test1+size;
    current = current->next;
    current->size = zbytek;
    return ptr;


}

int memory_free(void* valid_ptr) {
    Header *curr,*prev,*nxt;
    curr = list;
    prev = NULL;
    nxt = NULL;
    int size_of_free_block_ahead=0;
    int size_of_free_block_behind=0;
    int size_of_alocated_block=0;
    while(curr->next != NULL){
        if (curr == valid_ptr){
            nxt = curr->next;

            size_of_free_block_ahead = nxt->size;
            size_of_free_block_behind = prev->size;

            if(nxt->size > 0 && prev->size < 0){              // Merging Deallocated block with free block (which is next one)

                nxt->next = NULL;
                nxt->size=0;
                curr->size = -curr->size + size_of_free_block_ahead;
                curr->next = NULL;
                return 0;
            }



            else if (nxt->size < 0 && prev->size > 0 ){          // Merging Deallocated block with free block ( which is behind )
                size_of_free_block_behind = prev->size;
            }



            else if(nxt->size <0 && prev->size < 0){         //Not merging anything ... There aren't any free blocks next to Dealocating Block
                curr->size = - curr->size;


            }

        }
        prev = curr;
        curr = curr->next;
    }
   return 1;

}
int memory_check(void* ptr) {
    Header *curr;
    curr = list;
    while(curr->next != NULL){
        if (curr == ptr){
            printf("ANO");
            return 1;
        }

        curr = curr->next;

    }
    printf("NIE");

    return 0;

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
    char *ptrs;
    memory_check(pointer1);
    char *pointer3 = (char *) memory_alloc(30);
    memory_free(pointer1);
    memset(region, 0, 10);
    return 0;
}

