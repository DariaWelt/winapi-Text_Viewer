#ifndef VIEWER_H_INCLUDED
#define VIEWER_H_INCLUDED


/**
 * @brief Режимы отображения текста на экране
 */
typedef enum {
    MODE_MARKING_ON, // отображение с версткой
    MODE_MARKING_OFF // отображение без верстки
} mode_t;

/**
 * @brief Viewer. Структура "знает", какую часть текста она показывает в данный момент
 */
typedef struct{
    UINT width;                 // количество символов, которые показываются
    UINT height;                // количество строк, которые показываются
    ULONG firstRow;             // строка, с которой начинается показ текста
    ULONG firstChar;            // ведущий символ в строках
    model_t* model_1;           // модель с данными, которая закреплена за этим вьювером
    model_t* model_formatted;   // форматированная модель с данными
    mode_t mode;                // режим отображения текста на экране
} viewerText_t;


/**
 * @brief Изменение "окна" вьювера, которое он видит и рисует
 * @param viewer указатель на вьювер, с которым мы работаем сейчас
 * @param rect структура хранит ширину и высоту области для рисования в пикселях
 * @param textInfo информация о тексте в окне, в который мы будем рисовать
 *                  (например, нам требуется ширина и высота символов)
 */
void SetViewerSize (viewerText_t* const viewer, SIZE rect, const TEXTMETRIC textInfo);


/**
 * @brief Сдвигаем "окно" вьювера, которое он рисует
 * @param viewer указатель на вьювер, с которым мы работаем сейчас
 * @param directionx направление и размер сдвига по координате х
 * @param directiony направление и размер сдвига по координате y
 */
void ShiftViewerWindow (viewerText_t* viewer, int directionx, int directiony);


/**
 * @brief Высчитывание разности положения данной строки в неформатированной модели и в форматированной
 * @param viewer указатель на вьювер, с которым мы работаем сейчас
 * @param mode режим, в котором определено текущее положение firstRow
 * @return положительное значение, величина разности
 */
long CountRowsGap(viewerText_t* viewer, mode_t mode);

/**
 * @brief Высчитывание сколько строк исходного текста соответствует сдвигу полосы прокрутки на один шаг
 * @param viewer указатель на вьювер, с которым мы работаем сейчас
 * @param mode режим полосы прокрутки (вертикальная или горизонтальная) допустимы значения SB_VERT и SB_HORZ
 * @return положительное значение, количество строк
 */
int GetScrollKoeff(const viewerText_t* viewer, UINT scrollMode);

#endif // VIEWER_H_INCLUDED
