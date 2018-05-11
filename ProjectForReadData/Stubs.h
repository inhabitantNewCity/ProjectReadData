#include "way_load.h"
#pragma once

using namespace System::Collections::Generic;
using namespace System::Windows::Shapes;
using namespace System::Windows::Forms;
using namespace System::IO;


void updateWay(System::Windows::Point start, System::Windows::Point finish, List<Line^>^ way) {
	way->Clear();

	char* basename = new char[11];
	basename[0] = 'C';
	basename[1] = 'O';
	basename[2] = 'R';
	basename[3] = 'P';
	basename[4] = '1';
	basename[5] = 'F';
	basename[6] = 'L';
	basename[7] = 'O';
	basename[8] = 'O';
	basename[9] = 'R';
	basename[10] = '2';
	basename[11] = '.';
	basename[12] = 'D';
	basename[13] = 'P';
	basename[14] = 'F';

	int countWay = 0;
	Coord start_c, finish_c;
	
	start_c.x_ = start.X;
	start_c.y_ = start.Y;
	finish_c.x_ = finish.X;
	finish_c.y_ = finish.Y;
	
	Coord* wayCor = GetWay(start_c, finish_c, &countWay, basename);
	way->Clear();

	for (int i = 0; i < countWay - 1; i++) {
		Line^ line = gcnew Line();
		line->X1 = (float)(wayCor[i].x_) / 10.0f;
		line->Y1 = (float)(wayCor[i].y_) / 10.0f;
		line->X2 = (float)(wayCor[i + 1].x_) / 10.0f;
		line->Y2 = (float)(wayCor[i + 1].y_) / 10.0f;
		way->Add(line);
	}
}


int simpleGenerate(List<Line^>^ map, List<Line^>^ way, PictureBox^ image) {
	
	Line^ line1 = gcnew Line();
	line1->X1 = 10;
	line1->Y1 = 10;
	line1->X2 = 10 + image->Width / 2;
	line1->Y2 = 10;
	map->Add(line1);
	way->Add(line1);

	Line^ line2 = gcnew Line();
	line2->X1 = line1->X2;
	line2->Y1 = line1->Y2;
	line2->X2 = line1->X2 + image->Width / 4;
	line2->Y2 = line1->Y2;
	map->Add(line2);

	Line^ line3 = gcnew Line();
	line3->X1 = line2->X2;
	line3->Y1 = line2->Y2;
	line3->X2 = line2->X2;
	line3->Y2 = line2->Y2 + image->Width / 4;
	map->Add(line3);

	Line^ line4 = gcnew Line();
	line4->X1 = line3->X2;
	line4->Y1 = line3->Y2;
	line4->X2 = line3->X2;
	line4->Y2 = line3->Y2 + image->Width / 2;
	map->Add(line4);

	Line^ line5 = gcnew Line();
	line5->X1 = line1->X2;
	line5->Y1 = line1->Y2;
	line5->X2 = line1->X2;
	line5->Y2 = line1->Y2 + image->Width / 4;
	map->Add(line5);
	way->Add(line5);

	Line^ line6 = gcnew Line();
	line6->X1 = line5->X2;
	line6->Y1 = line5->Y2;
	line6->X2 = line5->X2 + image->Width / 4;
	line6->Y2 = line5->Y2;
	map->Add(line6);
	way->Add(line6);
	way->Add(line4);

	return 0;
}

int databaseGenerate(List<Line^>^ map, List<Line^>^ way, PictureBox^ image) {
	
	char* basename = new char[11];
	basename[0] = 'C';
	basename[1] = 'O';
	basename[2] = 'R';
	basename[3] = 'P';
	basename[4] = '1';
	basename[5] = 'F';
	basename[6] = 'L';
	basename[7] = 'O';
	basename[8] = 'O';
	basename[9] = 'R';
	basename[10] = '2';
	basename[11] = '.';
	basename[12] = 'D';
	basename[13] = 'P';
	basename[14] = 'F';

	int countMap;
	int countWay;
	GBaseWay* mapCor = GetMap(&countMap, basename);
	Coord* wayCor = GetWay(mapCor[0].way[3], mapCor[0].way[mapCor[0].size - 1], &countWay, basename);

	StreamWriter^ sw = File::CreateText("../logs/LogLine.txt");
	sw->WriteLine("Replication data with parameter: count ways:" + countMap);
	
	for (int numberLine= 0, i = 0; i < countMap ; i++) {
		sw->WriteLine("Replication " + (i + 1) + " way data with parameter: count vertex:" + mapCor[i].size);
		for (int j = 0; j < mapCor[i].size-1; j++, numberLine++) {
			Line^ line = gcnew Line();
			line->X1 = (float)(mapCor[i].way[j].x_)/ 10.0f;
			line->Y1 = (float)(mapCor[i].way[j].y_) / 10.0f;
			line->X2 = (float)(mapCor[i].way[j+1].x_) / 10.0f;
			line->Y2 = (float)(mapCor[i].way[j+1].y_) / 10.0f;
			sw->WriteLine("Line: {("+ line->X1 +", " + line->Y1 + "), (" + line->X2 + ", " + line->Y2 + ")} number = " + numberLine);
			map->Add(line);
		}
	}

	for (int i = 0; i < countWay - 1; i++) {
		Line^ line = gcnew Line();
		line->X1 = (float)(wayCor[i].x_ )/ 10.0f;
		line->Y1 = (float)(wayCor[i].y_ )/ 10.0f;
		line->X2 = (float)(wayCor[i+1].x_ )/ 10.0f;
		line->Y2 = (float)(wayCor[i+1].y_ )/ 10.0f;
		way->Add(line);
	}

	delete[] mapCor;
	sw->Flush();
	return 0;

}
