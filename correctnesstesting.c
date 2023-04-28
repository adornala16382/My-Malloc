#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mymalloc.h"
 
// malloc() reserves unallocated memory
void c_test1(){
    // assign distinct values to 3 objects
    int size1 = 200;
    char *one = malloc(size1);
    for (int i = 0; i < size1; i++) { 
        one[i] = '1';
    }
    int size2 = 500;
    char *two = malloc(size2);
    for (int j = 0; j < size1; j++) { 
        two[j] = '2';
    }
    int size3 = 1000;
    char *three = malloc(size3);
    for (int k = 0; k < size1; k++) { 
        three[k] = '3';
    }
    // assert objects contain written value
    for (int i = 0; i < size1; i++) { 
        assert(one[i]=='1');
    }

    for (int j = 0; j < size1; j++) { 
        assert(two[j]=='2');
    }

    for (int k = 0; k < size1; k++) { 
        assert(three[k]=='3');
    }  
    free(one);
    free(two);
    free(three);
}

// free() deallocates memory
void c_test2(){
    void* one = malloc(2000);
    free(one);
    void* two = malloc(3000);
    free(two);
    void* three = malloc(4000);
    free(three);

}

// malloc() and free() arrange so that adjacent free blocks are coalesced
void c_test3(){
    void* one = malloc(2000);
    void* two = malloc(1000);
    free(two);
    void* three = malloc(2000);  
    free(one);
    free(three);
    void* four = malloc(4000);
    free(four);
}

//Errors are reported
void c_test4(){
    //Calling free() with an address not obtained from malloc().
    int x;
    free(&x);
    //Calling free() with an address not at the start of a chunk.
    int *p = malloc(sizeof(int)*2);
    free(p + 1);
    //Calling free() a second time on the same pointer.
    int *k = malloc(sizeof(int)*100);
    int *q = k;
    free(k);
    free(q);
    //Not freeing p results in Memory leak.
    // free(p);
    //Calling malloc() on negative space.
    int *a = malloc(-1);
    //Not enough space to call malloc() of that size
    int *b = malloc(4092);
}

int main(int argc, char **argv)
{

    c_test1();
    c_test2();
    c_test3();
    c_test4();

    return EXIT_SUCCESS;
}