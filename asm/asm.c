#include "asm.h"
#include "aeMap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum ASMRegister {
    Register_PA = 0b00000000, // 0
    Register_A1 = 0b00010000, // 16
    Register_A2 = 0b00100000, // 32
    Register_A3 = 0b00110000, // 48
    Register_C1 = 0b01000000, // 64
    Register_F1 = 0b01010000, // 80
};

enum ASMInstruction {
    Instruction_SET =  0b00000000, // 0
    Instruction_LOAD = 0b00000001, // 1
    Instruction_ADD =  0b00000010, // 2
    Instruction_SAVE = 0b00000011, // 3
    Instruction_COMPARE = 0b00000100, // 4
    Instruction_JUMP_IF_LESS = 0b00000101, // 5
    Instruction_JUMP = 0b00000110, // 6
    Instruction_SAVE_FROM_REGISTER = 0b00000111, // 7

    // 16位
    Instruction_SET2 = 0b00001000, // 8
    Instruction_LOAD2 = 0b00001001, // 9
    Instruction_ADD2 = 0b00001010, // 10
    Instruction_SAVE2 = 0b00001011, // 11
    Instruction_SUBTRACT2 = 0b00001100, // 12

    Instruction_LOAD_FROM_REGISTER = 0b00001101, // 13
    Instruction_LOAD_FROM_REGISTER2 = 0b00001110, // 14
    Instruction_SAVE_FROM_REGISTER2 = 0b00001111, // 15
    Instruction_JUMP_FROM_REGISTER = 0b00010000, // 16

    Instruction_HALT = 0b11111111, // 255
};

unsigned char
aeGetInstructions(aeString s) {
    unsigned char output = 0;
    if (aeStringEqualWithCString(s, "set")) {
        output = Instruction_SET;
    }else if (aeStringEqualWithCString(s, "set2")) {
        output = Instruction_SET2;
    } else if (aeStringEqualWithCString(s, "load")) {
        output = Instruction_LOAD;
    } else if (aeStringEqualWithCString(s, "load2")) {
        output = Instruction_LOAD2;
    } else if (aeStringEqualWithCString(s, "add")) {
        output = Instruction_ADD;
    } else if (aeStringEqualWithCString(s, "add2")) {
        output = Instruction_ADD2;
    } else if (aeStringEqualWithCString(s, "subtract2")) {
        output = Instruction_SUBTRACT2;
    } else if (aeStringEqualWithCString(s, "save")) {
        output = Instruction_SAVE;
    } else if (aeStringEqualWithCString(s, "save2")) {
        output = Instruction_SAVE2;
    } else if (aeStringEqualWithCString(s, "load_from_register")) {
        output = Instruction_LOAD_FROM_REGISTER;
    } else if (aeStringEqualWithCString(s, "load_from_register2")) {
        output = Instruction_LOAD_FROM_REGISTER2;
    } else if (aeStringEqualWithCString(s, "save_from_register2")) {
        output = Instruction_SAVE_FROM_REGISTER2;
    } else if (aeStringEqualWithCString(s, "jump_from_register")) {
        output = Instruction_JUMP_FROM_REGISTER;
    } else if (aeStringEqualWithCString(s, "jump")) {
        output = Instruction_JUMP;
    } else if (aeStringEqualWithCString(s, "jump_if_less")) {
        output = Instruction_JUMP_IF_LESS;
    } else if (aeStringEqualWithCString(s, "compare")) {
        output = Instruction_COMPARE;
    } else if (aeStringEqualWithCString(s, "halt")) {
        output = Instruction_HALT;
    } else if (aeStringEqualWithCString(s, "save_from_register")) {
        output = Instruction_SAVE_FROM_REGISTER;
    } else {
        printf("未找到指令\n");
        system("pause");
    }
    
    return output;
}

unsigned char
aeGetRegister(aeString s) {
    int output = 0;
    if (aeStringEqualWithCString(s, "a1")) {
        output = Register_A1;
    } else if (aeStringEqualWithCString(s, "a2")) {
        output = Register_A2;
    } else if (aeStringEqualWithCString(s, "a3")) {
        output = Register_A3;
    } else if (aeStringEqualWithCString(s, "c1")) {
        output = Register_C1;
    } else if (aeStringEqualWithCString(s, "f1")) {
        output = Register_F1;
    } else if (aeStringEqualWithCString(s, "pa")) {
        output = Register_PA;
    } else {
        printf("未找到寄存器\n");
        aeStringLog(s);
        system("pause");
    }
    
    return output;
}        

