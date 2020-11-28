#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

void DLL_EXPORT readFile(const char* const fileName, char* const data);

ULONG DLL_EXPORT GetSizeOfFile(const char* const fileName);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
