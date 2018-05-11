#pragma once

#include <windows.h>
#include <iostream>
#include "gdbms01.h"
#include "stdafx.h"

#define LoadFunction(func, type, typeStr)										\
	func = (type)GetProcAddress(hinstLib, typeStr);								\
	if (NULL == func)															\
	{																			\
		return;																	\
	}


class GeneralizationDataBaseLoadFuncs
{
private:
	HINSTANCE hinstLib;
public:
	BaseOpen _BaseOpen;
	BaseOpenTextError _BaseOpenTextError;
	BaseCloseObject _BaseCloseObject;
	BaseClose _BaseClose;
	BaseInitObject _BaseInitObject;
	BaseInitQuery _BaseInitQuery;
	BaseTransMKO _BaseTransMKO;
	BaseReadObject _BaseReadObject;
	BaseObjectCount _BaseObjectCount;
	BaseCodeStr _BaseCodeStr;

	GeneralizationDataBaseLoadFuncs() {
		hinstLib = LoadLibrary(TEXT("Gdbms01.dll"));
		if (hinstLib != NULL)
		{
			LoadFunction(_BaseOpen, BaseOpen, "BaseOpen");
			LoadFunction(_BaseOpenTextError, BaseOpenTextError, "BaseOpenTextError");
			LoadFunction(_BaseCloseObject, BaseCloseObject, "BaseCloseObject");
			LoadFunction(_BaseClose, BaseClose, "BaseClose");
			LoadFunction(_BaseInitObject, BaseInitObject, "BaseInitObject");
			LoadFunction(_BaseInitQuery, BaseInitQuery, "BaseInitQuery");
			LoadFunction(_BaseReadObject, BaseReadObject, "BaseReadObject");
			LoadFunction(_BaseObjectCount, BaseObjectCount, "BaseObjectCount");
			LoadFunction(_BaseCodeStr, BaseCodeStr, "BaseCodeStr");
		}
	}
	virtual ~GeneralizationDataBaseLoadFuncs() {};
};

