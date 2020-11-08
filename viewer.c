//
// Created by dasha on 17.10.2020.
//
#include "header.h"
#include <windows.h>
#include <limits.h>
#include <math.h>

int ViewerIsInvalidate(const viewerText_t* const viewer) {
    return (viewer == NULL || viewer->model_1 == NULL || viewer->model_1->arraySize == 0 ||
            (viewer->mode == MODE_MARKING_ON && (viewer->model_formatted == NULL || viewer->model_formatted->arraySize == 0)));
}


void SetViewerSize (viewerText_t* const viewer, SIZE rect, const TEXTMETRIC textInfo) {
    if (viewer == NULL)
        return;
    viewer->width = (int) (rect.cx / textInfo.tmAveCharWidth);
    viewer->height = (int) (rect.cy / (textInfo.tmHeight + textInfo.tmInternalLeading));
}

void Print (const viewerText_t* const viewer, HDC hdc, const TEXTMETRIC textInfo) {
    if (ViewerIsInvalidate(viewer))
        return;
    model_t model = *viewer->model_1;
    if (viewer->mode == MODE_MARKING_ON)
        model = *viewer->model_formatted;
    int height = textInfo.tmHeight + textInfo.tmInternalLeading;
    for (int i = 0; i < viewer->height; ++i) {
        if (model.arraySize <= i + viewer->firstRow)
            return;
        char* curString = model.charArray[i + viewer->firstRow];
        unsigned int curLen = (i + viewer->firstRow + 1 == model.arraySize) ? model.lengthLast :
                model.charArray[viewer->firstRow+i+1] - model.charArray[viewer->firstRow+i];

        if (curLen <= viewer->firstChar) {
            TextOut(hdc, textInfo.tmAveCharWidth, height * i, NULL, 0);
        }
        else {
            curLen -= viewer->firstChar;
            curString += viewer->firstChar;
            TextOut(hdc, textInfo.tmAveCharWidth, height * i, curString, curLen);
        }
    }
}

void UpdateFormatedModel(viewerText_t * viewer) {
    if (ViewerIsInvalidate(viewer))
        return;
    ModelFillFormatted(viewer->model_1, viewer->model_formatted, viewer->width);
    ModelSetMaxSizeElem(viewer->model_formatted);
}

int GetScrollKoeff(const viewerText_t* viewer, UINT scrollMode) {
    if (ViewerIsInvalidate(viewer))
        return 1;
    model_t* model = viewer->mode == MODE_MARKING_OFF ? viewer->model_1 : viewer->model_formatted;
    ULONG tmp = 0;
    if (scrollMode == SB_HORZ){
        if (model->max_size_elem > viewer->width)
            tmp = model->max_size_elem - viewer->width;
    }
    else if (scrollMode == SB_VERT) {
        if (model->arraySize > viewer->height)
            tmp = model->arraySize - viewer->height;
    }
    if (tmp > MAX_SCROLL)
        return (int)ceil((double)tmp/MAX_SCROLL);
    else
        return 1;
}

long CountRowsGap(viewerText_t* viewer, mode_t mode) {
    if (viewer == NULL || viewer->model_1 == NULL)
        return 0;
    long scrollGapSize = 0;
    model_t model = *viewer->model_1;
    for (UINT i = 0; i < model.arraySize; ++i) {
        ULONG size = ModelGetRowLen(&model, i);
        UINT k = 0;
        while ((k + 1) * viewer->width < size) ++k;
        scrollGapSize += k;
        if (mode == MODE_MARKING_OFF && viewer->firstRow <= (i + 1)) {
            break;
        }
        if (mode == MODE_MARKING_ON && viewer->firstRow <= (ULONG)scrollGapSize + i){
            if (scrollGapSize > 0)
                scrollGapSize -= (scrollGapSize + i) - (long)viewer->firstRow;
            break;
        }
    }
    return scrollGapSize;
}

void ReactResize(HWND hwnd, viewerText_t* const viewer, SIZE rect, const TEXTMETRIC textInfo) {

    // переменная хранит, на какое количество строк различаются положение в форматированном
    // виде и нет одной и той же строки текста (на которой мы находимся перед именениями)
    long gap = viewer->width > 0 ? CountRowsGap(viewer, MODE_MARKING_ON) : 0;

    SetViewerSize(viewer, rect, textInfo);
    if (ViewerIsInvalidate(viewer))
        return;
    UpdateFormatedModel(viewer);
    UpdateScrollRanges(hwnd, viewer);

    // для режима форматирования высчитаем новое положение первой строки в model_formatted
    if (viewer->mode == MODE_MARKING_ON) {
        // сдвигаем значение так, будто мы храним значение для model_1
        viewer->firstRow = min(max(0 , viewer->firstRow - (ULONG)gap), viewer->model_1->arraySize);
        // высчитываем разницу для новой ширины окна
        gap = CountRowsGap(viewer, MODE_MARKING_OFF);
        // сдвигаем значение обратно
        viewer->firstRow = min(viewer->firstRow + (ULONG)gap, viewer->model_formatted->arraySize);
    }
    // установим новые позиции скролла
    SetScrollPos(hwnd, SB_VERT, viewer->firstRow / GetScrollKoeff(viewer, SB_VERT), TRUE);
    SetScrollPos(hwnd, SB_HORZ, viewer->firstChar / GetScrollKoeff(viewer, SB_HORZ), TRUE);
}
