aeList提供数组的api
aeString提供字符串处理的api
aeMap提供映射的api，用来做label的处理
aeUtils提供了log、assert、取指令、寄存器机器码的功能

util提供了ensure、apartData的功能，这个需要自己扩充

asm是实现汇编器的模块

世终
1. 整理的工程模块化的层级，现在只有一个aeUtils；
2. 替换了asm.c里面的逻辑，使用aeList来做作业；