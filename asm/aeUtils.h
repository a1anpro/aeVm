#ifndef aeUtils_h
#define aeUtils_h

#include <stdio.h>
#include <stdbool.h>
#include "aeString.h"
#include "aeList.h"
#include <assert.h>

#define _GuaStringMulti(s) #s
#define GuaStringMulti(s) _GuaStringMulti(s)

#define aeLog(M, ...) fprintf(stderr, "[aeLog] (%s:%d): " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define aeAssert(CONDITION, MESSAGGE)                                                        \
do {                                                                                           \
if (!(CONDITION)) {                                                                        \
aeLog(MESSAGGE);                                                                       \
}                                                                                          \
assert(CONDITION);                                                                         \
} while (0)


void
ensure(bool condition, const char *msg);

// 把数据拆分成高低位
aeList
utilApartData(int data);

// 把low和high组成16位
int
utilCombineData(unsigned char low, unsigned char high);

// 还没写
void
splitToTokens(char sourceCode[], char asmCode[][15]);

// 输出aeString的list
void
utilLogStringList(aeList list);

void
utilLogIntList(aeList list);

void
utilLogUnsignedList(aeList list);

// 根据c数组 得到list
aeList
utilGetListByCArray(int *data, int len);

// 根据个数生成list, 初始值为0
aeList
utilGetListByLen(size_t len);

void
utilFillList(aeList list, size_t size);

#endif /* utils_h */
