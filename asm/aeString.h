#ifndef aeString_h
#define aeString_h


#include "aeList.h"
#include "aeUtils.h"

#define TRUE 1
#define FALSE 0


typedef struct _aeString *aeString;

aeString
aeStringNewWithChars(const char *raw);

size_t
aeStringLength(const aeString s);

const char *
aeStringCString(const aeString s);

aeString
aeStringConcat(const aeString s1, const aeString s2);

void
aeStringLog(const aeString s);

bool
aeStringEqual(aeString a, aeString b);

bool
aeStringEqualWithCString(aeString a, char *b);

bool
aeStringIsNumber(aeString str);

int
aeStringToInt(aeString str);

unsigned char
aeStringToByte(aeString str);

bool
aeStringIsEmpty(aeString s);

aeString
aeStringSlice(aeString s, size_t begin, size_t end);

char
aeStringCharAt(aeString s, size_t index);

aeString
aeStringLeftTrim(aeString s);

aeString
aeStringRightTrim(aeString s);

aeString
aeStringTrim(aeString s);

// string内容为空白 \t \n k空格
bool
aeStringIsEmptyContent(aeString s);

aeList
aeStringSplit(aeString s, char *sep);

aeList
aeStringSplitSpace(aeString s, char *sep);

int
aeStringIndexOf(aeString str, aeString flag);

#endif /* aeString_h */
