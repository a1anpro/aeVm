#include "asmTest.h"

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
void 
testHalt(void) {
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


void 
asmTest(){
    aeLog("开始测试:\n");
    testSet();
    testSet2();
    testLoad();
    testLoad2();
    testAdd();
    testSave();
    testSave2();
    testCompare();
    testJumpIfLess();
    testJump();
    testSaveFromRegister();
    testSet2Load2Save2();
    testSubtract2();
    testLoadFromRegister();
    testLoadFromRegister2();
    testSaveFromRegister2();
    testSubtract2Jump();
    testJumpFromRegister();
}

void testSet() {
    // 用字符串切分测试，这样就可以切分源码
    char t[] = GuaStringMulti(
                              set a1 288\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    //    utilLogStringList(list);
    
    aeList result = run(list);
    
    int a[] = {
        0,
        16,
        32
    };
    aeList expected = _getExpectedList(a, 3);
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test set 失败");
}

void testSet2() {
    char t[] = GuaStringMulti(
                              set2 a1 288\n);
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");    
    aeList result = run(list);
    
    int a[] = {8, 16, 32, 1};
    aeList expected = _getExpectedList(a, 4);
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test set2 失败");
}

void testLoad() {
    char t[] = GuaStringMulti(
                        set a1 288\n
                        load @258 a1\n
                              );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 32,
        1, 2, 16
    };
    aeList expected = _getExpectedList(a, 6);
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test load 失败");
}

void testLoad2() {
    // 0xbaef是可以存的
    char t[] = GuaStringMulti(
                    set a1 288\n
                    load2 @258 a1\n
                              );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 32,
        9, 2, 1, 16
    };
    aeList expected = _getExpectedList(a, 7);
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test load2 失败");
}

void testAdd() {
    char t[] = GuaStringMulti(
                    set a1 1\n
                    set a2 2\n
                    add a1 a2 a1\n
                              );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        0, 32, 2,
        2, 16, 32, 16
    };
    aeList expected = _getExpectedList(a, 10);
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test add 失败");
}

void testSave() {
    char t[] = GuaStringMulti(
        set a1 1\n
        save a1 @258\n
        save a1 @387\n
        save a1 @128\n
    );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    // 一定要查看自己的方法调用是否正确
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        3, 16, 2,
        3, 16, 131,
        3, 16, 128
    };
    aeList expected = _getExpectedList(a, 12);
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test save 失败");
}

void testSave2() {
    char t[] = GuaStringMulti(
        set2 a1 258\n
        save2 a1 @258\n
        save2 a1 @387\n
        save2 a1 @128\n
    );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    // 一定要查看自己的方法调用是否正确
    aeList result = run(list);
    
    int a[] = {
        8, 16, 2, 1,
        11, 16, 2, 1,
        11, 16, 131, 1,
        11, 16, 128, 0
    };
    aeList expected = _getExpectedList(a, 16);
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test save2 失败");
}

void testCompare() {
    char t[] = GuaStringMulti(
                    set a1 1\n
                    set a2 2\n
                    compare a1 a2\n
                              );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        0, 32, 2,
        4, 16, 32
    };
    aeList expected = _getExpectedList(a, 9);
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test compare 失败");
}

void testJumpIfLess() {
    char t[] = GuaStringMulti(
                    set2 a1 288\n
                    set2 a2 157\n
                    compare a1 a2\n
                    jump_if_less @label1\n
                    set2 a1 100\n
                    @label1\n

                    halt\n
                            );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        8, 16, 32, 1,
        8, 32, 157, 0,
        4, 16, 32,
        5, 18, 0,
        8, 16, 100, 0,
        255
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    ensure(aeListEqual(result, expected), "test jump_if_less 失败");
}

void testJump() {
    char t[] = GuaStringMulti(
                        set a1 1\n
                        set a2 2\n
                        jump @label1\n

                        @label1\n
                        set2 a1 10\n
                            );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        0, 32, 2,
        6, 9, 0,
        8, 16, 10, 0
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    ensure(aeListEqual(result, expected), "test jump 失败");
}

void testSaveFromRegister(void) {
    char t[] = GuaStringMulti(
                    set a1 1\n
                    set a2 2\n
                    save_from_register a1 a2\n
                             );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        0, 32, 2,
        7, 16, 32
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test save_from_register 失败");
}

void testSet2Load2Save2(void) {
    char t[] = GuaStringMulti(
                    set2 a1 288\n
                    load2 @599 a1\n
                    save2 a1 @777\n
                             );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        8, 16, 32, 1,
        9, 87, 2, 16,
        11, 16, 9, 3
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test testSet2Load2Save2 失败");
}

void testLoadLoad2(void) {
    char t[] = GuaStringMulti(
                    load @100 a1\n
                    load2 @100 a2\n
                             );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        1, 100, 16,
        9, 100, 0, 32
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test testLoadLoad2 失败");
}

void testSubtract2(void) {
    char t[] = GuaStringMulti(
                set a1 1\n
                set a2 2\n
                subtract2 f1 a3 f1\n
                             );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        0, 32, 2,
        12, 80, 48, 80
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test testSubtract2 失败");
}

void
testLoadFromRegister(void) {
    // TODO: 拿到result的可以抽象成函数
    char t[] = GuaStringMulti(
                    set a1 1
                    set a2 2
                    load_from_register f1 a2
                );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        0, 32, 2,
        13, 80, 32
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test testLoadFromRegister 失败");
}

void
testLoadFromRegister2(void) {
    char t[] = GuaStringMulti(
                    set a1 1\n
                    set a2 2\n
                    load_from_register f1 a2\n
                    load_from_register2 f1 a2\n
                );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        0, 32, 2,
        13, 80, 32,
        14, 80, 32
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test testLoadFromRegister2 失败");
}

void
testSaveFromRegister2(void) {
    char t[] = GuaStringMulti(
                    set2 a1 1\n
                    set2 a2 2\n
                    save_from_register2 a1 a2\n
                );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        8, 16, 1, 0,
        8, 32, 2, 0,
        15, 16, 32
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test testSaveFromRegister2 失败");

}

void
testSubtract2Jump(void) {
    char t[] = GuaStringMulti(
                    set2 a1 1\n
                    set a2 2\n
                    subtract2 a1 a2 a1\n
                    compare a1 a2\n
                    jump @label1\n

                    @label1\n
                    halt\n
                );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        8, 16, 1, 0,
        0, 32, 2,
        12, 16, 32, 16,
        4, 16, 32,
        6, 17, 0,
        255
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test testSubtract2Jump 失败");

}

void
testJumpFromRegister(void) {
        char t[] = GuaStringMulti(
                    set a1 1\n
                    set a2 2\n
                    jump_from_register a1\n
                );
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    
    aeList result = run(list);
    
    int a[] = {
        0, 16, 1,
        0, 32, 2,
        16, 16
    };
    aeList expected = _getExpectedList(a, sizeof(a)/sizeof(int));
    
    // ensure方法
    ensure(aeListEqual(result, expected), "test testJumpFromRegister 失败");

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
