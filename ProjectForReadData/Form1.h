#pragma once

#include "com_port.h"
#include "math.h"
#include "filtering.h"
#include "Stubs.h"
#include "probability_cor_map.h"
#include "way_load.h"
#include "WifiFingerPrinting.h"


#define EPS 2

using namespace std;
namespace ProjectForReadData {

	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms::DataVisualization::Charting;
	using namespace System::Data;
	using namespace System::IO;
	using namespace System::Xaml;

	/// <summary>
	/// —водка дл€ Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//

			pointsX = gcnew List<float>();
			pointsY = gcnew List<float>();

			firstPhaseOfStepDetected = true;
			secondPhaseOfStepDetected = false;
			thridPhaseOfStepDetected = false;

			current_time = 0;
			// const
			CONST_G = 9.8f;

			CONST_ACCELERATION_MIN = 0.8f * CONST_G;
			CONST_H_MIN = 0.6 * CONST_G;

			CONST_ACCELERATION_MAX = 2.3 * CONST_G;
			CONST_H_MAX = 2.0 * CONST_G;

			//set max and min value of accelerations
			double angleBettwenLegOnSecondPhaseAndNormal = PI / 6;
			double angleBettwenLegOnThridPhaseAndNormal = PI / 3;
			this->deltaTime = 100;
			UIOffset_x = 103.4f;
			UIOffset_y = 378.7f;

			SumRecivedAcceleration = 0;
			countStep = 0;

			// set max min for first phase
			accMinVerticalForFirstPart = -2.0f;
			accMaxVerticalForFirstPart = 2.0f;
			accMaxHorizontalForFirstPart = 1.0f;
			accMinHorizontalForFirstPart - 7.0f;

			// set max, min for second phase		
			accMinVerticalForSecondPart = -6.0f;//(CONST_ACCELERATION_MIN)* cos(angleBettwenLegOnSecondPhaseAndNormal) - ((CONST_ACCELERATION_MIN - CONST_G) * sin(angleBettwenLegOnSecondPhaseAndNormal)) - 6.5f;
			accMaxVerticalForSecondPart = -0.9234567f;//(CONST_ACCELERATION_MAX)* cos(angleBettwenLegOnSecondPhaseAndNormal) - ((CONST_ACCELERATION_MAX - CONST_G) * sin(angleBettwenLegOnSecondPhaseAndNormal)) - 6.5f;
			accMinHorizontalForSecondPart = -8.0f;// ((CONST_H_MIN)* sin(angleBettwenLegOnSecondPhaseAndNormal)) + ((CONST_H_MIN - CONST_G) * cos(angleBettwenLegOnSecondPhaseAndNormal));
			accMaxHorizontalForSecondPart = -0.5f; ((CONST_H_MAX)* sin(angleBettwenLegOnSecondPhaseAndNormal)) + ((CONST_H_MAX - CONST_G) * cos(angleBettwenLegOnSecondPhaseAndNormal));

			//set max, min for thrid phase
			accMaxVerticalForThridPart = 6.0f;// (CONST_ACCELERATION_MAX - CONST_G) * sin(angleBettwenLegOnThridPhaseAndNormal) + CONST_ACCELERATION_MAX * cos(angleBettwenLegOnThridPhaseAndNormal) - 2.0f;
			accMinVerticalForThridPart = 0.945678f; //(CONST_ACCELERATION_MIN - CONST_G) * sin(angleBettwenLegOnThridPhaseAndNormal) + CONST_ACCELERATION_MIN * cos(angleBettwenLegOnThridPhaseAndNormal) - 2.0f;
			accMaxHorizontalForThridPart = 0.8f;// ((CONST_H_MAX - CONST_G) * cos(angleBettwenLegOnThridPhaseAndNormal)) - ((CONST_H_MAX)* sin(angleBettwenLegOnThridPhaseAndNormal)) + 7.0f;
			accMinHorizontalForThridPart = 6.0f;// ((CONST_H_MIN - CONST_G) * cos(angleBettwenLegOnThridPhaseAndNormal)) - ((CONST_H_MIN)* sin(angleBettwenLegOnThridPhaseAndNormal)) + 7.0f;

			double firstAccelerationY = 0;

			x_patern = gcnew List<float>();
			y_patern = gcnew List<float>();

