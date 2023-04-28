#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mymalloc.h"

#define FREE 'F'
#define NOTFREE 'N'
#define MEMSIZE 4096
//#define PRINT_MEM void* x = (void *)&memory[0];for(int i = 0; i < 40; i++){char* tmp = (char*) x;int* tmp2 = (int*) x;if(*tmp=='F' || *tmp=='N'){printf("[%c]-", *tmp);x += 1;}else{printf("[%d]-", *tmp2);x += 4;}}printf("\n\n");
#define PRINT_MEM //void *itrPtr2 = &memory[0];printf("\n");int countz = 0;while((void *)itrPtr2 < (void *)&(memory[MEMSIZE]) && countz < 10){int* itrChunkSize3 = itrPtr2 + sizeof(char);char* itrChunk3 = (char*) itrPtr2;printf("(PTR[%ld]: %c-%d) ", MEMSIZE - ((void *)&(memory[MEMSIZE]) - (void *)itrPtr2), *itrChunk3, *itrChunkSize3);itrPtr2 += sizeof(char) + sizeof(int) + *itrChunkSize3;countz += 1;}printf("\n\n");     
    // void *itrPtr2 = &memory[0];
    //     printf("\n");
    //     int countz = 0;
    //     while((void *)itrPtr2 < (void *)&(memory[MEMSIZE]) && countz < 10){
    //         int* itrChunkSize3 = itrPtr2 + sizeof(char);
    //         char* itrChunk3 = (char*) itrPtr2;
    //         printf("(PTR[%ld]: %c-%d) ", MEMSIZE - ((void *)&(memory[MEMSIZE]) - (void *)itrPtr2), *itrChunk3, *itrChunkSize3);
    //         itrPtr2 += sizeof(char) + sizeof(int) + *itrChunkSize3;
    //         countz += 1;
    //     }
    //     printf("\n\n");
static char memory[MEMSIZE];

void *mymalloc(size_t size, char *file, int line){
    if(memory[0] == 0){
        memory[0] = FREE;
        int* memsize = (int*)&memory[1];
        *memsize = MEMSIZE - (sizeof(int) + sizeof(char));
        //printf("(%d)\n",*memsize);
        PRINT_MEM
    }

    void *ptr = &memory[0];
    // cant allocate negative space
    if((int)size < 0){
        fprintf(stderr, "ERROR: %s: line %d: Can't call malloc() on negative space. \n",file, line);
        return NULL;
    }
    while((void *)(ptr + sizeof(int) + size) < (void *)&(memory[MEMSIZE])){
        char* curBlock = (char*) ptr;
        int* curBlockSize = ptr + sizeof(char);
        // there is a free block and adequate space
        if(*curBlock == FREE && *curBlockSize >= size){
            char* nextBlock = (ptr + sizeof(int) + sizeof(char) + size);
            int* nextBlockSize = (ptr + sizeof(int) + sizeof(char) + size + sizeof(char));
            //printf("\nSPACE AVAILABLE: %ld\n",*curBlockSize - (size + sizeof(char) + sizeof(int)));
            if((int)(*curBlockSize - (size + sizeof(char) + sizeof(int))) >= 0){
                *nextBlock = FREE;
                *nextBlockSize = *curBlockSize - (size + sizeof(char) + sizeof(int));
            }
            *curBlock = NOTFREE;
            *curBlockSize = size;
            //printf("INSERTING: %d at %ld\n", *curBlockSize, MEMSIZE - ((void *)&(memory[MEMSIZE]) - (void *)ptr));
            //printf("\n%c-%d   %c-%d\n", *curBlock, *curBlockSize, *nextBlock, *nextBlockSize);
            PRINT_MEM
            //printf("INSERTING at %ld\n",MEMSIZE - ((void *)&(memory[MEMSIZE]) - (void *)(ptr + sizeof(char) + sizeof(int))));
            return ptr + sizeof(char) + sizeof(int);
        }
        ptr += sizeof(char) + sizeof(int) + *curBlockSize;
    }
    // no space otherwise
    fprintf(stderr, "ERROR: %s: line %d: Not enough space to call malloc() of size %ld. \n",file, line, size);
    return NULL;
}

