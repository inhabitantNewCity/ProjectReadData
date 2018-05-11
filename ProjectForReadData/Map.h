#include "Stubs.h"
#include <math.h>

#pragma once
using namespace System::Windows::Media;
using namespace System;
using namespace System::Windows::Shapes;
using namespace System::Collections::Generic;

#define ESP 10
#define PI 3.141592653589793238462643383279502884

ref class SecondaryInterval {
public:
	Line^ interval;
	Line^ prevInterval;
	
private:
	float angle;
	Windows::Point join_point;
	Windows::Point direction_point;

	double module(Line^ interval) {
		return sqrt(pow(interval->X2 - interval->X1, 2) + pow(interval->Y2 - interval->Y1, 2));
	}

	double scalr(Line^ interval1, Line^ interval2) {
		return (interval1->X2 - interval1->X1)*(interval2->X2 - interval2->X1) + (interval1->Y2 - interval1->Y1)*(interval2->Y2 - interval2->Y1);
	}
	Windows::Point projVectorToVector(Windows::Vector vector1, Windows::Vector vector2, Windows::Point startPoint) {
		vector2.Normalize();
		return startPoint + vector2 * Windows::Vector::Multiply(vector1, vector2);
	}
public:
	SecondaryInterval(Line^ interval, Line^ prevInterval) {
		this->interval = interval;
		this->prevInterval = prevInterval;

		float mod_int = module(interval);
		float mod_int_prev = module(prevInterval);
		float mod_mult = mod_int_prev*mod_int;
		float scalar = scalr(interval, prevInterval);
		if (scalar > mod_mult) {
			angle = acos(Math::Sign(scalar*mod_mult));
		}
		else {
			angle = Math::Acos(scalar / mod_mult);
		}
		
		float int_x1 = interval->X1;
		float int_Y1 = interval->Y1;
		float prev_x1 = prevInterval->X1;
		float prev_Y1 = prevInterval->Y1;

		float int_x2 = interval->X2;
		float int_Y2 = interval->Y2;
		float prev_x2 = prevInterval->X2;
		float prev_Y2 = prevInterval->Y2;

		if (interval->X1 == prevInterval->X1 && interval->Y1 == prevInterval->Y1) {
			join_point.X = interval->X1;
			join_point.Y = interval->Y1;
			direction_point.X = prevInterval->X2;
			direction_point.Y = prevInterval->Y2;
		}
		else if (interval->X2 == prevInterval->X2 && interval->Y2 == prevInterval->Y2) {
			join_point.X = interval->X2;
			join_point.Y = interval->Y2;
			direction_point.X = prevInterval->X1;
			direction_point.Y = prevInterval->Y1;
		}
		else if (interval->X2 == prevInterval->X1 && interval->Y2 == prevInterval->Y1) {
			join_point.X = interval->X2;
			join_point.Y = interval->Y2;
			direction_point.X = prevInterval->X2;
			direction_point.Y = prevInterval->Y2;
		}
		else if (interval->X1 == prevInterval->X2 && interval->Y1 == prevInterval->Y2) {
			join_point.X = interval->X1;
			join_point.Y = interval->Y1;
			direction_point.X = prevInterval->X1;
			direction_point.Y = prevInterval->Y1;
		}
	}

	double getAngle() {
		return angle;
	}

	Windows::Point getJoinPoint() {
		return join_point;
	}

	Windows::Point rotate(Windows::Point point) {
		Windows::Point result (point.X - join_point.X, point.Y - join_point.Y);
		
		float cos_a = Math::Cos(angle);
		float sin_a = Math::Sin(angle);
		if (direction_point.X < join_point.X)
			sin_a *= -1;

		float x = result.X;
		float y = result.Y;

		result.X = join_point.X + x * cos_a - y * sin_a;
		result.Y = join_point.Y + x * sin_a + y * cos_a;

		return result;
	}
};

