#pragma once

#include "wifiListener.h" 
#include "com_port.h"

using namespace System;
using namespace System::Windows::Shapes;
using namespace System::Management;
using namespace System::Collections::Generic;
using namespace System::Xml;
using namespace System::Data;


ref class WifiPk
{
public:
	String^ MAC_ADDRESS;
	int SignelLevel;
};

ref class WifiWorker {
protected:
	XmlDocument^ doc;
	XmlWriter ^writer;
	bool Ready;
	String^ initialString = "<root isReady=\"false\">\n</root>";
	StreamWriter^ LOGER;
	String^ filename;
	String^ POINT;
	String^ MAC;

public:
	WifiWorker(String^ filename, String^ logFileName):filename(filename) {
		
		File::Delete(logFileName);
		LOGER = File::CreateText(logFileName);

		POINT = "wifiPoint";
		MAC = "mac";

		this->Ready = File::Exists(filename);
		doc = gcnew XmlDocument();
		if (!Ready) {
			doc->LoadXml(initialString);
			doc->Save(filename);
		}
		else {
			doc->Load(filename);
		}
	}

	virtual List<WifiPk^>^ getCurrentSignal() {
		int size = 0;
		List<WifiPk^>^ result = gcnew List<WifiPk^>();
		WIFIPoint* source = getWIFIPoints(&size);
		for (int i = 0; i < size; i++) {
			String^ Mac = gcnew String("");
			WifiPk^ point = gcnew WifiPk();
			Mac = Convert::ToString(source[i].ssid[0]);
			for (int j = 1; j < source[i].size; j++) {
				Mac += Mac + ":" + source[i].ssid[j];
			}
			point->MAC_ADDRESS = Mac;
			point->SignelLevel = source[i].level;
			result->Add(point);
		}
		return result;
	}
		
	
	
	List<Windows::Point>^ getSquery(List<WifiPk^>^  readed, Windows::Point current_point) {
		

		List<Windows::Point>^ result = gcnew List<Windows::Point>();
		XmlNode^ root = doc->DocumentElement;

		XmlNodeList^ list = root->ChildNodes;
		for (int i = 0; i < list->Count; i++) {
			for (int j = 0; j < readed->Count; j++) {
				if (list[i]->Attributes[MAC]->Value->Equals(readed[j]->MAC_ADDRESS)
					&& list[i]->Attributes["level"]->Value->Equals(Convert::ToString(readed[j]->SignelLevel))) {

					String^ x_str = list[i]->ChildNodes[0]->InnerText;
					String^ y_str = list[i]->ChildNodes[1]->InnerText;
					Windows::Point point(Convert::ToDouble(x_str), Convert::ToDouble(y_str));
					result->Add(point);
					LOGER->WriteLine("Signal Level = " + readed[j]->SignelLevel + " Point: (" + x_str + ", " + y_str + ")" );
				}
			}
		}
		updateCurrentState(current_point, readed);
		return result;
	}

	List<Line^>^ getNeighbors(List<WifiPk^>^  readed, Windows::Point current_point) {
		int* min = new int[readed->Count], *max = new int[readed->Count];
		int minLevel = 101, maxLevel = -101;
		List<Windows::Point> ^minPoint = gcnew List<Windows::Point>(readed->Count),
			^maxPoint = gcnew List<Windows::Point>(readed->Count);

		for (int i = 0; i < readed->Count; i++) {
			min[i] = 0;
			max[i] = 0;
			minPoint->Add(current_point);
			maxPoint->Add(current_point);
		}

		List<Line^>^ result = gcnew List<Line^>();
		XmlNode^ root = doc->DocumentElement;

		XmlNodeList^ list = root->ChildNodes;
		for (int i = 0; i < list->Count; i++) {
			for (int j = 0; j < readed->Count; j++) {
				if (list[i]->Attributes[MAC]->Value->Equals(readed[j]->MAC_ADDRESS)) {
					
					int delta = Convert::ToDouble(list[i]->Attributes["level"]->Value) - readed[j]->SignelLevel;
					if (delta > 0) {
						if (delta < min[j]) {
							min[j] = delta;
							String^ x_str = list[i]->ChildNodes[0]->InnerText;
							String^ y_str = list[i]->ChildNodes[1]->InnerText;
							maxPoint[j].X = Convert::ToDouble(x_str);
							maxPoint[j].Y = Convert::ToDouble(y_str);
						}
					}
					else {
						if (delta > max[j]) {
							max[j] = delta;
							String^ x_str = list[i]->ChildNodes[0]->InnerText;
							String^ y_str = list[i]->ChildNodes[1]->InnerText;
							minPoint[j].X = Convert::ToDouble(x_str);
							minPoint[j].Y = Convert::ToDouble(y_str);
						}
					}
				}
			}
		}
		for (int i = 0; i < readed->Count; i++) {
			Line^ line = gcnew Line();
			line->X1 = minPoint[i].X;
			line->Y1 = minPoint[i].Y;
			line->X2 = maxPoint[i].X;
			line->Y2 = maxPoint[i].Y;
			LOGER->WriteLine("Signal Level = " + readed[i]->SignelLevel + "min point: (" + minPoint[i].X + ", " + minPoint[i].Y + ") max point: (" + maxPoint[i].X + ", " + maxPoint[i].Y + ")");
			result->Add(line);
		}


		updateCurrentState(current_point, readed);
		return result;
	}

	bool isReady() {
		return this->Ready;
	}

	void updateCurrentState(Windows::Point current_point, List<WifiPk^>^ state) {
		
		XmlNode^ root = doc->DocumentElement;
		for (int i = 0; i < state->Count; i++) {
			XmlElement^ elem = doc->CreateElement(POINT);

			XmlAttribute^ mac = doc->CreateAttribute(MAC);
			XmlAttribute^ signal = doc->CreateAttribute("level");
			mac->Value = state[i]->MAC_ADDRESS;
			signal->Value = Convert::ToString(state[i]->SignelLevel);
			
			XmlElement^ x = doc->CreateElement("x");
			XmlElement^ y = doc->CreateElement("y");
			x->InnerText = Convert::ToString(current_point.X);
			y->InnerText = Convert::ToString(current_point.Y);

			elem->Attributes->Append(mac);
			elem->Attributes->Append(signal);
			elem->AppendChild(x);
			elem->AppendChild(y);
			root->AppendChild(elem);
		}
		doc->Save(filename);
	}

	~WifiWorker() {
		LOGER->Close();
	}
};


