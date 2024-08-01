#include <contexts.h>
#include <stdio.h>

void printWhereErrorIs(programCtx *pCtx) {
    printf("\n");

    for (int i = -5; i < 5; i++) {
        if (pCtx->instructionPtr + i < pCtx->startInstructions ||
            pCtx->instructionPtr + i >= pCtx->endInstruction) {
            continue;
        }

        printf("%c", *(pCtx->instructionPtr + i));
    }

    printf("\n");

    for (int i = -5; i <= 0; i++) {
        if (pCtx->instructionPtr + i < pCtx->startInstructions ||
            pCtx->instructionPtr + i >= pCtx->endInstruction) {
            continue;
        }

        if (i == 0) {
            printf("^\n");
            break;
        }

        printf(" ");
    }
}
