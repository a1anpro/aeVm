#include "vmTest.h"

void 
vmTest(){
    // vmTestSetAdd();
    // vmTestLoadSave();
    // vmTestCompare();
    // vmTestDraw();

    // 乘法
    vmTestMultiply();

    // vmTestSet();
    // vmTestSet2();
    // vmTestSave();
    // vmTestSave2();
    // vmTestLoad();

    // vmTestLoad2();
    // vmTestAdd2();
    // vmTestSubtract2();

    // vmTestSaveFromRegister();
    // vmTestSaveFromRegister2();
    // vmTestLoadFromRegister();
    // vmTestLoadFromRegister2();
    // vmTestJumpIfLess();
    // vmTestJump();
    // vmTestJumpFromRegister();
}

aeList 
vmTestDraw() {
    int mcode[] = {
        0b00000000, // set
        0b00100000, // a2
        0b10011100, // 156, 左上角第一个像素

        0b00000000, // set
        0b00110000, // a3
        0b00010110, // 22, 用于斜方向设置像素，每两排设置一个

        0b00000000, // set
        0b00010000, // a1
        0b11000011, // 红色，我们用一字节表示 RGBA 颜色，所以这里红色就是 11000011
        
        0b00000111, // save_from_register
        0b00010000, // a1
        0b00100000, // a2

        // 设置新像素点
        0b00000010, // add
        0b00100000, // a2
        0b00110000, // a3
        0b00100000, // a2

        0b00000111, // save_from_register
        0b00010000, // a1
        0b00100000, // a2

        // 设置新像素点
        0b00000010, // add
        0b00100000, // a2
        0b00110000, // a3
        0b00100000, // a2

        0b00000111, // save_from_register
        0b00010000, // a1
        0b00100000, // a2

        // 设置新像素点
        0b00000010, // add
        0b00100000, // a2
        0b00110000, // a3
        0b00100000, // a2

        0b00000111, // save_from_register
        0b00010000, // a1
        0b00100000, // a2

        // 设置新像素点
        0b00000010, // add
        0b00100000, // a2
        0b00110000, // a3
        0b00100000, // a2

        0b00000111, // save_from_register
        0b00010000, // a1
        0b00100000, // a2

        // 结果是在显示屏上显示一条斜线，一共 5 个红色的像素点
        0b11111111, // 停机
    };
    axeVm vm = axeVmNew();
    aeList machineCode = utilGetListByCArray(mcode, sizeof(mcode)/sizeof(int));
    runVm(vm, machineCode);
    
    // 显存
    aeList mem = axeVmSliceMemory(vm, 0, 256);
    return mem;
}

void 
vmTestCompare() {
    axeVm vm = axeVmNew();

    int mcode[] = {
        0b00000000, // set a1 10
        0b00010000, 
        10,
        0b00000000, // set a2 20
        0b00100000,
        20,
        0b00000100,
        0b00010000,
        0b00100000,

        0b11111111,  // halt
    };
    aeList machineCode = utilGetListByCArray(mcode, sizeof(mcode)/sizeof(int));
    runVm(vm, machineCode);

    // 对比寄存器的值 
    aeString reg = aeStringNewWithChars("c1");
    int regValue = axeVmGetRegisterValueByName(vm, reg);

    int output = regValue;
    int expected = 0;
    ensure(output == expected, "set compare 测试失败\n");
}

void 
vmTestLoadSave() {
    axeVm vm = axeVmNew();

    // 设置内存@的值, 设置内存的值 只应该在里面设置
    // axeVmSetMemoryAtAddr(vm, 10, 99);

    int mcode[] = {
        0b00000001,  // load
        0b00000010,  // @2
        0b00010000,  // a1
        0b00000011,  // save
        0b00010000,  // a1
        0b00000000,  // @0
        0b11111111,  // halt
    };
    aeList machineCode = utilGetListByCArray(mcode, sizeof(mcode)/sizeof(int));
    runVm(vm, machineCode);

    // 对比寄存器的值 
    aeString reg = aeStringNewWithChars("a1");
    int regValue = axeVmGetRegisterValueByName(vm, reg);

    // 对比内存@0的值   
    int output = axeVmGetMemoryAtAddr(vm, 0);
    int expected = regValue;
    ensure(output == expected, "set add 测试失败\n");
}

void 
vmTestSetAdd() {
    // 初始化
    axeVm vm = axeVmNew();
    int mcode[] = {
        0b00000000,  // set
        0b00100000,  // a2
        0b10011100,  // 156
        0b00000000,  // set
        0b00110000,  // a3
        0b00010110,  // 22
        0b00000010,  // add
        0b00100000,  // a2
        0b00110000,  // a3
        0b00010000,  // a1
        0b11111111,  // halt
    };
    aeList machineCode = utilGetListByCArray(mcode, sizeof(mcode)/sizeof(int));
    runVm(vm, machineCode);

    // a1的值为178
    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a1"));
    int expected = 178;
    ensure(output == expected, "set add 测试失败\n");
}

