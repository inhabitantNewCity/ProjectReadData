#include "stdafx.h"
#include "way_load.h"
#include "graph_dll.h"
#include "GeneralizationDataBaseLoadFuncs.h"
#include <conio.h>
#include <iostream>
#include <fstream>


struct {
	GBASE *dataBase;
} db;

struct GBaseCoord
{
	int x;
	int y;
};

using namespace std;
ofstream myfile;

Coord* GetWay(Coord coordStart, Coord coordEnd, int *n, char* namebase)
{
	bool graph;
	FILE *f;
	int error_1;
	HINSTANCE hdll00;

	hdll00=BaseFun(&error_1,NULL);
	if (hdll00 == NULL)
		return NULL;

//проверка на наличие такого графа!!
		f = fopen("CORP1FLOOR2.g","r+");
		if(!f)
		{
			cout<<"not_graph_file"<<endl;				
			return 0;	
		}
		else
		{
		bool isLoaded = _Load_Graph("CORP1FLOOR2.g");
	  
		int pointsCount=0;  
		Coord *path;

		//(461, 535)
		//coordStart.x_ = 951;
		pointsCount =_FindPath_Make(coordStart, coordEnd );
		*n = pointsCount;
		if (pointsCount==-1)
		 {  
			printf("pCount==-1\n");
			return NULL;
		 }
		path=new Coord[pointsCount]; 
		_GetResultPath(path); 
		return path;	
		}
}

Coord* readDataFromObject(GBASE_OBJECT object, int* countPoints, int i) {
	
	Coord* result = new Coord[object.qMet];
	*countPoints = object.qMet;
	GBaseCoord* points = (GBaseCoord*)object.pMet;
	long Xiter = 0;
	long Yiter = 0;

	int effectiveIter = 0;

	for (int i = 0; i < object.qMet*6; i++) {
		if ((BASE_INT)((BASE_INT*)object.pMet)[i] <= 0  )
			continue;
		if (!(effectiveIter % 2)) {
			result[Xiter++].x_ = (BASE_INT)((BASE_INT*)object.pMet)[i];
			myfile << Xiter << ". ( x = " << (BASE_INT)((BASE_INT*)object.pMet)[i];
		}
		else {
			result[Yiter++].y_ = (BASE_INT)((BASE_INT*)object.pMet)[i];
			myfile << ", y = " << (BASE_INT)((BASE_INT*)object.pMet)[i] << ") " << endl;
		}
		effectiveIter++;
	}
	
	return result;
}

GBaseWay* GetMap(int *n, char* namebase) {
	myfile.open("example.txt");
	myfile << "Map is generating.\n";
	int err = 0;

	GBaseWay* result;
	int countPoints = 0;

	GBASE_QUERY Query;
	long countWays = 0;
	BASE_INT Code[10] = { 4, 30, 3, 1, 1 };
	GBASE_OBJECT Object;
	GeneralizationDataBaseLoadFuncs* loader = new GeneralizationDataBaseLoadFuncs();

	db.dataBase = loader->_BaseOpen(&err, "CORP1FLOOR2.DPF", "R", NULL, NULL);

	loader->_BaseInitQuery(&Query, 32767, 1);

	err = loader->_BaseObjectCount(db.dataBase, Code,
		OPEN_TREE, &countWays);
	
	err = loader->_BaseInitObject(&Object, NULL, 0, NULL, 1,
		NULL, 1, NULL, 0, NULL, 0,
		NULL, 0, NULL, 0);

	Object.FlagVideo = Object.FlagVideo | OBJ_VIDEO_ALL;

	err = loader->_BaseReadObject(db.dataBase, &Query, NULL,
		OPEN_TREE, OBJ_METRIC,
		&Object, Code, 0);

	result = new GBaseWay[countWays];
	
	if (err == 0) {
		myfile << endl << "Object # " << 1 << " generating " << endl;
		result[0].way = readDataFromObject(Object, &countPoints, 0);
		result[0].size = countPoints;
	}
	
	for (int i = 1; err == 0; i++) {
		myfile << endl << "Object # " << i+1 << " generating " << endl;
		loader->_BaseCloseObject(&Object);
		err = loader->_BaseReadObject(db.dataBase, &Query, NULL,
			READ_NEXT, OBJ_FULL,
			&Object, Code, 0);
		if (err == 0) {
			if (i != 35 && i != 34 && i != 32) {
				result[i].way = readDataFromObject(Object, &countPoints, i);
				result[i].size = countPoints;
			}
		}
		
	}
		
	err = loader->_BaseClose(db.dataBase);
	
	delete loader;

	*n = countWays;
	myfile.close();
	return result;

}