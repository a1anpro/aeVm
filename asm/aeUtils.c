#include <stdlib.h>
#include <string.h>
#include "aeUtils.h"

void
ensure(bool condition, const char *msg) {
    if (!condition) {
        printf("%s\n", msg);
    } else {
        printf("测试成功\n");
    }
    return;
}

aeList
utilApartData(int data) {
    aeList output = aeListNew();
    // 将data分成高8位和低8位
    unsigned char low = data & 0xff;
    unsigned char high = data >> 8 & 0xff;
    aeListAdd(output, low);
    aeListAdd(output, high);
    return output;
}

// 输出string的list
void
utilLogStringList(aeList list) {
    size_t len = aeListLength(list);
    size_t i;
    for (i = 0; i < len; i += 1) {
        aeString element = aeListGetItem(list, i);
        aeStringLog(element);
    }
}


void
utilLogIntList(aeList list) {
    size_t len = aeListLength(list);
    size_t i;
    for (i = 0; i < len; i += 1) {
        int element = (int)aeListGetItem(list, i);
        printf("index:%zu,value:%d\n", i, element);
    }
}

void
utilLogUnsignedList(aeList list) {
    size_t len = aeListLength(list);
    size_t i;
    for (i = 0; i < len; i += 1) {
        unsigned char element = (unsigned char)aeListGetItem(list, i);
        printf("index:%zu,value:%d\n", i, element);
    }
}

// 根据c数组 得到list
aeList
utilGetListByCArray(int *data, int len) {
    int i;
    aeList expected = aeListNew();

    for (i = 0; i < len; ++i)
    {
        int d = data[i];
        aeListAdd(expected, (void *)d);
    }
    return expected;
}