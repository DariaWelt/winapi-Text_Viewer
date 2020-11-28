#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED


/**
 * @brief ������. ������ ���������� � ������, ����� � ���� ������� �����, �.�. ����� ������ �� \n
 * @param stringArr     ��������� �� ������ ������� �����
 * @param size ������   �������, ������������� ������� �� 0 �� size-1 ������������
 * @param max_size_elem ������ ������ �������� ����������
 */
typedef struct {
    char** charArray;
    ULONG arraySize;
    ULONG lengthLast;
    ULONG max_size_elem;
} model_t;

/**
 * @brief �� ����� ����� ����������� ������, ���������������� ��� ����, ����� max_size_elem
 * @param fileName      ���� � �����, ������� �������� �����, ������� �����
 * @param model         ��������� �� ������, ������� �� ��������������
 * @return ��� ������
 */
void BuildModel(const char* const fileName, model_t* const model);

/**
 * @brief �������� ������, ���������� ��� ������
 * @param model         ��������� �� ������, ������� �� �������
 */
void EraseModel (model_t* model);

/**
 * @brief ���������� ����� ������ ��� ������� row
 * @param model         ��������� �� ������, � ������� �� ����� ��������
 * @param row           ����� ������ ������
 * @return ����� ������
 */
ULONG ModelGetRowLen(const model_t* model, ULONG row);

/**
 * @brief ������������� ��������������� ������ ������ ����������� � ������������ � �������� ������� � ����������� model
 * @param model         ��������� �� ������ ��� ��������������, �������� ������ �� \n
 * @param formatted     ��������� �� ��������������� ������, ������� �� �������������
 * @param width         ����������� ���������� ���������� �������� � ������ ��������������� ������
 */
void ModelFillFormatted (const model_t* model, model_t* formatted, UINT width);


/**
 * @brief ������������� (�����������) ��� ������ ������ ������������ �������� ����� ������ � ���������� � ���� ������
 * @param model         ��������� �� ������, � ������� �� ����� ��������
 */
void ModelSetMaxSizeElem(model_t* model);




#endif // MODEL_H_INCLUDED
