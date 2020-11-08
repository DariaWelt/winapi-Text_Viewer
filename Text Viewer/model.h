#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


/**
 * @brief Модель. Хранит информацию о тексте, текст в виде массива строк, т.е. текст разбит по \n
 * @param stringArr     указатель на начало массива Строк
 * @param size размер   массива, итерироваться следует от 0 до size-1 включительно
 * @param max_size_elem размер самого длинного подмассива
 */
typedef struct {
    char** charArray;
    ULONG arraySize;
    ULONG lengthLast;
    ULONG max_size_elem;
} model_t;

/**
 * @brief По имени файла заполняется модель, инициализируются все поля, кроме max_size_elem
 * @param fileName      путь к файлу, который содержит текст, который будет
 * @param model         указатель на модель, которую мы инициализируем
 * @return код ошибки
 */
void readFile(const char* const fileName, model_t* const model);

/**
 * @brief Отчистка памяти, выделенной под модель
 * @param model         указатель на модель, которую мы удаляем
 */
void EraseModel (model_t* model);

/**
 * @brief Возвращает длину строки под номером row
 * @param model         указатель на модель, с которой мы хотим работать
 * @param row           номер строки текста
 * @return длина строки
 */
ULONG ModelGetRowLen(const model_t* model, ULONG row);

/**
 * @brief Перезаполняет форматированную модель новыми указателями в соответствии с заданной шириной и содержанием model
 * @param model         указатель на модель без форматирования, разбитую только по \n
 * @param formatted     указатель на форматированную модель, которую мы перезаполняем
 * @param width         максимально допустимое количество символов в строке форматированной модели
 */
void ModelFillFormatted (const model_t* model, model_t* formatted, UINT width);


/**
 * @brief Устанавливает (высчитывает) для данной модели максимальное значение длины строки и записывает в поле модели
 * @param model         указатель на модель, с которой мы хотим работать
 */
void ModelSetMaxSizeElem(model_t* model);




#endif // MODEL_H_INCLUDED
