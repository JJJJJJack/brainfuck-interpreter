#include <contexts.h>
#include <macros.h>
#include <stdint.h>
#include <stdio.h>

uint8_t preCheckLoopBracketsPairs(programCtx *pCtx) {
    int64_t depth             = 0;
    uint8_t *cpInstructionPtr = *(uint8_t **)&pCtx->instructionPtr;

    for (; *cpInstructionPtr != 0; cpInstructionPtr++) {
        if (depth < 0) {
            pCtx->errPtr = cpInstructionPtr;
            printf("Error: bracket was closed before being opened\n");

            return 1;
        }
        if (*cpInstructionPtr == '[') {
            depth++;
            continue;
        }
        if (*cpInstructionPtr == ']') {
            depth--;
        }
    }

    if (depth != 0) {
        printf("Error: some open bracket was not closed\n");
        return 1;
    }

    return 0;
}

uint8_t checkDataMemorySegfaults(programCtx *pCtx) {
    if (pCtx->dataPtr < pCtx->startDataMemory || pCtx->dataPtr >= pCtx->endDataMemory) {
        pCtx->errPtr = pCtx->instructionPtr;
        return 1;
    }

    return 0;
}
