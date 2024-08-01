#include <checks.h>
#include <contexts.h>
#include <loadFile.h>
#include <macros.h>
#include <memory.h>
#include <operations.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        printf("usage:\n"
               "\tWith a file: brainfuck <file>"
               "\n\tWith inline code: brainfuck -e <stringofcode>");
        return 0;
    }

    uint8_t *memoryBuffer = malloc(MEM_BUFF_SIZE);
    programCtx pCtx;

    pCtx.dataPtr         = memoryBuffer;
    pCtx.startDataMemory = memoryBuffer;
    pCtx.endDataMemory   = memoryBuffer + MEM_BUFF_SIZE;

    pCtx.errPtr = NULL;

    if (argc == 3 && strcmp(argv[1], "-e") == 0) {
        pCtx.instructionPtr    = (uint8_t *)(argv[2]);
        pCtx.startInstructions = pCtx.instructionPtr;

        while (*pCtx.instructionPtr != 0) {
            INC(pCtx.instructionPtr);
        }
        pCtx.endInstruction = pCtx.instructionPtr;

        pCtx.instructionPtr = pCtx.startInstructions;
    }

    if (argc == 2) {
        if (loadFile(&pCtx, argv[1]) != 0) {
            printf("Quitting program...");
            return 1;
        }
    }

    if (preCheckLoopBracketsPairs(&pCtx) != 0) {
        if (pCtx.errPtr != NULL) {
            printf("Error at %p", pCtx.errPtr);
        }
        return 1;
    }

    memset(memoryBuffer, 0, MEM_BUFF_SIZE * sizeof(uint8_t));

    setvbuf(stdout, NULL, _IONBF, 0);

    while (*pCtx.instructionPtr != 0) {
        switch (*pCtx.instructionPtr) {
            case '>':
                nextCellDataPtr(&pCtx);
                break;
            case '<':
                previousCellDataPtr(&pCtx);
                break;
            case '+':
                incDataPtrCellValue(&pCtx);
                break;
            case '-':
                decDataPtrCellValue(&pCtx);
                break;
            case '.':
                putchar(*pCtx.dataPtr);
                break;
            case ',':
                *pCtx.dataPtr = (uint8_t)getchar();
                break;
            case '[':
                if (*pCtx.dataPtr == 0) {
                    pCtx.instructionPtr = findLoopEnd(pCtx.instructionPtr);
                    break;
                }

                runLoop(&pCtx);
                break;
            case ']':
                if (*pCtx.dataPtr != 0) {
                    pCtx.instructionPtr = findLoopStart(pCtx.instructionPtr);
                }
                break;
        }

        INC(pCtx.instructionPtr);
    }

    return 0;
}