ref class Map {
	
private:
	PathGeometry^ intervals;
	List<Line^>^ lines;
	Map^ optWay;
	Line^ currentInterval;
	List<SecondaryInterval^>^ usedLines;
	
public:
	
	Map(List<Line^>^ intervals) {
		this->lines = intervals;
		this->intervals = gcnew PathGeometry();
		for (int i = 0; i < intervals->Count; i++) {
			Windows::Point point1(intervals[i]->X1, intervals[i]->Y1);
			Windows::Point point2(intervals[i]->X2, intervals[i]->Y2);
			this->intervals->AddGeometry(gcnew LineGeometry(point1, point2));
		}
		if(intervals->Count)
			currentInterval = intervals[0];
		usedLines = gcnew List<SecondaryInterval^>();
	}

	bool intervalContains(Line^ line) { 
		Windows::Point point1(line->X1, line->Y1);
		Windows::Point point2(line->X2, line->Y2);
		return intervals->FillContains(gcnew LineGeometry( point1, point2));
	}
	
	bool isUsedLine(SecondaryInterval^ line) {
		for (int i = 0; i < usedLines->Count; i++) {
			if (usedLines[i]->interval->X1 == line->interval->X1 &&
				usedLines[i]->interval->Y1 == line->interval->Y1 &&
				usedLines[i]->interval->X2 == line->interval->X2 &&
				usedLines[i]->interval->Y2 == line->interval->Y2)
				return true;
		}
		return false;
	}

	bool isPointOnMap(Windows::Point point) {
		
		for (int i = 0; i < lines->Count; i++) {
			if (isFillIntervalsByIndex(lines[i], point, lines[i]))
				return true;
		}
		return false;
	}

	List<SecondaryInterval^>^ getRelatedEges(Line^ line){
		List<SecondaryInterval^>^ result = gcnew List<SecondaryInterval^>();
		for (int i = 0; i < lines->Count; i++) {
			
			if (line == lines[i]) {
				continue;
			}
			else if (line->X1 == lines[i]->X1 && line->Y1 == lines[i]->Y1) {
				SecondaryInterval^ intr = gcnew SecondaryInterval(lines[i], line);
				
				result->Add(intr);
				usedLines->Add(intr);
			
			}
			else if (line->X1 == lines[i]->X2 && line->Y1 == lines[i]->Y2 ) {
				SecondaryInterval^ intr = gcnew SecondaryInterval(lines[i], line);
				result->Add(intr);
				usedLines->Add(intr);
			}
		}
		return result;
	}

	bool isFillIntervalsByIndex(Line^ line, Windows::Point point, Line^ current_interval) {
		Windows::Point point1(line->X1, line->Y1);
		Windows::Point point2(line->X2, line->Y2);
		LineGeometry^ lineCheck = gcnew LineGeometry(point1, point2);
		return lineCheck->FillContains(point, ESP, ToleranceType::Absolute);
	}

	bool iFillNextIntervalsByInterval(Line^ line, Windows::Point) {
		return false;
	}

	Line^ getCurrentInterval(Line^ prev, Windows::Point point) {
		
			Windows::Point point1(prev->X1, prev->Y1);
			Windows::Point point2(prev->X2, prev->Y2);
			LineGeometry^ line = gcnew LineGeometry(point1, point2);

			int index = lines->IndexOf(prev);
			Line^ next = lines[index + 1 < lines->Count ? index + 1 : index];
			Windows::Point point1Next(next->X1, next->Y1);
			Windows::Point point2Next(next->X2, next->Y2);
			LineGeometry^ lineNext = gcnew LineGeometry(point1Next, point2Next);


			Line^ result;
			if (line->FillContains(point, ESP, ToleranceType::Absolute)) {
				result = prev;
			}
			else if (lineNext->FillContains(point, ESP, ToleranceType::Absolute)) {
				result = next;
			}
			return next;
	}


	void setOptimWay(Map^ way) {
		this->optWay = way;
	}

	Map^ getOptimWay() {
		return optWay;
	}

	List<Line^>^ getList() {
		return lines;
	}

};


ref class Way : public Map {
private:
	bool isOptim;
public: 
	Way(List<Line^>^ intervals) :Map(intervals) {
		isOptim = false;
	}

	void setOptimFlag(bool flag) {
		isOptim = flag;
	}
	bool isOptimWay() {
		return isOptim;
	}
};

