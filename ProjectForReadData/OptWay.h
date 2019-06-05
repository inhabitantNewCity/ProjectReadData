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

	Way^ buildWay(List<int>^ indexesWay) {
		List<Line^>^ lines = gcnew List<Line^>();
		for (int i = 0; i < indexesWay->Count; i++) {
			lines->Add(map->getList()[i]);
		}
		return gcnew Way(lines);
	}

	List<List<int>^>^ buildGraph(Map^ map)
	{
		List<List<int>^>^ result = gcnew List<List<int>^>();
		List<Line^>^ lines = map->getList();
		for (int i = 0; i < lines->Count; i++) {
			List<int>^ relatedEdges = gcnew List<int>();
			List<Line^>^ relatedLines = map->getRelatedEges(lines[i]);
			for (int j = 0; j < relatedLines->Count; j++) {
				relatedEdges->Add(getIndex(relatedLines[j]));
			}
		}
		return result;
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

		List<int>^ parrent = gcnew List<int>(countVertex);
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
			List<int>^ path;
			
			for (int v = indexEnd; v != -1; v = path[v])
				path->Add(v);
			
			path->Reverse();
			
			return buildWay(path);
		}
		return nullptr;
	}

};