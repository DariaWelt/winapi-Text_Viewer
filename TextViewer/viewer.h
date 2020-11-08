#ifndef VIEWER_H_INCLUDED
#define VIEWER_H_INCLUDED


/**
 * @brief ������ ����������� ������ �� ������
 */
typedef enum {
    MODE_MARKING_ON, // ����������� � ��������
    MODE_MARKING_OFF // ����������� ��� �������
} mode_t;

/**
 * @brief Viewer. ��������� "�����", ����� ����� ������ ��� ���������� � ������ ������
 */
typedef struct{
    UINT width;                 // ���������� ��������, ������� ������������
    UINT height;                // ���������� �����, ������� ������������
    ULONG firstRow;             // ������, � ������� ���������� ����� ������
    ULONG firstChar;            // ������� ������ � �������
    model_t* model_1;           // ������ � �������, ������� ���������� �� ���� ��������
    model_t* model_formatted;   // ��������������� ������ � �������
    mode_t mode;                // ����� ����������� ������ �� ������
} viewerText_t;


/**
 * @brief ��������� "����" �������, ������� �� ����� � ������
 * @param viewer ��������� �� ������, � ������� �� �������� ������
 * @param rect ��������� ������ ������ � ������ ������� ��� ��������� � ��������
 * @param textInfo ���������� � ������ � ����, � ������� �� ����� ��������
 *                  (��������, ��� ��������� ������ � ������ ��������)
 */
void SetViewerSize (viewerText_t* const viewer, SIZE rect, const TEXTMETRIC textInfo);


/**
 * @brief �������� "����" �������, ������� �� ������
 * @param viewer ��������� �� ������, � ������� �� �������� ������
 * @param directionx ����������� � ������ ������ �� ���������� �
 * @param directiony ����������� � ������ ������ �� ���������� y
 */
void ShiftViewerWindow (viewerText_t* viewer, int directionx, int directiony);


/**
 * @brief ������������ �������� ��������� ������ ������ � ����������������� ������ � � ���������������
 * @param viewer ��������� �� ������, � ������� �� �������� ������
 * @param mode �����, � ������� ���������� ������� ��������� firstRow
 * @return ������������� ��������, �������� ��������
 */
long CountRowsGap(viewerText_t* viewer, mode_t mode);

/**
 * @brief ������������ ������� ����� ��������� ������ ������������� ������ ������ ��������� �� ���� ���
 * @param viewer ��������� �� ������, � ������� �� �������� ������
 * @param mode ����� ������ ��������� (������������ ��� ��������������) ��������� �������� SB_VERT � SB_HORZ
 * @return ������������� ��������, ���������� �����
 */
int GetScrollKoeff(const viewerText_t* viewer, UINT scrollMode);

int ViewerIsInvalidate(const viewerText_t* const viewer);

#endif // VIEWER_H_INCLUDED