// 地址打成高低8位
void
_apartDataIntoList(int data, aeList codeList) {
    aeList d = utilApartData(data);
    unsigned char low = (int)aeListGetItem(d, 0);
    unsigned char high = (int)aeListGetItem(d, 1);

    aeListAdd(codeList, low);
    aeListAdd(codeList, high);
}

// 根据下标把寄存器放入list
void
_addRegisterIntoList(size_t index, aeList srcList, aeList codeList) {
    aeString asmReg = aeListGetItem(srcList, index);
    unsigned char reg = aeGetRegister(asmReg);
    aeListAdd(codeList, reg);
}

void
_add8BitsDataIntoList(size_t index, aeList srcList, aeList codeList) {
    aeString asmData = aeListGetItem(srcList, index);
    unsigned char data = aeStringToByte(asmData);
    aeListAdd(codeList, data);
}


// 把16位数据拆成高低位放入list
void
_add16BitsDataIntoList(size_t index, aeList srcList, aeList codeList) {
    // 数据
    aeString asmData = aeListGetItem(srcList, index);
    // 用int装
    int data = aeStringToInt(asmData);
    _apartDataIntoList(data, codeList);
}

// 把16位地址拆成高低位
void
_add16BitsAddressIntoList(size_t index, aeList srcList, aeList codeList) {
    // 地址需要切分
    aeString tempA = aeListGetItem(srcList, index);
    aeString address = aeStringSlice(tempA, 1, aeStringLength(tempA));
    // 16位
    int data = aeStringToInt(address);
    // 高低8位
    _apartDataIntoList(data, codeList);
}

// 8位地址
void
_add8BitsAddressIntoList(size_t index, aeList srcList, aeList codeList) {
    aeString tempA = aeListGetItem(srcList, index);
    aeString address = aeStringSlice(tempA, 1, aeStringLength(tempA));

    unsigned char data = aeStringToByte(address);
    aeListAdd(codeList, data);
}

void
_addInsIntoList(aeString code, aeList codeList) {
    // 指令
    unsigned char ins = aeGetInstructions(code);
    aeListAdd(codeList, ins);
}

