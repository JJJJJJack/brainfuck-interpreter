#include <contexts.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t loadFile(programCtx *pCtx, const char *filename) {
    uint64_t fileLength = 0;
    FILE *fp;

    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("Could not open file %s", filename);
        return 1;
    }

    if (fseek(fp, 0L, SEEK_END) != 0) {
        printf("Could not fseek to end of file");
        return 1;
    }

    fileLength = ftell(fp);

    pCtx->instructionPtr    = calloc(sizeof(uint8_t), fileLength);
    pCtx->startInstructions = pCtx->instructionPtr;
    pCtx->endInstruction    = pCtx->instructionPtr + fileLength;
    *pCtx->endInstruction   = 0;

    if (fseek(fp, 0, SEEK_SET) != 0) {
        printf("Could not fseek to start of file");
        return 1;
    }

    if (fileLength != fread(pCtx->startInstructions, sizeof(uint8_t), fileLength, fp)) {
        printf("File reading error: bytes read != file length, file is corrupted or changed while reading");
        return 1;
    }

    if (fclose(fp) != 0) {
        printf("Could not close file %s", filename);
        return 1;
    }

    return 0;
}