ref class MapChecker {

private:
	Map^ map;
	Way^ way;
	List<Line^>^ intervals;
	List<Line^>^ prev_interval;
	int count_diveded;
	List<System::Collections::Generic::Stack<Windows::Point>^>^ stacks;
	Windows::Point divided_point;
	Windows::Point prev_point;
	List<Windows::Point>^ techicalPoints;
	List<Windows::Point>^ techicalPoints_prev;

	Windows::Point projVectorToVector(Windows::Vector vector1, Windows::Vector vector2, Windows::Point startPoint) {
		vector2.Normalize();
		return startPoint + vector2 * Windows::Vector::Multiply(vector1,vector2) ;
	}

	Windows::Point calculate_point(float x, float y, Line^ interval) {
		Windows::Point pointCheck(x, y);
		Windows::Vector vector1(x- interval->X1, y- interval->Y1);
		Windows::Vector vector2(interval->X2 - interval->X1, interval->Y1 - interval->Y2);
		Windows::Point startPoint(interval->X1, interval->Y1);

		Windows::Point point1(interval->X1, interval->Y1);
		Windows::Point point2(interval->X2, interval->Y2);
		LineGeometry^ lineCheck = gcnew LineGeometry(point2, point1);

		return lineCheck->FillContains(pointCheck, ESP, ToleranceType::Absolute) ? projVectorToVector(vector1, vector2, startPoint) : pointCheck;
	}
	
	void divide_calculation(Line^ prev, int count, Windows::Point current_point, List<SecondaryInterval^>^ related_intervals) {
		
		for (int i = 0; i < related_intervals->Count; i++) {
			techicalPoints_prev->Add(related_intervals[i]->rotate(current_point));
			prev_interval->Add(prev);
			intervals->Add(related_intervals[i]->interval); // добавляем разделённый интервал
			stacks->Add(gcnew System::Collections::Generic::Stack<Windows::Point>()); // добавление нового стека для хранения точек
			if (count_diveded == 0) {
				divided_point.X = related_intervals[i]->getJoinPoint().X; // сохранение точки разветления только один раз
				divided_point.Y = related_intervals[i]->getJoinPoint().Y;
			}
		}
	}
	
	bool check_interval_select_way(Line^ interval) {
		return way->intervalContains(interval);
	}
	
	Windows::Point update_way(Windows::Point point) {
		Windows::Point result = divided_point;

		for (int i = 0; i < stacks->Count; i++) { // итеративно опусташаем все ранее созданные стэки и вычисляем коректную точку		
			while (stacks[i]->Count != 0) {
				Windows::Point point = stacks[i]->Pop();
				result.X += point.X;
				result.Y += point.Y;
			}
		}

		Windows::Point end(way->getList()[way->getList()->Count - 1]->X2, way->getList()[way->getList()->Count - 1]->Y2);
		updateWay(result, end, way->getList());

		return result;

	}

public:
	MapChecker(List<Line^>^ map, List<Line^>^ way) {
		this->map = gcnew Map(map);
		this->way = gcnew Way(way);

		intervals = gcnew List<Line^>();
		
		prev_interval = gcnew List<Line^>();
		if(way->Count)
			prev_interval->Add(way[0]);
		count_diveded = 0;

		stacks = gcnew List<System::Collections::Generic::Stack<Windows::Point>^>();
		techicalPoints = gcnew List<Windows::Point>();
		techicalPoints_prev = gcnew List<Windows::Point>();
	}
	
	Windows::Point checkOnMap(float x, float y) {
		
		Windows::Point rowPoint(x, y);
		Windows::Point current_point(x, y);

		techicalPoints->Clear();
		techicalPoints->Add(rowPoint);
		for (int i = 0; i < techicalPoints_prev->Count; i++) {
			techicalPoints->Add(techicalPoints_prev[i]);
		}
		
		for (int i = 0; i < techicalPoints->Count; i++) {
			Line^ current_interval = map->getCurrentInterval(prev_interval[i], techicalPoints[i]);


			current_point = calculate_point(techicalPoints[i].X, techicalPoints[i].Y, current_interval);

			List<SecondaryInterval^>^ related_intervals = map->getRelatedEges(current_interval);
			if (related_intervals->Count > 1) { // разветвление алгоритма при разветлении пути
				divide_calculation(prev_interval[i], count_diveded, current_point, related_intervals);
				count_diveded++;

			}

			if (intervals->Count > 1)
			{
				for (int i = 0; i < intervals->Count; i++) {
					if (!map->isFillIntervalsByIndex(intervals[i], current_point, current_interval)) {
						intervals->RemoveAt(i); // удаление интервала из в случае не попадания точки в расматриваемый интервал
						techicalPoints_prev->RemoveAt(i);
					}
					else {
						stacks[i]->Push(current_point); 
					}
				}
			}

			if (intervals->Count == 1) {
				if (!check_interval_select_way(intervals[0])) // вычисляем текущий путь если оставшийся интервал не попадает в выбранный путь
					count_diveded = 0;
			}

			prev_point = current_point;
			prev_interval[i] = current_interval;
		}
		
		for (int i = 0; i < techicalPoints_prev->Count; i++) {
			techicalPoints->Add(techicalPoints_prev[i]);
		}

		return techicalPoints[0];
	}

	List<Windows::Point>^ getTechicalPoints() {
		List<Windows::Point>^ result = gcnew List<Windows::Point>();
		for (int i = 0; i < techicalPoints->Count; i++) {
			if (map->isPointOnMap(techicalPoints[i]))
				result->Add(techicalPoints[i]);
		}
		return result;
	}

	void updateCurentIntervals(List<Windows::Point>^ points) {
		if (points->Count > 2) {
			for (int i = 0; i < intervals->Count; i++) {
				bool isUsed = false;
				for (int j = 0; j < points->Count; j++) {
					if (map->isFillIntervalsByIndex(intervals[i], points[j], intervals[i]))
						isUsed = true;
				}
				if (!isUsed) {
					intervals->RemoveAt(i);
					techicalPoints->RemoveAt(i);
				}
			}
		}
	}
};