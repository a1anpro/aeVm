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
    Instruction_JUMP_IF_GREAT = 0b00000101, // 5
    Instruction_JUMP = 0b00000110, // 6
    Instruction_SAVE_FROM_REGISTER = 0b00000111, // 7
    Instruction_SET2 = 0b00001000, // 8
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
    } else if (aeStringEqualWithCString(s, "add")) {
        output = Instruction_ADD;
    } else if (aeStringEqualWithCString(s, "save")) {
        output = Instruction_SAVE;
    } else if (aeStringEqualWithCString(s, "jump")) {
        output = Instruction_JUMP;
    } else if (aeStringEqualWithCString(s, "jump_if_great")) {
        output = Instruction_JUMP_IF_GREAT;
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
        aeString code = aeListGetItem(list, i);

        if (aeStringEqualWithCString(code, "set")) {
            // data占1字节
            // 这里的insturction需要排查
            unsigned char ins = aeGetInstructions(code);
            aeListAdd(codeList, ins);
            // 寄存器
            aeString asmReg = aeListGetItem(list, i + 1);
            unsigned char reg = aeGetRegister(asmReg);
            aeListAdd(codeList, reg);
            // 数据
            aeString asmData = aeListGetItem(list, i + 2);
            unsigned char data = aeStringToByte(asmData);
            aeListAdd(codeList, data);
            
            i += 3;
        } else if (aeStringEqualWithCString(code, "set2")) {
            unsigned char ins = aeGetInstructions(code);
            aeListAdd(codeList, ins);
            // 寄存器
            aeString asmReg = aeListGetItem(list, i + 1);
            unsigned char reg = aeGetRegister(asmReg);
            aeListAdd(codeList, reg);
            
            // 数据
            aeString asmData = aeListGetItem(list, i + 2);
            unsigned char data = aeStringToByte(asmData);
            
            // 16位的需要拆分
            aeList d = utilApartData(data);
            unsigned char low = (int)aeListGetItem(d, 0);
            unsigned char high = (int)aeListGetItem(d, 1);
            
            aeListAdd(codeList, low);
            aeListAdd(codeList, high);
            
            i += 3;
        }
        else if (aeStringEqualWithCString(code, "load")) {
            // load @100 a1
            // 指令
            unsigned char ins = aeGetInstructions(code);
            aeListAdd(codeList, ins);
            
            // 地址
            aeString tempA = aeListGetItem(list, i + 1);
            aeString address = aeStringSlice(tempA, 1, aeStringLength(tempA));
//            aeStringLog(address);
            unsigned char data = aeStringToByte(address);
            aeListAdd(codeList, data);
//            aeLog("address=(%d)\n", data);
            
            // 寄存器
            aeString asmReg = aeListGetItem(list, i + 2);
            unsigned char reg = aeGetRegister(asmReg);
            aeListAdd(codeList, reg);
            
            i += 3;
         } else if (aeStringEqualWithCString(code, "add")) {
             unsigned char ins = aeGetInstructions(code);
             aeListAdd(codeList, ins);
             
             // 寄存器1
             aeString asmReg1 = aeListGetItem(list, i + 1);
             unsigned char reg1 = aeGetRegister(asmReg1);
             aeListAdd(codeList, reg1);
             
             aeString asmReg2 = aeListGetItem(list, i + 2);
             unsigned char reg2 = aeGetRegister(asmReg2);
             aeListAdd(codeList, reg2);
             
             aeString asmReg3 = aeListGetItem(list, i + 3);
             unsigned char reg3 = aeGetRegister(asmReg3);
             aeListAdd(codeList, reg3);
             
             i += 4;
         } else if (aeStringEqualWithCString(code, "save")) {
             // save a1 @100
             // 指令
             unsigned char ins = aeGetInstructions(code);
             aeListAdd(codeList, ins);
             
             // 寄存器
             aeString asmReg = aeListGetItem(list, i + 1);
             unsigned char reg = aeGetRegister(asmReg);
             aeListAdd(codeList, reg);
             
             // 地址
             aeString tempA = aeListGetItem(list, i + 2);
             aeString address = aeStringSlice(tempA, 1, aeStringLength(tempA));
             //            aeStringLog(address);
             unsigned char data = aeStringToByte(address);
             aeListAdd(codeList, data);
             //            aeLog("address=(%d)\n", data);
             
             i += 3;
         } else if (aeStringEqualWithCString(code, "compare")) {
             // compare a1 a2
             // 指令
             unsigned char ins = aeGetInstructions(code);
             aeListAdd(codeList, ins);
             
             // 寄存器
             aeString asmReg1 = aeListGetItem(list, i + 1);
             unsigned char reg1 = aeGetRegister(asmReg1);
             aeListAdd(codeList, reg1);
             
             // 寄存器
             aeString asmReg2 = aeListGetItem(list, i + 2);
             unsigned char reg2 = aeGetRegister(asmReg2);
             aeListAdd(codeList, reg2);
             
             i += 3;
         } else if (aeStringEqualWithCString(code, "jump") || aeStringEqualWithCString(code, "jump_if_great")) {
             // 内存：jump low_addr, high_addr
             // jump @label, jump @123
             unsigned char ins = aeGetInstructions(code);
             aeListAdd(codeList, ins);
             
//              label或者地址
             aeString tempA = aeListGetItem(list, i + 1);
             
             // 可以判断一下是不是@开头， 但是这个可以先用lexer来做
             aeString address = aeStringSlice(tempA, 1, aeStringLength(tempA));
             
             if (aeStringIsNumber(address)) {
                 // 是地址，直接跳转
                 unsigned char data = aeStringToByte(address);
                 // 16位的需要拆分
                 aeList d = utilApartData(data);
                 unsigned char low = (int)aeListGetItem(d, 0);
                 unsigned char high = (int)aeListGetItem(d, 1);

                 // 16位地址
                 aeListAdd(codeList, low);
                 aeListAdd(codeList, high);
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
//             int val2 = aeMapGet(labelMap, aeStringNewWithChars("@label1"));
             i += 1;
         } else if (aeStringEqualWithCString(code, "halt")) {
             // halt
             unsigned char ins = aeGetInstructions(code);
             aeListAdd(codeList, ins);
             i += 1;
         } else if (aeStringEqualWithCString(code, "save_from_register")) {
             // save_from_register a1 a2 把a1的值写入a2表示的内存中
             unsigned char ins = aeGetInstructions(code);
             aeListAdd(codeList, ins);
             
             aeString asmReg1 = aeListGetItem(list, i + 1);
             unsigned char reg1 = aeGetRegister(asmReg1);
             aeListAdd(codeList, reg1);
             
             aeString asmReg2 = aeListGetItem(list, i + 2);
             unsigned char reg2 = aeGetRegister(asmReg2);
             aeListAdd(codeList, reg2);
             i += 3;
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
//    utilLogIntList(codeList);
    return codeList;
}
