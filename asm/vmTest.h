#ifndef vmTest_h
#define vmTest_h

#include <stdio.h>
#include "asm.h"
#include "aeMap.h"
#include "vm.h"
#include "aeUtils.h"

void 
vmTest();

aeList 
vmTestDraw();

void 
vmTestSetAdd();

void 
vmTestLoadSave();

void 
vmTestCompare();

void 
vmTestSet();

void 
vmTestSet2();

void 
vmTestSave();

void 
vmTestSave2();

void 
vmTestLoad();

void 
vmTestLoad2();

void 
vmTestAdd2();

void 
vmTestSubtract2();

void 
vmTestSaveFromRegister();

void 
vmTestSaveFromRegister2();

void 
vmTestLoadFromRegister();

void 
vmTestLoadFromRegister2();

void 
vmTestJumpIfLess();

void 
vmTestJump();

void 
vmTestJumpFromRegister();

void
vmTestMultiply();

#endif /* vmTest_h */
