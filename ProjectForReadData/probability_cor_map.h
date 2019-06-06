#pragma once
#include "Stubs.h"
#include "util.h"
#include <math.h>

#pragma once
using namespace System::Windows::Media;
using namespace System;
using namespace System::Windows::Shapes;
using namespace System::Collections::Generic;

#define ESP 10
#define ERR 0.1
#define CLOSED_ENVIRONS 10

//current class provide posibility to simple operation for vectors
ref class LineSegmentCustom {
private:
	//presented by point(derection and length) from (0,0) point 
	Windows::Vector line_segment;
	//point for stor information about start point for current vector
	Windows::Point start_point;
	
	//creating vector for two points
	static Windows::Vector transforPointToVector(Windows::Point point1, Windows::Point point2) {
		Windows::Vector vector(point2.X - point1.X, point2.Y - point1.Y);
		return vector;
	}
	// creating vector from simple Window Line
	static Windows::Vector transforLineToVector(Line^ line) {
		Windows::Vector vector(line->X2 - line->X1, line->Y2 - line->Y1);
		return vector;
	}
public:
	LineSegmentCustom(Windows::Vector vector, Windows::Point point) {
		this->line_segment = vector;
		this->start_point = point;
	}
	
	LineSegmentCustom(Windows::Point point1, Windows::Point point2) {
		this->start_point = point1;
		this->line_segment = LineSegmentCustom::transforPointToVector(point1, point2);
	}

	LineSegmentCustom(Line^ line) {
		Windows::Point first_point(line->X1, line->Y1);
		this->start_point = first_point;
		this->line_segment = LineSegmentCustom::transforLineToVector(line);
	}
	//standart geometry algorithm for calculating distance between to vectors(http://forum.algolist.ru/algorithm-geometry/661-ishu-algoritm-kratchaishee-rasstoianie-mejdu-otrezkami.html)
	//for current task we have assumption that start points qual for both vectors
	double getDistanceBetweenVectors(Windows::Vector vector) {
		Windows::Vector neg_line_segment = line_segment;
		neg_line_segment.Negate();
		Windows::Vector supVector = Windows::Vector::Subtract(this->line_segment, vector);
		if (Windows::Vector::Multiply(vector, line_segment) <= 0) {
			return vector.Length;
		}
		else if (Windows::Vector::Multiply(supVector, neg_line_segment) >= 0) {
			return supVector.Length;
		}
		return Windows::Vector::Multiply(vector, line_segment) / line_segment.Length;
	}

	double getAngleBetweenVectores(Windows::Vector vector) {
		return PI * Windows::Vector::AngleBetween(line_segment, vector) / 180.0;
	}

	Windows::Vector getVector() {
		return line_segment;
	}

	double getDistanceBetweenVectors(LineSegmentCustom^ source) {
		Windows::Vector vector = source->getVector();
		return this->getDistanceBetweenVectors(vector);
	}

	double getAngleBetweenVectores(LineSegmentCustom^ source) {
		Windows::Vector vector = source->getVector();
		return this->getAngleBetweenVectores(vector);
	}

	Windows::Point getStartPoint() {
		return start_point;
	}

	void setStartPoint(Windows::Point point) {
		this->start_point = point;
	}

	double getModule() {
		return line_segment.Length;
	}
	//turning current vector by angles matrix
	LineSegmentCustom^ turnVector(double angle) {
		double x = Math::Round(this->line_segment.X * cos(angle) - this->line_segment.Y * sin(angle), 2);
		double y = Math::Round(this->line_segment.X * sin(angle) + this->line_segment.Y * cos(angle), 2);
		Windows::Vector vector(x, y);
	
		return gcnew LineSegmentCustom(vector, this->start_point);
	}
	//move current point to the vector
	Windows::Point shiftPointToVector(Windows::Point point) {
		Windows::Point result(point.X + line_segment.X, point.Y + line_segment.Y);
		return result;
	}

	LineSegmentCustom^ subtruck(LineSegmentCustom^ segment) {
		return gcnew LineSegmentCustom(Windows::Vector::Subtract(this->line_segment, segment->getVector()), this->start_point);
	}

	//using for calculating destination between vectors. calclulating mapping to current path
	LineSegmentCustom^ procFromVector(LineSegmentCustom^ segment) {
		LineSegmentCustom^ proc2way = segment->turnVector(segment->getAngleBetweenVectores(this));
		this->start_point = proc2way->shiftPointToVector(this->start_point);
		return gcnew LineSegmentCustom(Windows::Vector::Subtract(this->line_segment, proc2way->getVector()), this->start_point);
	}

	Line^ getLine() {
		Line^ result = gcnew Line();

		result->X1 = start_point.X;
		result->Y1 = start_point.Y;
		result->X2 = start_point.X + line_segment.X;
		result->Y2 = start_point.Y + line_segment.Y;

		return result;
	}
};


