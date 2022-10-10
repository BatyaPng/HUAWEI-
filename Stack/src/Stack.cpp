#include "stack.hpp"

#define POISON NAN
#define POISON_SPECIFIER "lg"

int StackCtor(Stack *stk, elem_t value) {
    ASSERTED(NP, stk, NULL, 1);

    stk->data = (elem_t *) calloc(FIRST_CAPACITY_STACK, sizeof(elem_t));
    ASSERTED(calloc, stk->data, NULL, 2);

    for (int i = 0; i < FIRST_CAPACITY_STACK; i++) {
        stk->data[i] = POISON;
    }

    stk->capacity = FIRST_CAPACITY_STACK;
    stk->size++;
    stk->data[0] = value;

    return 0;
}

int StackDtor(Stack *stk) {
    // StackVerify;

    for (size_t i = 0; i < stk->capacity; i++) {
        stk->data[i] = POISON;
    }

    free(stk->data);

    return 0;
}

static int StackResize(Stack *stk, MODE_STACK_RESIZE mode) {
    // StackVerify;

    void *tmpBuf = NULL;
    
    if (mode == UP) {
        tmpBuf = realloc(stk->data, 2 * (stk->capacity) * sizeof(elem_t));
        ASSERTED(realloc, tmpBuf, NULL, 1);

        stk->data = (elem_t *) tmpBuf;
        stk->capacity *= 2;
    } else {
        tmpBuf = realloc(stk->data, (stk->capacity)  / 2 * sizeof(elem_t));
        ASSERTED(realloc, tmpBuf, NULL, 1);

        stk->data = (elem_t *) tmpBuf;
        stk->capacity /= 2;
    }

    for (size_t i = stk->size; i < stk->capacity; i++) {
        (stk->data)[i] = POISON;
    }

    return 0;     
}

int StackPush(Stack *stk, elem_t value) {
    // StackVerify

    if (stk->size == stk->capacity) {
        int result = StackResize(stk, UP);
        ASSERTED(StackResize, result, 1, 1);
    }

    stk->data[stk->size] = value;
    stk->size++;

    return 0;
}

int StackPop(Stack *stk, elem_t *var) {
    // StackVerify

    if (stk->size != 0) {
        *var = stk->data[stk->size - 1];
        stk->size--;

        if ((stk->size != 0 ) && (stk->capacity / stk->size == 4)) {
            StackResize(stk, DOWN);
        }
    } else {
        return 1;
    }

    return 0;
}

int StackDump(const Stack stk) {
    // StackVerify

    printf("================================================================\n");

    printf("size = %lu\n", stk.size);
    printf("capacity = %lu\n", stk.capacity);
    printf("\ndata[%p] {\n", (void *) &stk.data);

    for (size_t i = 0; i < stk.size; i++) {
        printf("    [%lu] = %" POISON_SPECIFIER "\n", i, stk.data[i]);
    }

    for (size_t i = stk.size; i < stk.capacity; i++) {
        printf("    [%lu] = POISON\n", i);
    }

    printf("}\n");
    printf("================================================================\n");

    return 0;    
}