#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

void _irq00Handler(void);
void _irq01Handler(void);
void _sysCallHandler(void); // Asegúrate de tener esta línea
void sysCallDispatcher(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3);

void _cli(void);
void _sti(void);
void _hlt(void);
void picMasterMask(uint8_t mask);
void picSlaveMask(uint8_t mask);

// Termina la ejecución de la cpu.
void haltcpu(void);

#endif /* INTERRUPTS_H_ */