//store information about lines which system find out in database
ref class Map {

protected:
	List<Line^>^ lines;
public:
	
	Map(List<Line^>^ intervals) {
		this->lines = intervals;
		for (int i = 0; i < intervals->Count; i++) {
			Windows::Point point1(intervals[i]->X1, intervals[i]->Y1);
			Windows::Point point2(intervals[i]->X2, intervals[i]->Y2);
		}
	}

	List<Line^>^ getRelatedEges(Line^ line){

		List<Line^>^ result = gcnew List<Line^>();
		for (int i = 0; i < lines->Count; i++) {
			
			if (line == lines[i])
				continue;
			else if (line->X2 == lines[i]->X1 && line->Y2 == lines[i]->Y1)
				result->Add(lines[i]);
			else if (line->X2 == lines[i]->X2 && line->Y2 == lines[i]->Y2 ) 
				result->Add(lines[i]);
		}
		return result;
	}

	List<Line^>^ getBothSideRelatedEges(Line^ line) {

		List<Line^>^ result = gcnew List<Line^>();
		for (int i = 0; i < lines->Count; i++) {

			if (line == lines[i])
				continue;
			else if (line->X2 == lines[i]->X1 && line->Y2 == lines[i]->Y1)
				result->Add(lines[i]);
			else if (line->X2 == lines[i]->X2 && line->Y2 == lines[i]->Y2)
				result->Add(lines[i]);
			else if (line->X1 == lines[i]->X1 && line->Y1 == lines[i]->Y1)
				result->Add(lines[i]);
			else if (line->X1 == lines[i]->X2 && line->Y1 == lines[i]->Y2)
				result->Add(lines[i]);
		}
		return result;
	}
	List<Line^>^ getList() {
		return lines;
	}

	Windows::Point getNearestPoint(Windows::Point source_point) {
		/// TODO: get nerest point of current map
		return source_point;
	}

	Line^ getLineByPoint(Windows::Point source_point) {
		for (int i = 0; i < lines->Count; i++) {
			if (GeomUtill::isPointInhereLine(source_point, lines[i]))
				return lines[i];
		}
		return nullptr;
	}

};

