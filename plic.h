#ifndef PLIC_H

#include <stdint.h>

void plic_init(void);
void plicinithart(void);
int plic_claim(void);
void plic_complete(int irq);


#endif