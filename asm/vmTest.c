#include "vmTest.h"

void 
vmTest(){
    // vmTestSetAdd();
    // vmTestLoadSave();
    // vmTestCompare();
    // vmTestDraw();
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

    aeList mem = axeVmSliceMemory(vm, 0, 256);
    utilLogUnsignedList(mem);
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
    int expected = 2;
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

    // axeVmSetRegister(vm, 0, 123);
    // axeVmSetRegisterByName(vm, aeStringNewWithChars("pa"), 11);
    // axeVmLogRegisters(vm);
    // aeList mem = axeVmSliceMemory(vm, 0, 10);
    // utilLogUnsignedList(mem);

    // aeLog("(%d)\n", axeVmGetRegister(vm, aeStringNewWithChars("pa")));
    
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
