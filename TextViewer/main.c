#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include "header.h"
#include "menu.h"
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <scrnsave.h>


LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[] = _T("CodeBlocksWindowsApp");
static HWND hDlgModeless;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;
    MSG messages;
    WNDCLASSEX wincl;
    HACCEL hAccel;

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof (WNDCLASSEX);


    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(MY_MENU_NAME);
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = sizeof(WNDInfo_t*);
    wincl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);;

    if (!RegisterClassEx (&wincl))
        return 0;

    model_t model = {};
    viewerText_t viewerText = {};
    HGDIOBJ hFont = CreateFont(0,0,0,0,0,0,0,0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                               CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FIXED_PITCH, TEXT("Consolas"));
    TEXTMETRIC tm;
    WNDInfo_t winInfo = {lpszArgument,
                         model,
                         viewerText,
                         hFont,
                         tm};

    hwnd = CreateWindowEx (
           0,
           szClassName,
           _T("My App"),
           WS_OVERLAPPEDWINDOW | WS_VSCROLL| WS_HSCROLL,
           CW_USEDEFAULT,
           CW_USEDEFAULT,
           544,
           375,
           HWND_DESKTOP,
           LoadMenu(hThisInstance, MY_MENU_NAME),
           hThisInstance,
           &winInfo
           );

    ShowWindow (hwnd, nCmdShow);
    UpdateWindow(hwnd);
    hAccel = LoadAccelerators(hThisInstance, szClassName);

    while (GetMessage (&messages, NULL, 0, 0)) {
        if(hDlgModeless == NULL || !IsDialogMessage(hDlgModeless, &messages)) {
            if (!TranslateAccelerator(hwnd, hAccel, &messages)) {
                TranslateMessage(&messages);
                DispatchMessage(&messages);
                UpdateWindow(hwnd);
            }
        }
    }

    return messages.wParam;
}


LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SIZE rect = {0};
    HDC hdc;
    OPENFILENAME ofn = {0};
    PAINTSTRUCT ps;
    HMENU hMenu;
    WNDInfo_t* wndInfo = NULL;

    switch (message)
    {
        case WM_DESTROY: {
            wndInfo = (WNDInfo_t *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            free(wndInfo->viewer.model_formatted->charArray);
            free(wndInfo->viewer.model_formatted);
            EraseModel(wndInfo->viewer.model_1); // очищаем поля структуры модели
            PostQuitMessage(0);
            break;

        }
        case WM_CREATE:{
            wndInfo = (WNDInfo_t *)((LPCREATESTRUCT)lParam)->lpCreateParams;
            hdc = GetDC(hwnd);    // получили дискриптор дисплея
            SelectObject(hdc, wndInfo->hFont); // установили новый шрифт
            GetTextMetrics(hdc, &wndInfo->tm); // хотим узнать информацию о установленном в окне шрифте
            ReleaseDC(hwnd, hdc);  // возвращаем дискриптор

            __init__(&wndInfo->model, wndInfo->filePath, &wndInfo->viewer); // инициализируем модель и вьювер
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)(wndInfo)); // добавили данные к этому конкретному окну о его настройках
            SendMessage(hwnd, WM_COMMAND, IDM_UNFORMATTED, 0L); // установили режим по умолчанию: без форматирования
            wndInfo->filePath = (char*)calloc(sizeof(char), MY_MAX_PATH); // зарезервировали память под имя файла, если будут открывать его через "open"
            break;
        }
        // обработка сообщений меню
        case WM_COMMAND : {
            wndInfo = (WNDInfo_t *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
            hMenu = GetMenu(hwnd);
            switch (LOWORD(wParam)) {
                case IDM_EXIT :
                    SendMessage(hwnd, WM_CLOSE, 0, 0L);
                    return 0;
                case IDM_UNFORMATTED:
                case IDM_FORMATTED: {
                    int id = LOWORD(wParam);
                    CheckMenuItem(hMenu, id, MF_CHECKED);

                    // убираем галочку у невыбранного поля
                    if (id == IDM_UNFORMATTED)
                        CheckMenuItem(hMenu, IDM_FORMATTED, MF_UNCHECKED);
                    else
                        CheckMenuItem(hMenu, IDM_UNFORMATTED, MF_UNCHECKED);

                    // если значение режима у просмоторщика не совпадает с выбранным, отправляем сообщение для его изменения
                    if ((id == IDM_UNFORMATTED && wndInfo->viewer.mode == MODE_MARKING_ON) ||
                        (id == IDM_FORMATTED && wndInfo->viewer.mode == MODE_MARKING_OFF)) {
                        SendMessage(hwnd, WM_KEYDOWN, CHANGE_MODE, 0L);
                    }
                    break;
                }
                case IDM_OPEN: {
                    SendMessage(hwnd, WM_CHAR, (WPARAM)OPENFILE, 0L);
                }
            }
            break;
        }
        case WM_SIZE: {
            wndInfo = (WNDInfo_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            rect.cx = LOWORD(lParam), rect.cy = HIWORD(lParam);
            ReactResize(hwnd, &wndInfo->viewer, rect, wndInfo->tm);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_PAINT: {
            wndInfo = (WNDInfo_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            hdc = BeginPaint(hwnd, &ps);
            SelectObject(hdc, wndInfo->hFont);
            Print(&wndInfo->viewer, hdc, wndInfo->tm);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_KEYDOWN: {
            wndInfo = (WNDInfo_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            int mode = SB_VERT;
            ProcessKeyDown(hwnd, wParam, &wndInfo->viewer, &mode);
            int scrollKoeff = GetScrollKoeff(&wndInfo->viewer, mode);
            int scrollPos = (wndInfo->viewer.firstRow / (ULONG)scrollKoeff);
            // если позиция скролл-бара поменялась вносим изменения
            if (scrollPos != GetScrollPos(hwnd, mode) ) {
                SetScrollPos(hwnd, mode, scrollPos, TRUE);
            }
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_CHAR: {
            wndInfo = (WNDInfo_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            switch(wParam) {
                case OPENFILE: {
                    ZeroMemory(&ofn, sizeof(OPENFILENAME));
                    initOFN(hwnd, _TEXT_FILES, &ofn, "txt");
                    openFile(&wndInfo->viewer, &ofn, wndInfo->filePath);
                    for (UINT i = 0; i < MY_MAX_PATH; ++i)
                        wndInfo->filePath[i] = 0;
                    UpdateScrollRanges(hwnd, &wndInfo->viewer);
                    SetScrollPos(hwnd, SB_VERT, 0, TRUE);
                    SetScrollPos(hwnd, SB_HORZ, 0, TRUE);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                }
            }
            break;
        }
        case WM_VSCROLL:{
            wndInfo = (WNDInfo_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            model_t model = wndInfo->viewer.mode == MODE_MARKING_OFF ?
                    *wndInfo->viewer.model_1 : *wndInfo->viewer.model_formatted;
            int scrollKoeff = GetScrollKoeff(&wndInfo->viewer, SB_VERT);
            ReactScroll(wParam, &wndInfo->viewer.firstRow, wndInfo->viewer.height, model.arraySize, scrollKoeff);
            int scrollPos = (wndInfo->viewer.firstRow / (ULONG)scrollKoeff);
            if (scrollPos != GetScrollPos(hwnd, SB_VERT)) {
                SetScrollPos(hwnd, SB_VERT, scrollPos, TRUE);
            }
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_HSCROLL:{
            wndInfo = (WNDInfo_t*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            model_t model = wndInfo->viewer.mode == MODE_MARKING_OFF ?
                            *wndInfo->viewer.model_1 : *wndInfo->viewer.model_formatted;
            int scrollKoeff = GetScrollKoeff(&wndInfo->viewer, SB_HORZ);
            ReactScroll(wParam, &wndInfo->viewer.firstChar, wndInfo->viewer.width, model.max_size_elem, scrollKoeff);
            int scrollPos = (wndInfo->viewer.firstChar / (ULONG)scrollKoeff);
            if (scrollPos != GetScrollPos(hwnd, SB_HORZ)) {
                SetScrollPos(hwnd, SB_HORZ, scrollPos, TRUE);
            }
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        default:
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
