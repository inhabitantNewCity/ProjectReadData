#include "stdafx.h"
#include "graph_dll.h"
#include "gdbms01.h"

Load_Graph _Load_Graph;
Close_Graph _Close_Graph;
FindPath_Make _FindPath_Make;
GetResultPath _GetResultPath;
ClearCurrentPath _ClearCurrentPath;

int FunctionNoLoad();

HINSTANCE BaseFun(int *err,FUN *Fun1)
{
 FUN *Fun, Fun2;
 if (Fun1 == NULL) 
	 Fun=&Fun2; 
 else 
	 Fun=Fun1;
 *err=BERR_OK;
 memset(Fun,0,sizeof(FUN));
 HINSTANCE	hdll = LoadLibrary(TEXT("Graph_DLL_Project.dll"));
 if (hdll == NULL) 
	 return NULL;

_Load_Graph =(Load_Graph)GetProcAddress(hdll,"@Load_Graph$qpxc");
 if (_Load_Graph != NULL)
 {
  Fun->Load_Graph=1; 
 }
 else 
 {
	 *err=BERR_NOINF;
	  _Load_Graph= (Load_Graph)FunctionNoLoad;
 }

_Close_Graph =(Close_Graph)GetProcAddress(hdll,"Close_Graph$qv");
 if (_Close_Graph != NULL)
 {
  Fun->Close_Graph=1; 
 }
 else 
 {
	 *err=BERR_NOINF;
	  _Close_Graph= (Close_Graph)FunctionNoLoad;
 }

_FindPath_Make =(FindPath_Make)GetProcAddress(hdll,"@FindPath_Make$qrx5Coordt1");
 if (_FindPath_Make != NULL)
 {
  Fun->FindPath_Make=1; 
 }
 else 
 {
	 *err=BERR_NOINF;
	  _FindPath_Make=(FindPath_Make)FunctionNoLoad;
 }

_GetResultPath =(GetResultPath)GetProcAddress(hdll,"@GetResultPath$qp5Coord");
 if (_GetResultPath != NULL)
 {
  Fun->GetResultPath=1; 
 }
 else 
 {
	 *err=BERR_NOINF;
	  _GetResultPath= (GetResultPath)FunctionNoLoad;
 }

_ClearCurrentPath = (ClearCurrentPath)GetProcAddress(hdll,"@ClearCurrentPath$qv");
 if (_ClearCurrentPath != NULL)
 {
  Fun->ClearCurrentPath=1; 
 }
 else 
 {
	 *err=BERR_NOINF;
	  _ClearCurrentPath= (ClearCurrentPath)FunctionNoLoad;
 }

 return hdll;
}


int FunctionNoLoad()
{
 exit(0);
 return BERR_NOINF;
}