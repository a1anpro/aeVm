#ifndef aeMap_h
#define aeMap_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aeString.h"
#include "aeList.h"
#include "aeUtils.h"

typedef struct _aeMap *aeMap;

aeMap
aeMapNew(void);

void
aeMapPut(aeMap map, aeString label, int position);

int
aeMapGet(aeMap map, aeString label);

int
aeMapGetLen(aeMap map);

int
aeMapGetByIndex(aeMap map, size_t index);

aeList
aeMapKeys(aeMap map);

size_t
aeMapKeyLength(aeMap map);

void
aeMapLogKeys(aeMap map);

void
aeMapLogValues(aeMap map);

void 
aeMapLogItems(aeMap map);


#endif /* aeMap_h */
