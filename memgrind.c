#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>
#include "mymalloc.h"

void test1(){
    struct timeval current_time;
    long start_s;
    long start_ms;
    long end_s;
    long end_ms;
    gettimeofday(&current_time, NULL);
    start_s = current_time.tv_sec;
    start_ms = current_time.tv_usec;
    for(int j = 0; j < 50; j++){
        for(int i = 0; i < 120; i++){
            void *x = malloc(1);
            free(x);
        }
    }
    gettimeofday(&current_time, NULL);
    end_s = current_time.tv_sec;
    end_ms = current_time.tv_usec;
    printf("test 1 avg time:\n%ld seconds, %ld microseconds\n", (end_s - start_s), (end_ms - start_ms)/50);
}

void test2(){
    struct timeval current_time;
    long start_s;
    long start_ms;
    long end_s;
    long end_ms;
    gettimeofday(&current_time, NULL);
    start_s = current_time.tv_sec;
    start_ms = current_time.tv_usec;
    for(int j = 0; j < 50; j++){
        void *arr[120];
        for(int i = 0; i < 120; i++){
            void *x = malloc(1);
            arr[i] = x;
        }
        for(int j = 0; j < 120; j++){
            free(arr[j]);
        }
    }
    gettimeofday(&current_time, NULL);
    end_s = current_time.tv_sec;
    end_ms = current_time.tv_usec;
    printf("test 2 avg time:\n%ld seconds, %ld microseconds\n", (end_s - start_s), (end_ms - start_ms)/50);
}

void test3(){
    struct timeval current_time;
    long start_s;
    long start_ms;
    long end_s;
    long end_ms;
    gettimeofday(&current_time, NULL);
    start_s = current_time.tv_sec;
    start_ms = current_time.tv_usec;
    for(int j = 0; j < 50; j++){
        int randomBinary;
        void *arr[120];
        int counter = 0;
        int counter2 = 0;
        while(counter < 120){
            randomBinary = rand() % 2 + 0;
            // printf("%d - %d - %d\n", randomBinary, counter, counter2);
            // Allocate a 1-byte chunk and store the pointer in an array
            if(randomBinary == 0){
                void *x = malloc(1);
                arr[counter] = x;
                counter += 1;
            }
            // Deallocate one of the chunks in the array (if any)
            else{ 
                if(counter > counter2){
                    free(arr[counter2]);
                    counter2 += 1;
                }
            }
        }
        while(counter > counter2){
            free(arr[counter2]);
            counter2 += 1;
        }
    }
    gettimeofday(&current_time, NULL);
    end_s = current_time.tv_sec;
    end_ms = current_time.tv_usec;
    printf("test 3 avg time:\n%ld seconds, %ld microseconds\n", (end_s - start_s), (end_ms - start_ms)/50); 
}

void test4(){
    struct timeval current_time;
    long start_s;
    long start_ms;
    long end_s;
    long end_ms;
    gettimeofday(&current_time, NULL);
    start_s = current_time.tv_sec;
    start_ms = current_time.tv_usec;
    for(int j = 0; j < 50; j++){
        void *arr[120];
        for(int i = 0; i < 120; i++){
            void *x = malloc(1);
            arr[i] = x;
        }
        for(int j = 119; j >= 0; j--){
            free(arr[j]);
        }
    }
    gettimeofday(&current_time, NULL);
    end_s = current_time.tv_sec;
    end_ms = current_time.tv_usec;
    printf("test 4 avg time:\n%ld seconds, %ld microseconds\n", (end_s - start_s), (end_ms - start_ms)/50);
}

void test5(){
    struct timeval current_time;
    long start_s;
    long start_ms;
    long end_s;
    long end_ms;
    gettimeofday(&current_time, NULL);
    start_s = current_time.tv_sec;
    start_ms = current_time.tv_usec;
    for(int j = 0; j < 50; j++){
        for(int i = 0; i < 120; i++){
            void *x = malloc(1000);
            free(x);
        }
    }
    gettimeofday(&current_time, NULL);
    end_s = current_time.tv_sec;
    end_ms = current_time.tv_usec;
    printf("test 5 avg time:\n%ld seconds, %ld microseconds\n", (end_s - start_s), (end_ms - start_ms)/50);
}

int main(int argc, char **argv)
{

    test1();
    test2();
    test3();
    test4();
    test5();

    return EXIT_SUCCESS;
}