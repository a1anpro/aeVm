#include "test.h"

void test(){
    // testCode();
    // testHalt();
    // testSaveFromRegister();
    // testSet();
    // testSet2();
    // testLoad();
    // testAdd();
    // testSave();

    // testCompare();
    // testLabel();
    // testJump();
    // testJump1();
    // testJumpIfGreat();

    testMap();
    // testMap2();
}

// 参数1：期望输出int数组，参数2：长度
aeList
_getExpectedList(int *data, int len)
{
    int i;
    aeList expected = aeListNew();

    for (i = 0; i < len; ++i)
    {
        int d = data[i];
        aeListAdd(expected, (void *)d);
    }
    return expected;
}

void testLabel()
{
    char t[] = GuaStringMulti(
        @labelxx\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    aeList result = run(list);
}

// 测试完整代码
void
testCode(void) {
    char t[] = GuaStringMulti(
        set a2 10\n
        save a1 @100\n
        load @101 f1\n
        add a1 a3 pa\n
        save_from_register a1 a2\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    // utilLogStringList(list);
    // utilLogIntList(result);

    int a[] = {
        0, 32, 10,
        3, 16, 100,
        1, 101, 80,
        2, 16, 48, 0,
        7, 16, 32,
    };
    aeList expected = _getExpectedList(a, 16);

    // ensure方法
    ensure(aeListEqual(result, expected), "test code 失败");

}
void testHalt(void) {
    char t[] = GuaStringMulti(
        halt\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    aeList result = run(list);

    int a[] = {
        255
    };
    aeList expected = _getExpectedList(a, 1);

    // ensure方法
    ensure(aeListEqual(result, expected), "test halt 失败");
}

void testSaveFromRegister(void) {
    char t[] = GuaStringMulti(
        save_from_register a1 a2\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    aeList result = run(list);

    int a[] = {
        7, 
        16,
        32,
    };
    aeList expected = _getExpectedList(a, 3);

    // ensure方法
    ensure(aeListEqual(result, expected), "test save_from_register 失败");
}


void testSet() {
    // 用字符串切分测试，这样就可以切分源码
    char t[] = GuaStringMulti(
        set a1 1\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    //    utilLogStringList(list);

    aeList result = run(list);

    int a[] = {
        0,
        16,
        1,
    };
    aeList expected = _getExpectedList(a, 3);

    // ensure方法
    ensure(aeListEqual(result, expected), "test set 失败");
}

void testSet2() {
    char t[] = GuaStringMulti(
        set2 a2 257\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    //    utilLogStringList(list);

    aeList result = run(list);

    int a[] = {8, 32, 1, 0};
    aeList expected = _getExpectedList(a, 4);

    // ensure方法
    ensure(aeListEqual(result, expected), "test set2 失败");
}

void testLoad() {
    char t[] = GuaStringMulti(
        load @100 a1\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    aeList result = run(list);

    int a[] = {1, 100, 16};
    aeList expected = _getExpectedList(a, 3);

    // ensure方法
    ensure(aeListEqual(result, expected), "test load 失败");
}

void testAdd() {
    char t[] = GuaStringMulti(
        add a1 a2 a3\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    aeList result = run(list);

    int a[] = {2, 16, 32, 48};
    aeList expected = _getExpectedList(a, 4);

    // ensure方法
    ensure(aeListEqual(result, expected), "test add 失败");
}

void testSave() {
    char t[] = GuaStringMulti(
        save a1 @100\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    // 一定要查看自己的方法调用是否正确
    aeList result = run(list);

    int a[] = {3, 16, 100};
    aeList expected = _getExpectedList(a, 3);

    // ensure方法
    ensure(aeListEqual(result, expected), "test save 失败");
}

void testCompare() {
    char t[] = GuaStringMulti(
        compare a1 a2\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    aeList result = run(list);

    int a[] = {4, 16, 32};
    aeList expected = _getExpectedList(a, 3);

    // ensure方法
    ensure(aeListEqual(result, expected), "test compare 失败");
}

void testJumpIfGreat() {
    char t[] = GuaStringMulti(
        jump_if_great @label1\n
        @label1\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    aeList result = run(list);

    int a[] = {5, 3, 0};
    aeList expected = _getExpectedList(a, 3);

    ensure(aeListEqual(result, expected), "test jump_if_great 失败");
}

void testJump() {
    char t[] = GuaStringMulti(
        jump @label1\n
        @label1\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    aeList result = run(list);

    int a[] = {6, 3, 0};
    aeList expected = _getExpectedList(a, 3);

    ensure(aeListEqual(result, expected), "test jump 失败");
}

void testJump1() {
    char t[] = GuaStringMulti(
        jump @258\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");

    aeList result = run(list);

    int a[] = {6, 2, 0};
    aeList expected = _getExpectedList(a, 3);

    ensure(aeListEqual(result, expected), "test jump1 失败");
}

void testMap() {
    aeMap map = aeMapNew();
    aeMapPut(map, aeStringNewWithChars("label1"), 3);
    aeMapPut(map, aeStringNewWithChars("label1"), 6);
    int val = aeMapGet(map, aeStringNewWithChars("label1"));
    int except = 6;
    if (val == except)
    {
        aeLog("map 测试成功\n");
    }
}

void testMap2() {
    aeMap map = aeMapNew();
    aeMapPut(map, aeStringNewWithChars("label1"), 3);
    aeMapPut(map, aeStringNewWithChars("label1"), 6);
    int val = aeMapGet(map, aeStringNewWithChars("label1"));
    int except = 6;
    if (val == except)
    {
        aeLog("map 覆盖 测试成功\n");
    }
}
