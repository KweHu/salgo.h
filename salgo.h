// salgo.h - v1.0
//
// # STB-style Single Header Library
//
// This is a classic stb-style single header library. See
// https://github.com/nothings/stb/blob/master/docs/stb_howto.txt
// for more info.
//
// So if you just include it as is it acts as a header file, that is it only provides declarations
// without implementations. To make it also provide implementations you define SALGO_IMPLEMENTATION macro:
//
// ```c
// #define SALGO_IMPLEMENTATION
// #include "salgo.h"
// ```
//
// # Performance and Bugs
//
// None of these sorting functions is optimal or bug free.
// I'm beginner "programmer" which found fun in making that little project

#ifndef SALGO_H_
#define SALGO_H_

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifndef SALGODEF
#   define SALGODEF static inline
#endif

typedef enum {
    SALGO_ERR_OK,
    SALGO_ERR_ARR_NULL,
    SALGO_ERR_COMP_NULL,
    SALGO_ERR_ALLOC,
    UNKNOWN
} errno_salgo_e;

// ========================
// --- COMPARISON SORTS ---
// ========================

SALGODEF errno_salgo_e salgo_bubble_sort         (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_cocktail_shaker_sort(void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_comb_sort           (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_comb_11_sort        (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_cycle_sort          (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_gnome_sort          (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_insertion_sort      (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_odd_even_sort       (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_quick_sort          (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));
SALGODEF errno_salgo_e salgo_selection_sort      (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));

// ===================
// --- OTHER SORTS ---
// ===================

SALGODEF errno_salgo_e salgo_bogo_sort           (void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*));

// ==================
// --- MEME SORTS ---
// ==================

SALGODEF errno_salgo_e salgo_stalin_sort         (void* ptr, size_t* count, const size_t size, int (*comp)(const void*, const void*));

#if !defined(SALGO_FREE) && !defined(SALGO_MALLOC)
#   include <stdlib.h>
#   define SALGO_FREE   free
#   define SALGO_MALLOC malloc
#elif !defined(SALGO_FREE) || !defined(SALGO_MALLOC)
#   error "Both SALGO_FREE and SALGO_MALLOC must be defined together"
#endif

#if !defined(SALGO_REALLOC) && !defined(SALGO_NO_MEME_SORTS)
#   include <stdlib.h>
#   define SALGO_REALLOC realloc
#endif

#if !defined(SALGO_RAND)
#   include <stdlib.h>
#   define SALGO_RAND rand
#endif

#endif // SALGO_H_

#ifdef SALGO_IMPLEMENTATION

static inline void salgo__swap(void* a, void* b, void* temp, const size_t size){
    memcpy(temp, b,    size);
    memcpy(b,    a,    size);
    memcpy(a,    temp, size);
}

SALGODEF errno_salgo_e salgo_bubble_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char  temp[size];

    size_t limit = count - 1;

    while(limit > 0){
        size_t new_limit = 0;

        char* a = base;
        char* b = base + size;

        for(size_t j = 0; j < limit; ++j){
            if(comp(a, b) > 0) {
                salgo__swap(a, b, temp, size);
                new_limit = j;
            }

            a = b;
            b += size;
        }

        limit = new_limit;
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_cocktail_shaker_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char temp[size];

    size_t left = 0;
    size_t right = count - 1;

    bool swapped = true;

    while(swapped && left < right){
        for(size_t i = 0; i < right; ++i){
            char* a = base + (i * size);
            char* b = base + ((i + 1) * size);

            if(comp(a, b) > 0) {
                salgo__swap(a, b, temp, size);

                swapped = true;
            }
        }

        if(!swapped) break;
        right--;

        swapped = false;

        for(size_t i = right; i > left; --i){
            char* a = base + ((i - 1) * size);
            char* b = base + (i * size);

            if(comp(a, b) > 0) {
                salgo__swap(a, b, temp, size);

                swapped = true;
            }
        }

        left++;
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_comb_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char temp[size];

    size_t gap = count;
    const float shrink = 1.3f;

    bool swapped = true;

    while(gap > 1 || swapped){

        gap = gap / shrink;

        if(gap == 0){
            gap = 1;
        }

        swapped = false;

        for(size_t i = 0; i + gap < count; ++i){
            char* a = base + (i * size);
            char* b = base + ((i + gap) * size);

            if(comp(a, b) > 0) {
                salgo__swap(a, b, temp, size);

                swapped = true;
            }
        }
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_comb_11_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char temp[size];

    size_t gap = count;
    const float shrink = 1.3f;

    bool swapped = true;

    while(gap > 1 || swapped){

        gap = gap / shrink;

        if(gap == 0){
            gap = 1;
        }else if(gap == 9 || gap == 10){
            gap = 11;
        }

        swapped = false;

        for(size_t i = 0; i + gap < count; ++i){
            char* a = base + (i * size);
            char* b = base + ((i + gap) * size);

            if(comp(a, b) > 0) {
                salgo__swap(a, b, temp, size);

                swapped = true;
            }
        }
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_cycle_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char item[size];
    char temp[size];

    for(size_t cycle_start = 0; cycle_start < count - 1; ++cycle_start){
        memcpy(item, base + (cycle_start * size), size);

        size_t pos = cycle_start;

        for(size_t i = cycle_start + 1; i < count; ++i){
            if(comp(item, base + (i * size)) > 0) pos++;
        }

        if(pos == cycle_start) continue;

        while(comp(item, base + (pos * size)) == 0) pos++;

        char* arr_pos = base + (pos * size);
        salgo__swap(arr_pos, item, temp, size);

        while(pos != cycle_start){
            pos = cycle_start;

            for(size_t i = cycle_start + 1; i < count; ++i){
                if(comp(item, base + (i * size)) > 0) pos++;
            }

            while(comp(item, base + (pos * size)) == 0) pos++;

            arr_pos = base + (pos * size);
            salgo__swap(arr_pos, item, temp, size);
        }
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_gnome_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char temp[size];

    for(size_t i = 1; i < count;){
        if(i == 0){
            ++i;
            continue;
        }

        char* a = base + ((i - 1) * size);
        char* b = base + (i * size);

        if(comp(a, b) > 0){
            salgo__swap(a, b, temp, size);

            --i;
        }else{
            ++i;
        }
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_insertion_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char temp[size];

    for(size_t i = 1; i < count; ++i){
        memcpy(temp, base + (i * size), size);

        size_t j = i;
        for(; j > 0 && comp(temp, base + (j - 1) * size) < 0; --j){
            memcpy(base + (j * size), base + ((j - 1) * size), size);
        }

        memcpy(base + (j * size), temp, size);
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_odd_even_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char temp[size];

    bool sorted = false;

    while(!sorted){
        sorted = true;

        for(size_t i = 1; i < count - 1; i += 2){
            char* a = base + (i * size);
            char* b = base + ((i + 1) * size);

            if(comp(a, b) > 0){
                salgo__swap(a, b, temp, size);

                sorted = false;
            }
        }

        for(size_t i = 0; i < count - 1; i += 2){
            char* a = base + (i * size);
            char* b = base + ((i + 1) * size);

            if(comp(a, b) > 0){
                salgo__swap(a, b, temp, size);
                
                sorted = false;
            }
        }
    }

    return SALGO_ERR_OK;
}


#ifndef SALGO_QUICK_SORT_CUTOFF
#   define SALGO_QUICK_SORT_CUTOFF 12
#endif

static inline void salgo__find_median_pivot(char* base, const size_t count, const size_t size, int (*comp)(const void*, const void*), void* pivot_dest, void* temp){
    char* pivot_left   = base;
    char* pivot_middle = base + ((count / 2) * size);
    char* pivot_right  = base + ((count - 1) * size);

    if(comp(pivot_middle, pivot_left) < 0){
        salgo__swap(pivot_middle, pivot_left, temp, size); 
    }

    if(comp(pivot_right, pivot_middle) < 0){
        salgo__swap(pivot_middle, pivot_right, temp, size);
    
        if(comp(pivot_middle, pivot_left) < 0){
            salgo__swap(pivot_middle, pivot_left, temp, size);
        }
    }

    memcpy(pivot_dest, pivot_middle, size);
}

SALGODEF errno_salgo_e salgo_quick_sort(void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;
    
    char* base = (char*)ptr;

    char temp[size];
    char pivot[size];
    
    if(temp  == NULL
    || pivot == NULL) return SALGO_ERR_ALLOC;
    
    while(count > SALGO_QUICK_SORT_CUTOFF){
        char* lesser_idx  = base;
        char* equal_idx   = base;
        char* greater_idx = base + ((count - 1) * size);

        salgo__find_median_pivot(base, count, size, comp, pivot, temp);

        while(equal_idx <= greater_idx){
            if(comp(equal_idx, pivot) < 0){
                if(equal_idx != lesser_idx){
                    salgo__swap(equal_idx, lesser_idx, temp, size);
                }
                
                lesser_idx += size;
                equal_idx  += size;
            }else if(comp(equal_idx, pivot) > 0){
                if(equal_idx != greater_idx){
                    salgo__swap(equal_idx, greater_idx, temp, size);
                }

                greater_idx -= size;
            }else{
                equal_idx   += size;
            }
        }

        char* right_ptr = greater_idx + size;

        size_t left_count  = (lesser_idx - base) / size;
        size_t right_count = count - ((right_ptr - base) / size);

        if(left_count < right_count){
            errno_salgo_e recur_err = salgo_quick_sort(base, left_count, size, comp);
            if(recur_err) return recur_err;
            
            base = right_ptr;
            count = right_count;
        }else{
            errno_salgo_e recur_err = salgo_quick_sort(right_ptr, right_count, size, comp);
            if(recur_err) return recur_err;

            count = left_count;
        }
    }
    
    if(count > 1){
        char* i_idx = base + size;
        for(size_t i = 1; i < count; ++i){
            memcpy(temp, i_idx, size);

            size_t j = i;
            for(; j > 0 && comp(temp, base + (j - 1) * size) < 0; --j){
                memcpy(base + (j * size), base + ((j - 1) * size), size);
            }

            memcpy(base + (j * size), temp, size);

            i_idx += size;
        }
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_selection_sort(void* ptr, const size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    char* base = (char*)ptr;
    char temp[size];
    
    char* i_idx = base;

    for(size_t i = 0; i < count - 1; ++i){
        char* min = i_idx;
        char* j_idx = i_idx + size;

        for(size_t j = i + 1; j < count; ++j){
            if(comp(min, j_idx) > 0){
                min = j_idx;
            }

            j_idx += size;
        }

        if(min != i_idx){
            salgo__swap(min, i_idx, temp, size);
        }

        i_idx += size;
    }

    return SALGO_ERR_OK;
}

static inline bool salgo__is_sorted(char* base, const size_t count, const size_t size, int (*comp)(const void*, const void*)){

    char* a = base;
    char* b = base + size;

    for(size_t i = 1; i < count; ++i){
        if(comp(a, b) > 0) return false;

        a += size;
        b += size;
    }

    return true;
}

static inline void salgo__shuffle(char* base, const size_t count, const size_t size){
    char temp[size];

    char* i_idx = base;
    char* rand_idx;

    for (size_t i = 0; i < count - 1; ++i) {
        rand_idx = base + ((i + (SALGO_RAND() % (count - i))) * size);
    
        salgo__swap(i_idx, rand_idx, temp, size);
        i_idx += size;
    }
}

SALGODEF errno_salgo_e salgo_bogo_sort(void* ptr, size_t count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || count < 2)    return SALGO_ERR_OK;

    while(!salgo__is_sorted(ptr, count, size, comp)){
        salgo__shuffle(ptr, count, size);
    }

    return SALGO_ERR_OK;
}

SALGODEF errno_salgo_e salgo_stalin_sort(void* ptr, size_t* count, const size_t size, int (*comp)(const void*, const void*)){
    if(ptr  == NULL) return SALGO_ERR_ARR_NULL;
    if(comp == NULL) return SALGO_ERR_COMP_NULL;
    if(size == 0
    || *count < 2)   return SALGO_ERR_OK;

    char* base = (char*)ptr;

    char* left   = base;
    char* middle = left + size;
    char* right  = middle + size;

    for(size_t i = 2; i < *count; ++i){
        if(comp(left, middle) > 0){
            memcpy(middle, right, size);

            right  += size;
        }else{
            left   += size;
            middle += size;
            right  += size;
        }
    }

    if(comp(left, middle) <= 0){
        middle += size;
    }

    *count = (middle - base) / size;

    ptr = SALGO_REALLOC(ptr, (middle - base));
    if(ptr == NULL) return SALGO_ERR_ALLOC;

    return SALGO_ERR_OK;
}

#endif // SALGO_IMPLEMENTATION