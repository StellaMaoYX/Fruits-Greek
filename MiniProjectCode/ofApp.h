#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"

using namespace cv;
using namespace ofxCv;

//fruits kinds
typedef struct FruitKind {
	int f_ID;
	string f_name;
	ofImage f_img;
	int f_point;

	void set(int num) {
		f_ID = num;
		switch (f_ID)
		{
		case 0:
			f_name = "bomb";
			f_img.load("bomb.png");
			f_point = -1;
			break;
		case 1:
			f_name = "durian";
			f_img.load("durian.png");
			f_point = -500;
			break;
		case 2:
			f_name = "grapes";
			f_img.load("grapes.png");
			f_point = 80;
			break;
		case 3:
			f_name = "watermelon";
			f_img.load("watermelon.png");
			f_point = 100;
			break;
		case 4:
			f_name = "mango";
			f_img.load("mango.png");
			f_point = 30;
			break;
		case 5:
			f_name = "orange";
			f_img.load("orange.png");
			f_point = 10;
			break;
		case 6:
			f_name = "peach";
			f_img.load("peach.png");
			f_point = 50;
			break;
		default:
			break;
		}
	}

} f_info;

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void randomDropDown(f_info& fruit);
	int randomDropDownPos();
	int randomDropDownTim();

	//Get the camera equipment
	ofVideoGrabber vidGrabber;
	ofImage vidFrame;
	Mat vidFrameMat;

	Mat mat_HSV;
	Mat mat_HSV_Threshold;

	//Get GUI
	ofxPanel gui;
	//threshold
	ofxIntSlider minH;
	ofxIntSlider maxH;
	ofxIntSlider minS;
	ofxIntSlider maxS;
	ofxIntSlider minV;
	ofxIntSlider maxV;

	Mat result;

	ofxCvContourFinder contourFinder;

	int objCentre_x = -1;
	int objCentre_y = -1;

	//images
	ofImage imgbasket;
	ofImage imgmiss;
	ofImage imggameover;
	ofImage imgpause;

	struct FRUIT
	{
		f_info info;//fruits' info (including fruits ID, fruits name, fruits img, fruits score)

		int tempY;
		int speed;
		int posX;
		int posY;
		bool state;
		
	};

	int time = 0;
	struct FRUIT fruArr[20];
	int totalScore = 0;

	int gameStateID = 0; // 0:initial  1:game playing  2:game over  3:pause
};