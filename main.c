#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>



typedef struct block{
    char* pointr;
    int free;
    struct zoznam* next;

}ZOZNAM;

typedef struct heap_block
{
    struct heap_block* next;
    size_t size;
    int isfree;
    int x; // for alligment
}header;
char region[100000];



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
    ZOZNAM pointers;
	//printf("vstupil si do memory init\n");
	//printf("pointer ptr ukazuje na adresu : %p\n", ptr);

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
	a = 5;
}


/*int main(){
    memory_init(region, 100);   // Initialization of my memory of 50 bytes
    char *pointer1 = (char *) memory_alloc(10);
    char *pointer2 = (char *) memory_alloc(12);
    char *pointer3 = (char *) memory_alloc(30);
    memset(region, 0, 10);
    return 0;
}*/
void z1_testovac(char *region, char **pointer, int minBlock, int maxBlock, int minMemory, int maxMemory, int testFragDefrag) {
    unsigned int allocated = 0;
    unsigned int mallocated = 0;
    unsigned int allocated_count = 0;
    unsigned int mallocated_count = 0;
    unsigned int i = 0;
    int random_memory = 0;
    int random = 0;
    memset(region, 0, 100000);
    random_memory = (rand() % (maxMemory-minMemory+1)) + minMemory;
    memory_init(region + 500, random_memory);
    if (testFragDefrag) {
        do {
            pointer[i] = memory_alloc(8);
            if (pointer[i])
                i++;
        } while (pointer[i]);
        for (int j = 0; j < i; j++) {
            if (memory_check(pointer[j])) {
                memory_free(pointer[j]);
            }
            else {
                printf("Error: Wrong memory check.\n");
            }
        }
    }
    i = 0;
    while (allocated <= random_memory-minBlock) {
        random = (rand() % (maxBlock-minBlock+1)) + minBlock;
        if (allocated + random > random_memory)
            continue;
        allocated += random;
        allocated_count++;
        pointer[i] = memory_alloc(random);
        if (pointer[i]) {
            i++;
            mallocated_count++;
            mallocated += random;
        }
    }
    for (int j = 0; j < i; j++) {
        if (memory_check(pointer[j])) {
            memory_free(pointer[j]);
        }
        else {
            printf("Error: Wrong memory check.\n");
        }
    }
    memset(region +500, 0, random_memory);
    for (int j = 0; j < 100000; j++) {
        if (region[j] != 0) {
            region[j] = 0;
            printf("Error: Modified memory outside the managed region. index: %d\n",j-500);
        }
    }
    float result = ((float)mallocated_count / allocated_count) * 100;
    float result_bytes = ((float)mallocated / allocated) * 100;
    printf("Memory size of %d bytes: allocated %.2f%% blocks (%.2f%% bytes).\n", random_memory, result, result_bytes);
}

int main() {
    char region[100000];
    char* pointer[13000];
    z1_testovac(region, pointer, 8, 24, 50, 100, 1);
    z1_testovac(region, pointer, 8, 1000, 10000, 20000, 0);
    z1_testovac(region, pointer, 8, 35000, 50000, 99000, 0);
    return 0;
}
