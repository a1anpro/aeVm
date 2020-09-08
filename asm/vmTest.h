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
#endif /* vmTest_h */
