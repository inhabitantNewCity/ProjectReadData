#pragma once

#include "probability_cor_map.h"


ref class OptWayBuilder {

private:
	int countVertex;
	List<List<int>^>^ graph;
	Map^ map;

	int getIndex(Line^ line) {
		return map->getList()->IndexOf(line);
	}

	void reverseWay(List<Line^>^ lines) {
		for (int i = 0; i < lines->Count - 1; i++) {
			if (lines[i]->X2 != lines[i + 1]->X1 ||
				lines[i]->Y2 != lines[i + 1]->Y1) {
				float x1 = lines[i]->X1;
				float y1 = lines[i]->Y1;
				
				lines[i]->X1 = lines[i]->X2;
				lines[i]->Y1 = lines[i]->Y2;
				lines[i]->X2 = x1;
				lines[i]->Y2 = y1;
			}
		}
	}

	Way^ buildWay(List<int>^ indexesWay) {
		List<Line^>^ lines = gcnew List<Line^>();
		for (int i = 0; i < indexesWay->Count; i++) {
			lines->Add(map->getList()[indexesWay[i]]);
		}
		reverseWay(lines);
		return gcnew Way(lines);
	}

	List<List<int>^>^ buildGraph(Map^ map)
	{
		List<List<int>^>^ result = gcnew List<List<int>^>();
		List<Line^>^ lines = map->getList();
		for (int i = 0; i < lines->Count; i++) {
			List<int>^ relatedEdges = gcnew List<int>();
			result->Add(relatedEdges);
			List<Line^>^ relatedLines = map->getBothSideRelatedEges(lines[i]);
			for (int j = 0; j < relatedLines->Count; j++) {
				relatedEdges->Add(getIndex(relatedLines[j]));
			}
		}
		return result;
	}

	void initFalseArray(List<bool>^ used, int count) {
		for (int i = 0; i < count; i++) {
			used->Add(false);
		}
	}

	void initMinus1Array(List<int>^ parrent, int count) {
		for (int i = 0; i < count; i++) {
			parrent->Add(-1);
		}
	}

public: 
	
	OptWayBuilder(Map^ map) {
		this->countVertex = map->getList()->Count;
		this->map = map;
		this->graph = buildGraph(map);
	}

	Way^ getOptWay(Line^ start, Line^ end) {
		System::Collections::Generic::Queue<int>^ q = gcnew System::Collections::Generic::Queue<int>();
		int indexStart = getIndex(start);
		int indexEnd = getIndex(end);
		q->Enqueue(indexStart);

		List<bool>^ used = gcnew List<bool>(countVertex);
		initFalseArray(used, countVertex);

		List<int>^ parrent = gcnew List<int>(countVertex);
		initMinus1Array(parrent, countVertex);

		used[indexStart] = true;
		parrent[indexStart] = -1;
		while (q->Count != 0) {
			int v = q->Peek();
			q->Dequeue();
			for (size_t i = 0; i < graph[v]->Count; ++i) {
				int to = graph[v]->GetRange(i, 1)[0];
				if (!used[to]) {
					used[to] = true;
					q->Enqueue(to);
					parrent[to] = v;
				}
			}
		}
		if (!used[indexEnd])
			System::Windows::Forms::MessageBox::Show("No Path");
		else {
			List<int>^ path = gcnew List<int>();
			
			for (int v = indexEnd; v != -1; v = parrent[v])
				path->Add(v);
			
			path->Reverse();
			
			return buildWay(path);
		}
		return nullptr;
	}

};