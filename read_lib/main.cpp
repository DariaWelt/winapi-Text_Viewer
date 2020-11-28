#include "main.h"
#include <stdio.h>

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

void readFile(const char* const fileName, char* const data) {
    if (data == NULL || fileName == NULL)
        return;
    ULONG sizeText = GetSizeOfFile(fileName);
    if (sizeText == 0)
        return;

    FILE* file = fopen( fileName, "rb" );
    if (file == NULL) {
        return;
    }
    fread( data, 1, sizeText, file );
    fclose(file);
    return;
}


extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // successful
}