ref class MagneticWolker : public WifiWorker {

public:
	MagneticWolker(String^ filename, String^ logFileName) :WifiWorker(filename, logFileName) {
		POINT = "MagPoint";
		MAC = "asics";
	}

	virtual List<WifiPk^>^  getCurrentSignal() override {
		char* buff = new char[28];
		int statusResponseRead = Read(buff, 28);

		float comX = (float)((buff[17] << 8) + buff[16]) / 100.0f;
		float comY = (float)((buff[19] << 8) + buff[18]) / 100.0f;
		float comZ = (float)((buff[21] << 8) + buff[20]) / 100.0f;

		List<WifiPk^>^ result = gcnew List<WifiPk^>();
		
		WifiPk^ Pk_x = gcnew WifiPk();
		String^ mac_x = "1";
		Pk_x->MAC_ADDRESS = mac_x;
		Pk_x->SignelLevel = comX;
		result->Add(Pk_x);

		WifiPk^ Pk_y = gcnew WifiPk();
		String^ mac_y = "2";
		Pk_y->MAC_ADDRESS = mac_y;
		Pk_y->SignelLevel = comY;
		result->Add(Pk_y);

		WifiPk^ Pk_z = gcnew WifiPk();
		String^ mac_z = "3";
		Pk_z->MAC_ADDRESS = mac_z;
		Pk_z->SignelLevel = comZ;
		result->Add(Pk_z);

		return result;
	}
};