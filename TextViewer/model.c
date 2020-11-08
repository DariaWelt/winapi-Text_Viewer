//
// Created by dasha on 16.10.2020.
//

#include "header.h"
#include <stdio.h>
#include <stdlib.h>

ULONG GetSizeOfFile(const char* const fileName) {
    if (fileName == NULL)
        return 0;
    FILE* file = fopen(fileName, "rb");
    if(file == NULL) {
        return 0;
    }
    fseek(file, 0, SEEK_END);
    ULONG size = ftell(file);
    fclose(file);
    return size;
}

void readFile(const char* const fileName, model_t* const model) {
    if (model == NULL)
        return;
    ULONG sizeText = GetSizeOfFile(fileName);
    if (sizeText == 0) {
        model->arraySize = 0;
        return;
    }

    char* rawData = (char*) calloc (sizeof(char), sizeText);
    FILE* file = fopen( fileName, "rb" );
    if (rawData == NULL || file == NULL) {
        model->arraySize = 0;
        return;
    }
    fread( rawData, 1, sizeText, file );
    fclose(file);
    model->arraySize = 0;
    model->charArray = NULL;
    for (UINT i = 0; i < sizeText - 1; ++i) {
        ULONG k = 0;
        while (i+k < sizeText - 1 && rawData[i+k] != '\n' && rawData[i+k] != 0) k++;
        if (k == 0)
            continue;
        model->arraySize += 1;
        model->charArray = realloc(model->charArray, model->arraySize * sizeof(void*));
        model->charArray[model->arraySize - 1] = rawData + i;
        model->lengthLast = k - 1;
        if (rawData[i+k] == 0)
            break;
        i = i + k;
    }
    return;
}

void __init__(model_t* const model, const char* const fileName, viewerText_t* const viewer) {
    model->arraySize = 0;
    model->lengthLast = 0;
    model->max_size_elem = 0;
    model->charArray = NULL;
    readFile(fileName, model);
    ModelSetMaxSizeElem(model);
    // инициализация вьювера
    viewer->width = 0;
    viewer->height = 0;
    viewer->mode = MODE_MARKING_OFF;
    viewer->model_1 = model;
    viewer->firstRow = 0;
    viewer->firstChar = 0;
    viewer->model_formatted = (model_t*)malloc(sizeof(model_t));
    viewer->model_formatted->charArray = NULL;
    return;
}

void EraseModel (model_t* model){
    if (model == NULL)
        return;
    if (model->charArray != NULL) {
        free(model->charArray[0]);
        model->charArray[0] = NULL;
    }
    free(model->charArray);
    model->charArray = NULL;
    printf("model erased\n");
}

ULONG ModelGetRowLen(const model_t* model, ULONG row) {
    if (model == NULL)
        return 0;
    if (row < model->arraySize - 1) {
        return model->charArray[row+1] - model->charArray[row];
    } else {
        return model->lengthLast;
    }
}

void ModelSetMaxSizeElem(model_t* model) {
    if (model == NULL)
        return;
    ULONG maxSize = 0;
    for (ULONG i = 0; i < model->arraySize; ++i) {
        ULONG curLen = ModelGetRowLen(model, i);
        maxSize = maxSize < curLen ? curLen : maxSize;
    }
    model->max_size_elem = maxSize;
}

void ModelFillFormatted (const model_t* model, model_t* formatted, UINT width) {
    if (model == NULL || formatted == NULL || width == 0)
        return;
    if (formatted->charArray != NULL) {
        free(formatted->charArray);
        formatted->charArray = NULL;
    }
    formatted->arraySize = 0;
    for (UINT i = 0; i < model->arraySize; i++) {
        ULONG size = ModelGetRowLen(model, i);
        if ( size <= width) {
            formatted->arraySize += 1;
            formatted->charArray = (char**)realloc(formatted->charArray, formatted->arraySize * sizeof(char*));
            formatted->charArray[formatted->arraySize - 1] = model->charArray[i];
            formatted->lengthLast = size;
        } else {
            for (UINT k = 0; k < size; k+=width) {
                formatted->arraySize += 1;
                formatted->charArray = (char**)realloc(formatted->charArray, formatted->arraySize * sizeof(char*));
                formatted->charArray[formatted->arraySize - 1] = model->charArray[i] + k;
                formatted->lengthLast = size - k;
            }
        }
    }
}
