#include "vm.h"


typedef struct _axeVm *axeVm;

struct _axeVm {
    // 内存
    aeList memory;
    // 寄存器
    aeMap registers;
};

aeString
getRegisterByByte(unsigned char reg) {
    char str[25];
    sprintf(str, "%d", reg);
    aeString output = aeStringNewWithChars(str);
    return output;
}

aeString
_getRegisterNameByMcode(unsigned char reg) {
    // asm的机器码 -> 找到寄存器名 -> 找到寄存器里的值
    aeString output;
    if (reg == 0) {
        output = aeStringNewWithChars("pa");
    } else if (reg == 16) {
        output = aeStringNewWithChars("a1");    
    } else if (reg == 32) {
        output = aeStringNewWithChars("a2");    
    } else if (reg == 48) {
        output = aeStringNewWithChars("a3");    
    } else if (reg == 64) {
        output = aeStringNewWithChars("c1");    
    } else if (reg == 80) {
        output = aeStringNewWithChars("f1");    
    } 
    return output;
}

// 不暴露给外面的, 获取寄存器
aeMap
_axeVmGetRegisters(axeVm vm) {
    return vm->registers;
}
aeList
_axeVmGetMemory(axeVm vm) {
    return vm->memory;
}

// 设置memory
void 
_setMemory(axeVm vm, aeList list) {
    // 所以都要加destroy
    // 清除原来的memory 设置新的
    vm->memory = list;
}

// 设置寄存器
void 
_setRegisters(axeVm vm, aeMap regs) {
    vm->registers = regs;
}

// 填充n个内存数据
aeList
_initMemory() {
    aeList list = aeListNew();
    int cnt = 256;
    int i;
    // 可以优化
    for (i = 0; i < cnt; ++i) {
        aeListAdd(list, 0);
    }
    return list;
}

aeMap
_initRegisters() {
    aeMap registers = aeMapNew();
    // 往寄存器里塞值, asm给的寄存器是机器码，所以要转成字符串
    aeMapPut(registers, aeStringNewWithChars("pa"), 0);
    aeMapPut(registers, aeStringNewWithChars("a1"), 0);
    aeMapPut(registers, aeStringNewWithChars("a2"), 0);
    aeMapPut(registers, aeStringNewWithChars("a3"), 0);
    aeMapPut(registers, aeStringNewWithChars("c1"), 0);
    aeMapPut(registers, aeStringNewWithChars("f1"), 0);
    return registers;
}

axeVm
axeVmNew() {
    axeVm vm = malloc(sizeof(axeVm));

    aeList memory = _initMemory();
    // 需要初始化内存
    aeMap registers = _initRegisters();
    
    vm->memory = memory;
    vm->registers = registers;

    return vm;
}

void 
axeVmLogRegisters(axeVm vm) {
    aeMap regs = vm->registers;
    aeMapLogItems(regs);
}

// 通过机器码 获得寄存器名，再获得寄存器的值
void
axeVmSetRegisterByMCode(axeVm vm, unsigned char reg, int value) {
    aeString regName = _getRegisterNameByMcode(reg);
    aeMap registers = _axeVmGetRegisters(vm);
    aeMapPut(registers, regName, value);
}

void
axeVmSetRegisterByName(axeVm vm, aeString name, int value) {
    aeMap registers = _axeVmGetRegisters(vm);
    aeMapPut(registers, name, value);
}

int
axeVmGetRegisterValueByName(axeVm vm, aeString regName) {
    aeMap registers = _axeVmGetRegisters(vm);
    return aeMapGet(registers, regName);
}

aeList
axeVmSliceMemory(axeVm vm, size_t begin, size_t end) {
    // assert长度
    aeList memory = _axeVmGetMemory(vm);
    aeList list = aeListCut(memory, begin, end);
    return list;
}

// 取内存里某地址值
unsigned char 
axeVmGetMemoryAtAddr(axeVm vm, size_t addr) {
    aeList memory = _axeVmGetMemory(vm);
    return aeListGetItem(memory, addr);
}

// 在地址addr处设置值
void 
axeVmSetMemoryAtAddr(axeVm vm, size_t addr, unsigned char data) {
    aeList memory = _axeVmGetMemory(vm);
    aeListSetItem(memory, addr, data);
}

// 根据pa读指令 执行
bool
axeVmDoNextIns(axeVm vm) {
    int pa = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("pa"));

    // 如果pa大于了mem长度，直接接入
    aeList mem = _axeVmGetMemory(vm);
    size_t len = aeListLength(mem);
    // TODO: 可能有类型上的bug
    if (pa > len) {
        return false;
    }

    // 内存中addr的值
    unsigned char data = axeVmGetMemoryAtAddr(vm, pa);

    aeString paName = aeStringNewWithChars("pa");
    if (data == 0b00000000) {
        // set a1 1
        // 先调整pa寄存器的值
        axeVmSetRegisterByName(vm, paName, pa + 3);

        // 取寄存器的机器码
        unsigned char reg = axeVmGetMemoryAtAddr(vm, pa + 1);
        // 取寄存器名（用来做输出）
        aeString regName = _getRegisterNameByMcode(reg);
        
        // 取值, set是8位的
        unsigned char data = axeVmGetMemoryAtAddr(vm, pa + 2);

        // 设置寄存器的值
        axeVmSetRegisterByMCode(vm, reg, data);
        aeLog("执行set (%s) (%d)\n", aeStringCString(regName), data);
    } else if (data == 0b00000010) {
        // add a1 a2 a3
        axeVmSetRegisterByName(vm, paName, pa + 4);

        unsigned char reg1 = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName1 = _getRegisterNameByMcode(reg1);
        // 从寄存器中取值
        unsigned char data1 = axeVmGetRegisterValueByName(vm, regName1);

        unsigned char reg2 = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeString regName2 = _getRegisterNameByMcode(reg2);
        unsigned char data2 = axeVmGetRegisterValueByName(vm, regName2);

        unsigned char reg3 = axeVmGetMemoryAtAddr(vm, pa + 3);
        aeString regName3 = _getRegisterNameByMcode(reg3);

        axeVmSetRegisterByName(vm, regName3, data1 + data2);
        aeLog("执行add (%s) (%s) (%s)\n", aeStringCString(regName1), aeStringCString(regName2), aeStringCString(regName3));
    } else if (data == 0b11111111) {
        aeLog("执行结束\n");
        axeVmSetRegisterByName(vm, paName, pa + 1);
        return false;
    } else {
        aeLog("碰到错误指令\n");
        return false;
    }
    return true;
}

void 
runVm(axeVm vm, aeList machineCode) {
    // 设置内存值
    // aeList mem = _axeVmGetMemory(vm);
    _setMemory(vm, machineCode);

    bool status = true;
    while (status) {
        status = axeVmDoNextIns(vm);
    }
    // aeLog("结束\n");
}