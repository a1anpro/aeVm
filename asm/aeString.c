#include "aeString.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct _aeString {
    char *raw;
    size_t length;
};

aeString
aeStringNewWithChars(const char *raw) {
    size_t len = strlen(raw);
    // copy string
    char *new = malloc(len + 1);
    strncpy(new, raw, len + 1);
    
    aeString s = malloc(sizeof(struct _aeString));
    s->raw = new;
    s->length = len;
    return s;
}

size_t
aeStringLength(const aeString s) {
    return s->length;
}

aeString
aeStringConcat(const aeString s1, const aeString s2) {
    size_t len1 = aeStringLength(s1);
    size_t len2 = aeStringLength(s2);
    size_t len = len1 + len2;
    char *new = malloc(len + 1);
    // 先复制 再cat
    strncpy(new, aeStringCString(s1), len1);
    strncat(new, aeStringCString(s2), len2);
    
    aeString s = malloc(sizeof(struct _aeString));
    s->raw = new;
    s->length = len;
    return s;
}

const char *
aeStringCString(const aeString s) {
    return s->raw;
}

void
aeStringLog(const aeString s) {
    aeLog("aeString<%s>\n", aeStringCString(s));
}

bool
aeStringEqual(aeString a, aeString b) {
    char* as = a->raw;
    char* bs = b->raw;
    return strcmp(as, bs) == 0;
}

bool
aeStringEqualWithCString(aeString a, char *b) {
    char* as = a->raw;
    return strcmp(as, b) == 0;
}

bool
aeStringIsNumber(aeString str) {
    const char *temp = aeStringCString(str);
    size_t len = aeStringLength(str);
    int i;
    for (i = 0; i < len; i += 1) {
        char c = temp[i];
        if (!isdigit(c)) {
            return FALSE;
        }
    }
    return TRUE;
}

int
aeStringToInt(aeString str) {
    aeAssert(aeStringIsNumber(str) == TRUE, "aeStringToInt: str is not number");
    const char *temp = aeStringCString(str);
    return atoi(temp);
}

unsigned char
aeStringToByte(aeString str) {
    aeAssert(aeStringIsNumber(str) == TRUE, "aeStringToByte: str is not number");
    const char *temp = aeStringCString(str);
    return atoi(temp);
}


bool
aeStringIsEmpty(aeString s) {
    return s->length == 0;
}

aeString
aeStringSlice(aeString s, size_t begin, size_t end) {
    size_t len = s->length;
    aeAssert(begin >= 0 && end <= len, "aeStringSlice: slice out of range");
    const char *str = s->raw;

    char *result = (char *)malloc(end - begin + 1);
    int i;
    for (i = 0; i < end - begin; ++i) {
        result[i] = str[i + begin];
    }
    result[end - begin] = '\0';

    aeString res = malloc(sizeof(struct _aeString));
    size_t length = end - begin;
    res->length = length;
    res->raw = result;

    return res;
}

char
aeStringCharAt(aeString s, size_t index) {
    size_t len = s->length;
    aeAssert(index >= 0 && index < len, "aeStringCharAt: index out of range");
    return s->raw[index];
}


aeString
aeStringLeftTrim(aeString s) {
    size_t len = s->length;
    size_t i;
    for (i = 0; i < len; i++) {
        char c = aeStringCharAt(s, i);
        // isspace是库函数
        if (isspace(c) == 0) {
            return aeStringSlice(s, i, len);
        }
    }
    return s;
}

aeString
aeStringRightTrim(aeString s) {
    size_t len = s->length;
    size_t i;
    for (i = len - 1; i >= 0; i--) {
        char c = aeStringCharAt(s, i);
        if (isspace(c) == 0) {
            return aeStringSlice(s, 0, i + 1);
        }
    }
    return s;
}

aeString
aeStringTrim(aeString s) {
    aeString output = aeStringRightTrim(aeStringLeftTrim(s));
    return output;
}

bool
aeStringIsEmptyContent(aeString s) {
    size_t len = aeStringLength(s);
    size_t i;
    for (i = 0; i < len; i += 1) {
        char c = aeStringCharAt(s, i);
        if (!isspace(c)) {
            return FALSE;
        }
    }
    return TRUE;
}

aeList
aeStringSplit(aeString s, char *sep) {
    aeList list = aeListNew();
    char *result = NULL;
    result = strtok(s->raw, sep);
    while (result != NULL) {
        aeString temp = aeStringNewWithChars(result);
        aeListAdd(list, temp);
        result = strtok(NULL, sep);
    }
    return list;
}

aeList
aeStringSplitSpace(aeString s, char *sep) {
    aeList list = aeListNew();
    char *result = NULL;
    result = strtok(s->raw, sep);
    while (result != NULL) {
        aeString temp = aeStringNewWithChars(result);
        temp = aeStringTrim(temp);
        aeListAdd(list, temp);
        result = strtok(NULL, sep);
    }
    return list;
}

int
aeStringIndexOf(aeString str, aeString flag) {
    // 找到子串位置
    size_t len1 = aeStringLength(str);
    size_t len2 = aeStringLength(flag);

    int index = 0;
    int i;
    for (i = 0; i < len1; ++i) {
        char c1 = aeStringCharAt(str, i);
        char c2 = aeStringCharAt(flag, index);
        
        if (c1 == c2) {
            if (index == len2 -1) {
                return i - index;
            } else {
                index += 1;
            }
        } else {
            index = 0;
        }
    }
    return -1;
}

