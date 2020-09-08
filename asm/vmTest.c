#include "vmTest.h"

void vmTest(){
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