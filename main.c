#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>


char region[100000];
typedef struct block
{
    struct block* prev;
    struct block *next;
    int free;
    int size;

}BLOCK;


BLOCK *list= region;








void* memory_alloc(unsigned int size) {


    //printf("memory alloc");
    if( size <= 0 ){
        return NULL;
    }



    for (int i= 0 ; i < 100 ; i++){
	    if (region[i] == 2){
	        char *ptr_alloc;
	        ptr_alloc = &region[i];
            //printf("pointer validptr ukazuje na adresu : %p\n", ptr_alloc);
	        region[i] = size;
	        region[i+1] =0;
            region[i+2] =0;
            region[i+3] =0;
            for (int j = 0 ; j < size-4 ; j++ ){
                region[i+4]=-1;
                i++;

            }

            list->prev = NULL;
            list->next = ptr_alloc + size;
	        list->free = 0;
	        list->size = size;
	        list = list->prev;
	        list = (list->next);


            return ptr_alloc;

	    }
	}



}
int memory_free(void* valid_ptr) {
    valid_ptr = 5;
    //printf("Memory free");
    return 1;

}
int memory_check(void* ptr) {
   // printf("Memory check");
    return 1;

}
void memory_init(void* ptr, unsigned int size) {

	//printf("vstupil si do memory init\n");
	//printf("pointer ptr ukazuje na adresu : %p\n", ptr);

	list->next = ptr+4;
	list->size = size;
	list->free = 1;
	list->prev = NULL;

	char* a;
    a = ptr;
    //printf("pointer a ukazuje na adresu : %p\n", a);

    *a = size;
    *(a+1) = 0;
    *(a+2) = 0;
    *(a+3) = 0;
    a = a+4;


	for(int i = 3 ; i < size ;i++){

        *a = 2;
        a++;
	}
    //printf("pointer a ukazuje na adresu : %p\n", a);
	*a = "a";
	*a = 5;
}


int main(){
    memory_init(region, 100);   // Initialization of my memory of 50 bytes
    char *pointer1 = (char *) memory_alloc(10);
    char *pointer2 = (char *) memory_alloc(12);
    char *pointer3 = (char *) memory_alloc(30);
    memset(region, 0, 10);
    return 0;
}


