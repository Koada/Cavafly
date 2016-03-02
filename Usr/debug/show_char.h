#ifndef __SHOW_CHAR__
#define __SHOW_CHAR__
#include "stm32f10x.h"
#include <stdio.h>
int fputc(int ch, FILE* pf);
void show_char_init(void);
#endif
