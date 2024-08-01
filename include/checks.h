#ifndef checks_h
#define checks_h

#include <contexts.h>
#include <stdint.h>

uint8_t preCheckLoopBracketsPairs(programCtx *pCtx);
uint8_t checkDataMemorySegfaults(programCtx *pCtx);

#endif
