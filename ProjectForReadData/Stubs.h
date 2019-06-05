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

	Line^ line105 = gcnew Line();
	line105->X1 = line1->X2 / 4;
	line105->Y1 = line1->Y1;
	line105->X2 = line1->X2 / 4;
	line105->Y2 = line1->Y2 + 100;
	map->Add(line105);

	Line^ line11 = gcnew Line();
	line11->X1 = line1->X2 / 2;
	line11->Y1 = line1->Y1;
	line11->X2 = line1->X2 / 2;
	line11->Y2 = line1->Y2 + 50;
	map->Add(line11);

	Line^ line12 = gcnew Line();
	line12->X1 = line11->X2 ;
	line12->Y1 = line11->Y2;
	line12->X2 = line11->X2 + 50;
	line12->Y2 = line11->Y2;
	map->Add(line12);

	Line^ line13 = gcnew Line();
	line13->X1 = line12->X2;
	line13->Y1 = line12->Y2;
	line13->X2 = line12->X2;
	line13->Y2 = line12->Y2 + 100;
	map->Add(line13);

	Line^ line13l = gcnew Line();
	line13l->X1 = line13->X2;
	line13l->Y1 = line13->Y2 - 50;
	line13l->X2 = line13->X2 - 60;
	line13l->Y2 = line13->Y2 - 50;
	map->Add(line13l);

	Line^ line13r = gcnew Line();
	line13r->X1 = line13->X2;
	line13r->Y1 = line13->Y2;
	line13r->X2 = line13->X2 + 50;
	line13r->Y2 = line13->Y2;
	map->Add(line13r);

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

	Line^ line4d1 = gcnew Line();
	line4d1->X1 = line3->X2 - 60;
	line4d1->Y1 = line3->Y2;
	line4d1->X2 = line3->X2 - 60;
	line4d1->Y2 = line3->Y2 + 60;
	map->Add(line4d1);

	Line^ line4d2 = gcnew Line();
	line4d2->X1 = line3->X2 - 30;
	line4d2->Y1 = line3->Y2;
	line4d2->X2 = line3->X2 - 30;
	line4d2->Y2 = line3->Y2 + 40;
	map->Add(line4d2);

	Line^ line5 = gcnew Line();
	line5->X1 = line1->X2;
	line5->Y1 = line1->Y2;
	line5->X2 = line1->X2;
	line5->Y2 = line1->Y2 + image->Width / 4;
	map->Add(line5);
	way->Add(line5);

	Line^ line5l = gcnew Line();
	line5l->X1 = line5->X2;
	line5l->Y1 = line5->Y2 / 4;
	line5l->X2 = line5->X2 - 60;
	line5l->Y2 = line5->Y2 / 4;
	map->Add(line5l);

	Line^ line5ld = gcnew Line();
	line5ld->X1 = line5l->X2 + 30;
	line5ld->Y1 = line5l->Y2;
	line5ld->X2 = line5l->X2 + 30;
	line5ld->Y2 = line5l->Y2 + 30;
	map->Add(line5ld);

	Line^ line51 = gcnew Line();
	line51->X1 = line5->X2;
	line51->Y1 = line5->Y2 / 2;
	line51->X2 = line5->X2 + 50;
	line51->Y2 = line5->Y2 / 2;
	map->Add(line51);

	Line^ line52 = gcnew Line();
	line52->X1 = line51->X2;
	line52->Y1 = line51->Y2;
	line52->X2 = line51->X2;
	line52->Y2 = line51->Y2 - 40;
	map->Add(line52);

	Line^ line53 = gcnew Line();
	line53->X1 = line51->X2;
	line53->Y1 = line51->Y2;
	line53->X2 = line51->X2;
	line53->Y2 = line51->Y2 + 40;
	map->Add(line53);

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
	sw->WriteLine("Opt way");
	for (int i = countWay - 1; i > 0 ; i--) {
		Line^ line = gcnew Line();
		line->X1 = (float)(wayCor[i].x_ )/ 10.0f;
		line->Y1 = (float)(wayCor[i].y_ )/ 10.0f;
		line->X2 = (float)(wayCor[i-1].x_ )/ 10.0f;
		line->Y2 = (float)(wayCor[i-1].y_ )/ 10.0f;
		way->Add(line);
		sw->WriteLine("Line: {(" + line->X1 + ", " + line->Y1 + "), (" + line->X2 + ", " + line->Y2 + ")}");
	}

	delete[] mapCor;
	sw->Flush();
	return 0;

}
