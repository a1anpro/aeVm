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

    // 如果不满65536的 就补充满
    // TODO: 填充
    utilFillList(list, 65536);
    vm->memory = list;

    aeLog("内存大小=%d\n", aeListLength(vm->memory));
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
    // 这里只是一个初始长度，应该由asm给的来设置，vm只负责运算
    // TODO: 改内存大小, 内存应该是根据传入的asm结果设置的
    utilFillList(list, 0);
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
    // TODO: 判断长度是否超出了mem
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
        aeLog("pa超过了内存\n");
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
        axeVmSetRegisterByMCode(vm, reg, data);    

        aeLog("执行set (%s) (%d)\n", aeStringCString(regName), data);
    } else if (data == 0b00001000) {
        // set2 a1 1
        axeVmSetRegisterByName(vm, paName, pa + 4);

        unsigned char reg = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName = _getRegisterNameByMcode(reg);

        unsigned char low = axeVmGetMemoryAtAddr(vm, pa + 2);
        unsigned char high = axeVmGetMemoryAtAddr(vm, pa + 3);
        int data = utilCombineData(low, high);
        axeVmSetRegisterByMCode(vm, reg, data);
        aeLog("执行set2 (%s) (%d)\n", aeStringCString(regName), data);
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
    } else if (data == 0b00000001) {
        // load @2 a1
        axeVmSetRegisterByName(vm, paName, pa + 3);

        // 取地址
        unsigned char addr = axeVmGetMemoryAtAddr(vm, pa + 1);
        // 拿内存[地址]里的值
        unsigned char data = axeVmGetMemoryAtAddr(vm, addr);

        unsigned char reg = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeString regName = _getRegisterNameByMcode(reg);

        // 设置寄存器的值
        axeVmSetRegisterByMCode(vm, reg, data);
        aeLog("执行load (@%d->%d) (%s)\n", addr, data, aeStringCString(regName));
    } else if (data == 0b00001001) {
        // load2 @2 a1
        axeVmSetRegisterByName(vm, paName, pa + 4);

        // 取地址
        unsigned char low = axeVmGetMemoryAtAddr(vm, pa + 1);
        unsigned char high = axeVmGetMemoryAtAddr(vm, pa + 2);
        int addr = utilCombineData(low, high);

        // 拿内存[地址]里的值，取值也是取2个字节
        unsigned char lowData = axeVmGetMemoryAtAddr(vm, addr);
        unsigned char highData = axeVmGetMemoryAtAddr(vm, addr + 1);
        int data = utilCombineData(lowData, highData);
        aeLog("data:(%d, %d)%d\n", lowData, highData, data);

        unsigned char reg = axeVmGetMemoryAtAddr(vm, pa + 3);
        aeString regName = _getRegisterNameByMcode(reg);

        // 设置寄存器的值
        axeVmSetRegisterByMCode(vm, reg, data);
        aeLog("执行load2 (@%d->%d) (%s)\n", addr, data, aeStringCString(regName));        
    } else if (data == 0b00000011) {
        // save a1 @10
        axeVmSetRegisterByName(vm, paName, pa + 3);

        unsigned char reg = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName = _getRegisterNameByMcode(reg);
        // TODO: 可以用mcode拿寄存器的值
        // TODO: 寄存器的值 需要拆解成字节
        int data = axeVmGetRegisterValueByName(vm, regName);

        unsigned char addr = axeVmGetMemoryAtAddr(vm, pa + 2);
        axeVmSetMemoryAtAddr(vm, addr, data);
        
        aeLog("执行save (%s->%d) (@%d) \n", aeStringCString(regName), data, addr);
    } else if (data == 0b00001011) {
        // save2 a1 @288, a1如果很大的话 需要拆分
        axeVmSetRegisterByName(vm, paName, pa + 4);

        unsigned char reg = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName = _getRegisterNameByMcode(reg);
        int data = axeVmGetRegisterValueByName(vm, regName);
        // 把寄存器的16位打散
        aeList d = utilApartData(data);
        unsigned char lowData = (int)aeListGetItem(d, 0);
        unsigned char highData = (int)aeListGetItem(d, 1);

        // 组合16位地址
        unsigned char low = axeVmGetMemoryAtAddr(vm, pa + 2);
        unsigned char high = axeVmGetMemoryAtAddr(vm, pa + 3);
        int addr = utilCombineData(low, high);

        axeVmSetMemoryAtAddr(vm, addr, lowData);
        axeVmSetMemoryAtAddr(vm, addr + 1, highData);
        
        aeLog("执行save2 (%s->%d) (@%d) \n", aeStringCString(regName), data, addr);            
    } else if (data == 0b00001010 || data == 0b00001100) {
        // add2 a1 a2 a2 或 subtract2 a1 a2 a2
        axeVmSetRegisterByName(vm, paName, pa + 4);
        
        unsigned char reg1 = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName1 = _getRegisterNameByMcode(reg1);
        int data1 = axeVmGetRegisterValueByName(vm, regName1);
        
        unsigned char reg2 = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeString regName2 = _getRegisterNameByMcode(reg2);
        int data2 = axeVmGetRegisterValueByName(vm, regName2);

        unsigned char reg3 = axeVmGetMemoryAtAddr(vm, pa + 3);
        aeString regName3 = _getRegisterNameByMcode(reg3);
        int ans = -1;
        if (data == 0b00001010) {
            // add2
            ans = data1 + data2;
            aeLog("执行add2 (%s->%d)(%s->%d)(%s->%d) \n", 
                aeStringCString(regName1), data1, 
                aeStringCString(regName2), data2,  
                aeStringCString(regName3), ans);
        } else {
            ans = data1 - data2;
            aeLog("执行subtract2 (%s->%d)(%s->%d)(%s->%d) \n", 
                aeStringCString(regName1), data1, 
                aeStringCString(regName2), data2,  
                aeStringCString(regName3), ans);
        }
        axeVmSetRegisterByName(vm, regName3, ans);
    } else if (data == 0b00001111) {
        // save_from_register2 a1 a2; a1的值存到a2的内存里
        axeVmSetRegisterByName(vm, paName, pa + 3);

        unsigned char reg1 = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName1 = _getRegisterNameByMcode(reg1);
        int data = axeVmGetRegisterValueByName(vm, regName1);

        unsigned char reg2 = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeString regName2 = _getRegisterNameByMcode(reg2);
        int addr = axeVmGetRegisterValueByName(vm, regName2);

        // 把data拆成2部分放进mem
        aeList d = utilApartData(data);
        unsigned char lowData = (int)aeListGetItem(d, 0);
        unsigned char highData = (int)aeListGetItem(d, 1);
        axeVmSetMemoryAtAddr(vm, addr, lowData);
        axeVmSetMemoryAtAddr(vm, addr + 1, highData);

        aeLog("save_from_register2 (%s) (%s) \n", aeStringCString(regName1), aeStringCString(regName2));
    } else if (data == 0b00001101) {
        // load_from_register a1 a2  a1的地址中的值 存到a2
        axeVmSetRegisterByName(vm, paName, pa + 3);

        unsigned char reg1 = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName1 = _getRegisterNameByMcode(reg1);
        int addr = axeVmGetRegisterValueByName(vm, regName1);
        aeList d = utilApartData(addr);
        // 8位大小, 只取低位
        unsigned char lowAddr = (int)aeListGetItem(d, 0);
        int data = axeVmGetMemoryAtAddr(vm, lowAddr);

        unsigned char reg2 = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeString regName2 = _getRegisterNameByMcode(reg2);
        axeVmSetRegisterByName(vm, regName2, data);

        aeLog("load_from_register (%s) (%s) \n", aeStringCString(regName1), aeStringCString(regName2));
    } else if (data == 0b00001110) {
        // load_from_register2
        axeVmSetRegisterByName(vm, paName, pa + 3);

        unsigned char reg1 = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName1 = _getRegisterNameByMcode(reg1);
        int addr = axeVmGetRegisterValueByName(vm, regName1);
        // 把内存里的组合起来
        unsigned char lowData = axeVmGetMemoryAtAddr(vm, addr);
        unsigned char highData = axeVmGetMemoryAtAddr(vm, addr + 1);
        int data = utilCombineData(lowData, highData);

        unsigned char reg2 = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeString regName2 = _getRegisterNameByMcode(reg2);
        axeVmSetRegisterByName(vm, regName2, data);

        aeLog("load_from_register2 (%s) (%s) \n", aeStringCString(regName1), aeStringCString(regName2));
    } else if (data == 0b00010000) {
        // jump_from_register
        axeVmSetRegisterByName(vm, paName, pa + 2);

        unsigned char reg = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName = _getRegisterNameByMcode(reg);
        int addr = axeVmGetRegisterValueByName(vm, regName);
        axeVmSetRegisterByName(vm, aeStringNewWithChars("pa"), addr);

        aeLog("jump_from_register (%s)\n", aeStringCString(regName));        
    } else if (data == 0b00000100) {
        // compare a1 a2
        axeVmSetRegisterByName(vm, paName, pa + 3);

        unsigned char reg1 = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName1 = _getRegisterNameByMcode(reg1);
        int data1 = axeVmGetRegisterValueByName(vm, regName1);

        unsigned char reg2 = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeString regName2 = _getRegisterNameByMcode(reg2);
        int data2 = axeVmGetRegisterValueByName(vm, regName2);

        int temp = 1;
        if (data1 > data2) {
            temp = 2;
        } else if (data1 < data2) {
            temp = 0;
        }
        axeVmSetRegisterByName(vm, aeStringNewWithChars("c1"), temp);

        aeLog("执行compare (%s) (%s) \n", aeStringCString(regName1), aeStringCString(regName2));
    } else if (data == 0b00000101) {
        // jump跳的都是3个
        // jump_if_less 22222; 
        axeVmSetRegisterByName(vm, paName, pa + 3);
        
        unsigned char lowAddr = axeVmGetMemoryAtAddr(vm, pa + 1);
        unsigned char highAddr = axeVmGetMemoryAtAddr(vm, pa + 2);
        int addr = utilCombineData(lowAddr, highAddr);

        // 判断c1的大小，如果是0则跳转
        int c1 = axeVmGetRegisterValueByName(vm, aeStringNewWithChars("c1"));
        if (c1 == 0) {
            axeVmSetRegisterByName(vm, aeStringNewWithChars("pa"), addr);
        }

        aeLog("执行jump_if_less (%d)\n", addr);    
    } else if (data == 0b00000110) {
        // jump 22222; 可以有16位地址, 这个已经是数字了
        axeVmSetRegisterByName(vm, paName, pa + 3);
        
        unsigned char lowAddr = axeVmGetMemoryAtAddr(vm, pa + 1);
        unsigned char highAddr = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeLog("lowAddr, highAddr = (%d, %d)\n", lowAddr, highAddr); 
        int addr = utilCombineData(lowAddr, highAddr);
        axeVmSetRegisterByName(vm, aeStringNewWithChars("pa"), addr);

        aeLog("jump (%d)\n", addr);        
    } else if (data == 0b00000111) {
        // save_from_register a1 a2; a1的值存到a2的内存里
        axeVmSetRegisterByName(vm, paName, pa + 3);

        unsigned char reg1 = axeVmGetMemoryAtAddr(vm, pa + 1);
        aeString regName1 = _getRegisterNameByMcode(reg1);
        int data = axeVmGetRegisterValueByName(vm, regName1);

        unsigned char reg2 = axeVmGetMemoryAtAddr(vm, pa + 2);
        aeString regName2 = _getRegisterNameByMcode(reg2);
        int addr = axeVmGetRegisterValueByName(vm, regName2);

        axeVmSetMemoryAtAddr(vm, addr, data);
        aeLog("save_from_register (%s) (%s) \n", aeStringCString(regName1), aeStringCString(regName2));
    } else if (data == 0b11111111) {
        // halt
        aeLog("执行结束\n");
        axeVmSetRegisterByName(vm, paName, pa + 1);
        return false;
    } else {
        aeLog("碰到错误指令:%d\n", data);
        return false;
    }
    return true;
}

void 
runVm(axeVm vm, aeList machineCode) {
    // 需要做list的concat，直接拷贝内存 而不是一个个add
    _setMemory(vm, machineCode);

    bool status = true;
    while (status) {
        status = axeVmDoNextIns(vm);
    }
    // aeLog("结束\n");
}