aeList
run(aeList list) {
    aeList codeList = aeListNew();
    // 这里我需要访问到长度
    size_t len = aeListLength(list);
    size_t i = 0;
    aeMap labelMap = aeMapNew();
    // 存放label在codelist里的下标
    aeMap labelIndexMap = aeMapNew();
    
    while (i < len) {
        // TODO: 可以把i收起来 让外循环来做
        aeString code = aeListGetItem(list, i);
        if (aeStringEqualWithCString(code, "set")) {
            // data占1字节
            // 这里的insturction需要排查
            _addInsIntoList(code, codeList);
            // 寄存器
            _addRegisterIntoList(i + 1, list, codeList);
            // 数据
            _add8BitsDataIntoList(i + 2, list, codeList);
            
            i += 3;
        } else if (aeStringEqualWithCString(code, "set2")) {
            _addInsIntoList(code, codeList);
            _addRegisterIntoList(i+1, list, codeList);
            _add16BitsDataIntoList(i + 2, list, codeList);
            
            i += 3;
        }
        else if (aeStringEqualWithCString(code, "load")
        ) {
            // load @100 a1
            _addInsIntoList(code, codeList);
            _add8BitsAddressIntoList(i + 1, list, codeList);
            _addRegisterIntoList(i + 2, list, codeList);
            
            i += 3;
        } else if (aeStringEqualWithCString(code, "load2")) {
            // load2 @100 a1
            _addInsIntoList(code, codeList);
            _add16BitsAddressIntoList(i + 1, list, codeList);
            _addRegisterIntoList(i + 2, list, codeList);
            
            i += 3;
        } else if (aeStringEqualWithCString(code, "add") 
                || aeStringEqualWithCString(code, "subtract2")
                || aeStringEqualWithCString(code, "add2")
                ) {
             _addInsIntoList(code, codeList);
             _addRegisterIntoList(i + 1, list, codeList);
             _addRegisterIntoList(i + 2, list, codeList);
             _addRegisterIntoList(i + 3, list, codeList);
             
             i += 4;
        } else if (aeStringEqualWithCString(code, "save")) {
             // save a1 @100
             _addInsIntoList(code, codeList);
             _addRegisterIntoList(i + 1, list, codeList);
             _add8BitsAddressIntoList(i + 2, list, codeList);
             
             i += 3;
        } else if (aeStringEqualWithCString(code, "save2")) {
            // save2 a1 @100
            _addInsIntoList(code, codeList);
            _addRegisterIntoList(i + 1, list, codeList);
            _add16BitsAddressIntoList(i + 2, list, codeList);

            i += 3;
         } else if (aeStringEqualWithCString(code, "jump") 
                 || aeStringEqualWithCString(code, "jump_if_less")
         ) {
             // 内存：jump low_addr, high_addr
             // jump @label, jump @123
             _addInsIntoList(code, codeList);
             
             aeString tempA = aeListGetItem(list, i + 1);
             // 可以判断一下是不是@开头， 但是这个可以先用lexer来做
             aeString address = aeStringSlice(tempA, 1, aeStringLength(tempA));
             
             if (aeStringIsNumber(address)) {
                 // 16位地址 没测到
                 int data = aeStringToInt(address);
                 _apartDataIntoList(data, codeList);
             } else {
                 // 当前在codeList的下标
                 size_t position = aeListLength(codeList);
                 aeMapPut(labelIndexMap, tempA, position);
                 
                 // -1占位
                 aeListAdd(codeList, -1);
                 aeListAdd(codeList, -1);
             }
             i += 2;
         } else if (aeStringIndexOf(code, aeStringNewWithChars("@")) != -1) {
             size_t tag = aeListLength(codeList);
             aeMapPut(labelMap, code, tag);
             i += 1;
         } else if (aeStringEqualWithCString(code, "halt")) {
             // halt
             unsigned char ins = aeGetInstructions(code);
             aeListAdd(codeList, ins);
             i += 1;
        } else if (aeStringEqualWithCString(code, "load_from_register")
                || aeStringEqualWithCString(code, "load_from_register2")
                || aeStringEqualWithCString(code, "save_from_register2")
                || aeStringEqualWithCString(code, "save_from_register")
                || aeStringEqualWithCString(code, "compare")
        ) {
             // load_from_register f1 a2
             _addInsIntoList(code, codeList);
             _addRegisterIntoList(i + 1, list, codeList);
             _addRegisterIntoList(i + 2, list, codeList);
             
             i += 3;
        } else if (aeStringEqualWithCString(code, "jump_from_register")) {
            // jump_from_register a1; 跳到a1中内存处
            _addInsIntoList(code, codeList);
            // 只是翻译！
            _addRegisterIntoList(i + 1, list, codeList);
            i += 2;
        // 需要用预处理器来解决.xxx的问题
        } else if (aeStringEqualWithCString(code, ".memory")) {
            // 直接往内存里塞空值，填充
            size_t len = aeListLength(codeList);
            utilFillList(codeList, 1024);

            i += 2;
        }
    }

    // labelIndexMap里取要替换的是谁，在哪(key value)
    // labelMap里找到要替换的position
    // 遍历map拿到 label的下标，然后去labelMap里取标记值
    size_t j;
    aeList keys = aeMapKeys(labelIndexMap);
    
    size_t keyLen = aeListLength(keys);
    for (j = 0; j < keyLen; ++j) {
        aeString k = aeListGetItem(keys, j);
        int index = aeMapGet(labelIndexMap, k);
        
        // 要key去取address,
        int address = aeMapGet(labelMap, k);
        // 16位地址的需要拆分
        aeList d = utilApartData(address);
        int low = (int)aeListGetItem(d, 0);
        int high = (int)aeListGetItem(d, 1);
        
        aeListSetItem(codeList, index, low);
        aeListSetItem(codeList, index + 1, high);
    }
    
    // 打印输出的指令列表
    // utilLogIntList(codeList);
    return codeList;    
}