aeList
_getMachineCode(char t[]) {
    // TODO: 需要抽到asm里  做一个预处理的操作
    // 给asm的只应该是源字符串
    aeString as = aeStringNewWithChars(t);
    aeList list = aeStringSplitSpace(as, " ");
    aeList result = run(list);
    return result;
}

void 
vmTestSet() {
    char t[] = GuaStringMulti(
                set a1 1
                halt
                );
    aeList machineCode = _getMachineCode(t);
    // utilLogUnsignedList(mCode);

    axeVm vm = axeVmNew();
    runVm(vm, machineCode);

    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a1"));
    int expected = 1;
    ensure(output == expected, "vmTestSet 测试失败\n");
}

void 
vmTestSet2() {
    char t[] = GuaStringMulti(
                set2 a1 288
                halt
                );
    aeList machineCode = _getMachineCode(t);

    // utilLogUnsignedList(machineCode);

    axeVm vm = axeVmNew();
    runVm(vm, machineCode);

    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a1"));
    int expected = 288;
    ensure(output == expected, "vmTestSet2 测试失败\n");
}

void 
vmTestSave() {
    char t[] = GuaStringMulti(
                set a1 10
                set a2 3
                save a1 @100
                halt
                );
    aeList machineCode = _getMachineCode(t);

    // utilLogUnsignedList(machineCode);

    axeVm vm = axeVmNew();
    runVm(vm, machineCode);

    // int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a1"));
    int output = axeVmGetMemoryAtAddr(vm, 100);
    int expected = 10;
    ensure(output == expected, "vmTestSave 测试失败\n");
}

void 
vmTestSave2() {
    char t[] = GuaStringMulti(
                set a1 10
                set2 a2 288
                save2 a2 @288
                halt
                );
    aeList machineCode = _getMachineCode(t);

    axeVm vm = axeVmNew();
    runVm(vm, machineCode);

    int output = axeVmGetMemoryAtAddr(vm, 288);
    int output1 = axeVmGetMemoryAtAddr(vm, 289);
    
    int expected = 32;
    ensure(output == expected, "vmTestSave2 测试失败\n");
}

void 
vmTestLoad() {
    char t[] = GuaStringMulti(
                set a1 10
                set a2 3
                save a1 @100
                load @100 a2
                halt
                );
    aeList machineCode = _getMachineCode(t);

    axeVm vm = axeVmNew();
    runVm(vm, machineCode);

    int output = axeVmGetMemoryAtAddr(vm, 100);
    int expected = 10;
    ensure(output == expected, "vmTestLoad 测试失败\n");

    output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a2"));
    expected = 10;
    ensure(output == expected, "vmTestLoad 测试失败\n");
}

void 
vmTestLoad2() {
    char t[] = GuaStringMulti(
                set2 a1 300
                save2 a1 @123
                load2 @123 a2
                halt
                );
    aeList machineCode = _getMachineCode(t);

    axeVm vm = axeVmNew();

    runVm(vm, machineCode);

    int output = axeVmGetMemoryAtAddr(vm, 123);
    int expected = 44;
    ensure(output == expected, "vmTestLoad2 测试失败\n");

    output = axeVmGetMemoryAtAddr(vm, 124);
    expected = 1;
    ensure(output == expected, "vmTestLoad2 测试失败\n");

    output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a2"));
    expected = 300;
    ensure(output == expected, "vmTestLoad2 测试失败\n");
}

void 
vmTestAdd2() {
    char t[] = GuaStringMulti(
                set2 a1 258
                set a2 10
                add2 a1 a2 a3
                halt
                );
    
    aeList machineCode = _getMachineCode(t);

    axeVm vm = axeVmNew();
    runVm(vm, machineCode);

    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a3"));
    int expected = 268;
    ensure(output == expected, "vmTestAdd2 测试失败\n");
}

void 
vmTestSubtract2() {
    char t[] = GuaStringMulti(
                set2 a1 288
                set a2 10
                subtract2 a1 a2 a3
                halt
                );
    
    aeList machineCode = _getMachineCode(t);

    axeVm vm = axeVmNew();
    runVm(vm, machineCode);

    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a3"));
    int expected = 278;
    ensure(output == expected, "vmTestAdd2 测试失败\n");
}

