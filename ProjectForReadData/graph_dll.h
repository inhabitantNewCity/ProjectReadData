#ifndef graph_dll_h
#define graph_dll_h

#define BASEEXPORTFUNC __stdcall
#define DLLEXPORT extern "C" __declspec(dllexport)
//---------------------------------------------------------------------------

#include "Coord.h"
#include <Windows.h>

#define BERR_OK             0  // Нормальное завершение
#define BERR_NOINF          2  // Нет информации по запросу
//---------------------------------------------------------------------------
typedef struct
{
 unsigned Load_Graph : 1; 
 unsigned Close_Graph : 1;
 unsigned FindPath_Make : 1;
 unsigned GetResultPath : 1;
 unsigned ClearCurrentPath : 1;
} FUN;

//------- Динамическая загрузка GDBMS00.DLL и формирование адресов функций
HINSTANCE BaseFun(
 int *err,         // Код завершения:
                   // BERR_OK - адреса всех функций загружены
                   // BERR_NOINF - хотя бы одна функция не найдена
 FUN *Fun		   // Битовый массив флагов функций. Если бит = 1,
                   //  функция найдена, если = 0, то не найдена
                   // Если Fun==NULL, то битовый массив не формируется
);

typedef bool (BASEEXPORTFUNC *Load_Graph)(const char *name); //wchar_t
typedef void (BASEEXPORTFUNC *Close_Graph)();
typedef int (BASEEXPORTFUNC *FindPath_Make)(const Coord &start, const Coord &end);
typedef void (BASEEXPORTFUNC *GetResultPath)(Coord *path);
typedef void (BASEEXPORTFUNC *ClearCurrentPath)();

extern Load_Graph _Load_Graph;
extern Close_Graph _Close_Graph;
extern FindPath_Make _FindPath_Make;
extern GetResultPath _GetResultPath;
extern ClearCurrentPath _ClearCurrentPath;

#endif