#ifndef contexts_h
#define contexts_h

#include <stdint.h>

typedef struct {
    uint8_t *instructionPtr;
    uint8_t *startInstructions;
    uint8_t *endInstruction;

    uint8_t *dataPtr;
    uint8_t *startDataMemory;
    uint8_t *endDataMemory;

    uint8_t *errPtr;
} programCtx;

#endif
