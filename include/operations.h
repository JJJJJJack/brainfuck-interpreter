#ifndef operations_h
#define operations_h

#include <contexts.h>
#include <stdint.h>

uint8_t runLoop(programCtx *pCtx);
uint8_t runLoopTailRecursive(programCtx *pCtx, uint64_t nestedLevel);
void nextCellDataPtr(programCtx *pCtx);
void previousCellDataPtr(programCtx *pCtx);
void incDataPtrCellValue(programCtx *pCtx);
void decDataPtrCellValue(programCtx *pCtx);
uint8_t *findLoopEnd(uint8_t *instPtr);
uint8_t *findLoopStart(uint8_t *instPtr);

#endif