void myfree(void *ptr, char *file, int line){
    ptr -= (sizeof(int) + sizeof(char));
    char* curChunk = (char*) ptr;
    // printf("FREEING %p at %ld\n", ptr, MEMSIZE - ((void *)&(memory[MEMSIZE]) - (void *)(ptr+6)));
    // out of bounds
    if((void *)curChunk >= (void *)&(memory[MEMSIZE])){
        fprintf(stderr,"ERROR: %s: line %d: Calling free() with an address not obtained from malloc(). \n", file, line);
    }
    // not the start of a chunk
    else if((*curChunk) != FREE && (*curChunk) != NOTFREE){
        fprintf(stderr, "ERROR: %s: line %d: Calling free() with an address not at the start of a chunk. \n",file, line);
    }
    // already freed
    else if((*curChunk) == FREE){
        PRINT_MEM
        fprintf(stderr, "ERROR: %s: line %d: Calling free() a second time on the same pointer. \n",file, line);
    }
    else{
        *curChunk = FREE;
        int* curChunkSize = ptr + sizeof(char);
        // check if adjacent chunk to the right is free
        if((void *)(ptr + sizeof(char) + sizeof(int) + *curChunkSize) < (void *)&(memory[MEMSIZE])){
            char* nextChunk = (ptr + sizeof(int) + *curChunkSize + sizeof(char));
            int* nextChunkSize = (ptr + sizeof(char) + sizeof(int) + *curChunkSize + sizeof(char));
            if(*(nextChunk) == FREE){
                *curChunkSize += *nextChunkSize + sizeof(char) + sizeof(int);
                *nextChunk = 0;
                *nextChunkSize = 0;
                *curChunk = FREE;
                // printf("COALESCING RIGHT\n");
                // printf("FREED1: %d at %ld\n", *curChunkSize, MEMSIZE - ((void *)&(memory[MEMSIZE]) - (void *)ptr));
            }
        }
        // check if adjacent chunk to the left is free
        void *itrPtr = &memory[0];
        // iterate from left
        while(itrPtr < ptr){ 
            int* itrChunkSize2 = itrPtr + sizeof(char);
            char* itrChunk2 = (char*) itrPtr;
            // left chunk is free
            if((itrPtr + sizeof(char) + sizeof(int) + *itrChunkSize2) == ptr && *itrChunk2 == FREE){
                // coalesce left
                *itrChunkSize2 += *curChunkSize + sizeof(char) + sizeof(int);
                *curChunk = 0;
                *curChunkSize = 0;
                // printf("COALESCING LEFT\n");
                // printf("FREED2: %d at %ld\n", *itrChunkSize2, MEMSIZE - ((void *)&(memory[MEMSIZE]) - (void *)itrPtr));
                break;
            }
            itrPtr += sizeof(char) + sizeof(int) + *itrChunkSize2;
        }
        PRINT_MEM
        return;   
    }  
}
void determineMemoryLeak(){

}

// int main(int argc, char **argv){
//     int x;
//     free(&x);
//     int *p = malloc(sizeof(int)*2);
//     free(p + 1);
//     int *k = malloc(sizeof(int)*10);
//     int *q = k;
//     free(k);
//     free(q);
//     free(p);
//     // int *p = malloc(2040);
//     // int *q = malloc(2046);
//     // free(q);
//     // free(p);

//     return EXIT_SUCCESS;
// }

void determine_memory_leaks (void) __attribute__ ((destructor));
// checks if there are memory leaks
void determine_memory_leaks (void)
{   
    PRINT_MEM
    int* memsize = (int*)&memory[1];
    if(memory[0] != FREE || *memsize != (MEMSIZE - (int)(sizeof(int) + sizeof(char)))){
        fprintf(stderr,"ERROR: Memory leak detected. \n");   
    }
}