#include <checks.h>
#include <contexts.h>
#include <errors.h>
#include <macros.h>
#include <operations.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void nextCellDataPtr(programCtx *pCtx) {
    INC(pCtx->dataPtr);

    if (checkDataMemorySegfaults(pCtx) != 0) {
        printf("Error: SEGFAULT: data memory pointer out of bounds at\n");

        printWhereErrorIs(pCtx);

        pCtx->errPtr = pCtx->instructionPtr;
        exit(1); // better handle quitting program
    }
}

void previousCellDataPtr(programCtx *pCtx) {
    DEC(pCtx->dataPtr);

    if (checkDataMemorySegfaults(pCtx) != 0) {
        printf("Error: SEGFAULT: data memory pointer out of bounds\n");

        printWhereErrorIs(pCtx);

        pCtx->errPtr = pCtx->instructionPtr;
        exit(1); // better handle quitting program
    }
}

void incDataPtrCellValue(programCtx *pCtx) {
    *pCtx->dataPtr = (*pCtx->dataPtr) + 1;
}

void decDataPtrCellValue(programCtx *pCtx) {
    *pCtx->dataPtr = (*pCtx->dataPtr) - 1;
}

uint8_t *findLoopEnd(uint8_t *instPtr) {
    uint8_t *cpInstPtr = instPtr;
    int64_t depth      = 0;

    for (; *cpInstPtr != 0; cpInstPtr++) {
        if (*cpInstPtr == '[') {
            depth++;
            continue;
        }
        if (*cpInstPtr == ']') {
            depth--;
            if (depth == 0) {
                return cpInstPtr;
            }
        }
    }

    return NULL;
}

uint8_t *findLoopStart(uint8_t *instPtr) {
    uint8_t *cpInstPtr = instPtr;
    int64_t depth      = 0;

    for (; *cpInstPtr != 0; cpInstPtr--) {
        if (*cpInstPtr == '[') {
            depth++;
            continue;
        }
        if (*cpInstPtr == ']') {
            depth--;
            if (depth == 0) {
                return cpInstPtr;
            }
        }
    }

    return NULL;
}

uint8_t runLoop(programCtx *pCtx) {
    return runLoopTailRecursive(pCtx, 1);
}

uint8_t runLoopTailRecursive(programCtx *pCtx, uint64_t nestedLevel) {
    if (nestedLevel > RECURSION_LIMIT) {
        printf("Error: max recursion reached (%d)\n", RECURSION_LIMIT);

        printWhereErrorIs(pCtx);

        exit(1); // better stopping program
    }

    uint8_t *currLoopStart = pCtx->instructionPtr;
    uint8_t *currLoopEnd   = findLoopEnd(pCtx->instructionPtr);
    if (currLoopEnd == NULL) {
        printf("Error: loop end not found\n");

        printWhereErrorIs(pCtx);

        exit(1); // better stopping program
    }

    while (*pCtx->instructionPtr != 0) {
        switch (*pCtx->instructionPtr) {
            case '>':
                nextCellDataPtr(pCtx);
                break;
            case '<':
                previousCellDataPtr(pCtx);
                break;
            case '+':
                incDataPtrCellValue(pCtx);
                break;
            case '-':
                decDataPtrCellValue(pCtx);
                break;
            case '.':
                putchar(*pCtx->dataPtr);
                break;
            case ',':
                *pCtx->dataPtr = (uint8_t)getchar();
                break;
            case '[':
                if (*pCtx->dataPtr == 0 && pCtx->instructionPtr == currLoopStart) {
                    pCtx->instructionPtr = currLoopEnd;
                    return 0;
                }
                if (*pCtx->dataPtr != 0 && pCtx->instructionPtr == currLoopStart) {
                    break;
                }

                runLoopTailRecursive(pCtx, nestedLevel + 1);
                break;
            case ']':
                pCtx->instructionPtr = currLoopStart - 1;
                break;
        }

        INC(pCtx->instructionPtr);
    }

    return 1;
}
