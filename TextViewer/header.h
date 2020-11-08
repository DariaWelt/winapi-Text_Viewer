#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <wingdi.h>
#include <afxres.h>
#include "model.h"
#include "viewer.h"



#define CHANGE_MODE VK_SPACE
#define OPENFILE 'o'
#define MAX_SCROLL 65535
#define MY_MAX_PATH 300
#define _TEXT_FILES "Text Files (*.txt)\0*.txt\0\0"


/**
 * @brief Структура хранит ширину и высоту области или прямоугольника
 */
typedef struct {
    ULONG width; // ширина прямоугольника
    ULONG height; // высота прямоугольника
} sizeRect_t;

/**
 * @brief Дополнительная информация о окне, с которым мы взаимодействуем
 */
typedef struct {
    char* filePath;         // путь к файлу, который мы читаем и отображаем в окне
    model_t model;          // модель с данными для отображения
    viewerText_t viewer;    // просмоторщик с информацией об отображении модели в окне
    HGDIOBJ hFont;          // дескриптор используемого шрифта в окне
    TEXTMETRIC tm;          // метрики текста о ширине, высоте и тд шрифта в окне
} WNDInfo_t;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////               FUNCTIONS                 //////////////////////////////////////

/**
 * @brief Инициализация структур model и viewer значениями по умолчанию
 * @param model    указатель на модель для инициализации ее параметров
 * @param viewer   указатель на зрителя для инициализации его полей
 * @param fileName имя файла, из которого читаются данные для модели
 * @return иентификатор ошибки, которая произошла в функции или ER_OKAY
 */
void __init__(model_t* const model, const char* const fileName, viewerText_t* const viewer);

/**
 * @brief Отрисовка информации, хранимой в вьювере в конкретное устройство вывода
 * @param viewer   указатель на вьювер, с которым мы работаем сейчас
 * @param hdc      дескриптор устройства
 * @param textInfo информация о тексте в окне, в который мы будем рисовать
 *                  (например, нам требуется ширина и высота символов)
 */
void Print (const viewerText_t* const viewer, HDC hdc, const TEXTMETRIC textInfo);

/**
 * @brief Изменение значения value в зависимости от сообщения типа WM_?SCROLL
 * @param wParam   значение word param, пришедшее в обработчик сообщений
 * @param value    указатель на значение, которое будет изменяться (минимальное для него значение считается нулем)
 * @param elemSize размер "окна" - сколько единиц значения *value соответствует сообщению SB_PAGE*
 * @param maxValue наибольшее значение, которое может принимать *value
 * @param koeff    повышающий коэффициент для текущего положения скрола (который
 *                 отображает значение скрола в значение номера строки или ведущего символа)
 */
void ReactScroll(WPARAM wParam, ULONG* value, ULONG elemSize, ULONG maxValue, int koeff);

/**
 * @brief Обработка сообщения WM_KEYDOWN
 * @param hwnd     дескриптор окна, с которым мы работаем
 * @param wParam   значение word param, пришедшее в обработчик сообщений
 * @param viewer   указатель на вьювер, c которым работаем сейчас
 * @param mode     режим полосы прокрутки (вертикальная или горизонтальная) допустимы значения SB_VERT и SB_HORZ
 */
void ProcessKeyDown(HWND hwnd, WPARAM wParam, viewerText_t* viewer, int* mode);

/**
 * @brief Согласно текущему размеру просмоторщика (его ширине в частности)
 *        пересчитываем указатели на начала строк в форматированной модели
 * @param viewer   указатель на вьювер, c которым работаем сейчас
 */
void UpdateFormatedModel(viewerText_t * viewer);

/**
 * @brief Установка новых крайних значений полосы прокрутки
 * @param hwnd     дескриптор окна, с которым мы работаем
 * @param viewer   указатель на вьювер, c которым работаем сейчас
 */
void UpdateScrollRanges (HWND hwnd, viewerText_t * viewer);


/**
 * @brief Изменение состояния просмоторщика, форматированнной модели и полос прокрутки после WM_SIZE сообщения
 * @param hwnd     дескриптор окна, с которым мы работаем
 * @param viewer   указатель на вьювер, c которым работаем сейчас
 * @param rect     новые размеры окна
 * @param textInfo TEXTMETRIC переменная, хранящая информацию о метриках шрифта, установленного в окне
 *                 и использующаяся для высчитывания размеров просмоторщика
 */
void ReactResize(HWND hwnd, viewerText_t* const viewer, SIZE rect, const TEXTMETRIC textInfo);


/**
 * @brief Инициализация структуры OPENFILENAME
 * @param hwnd     дескриптор окна, с которым мы работаем
 * @param filter   строка, инициализирующая в структуре OPENFILENAME поле lpstrFilter
 * @param ofn      OPENFILENAME структура, которую мы инициализируем в этой функции
 * @param defExt   строка, инициализирующая в структуре OPENFILENAME поле lpstrDefExt
 */
void initOFN(HWND hwnd, const char* filter, OPENFILENAME* ofn, const char* defExt);


/**
 * @brief Отображаем данные нового файла в нашей модели и просмоторщике
 * @param viewer   указатель на вьювер, c которым работаем сейчас
 * @param ofn      OPENFILENAME структура, предварительно проинициализированная в initOFN
 * @param fileName указатель на место в памяти, где выделено MY_MAX_PATH байт для записи туда имени нового файла
 */
void openFile(viewerText_t* viewer, OPENFILENAME* ofn, char* fileName);



#endif // HEADER_H_INCLUDED
