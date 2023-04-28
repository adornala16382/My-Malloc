Aryan Dornala
ad1496

I am submitting by myself.

Test Plan:

    (a) - What properties your library must have for you to consider it correct

            [Mymalloc.c]

                malloc():
                    - Reserves unallocated memory
                    - Returns NULL and prints unique error if:
                        ~ provided with a negative 'size'
                        ~ there is not enough space to allocate the 'size' given.
                    - Returns the pointer to the payload if able to allocate memory of size 'size' without 
                    overlapping with other allocated objects.

                free():
                    - Deallocates memory
                    - Prints unique error if:
                        ~ provided a pointer with an address not obtained in malloc().
                        ~ provided a pointer with an address not at the start of a chunk.
                        ~ provided a pointer to a chunk that has already been freed.
                    - Otherwise, sets the chunk to be FREE.
                    - If the chunk to the right is also free, then COALESCE the chunks.
                    - If the chunk to the left is also free, then COALESCE the chunks.

                General:
                    - Detect memory leaks.

    (b) - How you intend to check that your code has these properties

        malloc():
            - To check if malloc() reserves unallocated memory without overlapping with other objects, one could write a function 
            that allocated several large objects with distinct byte patterns and then checks that each object contains the distict 
            byte pattern.
            - To check if malloc() prints error when provided a negative size or a size that it is unable to allocate, one 
            could write a function that would:
                ~ pass in a negative size to malloc() check whether an error was printed related to that.
                ~ pass in a size larger than the space available in memory and check whether an error was printed related to that.
        
        free():
            - To check if free() deallocates memory, one could write a function that calls malloc() on a relatively large number 
            that is still smaller than available space, then call free() on that pointer, and then call malloc() on another relatively 
            large number that is smaller than available space that would otherwise print an error saying not enough space if the first 
            pointer had not been freed.
            - To check if free() prints error when provided a pointer with an address not obtained in malloc():
                ~ write function that would create an object without using malloc() and call free() on its pointer and check whether
                an error was printed related to that.
            - To check if free() prints error when provided a pointer with an address not at the start of a chunk:
                ~ write function that would call malloc() on a reasonable size and call free() on its pointer + 1 and check whether
                an error was printed related to that.
            - To check if free() prints error when provided a pointer to a chunk that has already been freed:
                ~ write function that would call malloc() on a reasonable size, create a new pointer that points to the same address as 
                the pointer created from the call to malloc(), free() the pointer created from malloc(), free() the 2nd pointer, and check
                whether an error was printed related to that.
            - To check coalescing, one could write a function that:
                check left coalesce: 
                    ~ calls malloc() twice with two relatively large numbers, calls free() on the 2nd pointer created from malloc(), then 
                    malloc() another relatively large number that would otherwise print an error saying not enough space if the chunk 
                    from the 2nd pointer did not coalesce with the chunk to the right.
                check right coalesce (continuing from left coalesce check):
                    ~ calls free() on 1st pointer created from malloc(), calls free() on 3rd pointer created from malloc(), then calls
                    malloc() on another relatively large number that would otherwise print an error saying not enough space if the chunk
                    from the 3rd pointer did not coalesce with the chunk from the 1st pointer.
            
            General:
                - To check whether memory is leaked, one could write a function that would run before the program exits and checks if 
                memory has a single chunk that is FREE and has the maximum possible size that the chunk can have given the macro MEMSIZE.

    (c) - The specific method(s) you will use to check each property

        [correctnesstesting.c]
            - Create a file called correctnesstesting.c with functions pertaining to the checks listed in above in (b). Run all checks 
            ensuring that output is as expected only if my program has all the properties I intend.
    
Descriptions of test programs:

    [correctnesstesting.c] - Confirms that my program works with all the properties I intended.
        Test 1: malloc() reserves unallocated memory:
            - Utilizes assert() to print error if any object gets overriden.
        Test 2: free() deallocates memory:
            - Errors will be printed only if not working as intended.
        Test 3: malloc() and free() arrange so that adjacent free blocks are coalesced
            - Errors will be printed only if not working as intended.
        Test 4: Errors are reported:
            - Each possible error will be printed once.

        Expected Output:
            ERROR: correctnesstesting.c: line 68: Calling free() with an address not obtained from malloc(). 
            ERROR: correctnesstesting.c: line 71: Calling free() with an address not at the start of a chunk. 
            ERROR: correctnesstesting.c: line 76: Calling free() a second time on the same pointer. 
            ERROR: correctnesstesting.c: line 80: Can't call malloc() on negative space. 
            ERROR: correctnesstesting.c: line 82: Not enough space to call malloc() of size 4092.  
    
    [memgrind.c] - Stress test for my program ensuring that calling malloc() and free() are done in reasonable speeds.
        Test 1, Test 2, Test 3: already described in assingment instructions.
        Test 4: 
            - Use malloc() to get 120 1-byte chunks, storing the pointers in an array, then use free() to
            deallocate the chunks, but starting at the end of the array and going to the beginning.

            Purpose: To check the speed of coalescing to the right.

        Test 5: 
            - malloc() and immediately free() a 1000-byte chunk, 120 times.

            Purpose: To check that calling malloc() and free() on a large chunk does not affect speeds.
    
        Output:
            test 1 avg time:
            0 seconds, 2 microseconds
            test 2 avg time:
            0 seconds, 46 microseconds
            test 3 avg time:
            0 seconds, 18 microseconds
            test 4 avg time:
            0 seconds, 90 microseconds
            test 5 avg time:
            0 seconds, 2 microseconds      

Design properties proven: 

    - All properties listed in part (a) work as intended since the checks listed in part (b) work as intended in correctnesstesting.c
    - Additionally, memgrind.c outputs results that show reasonable speeds for calling malloc() and free()