//provide information about selecting way
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
	Line^ getNext(List<Line^>^ neboures) {

		for (int i = 0; i < neboures->Count; i++) {
			for (int j = 0; j < lines->Count; j++) {
				if ((neboures[i]->X1 == lines[j]->X1 &&
					neboures[i]->Y1 == lines[j]->Y1) &&
					(neboures[i]->X2 == lines[j]->X2 &&
					neboures[i]->Y2 == lines[j]->Y2))
					return neboures[i];
			}
		}
		return nullptr;
	}

	Windows::Point getLastPoint() {
		Windows::Point result(lines[lines->Count - 1]->X2, lines[lines->Count - 1]->Y2);
		return result;
	}

};
// class store information about current  state of one of the used way
ref class CurrentStateWay: IComparable {
private:
	//value probability of current way is corrected 
	double probability;
	
	//current line when user presumably is 
	LineSegmentCustom^ line_segment;
	bool is_primary_way;
	Map^ map;
	Way^ way;
	
	//nearest of current nebourses (lines which have one equaled point)
	List<CurrentStateWay^>^ nebourses;
	
	//service for calculating and update probability in each step
	CalculaterProbability^ calculater_probability;
	
	bool is_updated;
	bool is_cloesed;


	double getCurrentProbability(LineSegmentCustom^ way_line_segment, LineSegmentCustom^ current_line_segment, double current_probability) {
		double angle = way_line_segment->getAngleBetweenVectores(current_line_segment);
		double distance = way_line_segment->getDistanceBetweenVectors(current_line_segment);
		
		//correcting current probability acording to information about current angle between ideal position of user and current position
		return current_probability * calculater_probability->getCurrentValue(angle, distance);
	}

	//method zeroning current error on turn
	//checker: if angle between current line and next line = pi that shift start point. 
	//if it is false then get start point form current line
	Line^ zeroning_error(Line^ current_line) {

		Line^ result = gcnew Line();
		LineSegmentCustom^ way_line = gcnew LineSegmentCustom(current_line);
		if (line_segment->getAngleBetweenVectores(way_line) > PI - ERR && line_segment->getAngleBetweenVectores(way_line) < PI + ERR) {
			result->X1 = line_segment->getStartPoint().X;
			result->Y1 = line_segment->getStartPoint().Y;
			return result;
		}	
		return current_line;
	};

	List<Line^>^ deletingUselessNebourses(List<Line^>^ lines, LineSegmentCustom^ segment) {
		Windows::Point startPoint = segment->getStartPoint();
		for (int i = 0; i < lines->Count; i++) {
			if (GeomUtill::isPointInhereLine(startPoint, lines[i]))
				lines->RemoveAt(i);
		}
		return lines;
	}

	Line^ getNextline(List<Line^>^ lines) {
		if (is_primary_way)
			return way->getNext(lines);
		if(lines->Count != 0)
			return lines[0];

	}
	LineSegmentCustom^ findCurrentLine(float angle, float length) {
		
		if (line_segment->getModule() > length)
			return line_segment;
		
		//get next, getting neibours, work with flag 
		
		is_updated = true;
		List<Line^>^ lines = map->getRelatedEges(line_segment->getLine());
		
		// deliting current line from ralated
		lines = deletingUselessNebourses(lines, line_segment);

		//selecting next lines among related enges 
		Line^ result = getNextline(lines);
		if (result == nullptr) {
			is_cloesed = true;
			is_updated = false;
			return nullptr;
		}

		lines->Remove(result);
		
		//it is neccessary for corrected probability
		result = zeroning_error(result);

		nebourses = gcnew List<CurrentStateWay^>();		
		for (int i = 0; i < lines->Count; i++) {
			LineSegmentCustom^ segment = gcnew LineSegmentCustom(lines[i]);
			nebourses->Add(gcnew CurrentStateWay(segment, probability, false, map, way, calculater_probability));
		}

		return gcnew LineSegmentCustom(result);
	};

	Windows::Point buildPoint(float angle, float length) {
		Windows::Vector vector(length, 0);
		LineSegmentCustom^ line = gcnew LineSegmentCustom(vector, line_segment->getStartPoint());
		line = line->turnVector(angle);
		return line->shiftPointToVector(line->getStartPoint());
	}

public:
	CurrentStateWay(LineSegmentCustom^ line, double probability, bool is_primary_way, Map^ map, Way^ way, CalculaterProbability^ calculater_probability) {
		this->line_segment = line;
		// FOR_TEST: 
		//this->line_segment = gcnew LineSegmentCustom(Windows::Point(1,1), Windows::Point(1, 5));

		this->probability = probability;
		this->is_primary_way = is_primary_way;
		this->map = map;
		this->way = way;
		this->calculater_probability = calculater_probability;
		this->is_cloesed = false;
	};

	//update probability and line if it is necessary line equals (first point  = last point(input point) in line segment, second point = second point from interval)
	void updateCurrentState(float angle, float length) {
		// FOR_TEST: 
		//point.X = 4;
		//point.Y = 5;
		//way->getNext(gcnew List<Line^>());
		
		is_updated = false;

		this->line_segment = findCurrentLine(angle, length);
		if (line_segment == nullptr)
			return;

		Windows::Point point = buildPoint(angle, length);

		LineSegmentCustom^ current_segment = gcnew LineSegmentCustom(line_segment->getStartPoint(), point);
		this->probability = getCurrentProbability(line_segment, current_segment, probability);

		//shift start point from line_segment to modelue from current vector
		this->line_segment = this->line_segment->procFromVector(current_segment);

	};
	//flag for true if line is updated
	bool isLineUpdated() { return is_updated; };

	//get nebourses without update for current point
	List<CurrentStateWay^>^ getNebourses() {
		return nebourses;
	};
	double getProbability() {
		return probability;
	};
	bool getPrimaryWay() {
		return is_primary_way;
	};
	// get first point from current line 
	Windows::Point getCurrentPoint() {
		return line_segment->getStartPoint();
	};

	void setProbability(double probability) {
		this->probability = probability;
	};
	void setPrimaryWay(bool flag) {
		this->is_primary_way = flag;
	};

	bool isClosed() {
		return is_cloesed;
	}

	int CompareTo(Object^ state) override {
		CurrentStateWay^ stateS = (CurrentStateWay^)state;
		Double^ this_porbability = gcnew Double(probability);
		Double^ state_porbability = gcnew Double(stateS->getProbability());
		return -1 * this_porbability->CompareTo(state_porbability);
	}
	//check if current line not has nebourses
	//and current point belongs to e-environs 
	bool isLastStep(Windows::Point point) {
		List<Line^>^ cur_nebourses = map->getRelatedEges(line_segment->getLine());
		bool is_last_segment = way->getNext(cur_nebourses)==nullptr;

		Windows::Vector way_vector(line_segment->getLine()->X2, line_segment->getLine()->Y2);
		Windows::Vector point_vector(point.X, point.Y);

		bool is_last_step = Windows::Vector::Subtract(way_vector, point_vector).Length < CLOSED_ENVIRONS;
		return is_last_segment && is_last_step;
	}

	String^ ToText() {
		
		String^ result = "p = " + probability + " is Primary = " + is_primary_way;
		
		if (line_segment != nullptr) {
			result += "(x = " + line_segment->getStartPoint().X + ", y = " + line_segment->getStartPoint().Y + ")";
		}

		return result;
	}
};
//provide result of mapping current information from PPS to map
ref class PredictionResult
{
public:
	//calculated point
	Windows::Point point;
	//flag provided information that current way was changed
	bool is_rebilded;
	//way is finished user aimed end of way
	bool is_closed;

	PredictionResult() {};
	PredictionResult(Windows::Point point, bool is_rebilded, bool is_closed) {
		this->point = point;
		this->is_rebilded = is_rebilded;
		this->is_closed = is_closed;
	};
};