void 
vmTestSaveFromRegister() {
    char t[] = GuaStringMulti(
                set2 a1 288
                set2 a2 100
                save_from_register a1 a2
                halt
            );
    aeList machineCode = _getMachineCode(t);
    axeVm vm = axeVmNew();

    runVm(vm, machineCode);

    int output = axeVmGetMemoryAtAddr(vm, 100);
    int expected = 32;
    ensure(output == expected, "vmTestSaveFromRegister 测试失败\n");
}

void 
vmTestSaveFromRegister2() {
    char t[] = GuaStringMulti(
                set2 a1 2000
                set2 a2 2
                save_from_register2 a1 a2
                halt
            );
    aeList machineCode = _getMachineCode(t);
    axeVm vm = axeVmNew();

    runVm(vm, machineCode);

    int output = axeVmGetMemoryAtAddr(vm, 2);
    int expected = 208;
    ensure(output == expected, "vmTestSaveFromRegister2 测试失败\n");
}

void 
vmTestLoadFromRegister() {
    // 寄存器能存16位，但是内存不行
    char t[] = GuaStringMulti(
                set2 a1 288
                set2 a2 258
                save2 a2 @32
                load_from_register a1 a2
                halt
            );
    aeList machineCode = _getMachineCode(t);
    axeVm vm = axeVmNew();

    runVm(vm, machineCode);

    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a2"));
    int expected = 2;
    ensure(output == expected, "vmTestLoadFromRegister 测试失败\n");
}

void 
vmTestLoadFromRegister2() {
    char t[] = GuaStringMulti(
            set2 a1 288
            set2 a2 258
            save2 a2 @288
            load_from_register2 a1 a2
            halt
            );
    aeList machineCode = _getMachineCode(t);
    axeVm vm = axeVmNew();

    runVm(vm, machineCode);

    aeLog("a2:%d\n", axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a2")));

    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a2"));
    int expected = 258;
    ensure(output == expected, "vmTestLoadFromRegister2 测试失败\n");

}

void 
vmTestJumpIfLess() {
     char t[] = GuaStringMulti(
            jump @1024
            .memory 1024
            set a1 5
            set a2 10
            compare a1 a2
            jump_if_less @label1

            @label1
            set a1 1
            set a2 99
            add a1 a2 a1
            save a1 @100
            halt
            );
    aeList machineCode = _getMachineCode(t);
    axeVm vm = axeVmNew();

    runVm(vm, machineCode);

    int output = axeVmGetMemoryAtAddr(vm, 100);
    int expected = 100;
    ensure(output == expected, "vmTestJumpIfLess 测试失败\n");
}

void 
vmTestJump() {
    char t[] = GuaStringMulti(
            set a1 5
            jump @label1

            @label1
            set a2 2
            add a1 a2 a1
            save a1 @100
            halt
            );
    aeList machineCode = _getMachineCode(t);
    axeVm vm = axeVmNew();

    runVm(vm, machineCode);

    int output = axeVmGetMemoryAtAddr(vm, 100);
    int expected = 7;
    ensure(output == expected, "vmTestJump 测试失败\n");
}

void 
vmTestJumpFromRegister() {
    char t[] = GuaStringMulti(
            set a1 9
            jump_from_register a1
            set a2 2
            halt

            set a2 10
            halt
            );
    aeList machineCode = _getMachineCode(t);
    axeVm vm = axeVmNew();

    runVm(vm, machineCode);

    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a2"));
    int expected = 10;
    ensure(output == expected, "vmTestJumpFromRegister 测试失败\n");
}


    // TODO: 注释的处理！需要加上去。label不存在也要加上去
void
vmTestMultiply() {
   char t[] = GuaStringMulti(
            jump @1024
            .memory 1024
            set2 f1 3 
            jump @function_end 

            @function_multiply 
            set2 a3 2 
            save2 a1 @65534

            @while_start  
            compare a2 a3 
            jump_if_less @while_end 

            save2 a2 @65532
            set2 a2 1
            add2 a3 a2 a3

            load2 @65534 a2
            add2 a1 a2 a1

            load2 @65532 a2
            jump @while_start 
            @while_end 

            set2 a3 2
            subtract2 f1 a3 f1
            load_from_register2 f1 a2
            jump_from_register a2

            @function_end 

            set2 a1 99
            set2 a2 10

            set2 a3 14
            add2 pa a3 a3

            save_from_register2 a3 f1
            set2 a3 2
            add2 f1 a3 f1
            jump @function_multiply
            halt
        );
    aeList machineCode = _getMachineCode(t);
    axeVm vm = axeVmNew();
    runVm(vm, machineCode);

    int output = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("a1"));
    int expected = 990;
    ensure(output == expected, "vmTestMultiply 测试失败\n");
}