			x_current = gcnew List<float>();
			y_current = gcnew List<float>();

			way = gcnew List<Line^>();
			map = gcnew List<Line^>();
			points = gcnew List<Windows::Point>();
			techicalPoints = gcnew List<Windows::Point>();
			wifis = gcnew List<Windows::Point>();

			pen = gcnew System::Drawing::Pen(System::Drawing::Color::Black, 2);
			penMap = gcnew System::Drawing::Pen(System::Drawing::Color::DeepPink, 1);
			penWay = gcnew System::Drawing::Pen(System::Drawing::Color::Blue, 2);
			penPoints = gcnew System::Drawing::Pen(System::Drawing::Color::Green, 3);
			brashPoints = gcnew System::Drawing::SolidBrush(System::Drawing::Color::Black);
			brashWifi = gcnew System::Drawing::SolidBrush(System::Drawing::Color::AliceBlue);

			current_x_single = 0;
			current_y_single = 0;

			numberStep = 0;
			numberReadInBetweenPhase = 0;

			stateX = kalman_init(0.0625, 1.0, 0.22069559, 0.02069557);
			stateY = kalman_init(0.0625, 1.0, 0.22069559, 0.02069557);

			SECOND_PHASE_TIME = 100;
			countTimeAfterScondPhase = 0;

			currentAngle = PI;
			this->KeyPreview = true;
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Label^ labelCountStep;
	private: System::Windows::Forms::Label^ labelCountStepNumber;



	private: float accX, accY, accZ;
	private: float angleX, angleY, angleZ;
	private: float startX;
	private: float startY;
	private: float finishX;
	private: float finishY;
	private: float lastAngle;
	private: List<float>^ pointsX;
			 List<float>^ pointsY;



    float current_time;
	bool firstPhaseOfStepDetected;
	bool secondPhaseOfStepDetected;
	bool thridPhaseOfStepDetected;
	
	float accMaxVerticalForFirstPart;
	float accMinVerticalForFirstPart;
	float accMaxHorizontalForFirstPart;
	float accMinHorizontalForFirstPart;

	float accMaxVerticalForSecondPart;
	float accMinVerticalForSecondPart;
	float accMaxHorizontalForSecondPart;
	float accMinHorizontalForSecondPart;

	float accMaxVerticalForThridPart;
	float accMinVerticalForThridPart;
	float accMaxHorizontalForThridPart;
	float accMinHorizontalForThridPart;

	double currentAngle;
	
	kalman_state* stateX;
	kalman_state* stateY;

	float CONST_G;

	float CONST_ACCELERATION_MIN;
	float CONST_H_MIN;

	float CONST_ACCELERATION_MAX;
	float CONST_H_MAX;

	int deltaTime;
	int SECOND_PHASE_TIME;
	int countTimeAfterScondPhase;

	float SumRecivedAcceleration;
	int countStep;
	int prev_countStep;
	StreamWriter^ sw;
	float UIOffset_x;
	float UIOffset_y;
	int numberStep;
	int numberReadInBetweenPhase;

	List<float>^ x_patern;
    List<float>^ y_patern;

	List<float>^ x_current;
	List<float>^ y_current;

	List<Line^>^ map;
	List<Line^>^ way;
	List<Windows::Point>^ points;
	ProbabilityMapChecker^ checker;
	WifiWorker^ fingerPrinting;
	MagneticWolker^ magnetic;
	List<Windows::Point>^ techicalPoints;
	List<Windows::Point>^ wifis;

	Windows::Point cur_point;

	System::Drawing::Pen^ pen;
	System::Drawing::Pen^ penMap;
	System::Drawing::Pen^ penWay;
	System::Drawing::Pen^ penPoints;
	System::Drawing::Brush^ brashPoints;
	System::Drawing::Brush^ brashWifi;
	
	float current_x_single;
	float current_y_single;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart2;
	private: System::Windows::Forms::Button^  buttonSuspend;
	private: System::Windows::Forms::Button^  buttonContinue;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  buttonClear;
private: System::Windows::Forms::Button^  buttonDraw;
private: System::Windows::Forms::Label^  labelWiFi;
private: System::Windows::Forms::Label^  WiFiResult;

		 bool allAccDetected;
	protected: 

