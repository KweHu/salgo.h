# Have you ever needed anything more than stadlib.h qsort?
This single header library will give you access to every sorting algorithm ever created.

## Quick Start

```c
#define SALGO_IMPLEMENTATION
#include "salgo.h"

#include <stddef.h>
#include <stdio.h>

#define ARR_SIZE 6

int int_desc_compare(const void* a, const void* b){
    return *(const int*)a - *(const int*)b;
}

int main(void){

    int small_int_arr[ARR_SIZE] = {6, 3, 2, 4, 5, 1};

    printf("\n Unsorted: ");
    for(size_t i = 0; i < ARR_SIZE; ++i){
        printf("%d,", small_int_arr[i]);
    }
    
    salgo_bogo_sort(&small_int_arr, ARR_SIZE, sizeof(int), int_desc_compare);

    printf("\n Sorted: ");
    for(size_t i = 0; i < ARR_SIZE; ++i){
        printf("%d,", small_int_arr[i]);
    }

    return 0;
}
```
