#include "aeMap.h"

struct _aeMap {
    aeString labels[1000];
    int position[1000];
    int len;
};

int
aeMapGetLen(aeMap map) {
    return map->len;
}

aeMap
aeMapNew(void) {
    aeMap map = malloc(sizeof(struct _aeMap));
    map->len = 0;
    return map;
}

int
_findKey(aeMap map, aeString label) {
    int len = map->len;
    for (int i = 0; i < len; i++) {
        aeString tmp = map->labels[i];
        if (aeStringEqual(tmp, label)) {
            return i;
        }
    }
    // -1 表示没找到
    return -1;
}

void
aeMapPut(aeMap map, aeString label, int position) {
    int findIndex = _findKey(map, label);
    if (findIndex != -1) {
        // 发现有key存在,覆盖掉原来的值
        map->position[findIndex] = position;
        return;
    }
    // 这里的操作应该封装
    size_t labelLen = aeStringLength(label);
    // 申请内存
    map->labels[map->len] = malloc(sizeof(void*) * labelLen);
    map->labels[map->len] = label;
    // printf("put key\n");
    map->position[map->len] = position;
    int size = map->len + 1;
    map->len = size;
}

int
aeMapGet(aeMap map, aeString label) {
    int index = _findKey(map, label);
    if (index == -1) {
        return -1;
    }
    return map->position[index];
}

int
aeMapGetByIndex(aeMap map, size_t index) {
    return map->position[index];
}

size_t
aeMapKeyLength(aeMap map) {
    return map->len;
}

aeList
aeMapKeys(aeMap map) {
    aeList list = aeListNew();
    int i;
    for (i = 0; i < map->len; ++i) {
        aeString temp = map->labels[i];
        aeListAdd(list, temp);
    }
    return list;
}


void
aeMapLogKeys(aeMap map) {
    int i;
    aeLog("Map Keys:\n");
    for (i = 0; i < map->len; ++i) {
        aeString temp = map->labels[i];
        aeStringLog(temp);
    }
}

void
aeMapLogValues(aeMap map) {
    int i;
    aeLog("Map Keys:\n");
    for (i = 0; i < map->len; ++i) {
        aeLog("%d\n", map->position[i]);
    }
}

void 
aeMapLogItems(aeMap map) {
    int i;
    for (i = 0; i < map->len; ++i) {
        aeString temp = map->labels[i];
        const char *s = aeStringCString(temp);
        
        aeLog("(%s)=(%d)\n", s, map->position[i]);
    }
}