	private:
		/// <summary>
		/// “ребуетс€ переменна€ конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// ќб€зательный метод дл€ поддержки конструктора - не измен€йте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea4 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series4 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->labelCountStep = (gcnew System::Windows::Forms::Label());
			this->labelCountStepNumber = (gcnew System::Windows::Forms::Label());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->buttonSuspend = (gcnew System::Windows::Forms::Button());
			this->buttonContinue = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->buttonClear = (gcnew System::Windows::Forms::Button());
			this->buttonDraw = (gcnew System::Windows::Forms::Button());
			this->labelWiFi = (gcnew System::Windows::Forms::Label());
			this->WiFiResult = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(665, 206);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(411, 437);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &Form1::pictureBox1_Click_1);
			this->pictureBox1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Form1::pictureBox1_Paint);
			this->pictureBox1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseClick);
			// 
			// timer1
			// 
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// labelCountStep
			// 
			this->labelCountStep->Location = System::Drawing::Point(667, 164);
			this->labelCountStep->Name = L"labelCountStep";
			this->labelCountStep->Size = System::Drawing::Size(115, 20);
			this->labelCountStep->TabIndex = 1;
			this->labelCountStep->Text = L"Count recognized step = ";
			this->labelCountStep->Click += gcnew System::EventHandler(this, &Form1::labelCountStep_Click);
			// 
			// labelCountStepNumber
			// 
			this->labelCountStepNumber->Location = System::Drawing::Point(794, 164);
			this->labelCountStepNumber->Name = L"labelCountStepNumber";
			this->labelCountStepNumber->Size = System::Drawing::Size(25, 15);
			this->labelCountStepNumber->TabIndex = 0;
			this->labelCountStepNumber->Text = L"0";
			// 
			// chart1
			// 
			chartArea3->AxisX->MajorGrid->IntervalType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
			chartArea3->AxisX->MajorGrid->LineColor = System::Drawing::Color::LightGray;
			chartArea3->AxisX->ScaleView->SizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
			chartArea3->AxisY->MajorGrid->IntervalType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
			chartArea3->AxisY->MajorGrid->LineColor = System::Drawing::Color::LightGray;
			chartArea3->CursorX->IntervalOffsetType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
			chartArea3->CursorX->IntervalType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
			chartArea3->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea3);
			legend3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(1), true));
			legend3->IsTextAutoFit = false;
			legend3->Name = L"Legend1";
			legend3->Title = L"Vertical";
			this->chart1->Legends->Add(legend3);
			this->chart1->Location = System::Drawing::Point(26, 24);
			this->chart1->Name = L"chart1";
			this->chart1->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::Grayscale;
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series3->Color = System::Drawing::Color::Yellow;
			series3->EmptyPointStyle->IsVisibleInLegend = false;
			series3->IsVisibleInLegend = false;
			series3->IsXValueIndexed = true;
			series3->Legend = L"Legend1";
			series3->Name = L"Vertical";
			series3->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::Double;
			series3->YValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::Double;
			this->chart1->Series->Add(series3);
			this->chart1->Size = System::Drawing::Size(538, 295);
			this->chart1->TabIndex = 2;
			this->chart1->Text = L"chartVert";
			this->chart1->Click += gcnew System::EventHandler(this, &Form1::chart1_Click);
			// 
			// chart2
			// 
			chartArea4->AxisX->LineColor = System::Drawing::Color::LightGray;
			chartArea4->AxisX->MajorGrid->LineColor = System::Drawing::Color::LightGray;
			chartArea4->AxisX->ScaleView->SizeType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
			chartArea4->AxisY->LineColor = System::Drawing::Color::LightGray;
			chartArea4->AxisY->MajorGrid->LineColor = System::Drawing::Color::LightGray;
			chartArea4->Name = L"ChartArea1";
			this->chart2->ChartAreas->Add(chartArea4);
			legend4->Name = L"Legend1";
			this->chart2->Legends->Add(legend4);
			this->chart2->Location = System::Drawing::Point(26, 338);
			this->chart2->Name = L"chart2";
			series4->ChartArea = L"ChartArea1";
			series4->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Spline;
			series4->Color = System::Drawing::Color::Yellow;
			series4->IsVisibleInLegend = false;
			series4->Legend = L"Legend1";
			series4->Name = L"Horizontal";
			this->chart2->Series->Add(series4);
			this->chart2->Size = System::Drawing::Size(538, 284);
			this->chart2->TabIndex = 3;
			this->chart2->Text = L"chartVert";
			// 
			// buttonSuspend
			// 
			this->buttonSuspend->Location = System::Drawing::Point(699, 65);
			this->buttonSuspend->Name = L"buttonSuspend";
			this->buttonSuspend->Size = System::Drawing::Size(75, 23);
			this->buttonSuspend->TabIndex = 4;
			this->buttonSuspend->Text = L"Suspend";
			this->buttonSuspend->UseVisualStyleBackColor = true;
			this->buttonSuspend->Click += gcnew System::EventHandler(this, &Form1::buttonSuspend_Click);
			// 
			// buttonContinue
			// 
			this->buttonContinue->Location = System::Drawing::Point(699, 117);
			this->buttonContinue->Name = L"buttonContinue";
			this->buttonContinue->Size = System::Drawing::Size(75, 23);
			this->buttonContinue->TabIndex = 5;
			this->buttonContinue->Text = L"Continue";
			this->buttonContinue->UseVisualStyleBackColor = true;
			this->buttonContinue->Click += gcnew System::EventHandler(this, &Form1::buttonContinue_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(23, 24);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(42, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Vertical";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(23, 338);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(54, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Horizontal";
			this->label2->Click += gcnew System::EventHandler(this, &Form1::label2_Click);
			// 
			// buttonClear
			// 
			this->buttonClear->Location = System::Drawing::Point(896, 65);
			this->buttonClear->Name = L"buttonClear";
			this->buttonClear->Size = System::Drawing::Size(75, 23);
			this->buttonClear->TabIndex = 8;
			this->buttonClear->Text = L"Clear";
			this->buttonClear->UseVisualStyleBackColor = true;
			this->buttonClear->Click += gcnew System::EventHandler(this, &Form1::buttonClear_Click);
			// 
			// buttonDraw
			// 
			this->buttonDraw->Location = System::Drawing::Point(896, 116);
			this->buttonDraw->Name = L"buttonDraw";
			this->buttonDraw->Size = System::Drawing::Size(75, 23);
			this->buttonDraw->TabIndex = 9;
			this->buttonDraw->Text = L"Draw Path";
			this->buttonDraw->UseVisualStyleBackColor = true;
			this->buttonDraw->Click += gcnew System::EventHandler(this, &Form1::buttonDraw_Click);
			// 
			// labelWiFi
			// 
			this->labelWiFi->AutoSize = true;
			this->labelWiFi->Location = System::Drawing::Point(886, 164);
			this->labelWiFi->Name = L"labelWiFi";
			this->labelWiFi->Size = System::Drawing::Size(64, 13);
			this->labelWiFi->TabIndex = 10;
			this->labelWiFi->Text = L"WIFI points:";
			this->labelWiFi->Click += gcnew System::EventHandler(this, &Form1::label3_Click);
			// 
			// WiFiResult
			// 
			this->WiFiResult->AutoSize = true;
			this->WiFiResult->Location = System::Drawing::Point(972, 164);
			this->WiFiResult->Name = L"WiFiResult";
			this->WiFiResult->Size = System::Drawing::Size(0, 13);
			this->WiFiResult->TabIndex = 11;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1088, 646);
			this->Controls->Add(this->WiFiResult);
			this->Controls->Add(this->labelWiFi);
			this->Controls->Add(this->buttonDraw);
			this->Controls->Add(this->buttonClear);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->buttonContinue);
			this->Controls->Add(this->buttonSuspend);
			this->Controls->Add(this->chart2);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->labelCountStep);
			this->Controls->Add(this->labelCountStepNumber);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Deactivate += gcnew System::EventHandler(this, &Form1::Form1_Deactivate);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::Form1_KeyDown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &Form1::Form1_KeyPress);
			this->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseClick);
			this->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &Form1::pictureBox1_MouseClick);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


