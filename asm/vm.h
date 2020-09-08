#ifndef vm_h
#define vm_h

#include <stdio.h>
#include <stdlib.h>
#include "aeList.h"
#include "aeMap.h"

typedef struct _axeVm *axeVm;

axeVm
axeVmNew();

void 
axeVmLogRegisters(axeVm vm);

aeString
getRegisterNameByMcode(unsigned char reg);

// 通过寄存器机器码设置寄存器值
void
axeVmSetRegisterByMCode(axeVm vm, unsigned char reg, int value);

// 通过寄存器名设置寄存器值
void
axeVmSetRegisterByName(axeVm vm, aeString name, int value);

int
axeVmGetRegisterValueByName(axeVm vm, aeString regName);

aeList
axeVmSliceMemory(axeVm vm, size_t begin, size_t end);

// 取内存里某地址值
unsigned char 
axeVmGetMemoryAtAddr(axeVm vm, size_t addr);

// 在地址addr处设置值
void
axeVmSetMemoryAtAddr(axeVm vm, size_t addr, unsigned char data);

// 虚拟机实现
void 
runVm(axeVm vm, aeList machineCode);

// 根据pa读指令 执行
bool
axeVmDoNextIns(axeVm vm);

#endif /* vm_h */
