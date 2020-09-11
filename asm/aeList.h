#ifndef aeList_h
#define aeList_h

#include <stdio.h>
#include <stdbool.h>
typedef struct _aeList *aeList;

aeList
aeListNew(void);

size_t
aeListLength(const aeList list);

void
aeListAdd(aeList list, void *item);

void *
aeListRemove(aeList list, size_t index);

void *
aeListPop(aeList list);

void *
aeListGetItem(const aeList list, size_t index);

void
aeListSetItem(aeList list, size_t index, void *item);

aeList
aeListCut(aeList list, size_t begin, size_t end);

bool
aeListEqual(aeList l1, aeList l2);

aeList
aeListConcat(aeList l1, aeList l2);

#endif /* aeList_h */