bool firstPhaseOfStepIsDetected(float accX, float accY, float accZ)
	{
		accMinHorizontalForFirstPart = -1.0f;
		if( ((accX > accMinVerticalForFirstPart) && (accX < accMaxVerticalForFirstPart)) && ((accY > accMinHorizontalForFirstPart) && (accY < accMaxHorizontalForFirstPart)))
			return true;
		return false;
	};
	bool secondPhaseOfStepIsDetected(float accX ,float accY, float accZ)
	{
		bool result = false;
		if( ((accY > accMinVerticalForSecondPart) && (accY < accMaxVerticalForSecondPart)) 
			&& ((accX > accMinHorizontalForSecondPart) && (accX < accMaxHorizontalForSecondPart)))
			result = true;

		return result;
	};
	bool thridPhaseOfStepIsDetected(float accX , float accY,float accZ)
	{
		bool result = false;
		if( ((accY > accMinVerticalForThridPart) && (accY < accMaxVerticalForThridPart))
			&& ((accX > accMaxHorizontalForThridPart) && (accX < accMinHorizontalForThridPart)))
			result = true;

		return result;
	};
	float calculateLengthOfStep(float acc)
	{
		SumRecivedAcceleration += acc;
		float result = 0;
		if (countStep)
			result = 10;//(CONST_G/countStep) * pow((double)SumRecivedAcceleration, 1.0 / 3.0);
		sw->WriteLine ("LENGTH OF STEP: result = " + result + " module acc = " + acc +
			" sum = " + SumRecivedAcceleration + "count step = " + countStep);
		return result;
	}
	float module(float accX, float accY, float accZ)
	{
		float summ = accX * accX + accY * accY;
		return sqrt(summ);
	}
	float execute(float accX, float accY, float accZ)
	{
		
		if(secondPhaseOfStepIsDetected(accX, accY, accZ)){
			secondPhaseOfStepDetected = true;
			thridPhaseOfStepDetected = false;

			DataPoint^ cur_ver_point = gcnew DataPoint(current_time, accY);
			DataPoint^ cur_hor_point = gcnew DataPoint(current_time, accY);

			cur_ver_point->Color = System::Drawing::Color::Red;
			cur_ver_point->MarkerColor = System::Drawing::Color::Red;
			cur_ver_point->MarkerSize = 1000;

			cur_hor_point->Color = System::Drawing::Color::Red;
			cur_hor_point->MarkerColor = System::Drawing::Color::Red;
			cur_hor_point->MarkerSize = 1000;


		}
		if (thridPhaseOfStepIsDetected(accX, accY, accZ) && secondPhaseOfStepDetected) {
			thridPhaseOfStepDetected = true;
			DataPoint^ cur_ver_point = gcnew DataPoint(current_time, accY);
			DataPoint^ cur_hor_point = gcnew DataPoint(current_time, accY);

			cur_ver_point->Color = System::Drawing::Color::Black;
			cur_ver_point->MarkerColor = System::Drawing::Color::Black;
			cur_ver_point->MarkerSize = 1000;

			cur_hor_point->Color = System::Drawing::Color::Black;
			cur_hor_point->MarkerColor = System::Drawing::Color::Black;
			cur_hor_point->MarkerSize = 1000;


		}
		if(firstPhaseOfStepDetected  & secondPhaseOfStepDetected & thridPhaseOfStepDetected )
		{
			secondPhaseOfStepDetected = false;
			thridPhaseOfStepDetected = false;
			countTimeAfterScondPhase = 0;
			float moduleAcceleration = module(accX, accY, accZ);
			countStep++;
			this->labelCountStepNumber->Text = Convert::ToString(countStep);
			
			return calculateLengthOfStep(moduleAcceleration);
		}
		if (countTimeAfterScondPhase > SECOND_PHASE_TIME) {
			secondPhaseOfStepDetected = false;
			countTimeAfterScondPhase = 0;
		}
		countTimeAfterScondPhase++;
		return 0;
	}


	float executeChatAlgorithm(float accX, float accY, float accZ){
		float result = 0;
		
		if(secondPhaseOfStepIsDetected(accX, accY, accZ)){
			secondPhaseOfStepDetected = true;
			DataPoint^ cur_ver_point = gcnew DataPoint(current_time, accY);
			
			cur_ver_point->Color = System::Drawing::Color::Red;
			cur_ver_point->MarkerColor = System::Drawing::Color::Red;
			cur_ver_point->MarkerSize = 1000;

			numberReadInBetweenPhase = 0;

		}
		if(thridPhaseOfStepIsDetected(accX, accY, accZ)){
			thridPhaseOfStepDetected = true;

			DataPoint^ cur_ver_point = gcnew DataPoint(current_time, accY);

			cur_ver_point->Color = System::Drawing::Color::Red;
			cur_ver_point->MarkerColor = System::Drawing::Color::Red;
			cur_ver_point->MarkerSize = 1000;

		}
		if(secondPhaseOfStepDetected && thridPhaseOfStepDetected){
			
			numberStep++;

			if(numberStep){
				merge();
				if(allAccDetected)
					return module(accX, accY, accZ);
			}
			
			x_current->Add(accY);	
			y_current->Add(accX);

			allAccDetected = checkCurrentSate(accX,accY,accZ, numberReadInBetweenPhase);
			numberReadInBetweenPhase++;
			
		}
		return 0;
	}

	void merge(){
		int limit = x_patern->Count >= x_current->Count?x_current->Count:x_patern->Count;
		
		array<float>^ x_curent_ar = x_current->ToArray(); 
		array<float>^ y_curent_ar = y_current->ToArray();

		array<float>^ x_patern_ar = x_patern->ToArray();
		array<float>^ y_patern_ar = y_patern->ToArray();

		x_current->Clear();
		y_current->Clear();
		x_patern->Clear();
		x_patern->Clear();

		for(int i = 0; i < limit; i++)
		{
			x_patern_ar[i] = (x_patern_ar[i] + x_curent_ar[i])/2;
			y_patern_ar[i] = (y_patern_ar[i] + y_curent_ar[i])/2;
		}
		
		x_patern->AddRange(x_patern_ar);
		y_patern->AddRange(y_patern_ar);

		delete[]x_curent_ar;
		delete[]y_curent_ar;
		delete[]x_patern_ar;
		delete[]y_patern_ar;

	}
	
	bool checkCurrentSate(float accX, float accY, float accZ, int index_patern){
		if(index_patern < x_patern->Count)
			return sqrt(pow(x_patern[index_patern] - accX, 2) + pow(y_patern[index_patern] - accY, 2)) < EPS * sqrt(2) / 2;
		return true;
	}