//new map checker
ref class ProbabilityMapChecker {
private:
	const static int OPT_WAYS_NUMBER = 6;

	List<CurrentStateWay^>^ current_states;
	
	Map^ map;
	Way^ way;
	
	CalculaterProbability^ calculater_probability;

	//techical methods
	//norming dispensation of Probability to current wais
	void normalizeState() {
		double sum = 0;
		
		for (int i = 0; i < OPT_WAYS_NUMBER && i < current_states->Count; i++)
			sum += current_states[i]->getProbability();
		for (int i = 0; i < OPT_WAYS_NUMBER && i < current_states->Count; i++)
			current_states[i]->setProbability(current_states[i]->getProbability() / sum);
	}
	void removeClosedWays() {
		List<CurrentStateWay^>^ closeds = gcnew List<CurrentStateWay^>();
		for (int i = 0; i < current_states->Count; i++)
			if (current_states[i]->isClosed())
				closeds->Add(current_states[i]);

		for (int i = 0; i < closeds->Count; i++)
			current_states->Remove(closeds[i]);
	}
	void normalize() {
		removeClosedWays();
		current_states->Sort();
		if (current_states->Count.CompareTo(OPT_WAYS_NUMBER) == 1 )
			current_states = current_states->GetRange(0, OPT_WAYS_NUMBER);
		normalizeState();
	};
	
	// get max from array, get start point and return them.
	// also provide information abaut way
	PredictionResult^ takeSolution() {
		CurrentStateWay^ currentSate = current_states[0];
		Windows::Point result = currentSate->getCurrentPoint();
		return gcnew PredictionResult(result, !currentSate->getPrimaryWay(), currentSate->isLastStep(result));
	};

public:
	ProbabilityMapChecker(Map^ map, Way^ way) {
		this->map = map;
		this->way = way;

		calculater_probability = gcnew CalculaterProbability(3, 1, gcnew NormalizedDistribution(0, 2), gcnew NormalizedDistribution(0, 0.6));

		current_states = gcnew List<CurrentStateWay^>(OPT_WAYS_NUMBER);
		LineSegmentCustom^ primary_way = gcnew LineSegmentCustom(way->getList()[0]);
		current_states->Add(gcnew CurrentStateWay(primary_way, 1, true, this->map, this->way, this->calculater_probability));
	};
	
	// main method for prediction current state
	PredictionResult^ checkOnMap(float angle, float length) {
		
		
		for (int i = 0; i < current_states->Count; i++) {
			current_states[i]->updateCurrentState(angle, length);
			if (current_states[i]->isLineUpdated()) {
				current_states->AddRange(current_states[i]->getNebourses());
			}
		}

		normalize();
		return takeSolution();
	};

	// clear currrent states
	void refreshChecker(Way^ way) {
		current_states->Clear();
		LineSegmentCustom^ primary_way = gcnew LineSegmentCustom(way->getList()[0]);
		current_states->Add(gcnew CurrentStateWay(primary_way, 1, true, this->map, this->way, this->calculater_probability));
	}

	//getting all current point(in additionals wais too)
	List<Windows::Point>^ getAllCurrentPoints() {
		List<Windows::Point>^ points = gcnew List<Windows::Point>();
		for (int i = 0; i < current_states->Count; i++) {
			points->Add(current_states[i]->getCurrentPoint());
		}
		return points;
	}

	Windows::Point getNearestPoint(Windows::Point source_point) {
		return map->getNearestPoint(source_point);
	}

	Windows::Point getLastPointOnWay() {
		return way->getLastPoint();
	}

	List<CurrentStateWay^>^ getCurrentSates() {
		return current_states;
	}
};
