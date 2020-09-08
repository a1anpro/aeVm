#include "aeString.h"
#include "aeList.h"
#include "aeUtils.h"
#include "test.h"
#include "vmTest.h"
#include "aeMap.h"
#include "asm.h"

void
listTest() {
    aeList list = aeListNew();
    aeString s1 = aeStringNewWithChars("Hello");
    aeString s2 = aeStringNewWithChars("gua");
    aeString s3 = aeStringNewWithChars("axe1");
    aeString s4 = aeStringNewWithChars("axe2");
    aeString s5 = aeStringNewWithChars("axe3");
    
    aeListAdd(list, (void*)s1);
    aeListAdd(list, (void*)s2);
    aeListAdd(list, (void*)s3);
    aeListAdd(list, (void*)s4);
    aeListAdd(list, (void*)s5);
    
    aeListSetItem(list, 1, aeStringNewWithChars("123"));
    utilLogStringList(list);
}

void
stringTest() {
    char t[] = GuaStringMulti(
                              set1 a1 1\n
                              );
    aeString as = aeStringNewWithChars(t);
    
    aeList list = aeStringSplit(as, " ");
    
    // 输出字符串数组
    utilLogStringList(list);
    
    run(list);
    
    // trim测试
    //    aeString as = aeStringNewWithChars("     1123123123   ");
    //    aeString output = aeStringTrim(as);
    //    aeStringLog(output);
    
    // 判断string内容为空
    //    aeString as = aeStringNewWithChars("  x   \n  \t\n   ");
    //    aeLog("(%d)", aeStringIsEmptyContent(as));
    
    
    //    int d = aeStringToInt(as);
    //    aeLog("(%d)\n", d);
    //    aeLog("(%d)\n", aeStringIsNumber(as));
}

void
testIndexOf() {
    aeString str = aeStringNewWithChars("12345");
    aeString flag = aeStringNewWithChars("1x");
    
    int index = aeStringIndexOf(str, flag);
    printf("%d\n", index);
}

void
testAeStringSplit1() {
    aeString s = aeStringNewWithChars("set a1 1");
    aeList list = aeStringSplit(s, " ");
    ensure(aeListLength(list) == 3, "test aeString split case1");
    for (size_t i = 0; i < aeListLength(list); i++) {
        aeString s = aeListGetItem(list, i);
        aeStringLog(s);
    }
    
    ensure(aeStringEqualWithCString(aeListGetItem(list, 0), "set"), "test aeString split case2");
    ensure(aeStringEqualWithCString(aeListGetItem(list, 1), "a1"), "test aeString split case3");
    ensure(aeStringEqualWithCString(aeListGetItem(list, 2), "1"), "test aeString split case4");
}

void
testAeStringSplit2() {
    aeString s = aeStringNewWithChars("set a1 1\nset a2 2");
    aeList list = aeStringSplit(s, "\n");
    ensure(aeListLength(list) == 2, "test aeString split case1");
    for (size_t i = 0; i < aeListLength(list); i++) {
        aeString s = aeListGetItem(list, i);
        aeStringLog(s);
    }
    
    ensure(aeStringEqualWithCString(aeListGetItem(list, 0), "set a1 1"),
           "test aeString split case2");
    ensure(aeStringEqualWithCString(aeListGetItem(list, 1), "set a2 2"),
           "test aeString split case3");
}

int main() {
    // test();
    vmTest();

    return 0;
}