private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				int statusResponse = COMOpen();

				if(statusResponse < 0)
				{
					System::Windows::Forms::MessageBox::Show("all are bad");
				}
				File::Delete("../logs/LogAngle.txt");
				sw = File::CreateText("../logs/LogAngle.txt");
				sw->WriteLine ("Second phage: vertical acceleration" + 
				" [ " + accMinVerticalForSecondPart + " ; " + accMaxVerticalForSecondPart + " ] " +
				"Horizontal acceleration" +
				" [ " + accMinHorizontalForSecondPart + " ; " + accMaxHorizontalForSecondPart + " ] ");

				sw->WriteLine ("Triad phage: vertical acceleration" + 
				" [ " + accMinVerticalForThridPart + " , " + accMaxVerticalForThridPart + " ] " +
				"Horizontal acceleration" +
				" [ " + accMinHorizontalForThridPart + " , " + accMaxHorizontalForThridPart + " ] ");

				timer1->Interval = 1;
				
			 }

	private: System::Void pictureBox1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		
				if((startX != finishX)||(startY != finishY))
				 {  
					startX = finishX;
					startY = finishY;
					pointsX->Add(finishX );
					pointsY->Add(finishY );

				 }
				for (int i = 0; i < techicalPoints->Count; i++) {
					e->Graphics->FillEllipse(brashPoints, techicalPoints[i].X, techicalPoints[i].Y, 8, 8);
				}
				for (int i = 0; i < wifis->Count; i++) {
					e->Graphics->FillEllipse(brashWifi, wifis[i].X, wifis[i].Y, 8, 8);
				}
					
				for (int i = 0; i < map->Count; i++) {
					e->Graphics->DrawLine(penMap, (float)map[i]->X1, (float)map[i]->Y1, (float)map[i]->X2, (float)map[i]->Y2);
				}


				for (int i = 0; i < way->Count; i++) {
					e->Graphics->DrawLine(penWay, (float)way[i]->X1, (float)way[i]->Y1, (float)way[i]->X2, (float)way[i]->Y2);
				}
					
				for (int i = 0; i < points->Count-1; i++) {
					e->Graphics->DrawLine(penPoints, (float)points[i].X, (float)points[i].Y, (float)points[i + 1].X, (float)points[i + 1].Y );
				}
					
				array<float>^ pointsArrayX = pointsX->ToArray();
				array<float>^ pointsArrayY = pointsY->ToArray();

			 }
	private: char*  readDataFromSensors()
			 {
				
				char* buff = new char[28];
				int statusResponseRead = Read(buff, 28);
				statusResponseRead /= 2;
				short* buffShort = (short*)buff;
				short tmp[14];

				for(int i = 0; i < statusResponseRead; i++)
				{
					tmp[i] = buffShort[i];
				}
				return buff;
			 }
	private: void parseReadDate(char* readDate)
			 {
				 //int tmpX = (int)readDate[3];
				 //accX = kalman_update(stateX, (double)((readDate[11] >> 8) + readDate[10]) / 256.0f * CONST_G)->x;
				 //accY = kalman_update(stateY, (float)((readDate[13] >> 8) + readDate[12]) / 256.0f * CONST_G)->x;
				
				 accX = (float)((readDate[11] >> 8) + readDate[10]) / 256.0f * CONST_G;
				 accY = (float)((readDate[13] >> 8) + readDate[12]) / 256.0f * CONST_G;
				 accZ = (float)((readDate[15] >> 8) + readDate[14]) / 256.0f * CONST_G;
				 

				 float giroX = (float)((readDate[17] << 8) + readDate[16]) / 100.0f;
				 float giroY = (float)((readDate[19] << 8) + readDate[18]) / 100.0f;
				 float giroZ = (float)((readDate[21] << 8) + readDate[20]) / 100.0f;

				 float comX = (float)((readDate[5] << 8) + readDate[4]) / 100.0f;
				 float comY = (float)((readDate[7] << 8) + readDate[6]) / 100.0f;
				 float comZ = (float)((readDate[9] << 8) + readDate[8]) / 100.0f;


				 angleX = (float)((readDate[23] << 8) + readDate[22]) / 100.0f;
				 angleY = (float)((readDate[25] << 8) + readDate[24]) / 100.0f;
				 angleZ = (float)((readDate[27] << 8) + readDate[26]) / 100.0f;

				 x_patern->Add(angleX);
				 y_patern->Add(angleY);
				 x_current->Add(angleX);

				sw->WriteLine("ANGLES: angleX = " + angleX + " angleY = " + angleY + " angleZ = " + angleZ);
				sw->WriteLine("COMPAS: comX = " + comX + " comY = " + comY + " comZ = " + comX);
				sw->WriteLine("COMPAS: giroX = " + giroX + " giroY = " + giroZ + " giroZ = " + giroZ);
				sw->WriteLine("");

			 }
	private: void calculateStartPointAndFinishpoint(float lengthOfstep, float angleX, float angleY, float angleZ)
			 {
				angleZ = getAngle(angleZ);
				float cos_z = cos(angleZ);
				float sin_z = -sin(angleZ);
				finishX = (float)(startX + sin_z*lengthOfstep);
				finishY = (float)(startY + cos_z*lengthOfstep);
				 
				lastAngle = angleZ;


			 }
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 char* readDate = readDataFromSensors();
				 parseReadDate(readDate);
				 float lengthStep = execute(accX, accY, accZ);
				 
				 if (lengthStep) {
					 calculateStartPointAndFinishpoint(lengthStep, angleX, angleY, angleZ);

					 PredictionResult^ report = checker->checkOnMap(angleZ, -lengthStep);
					 cur_point = report->point;

					 ///TODO: get current points from algorithm and update way and closed way.
					 List<WifiPk^>^ listWifi = fingerPrinting->getCurrentSignal();
					 List<Windows::Point>^ mags = magnetic->getSquery(magnetic->getCurrentSignal(), cur_point);
					 wifis->AddRange(fingerPrinting->getSquery(listWifi, cur_point));
					 
					 WiFiResult->Text = "";
					 for (int i = 0; i < listWifi->Count; i++) {
						 WiFiResult->Text += "MAC: " + listWifi[i]->MAC_ADDRESS + "\nSignal Level: " + listWifi[i]->SignelLevel;
					 }

					 points->Add(cur_point);
					 techicalPoints->AddRange(checker->getAllCurrentPoints());
					 
					 if (report->is_closed) {
						 timer1->Stop();
						 System::Windows::Forms::MessageBox::Show("you achived your aim");
					 }
					 if (report->is_rebilded) {
						 ///TODO: call rebuild way from way class
						 updateWay(checker->getNearestPoint(report->point), checker->getLastPointOnWay(), way);
						 checker->refreshChecker(way);
						 System::Windows::Forms::MessageBox::Show("way is rebuilded");
					 }
				 }
				 pictureBox1->Invalidate();
				 current_time += 1;
			 }
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Void pictureBox1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void chart1_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void buttonSuspend_Click(System::Object^  sender, System::EventArgs^  e) {
	timer1->Stop();
}
private: System::Void buttonContinue_Click(System::Object^  sender, System::EventArgs^  e) {
	timer1->Start();
}
private: System::Void labelCountStep_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void buttonClear_Click(System::Object^  sender, System::EventArgs^  e) {
	
	timer1->Stop();
	countStep = 0;
	secondPhaseOfStepDetected = false;
	thridPhaseOfStepDetected = false;
	labelCountStepNumber->Text = "0";
	pointsX->Clear();
	pointsY->Clear();
	map->Clear();
	way->Clear();
	
	startX = UIOffset_x;
	startY = UIOffset_y;
	finishX = UIOffset_x;
	finishY = UIOffset_y;
	points->Clear();
	pictureBox1->Invalidate();
}



