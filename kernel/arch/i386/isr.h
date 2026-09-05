#ifndef ISR_H
#define ISR_H

#include <stdint.h>

typedef struct {
	uint32_t ds;					 // saved data segment 
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Saved by pusha 
	uint32_t int_no, err_code;                       // pushed from the stub of the ISR
	uint32_t eip, cs, eflags, useresp, ss;           // Automatically saved by the cpu
}__attribute__((packed)) registers_t;

void fault_handler(registers_t* regs);
#endif
