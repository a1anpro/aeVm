#include "aeList.h"
#include "aeUtils.h"
#include <stdlib.h>
#include <stdio.h>

struct _aeList {
    void **buffer;
    size_t length;
    size_t capacity;
};


aeList
aeListNew(void) {
    aeList list = malloc(sizeof(struct _aeList));
    list->length = 0;
    // 分配初始容量
    size_t capacity = 16;
    list->capacity = capacity;
    list->buffer = malloc(capacity * sizeof(void *));
    return list;
    
}

size_t
aeListLength(const aeList list) {
    return list->length;
}

void
aeListAdd(aeList list, void *item) {
    if (list->length == list->capacity) {
        size_t new_capacity = list->capacity * 2;
        list->buffer = realloc(list->buffer, new_capacity * sizeof(void *));
        list->capacity = new_capacity;
    }
    list->buffer[list->length++] = item;
}

void *
aeListPop(aeList list) {
    aeAssert(list->length > 0, "aeList: pop from empty list");
    // 先不管容量收缩
    list->length -= 1;
    return list->buffer[list->length];
}

void *
aeListGetItem(const aeList list, size_t index) {
    aeAssert(index < list->length, "aeList: index out of range");
    return list->buffer[index];
}

void
aeListSetItem(aeList list, size_t index, void *item) {
    aeAssert(index < list->length, "aeList: index out of range");
    list->buffer[index] = item;
}

aeList
aeListCut(aeList list, size_t begin, size_t end) {
//    aeLog("长度=(%d)\n", list->length);
    aeAssert(begin >= 0 && end <= list->length, "aeListCut: cut out of range");
    aeList output = aeListNew();
    size_t i;
    for (i = begin; i < end; i += 1) {
        void* element = aeListGetItem(list, i);
        aeListAdd(output, element);
    }
    return output;
}

bool
aeListEqual(aeList l1, aeList l2) {
    if (aeListLength(l1) != aeListLength(l2)) {
        return FALSE;
    }
    size_t len = aeListLength(l1);
    for (size_t i = 0; i < len; i++) {
        void *a = aeListGetItem(l1, i);
        void *b = aeListGetItem(l2, i);
        if (a != b) {
            return FALSE;
        }
    }
    return TRUE;
}

aeList
aeListConcat(aeList l1, aeList l2) {
    size_t i;
    size_t len = aeListLength(l2);
    for (i = 0; i < len; ++i) {
        aeListAdd(l1, aeListGetItem(l2, i));
    }
    return l1;
}