private: System::Void buttonDraw_Click(System::Object^  sender, System::EventArgs^  e) {
	
	databaseGenerate(map, way, pictureBox1);
	checker = gcnew ProbabilityMapChecker(map, way);
	
	Windows::Point cur_point(UIOffset_x, UIOffset_y);
	fingerPrinting = gcnew WifiWorker("CORP1FLOOR2.xml", "../logs/WIFIlog.txt");
	magnetic = gcnew MagneticWolker("CORP1FLOOR2_mag.xml", "../logs/MagLog.txt");
	fingerPrinting->updateCurrentState(cur_point, fingerPrinting->getCurrentSignal());
	magnetic->updateCurrentState(cur_point, magnetic->getCurrentSignal());

	List<WifiPk^>^ listWifi = fingerPrinting->getCurrentSignal();
	List<WifiPk^>^ listMag = magnetic->getCurrentSignal();
	for (int i = 0; i < listWifi->Count; i++) {
		WiFiResult->Text += "MAC: " + listWifi[i]->MAC_ADDRESS + "\nSignal Level: " + listWifi[i]->SignelLevel;
	}
	points->Add(cur_point);
	startX = 0;
	startY = 0;
	finishX = UIOffset_x;
	finishY = UIOffset_y;
	pictureBox1->Invalidate();
	timer1->Start();
}

