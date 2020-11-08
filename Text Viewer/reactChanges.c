//
// Created by dasha on 25.10.2020.
//

#include <windef.h>
#include <winuser.h>
#include <afxres.h>
#include "header.h"
#include "menu.h"
#include <commdlg.h>
#include <stdio.h>

void UpdateScrollRanges (HWND hwnd, viewerText_t * viewer) {
    model_t* model = viewer->mode == MODE_MARKING_ON ? viewer->model_formatted : viewer->model_1;
    int maxScrollPos = max(0, (long) model->arraySize - (long) viewer->height);
    if (model->arraySize > viewer->height && model->arraySize - viewer->height > MAX_SCROLL)
        maxScrollPos = (model->arraySize - viewer->height) / GetScrollKoeff(viewer, SB_VERT);
    SetScrollRange(hwnd, SB_VERT, 0, maxScrollPos, TRUE);
    maxScrollPos = max(0, (long) model->max_size_elem - (long) viewer->width);
    if (model->max_size_elem > viewer->width && model->max_size_elem - viewer->width > MAX_SCROLL)
        maxScrollPos = (model->max_size_elem - viewer->width)  / GetScrollKoeff(viewer, SB_HORZ);
    SetScrollRange(hwnd, SB_HORZ, 0, maxScrollPos, TRUE);
}

void ProcessKeyDown(HWND hwnd, WPARAM wParam, viewerText_t* viewer, int* mode) {
    long scrollGapSize = 0;
    model_t model = viewer->mode == MODE_MARKING_ON ? *viewer->model_formatted : *viewer->model_1;
    int scrollKoeff = GetScrollKoeff(viewer, *mode);
    switch (wParam) {
        case VK_DOWN:
            ReactScroll(SB_LINEDOWN, &viewer->firstRow, viewer->height, model.arraySize, scrollKoeff);
            break;
        case VK_UP:
            ReactScroll(SB_LINEUP, &viewer->firstRow, viewer->height, model.arraySize, scrollKoeff);
            break;
        case VK_LEFT:
            *mode = SB_HORZ;
            scrollKoeff = GetScrollKoeff(viewer, *mode);
            ReactScroll(SB_LINEUP, &viewer->firstChar, viewer->width, model.max_size_elem, scrollKoeff);
            break;
        case VK_RIGHT:
            *mode = SB_HORZ;
            scrollKoeff = GetScrollKoeff(viewer, *mode);
            ReactScroll(SB_LINEDOWN, &viewer->firstChar, viewer->width, model.max_size_elem, scrollKoeff);
            break;
        case CHANGE_MODE:
            scrollGapSize = CountRowsGap(viewer, viewer->mode);
            if (viewer->mode == MODE_MARKING_OFF) {
                viewer->mode = MODE_MARKING_ON;
                viewer->firstChar = 0;
                viewer->firstRow += scrollGapSize;
                UpdateScrollRanges(hwnd, viewer);
                SendMessage(hwnd, WM_COMMAND, IDM_FORMATTED, 0L);
            } else {
                viewer->mode = MODE_MARKING_OFF;
                viewer->firstRow -= scrollGapSize;
                UpdateScrollRanges(hwnd, viewer);
                SendMessage(hwnd, WM_COMMAND, IDM_UNFORMATTED, 0L);
            }
            break;
        default:
            break;
    }
}

void ReactScroll(WPARAM wParam, ULONG* value, ULONG elemSize, ULONG maxValue, int koeff) {
    switch (LOWORD(wParam)){
        case SB_LINEUP :
            if (*value > 0)
                *value -= 1;
            break;
        case SB_LINEDOWN :
            if (*value < (maxValue - elemSize))
                *value += 1;
            break;
        case SB_PAGEUP :
            if (*value > elemSize - 1)
                *value = (*value + 1) - elemSize;
            else
                *value = 0;
            break;
        case SB_PAGEDOWN :
            if (*value < maxValue + elemSize - 1)
                *value = (*value + (elemSize - 1));
            else
                *value = maxValue;
            break;
        case SB_THUMBPOSITION :
            *value = min((ULONG)(HIWORD(wParam) * koeff), maxValue);
            break;
        default :
            break;
    }
    return;
}

void openFile(viewerText_t* viewer, OPENFILENAME* ofn, char* fileName) {
    ofn->lpstrFile = fileName;
    if (GetOpenFileName(ofn)) {
        free(viewer->model_formatted->charArray);
        free(viewer->model_formatted);
        EraseModel(viewer->model_1);
        mode_t mode = viewer->mode;
        SIZE rect = {viewer->width, viewer->height};
        __init__(viewer->model_1, ofn->lpstrFile, viewer);
        viewer->mode = mode;
        viewer->width = rect.cx, viewer->height = rect.cy;
        ModelFillFormatted (viewer->model_1, viewer->model_formatted, viewer->width);

    }
}

void initOFN(HWND hwnd, const char* filter, OPENFILENAME* ofn, const char* defExt) {
    ofn->hwndOwner = hwnd;
    ofn->lStructSize = sizeof(OPENFILENAME);
    ofn->hInstance = NULL;

    ofn->lpstrFile = NULL;
    ofn->nMaxFile = MY_MAX_PATH;

    ofn->lpstrFilter = filter;
    ofn->lpstrCustomFilter = NULL;
    ofn->nMaxCustFilter = 0;
    ofn->nFilterIndex = 1;

    ofn->lpstrFileTitle = NULL;
    ofn->nMaxFileTitle = 0;
    ofn->lpstrInitialDir = NULL;
    ofn->lpstrTitle = NULL;

    ofn->lpstrDefExt = defExt;
    ofn->Flags = OFN_FILEMUSTEXIST | OFN_CREATEPROMPT;
}