private: double getAngle(double rowAngle) {
	return currentAngle;

}
private: System::Void Form1_Deactivate(System::Object^  sender, System::EventArgs^  e) {
	float aveg_x, aveg_y, aveg_z;
	for (int i = 0; i < x_patern->Count; i++) {
		aveg_x += x_patern[i];
		aveg_y += y_patern[i];
		aveg_z += x_current[i];
	}

	sw->Flush();
}
private: System::Void Form1_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
	
	if (e->KeyChar == (char)Keys::Up) {
		currentAngle = PI;
	} else if(e->KeyChar == (char)Keys::Right) {
		currentAngle = PI/(double)2;
	} else if (e->KeyChar == (char)Keys::Left) {
		currentAngle = -PI / (double)2;
	} else if (e->KeyChar == (char)Keys::Down) {
		currentAngle = 0;
	}
}
private: System::Void Form1_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
	
	if (e->KeyCode == Keys::W) {
		currentAngle = PI;
	}
	else if (e->KeyCode == Keys::D) {
		currentAngle = -PI / (double)2;
	}
	else if (e->KeyCode == Keys::A) {
		currentAngle = PI / (double)2;
	}
	else if (e->KeyCode == Keys::S) {
		currentAngle = 0;
	}
}
private: System::Void label3_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void pictureBox1_Click_1(System::Object^  sender, System::EventArgs^  e) {
	
}
private: System::Void pictureBox1_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
	
}
};
}