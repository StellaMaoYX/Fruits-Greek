#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	//set up the GUI panel
	gui.setup();
	//add GUI elements
	gui.add(minH.setup("min H", 0, 0, 180));
	gui.add(maxH.setup("max H", 180, 0, 180));
	gui.add(minS.setup("min S", 0, 0, 255));
	gui.add(maxS.setup("max S", 255, 0, 255));
	gui.add(minV.setup("min V", 0, 0, 255));
	gui.add(maxV.setup("max V", 255, 0, 255));

	//Camera setting
	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(30);
	vidGrabber.initGrabber(640, 480);

	//load images
	imgbasket.load("basket.png");
	imgmiss.load("miss.png");
	imggameover.load("GAME_OVER.png");
	imgpause.load("pause.png");
}

//--------------------------------------------------------------
void ofApp::update() {
	vidGrabber.update();

	//blob detecction
	if (vidGrabber.isFrameNew()) {
		vidFrame.setFromPixels(vidGrabber.getPixels());
		vidFrameMat = toCv(vidFrame);
		cvtColor(vidFrameMat, mat_HSV, CV_BGR2HSV);
		inRange(mat_HSV, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), mat_HSV_Threshold);
		erode(mat_HSV_Threshold, mat_HSV_Threshold, Mat());
		dilate(mat_HSV_Threshold, mat_HSV_Threshold, Mat());

		ofImage im_temp;
		ofxCvGrayscaleImage im_temp_gray;

		toOf(mat_HSV_Threshold, im_temp);

		im_temp_gray.setFromPixels(im_temp.getPixels());

		contourFinder.findContours(im_temp_gray, 5, (640 * 480) / 4, 4, false, true);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	// initial
	if (gameStateID == 0) {
		putText(vidFrameMat, "press s to start the game", Point(50, 200), FONT_HERSHEY_SIMPLEX, 1, Scalar(25, 130, 10), 2, 4);
		// blob detection
		ofSetColor(255, 255, 255);
		drawMat(vidFrameMat, 0, 0, 1280, 960);
		for (int i = 0; i < contourFinder.nBlobs; i++) {
			objCentre_x = contourFinder.blobs.at(i).centroid.x;
			objCentre_y = contourFinder.blobs.at(i).centroid.y;
		}
		ofSetColor(255, 255, 255);
		drawMat(mat_HSV_Threshold, vidFrameMat.cols * 2, 0, 1280, 960);

		//draw the basket
		if (objCentre_x > 0 && objCentre_y > 0) {
			imgbasket.draw(objCentre_x * 2 - 150, objCentre_y * 2 - 100, 300, 200);
		}
	}
	
	// game playing
	if (gameStateID == 1) {
		putText(vidFrameMat, "Score  " + to_string(totalScore), Point(460, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(205, 23, 10), 4, 8);
		// blob detection
		ofSetColor(255, 255, 255);
		drawMat(vidFrameMat, 0, 0, 1280, 960);
		for (int i = 0; i < contourFinder.nBlobs; i++) {
			objCentre_x = contourFinder.blobs.at(i).centroid.x;
			objCentre_y = contourFinder.blobs.at(i).centroid.y;
		}
		ofSetColor(255, 255, 255);
		drawMat(mat_HSV_Threshold, vidFrameMat.cols * 2, 0, 1280, 960);

		//draw the basket
		if (objCentre_x > 0 && objCentre_y > 0) {
			imgbasket.draw(objCentre_x * 2 - 150, objCentre_y * 2 - 100, 300, 200);
		}

		//draw the fruits
		for (int i = 0; i < 20; i++) {
			if (fruArr[i].speed != 0) {
				fruArr[i].tempY = fruArr[i].posY;
				fruArr[i].posY += fruArr[i].speed;
				fruArr[i].info.f_img.draw(fruArr[i].posX, fruArr[i].posY, 150, 150);
				if (fruArr[i].tempY >= 800) {
					fruArr[i].speed = 0;
				}
				if (fruArr[i].tempY >= 800 && fruArr[i].info.f_ID != 0 && fruArr[i].info.f_ID != 1) {
					imgmiss.draw(fruArr[i].posX, fruArr[i].posY);
					totalScore -= 10;
				}
				if (((fruArr[i].posX < objCentre_x * 2 + 150) && (fruArr[i].posX > objCentre_x * 2 - 150))
					&& ((fruArr[i].posY < objCentre_y * 2 + 100) && (fruArr[i].posY > objCentre_y * 2 - 100))
					&& objCentre_x != -1 && objCentre_y != -1) {
					if (fruArr[i].info.f_ID == 0) {
						gameStateID = 2;
					}
					totalScore += fruArr[i].info.f_point; 
					fruArr[i].speed = 0;
				}
				if (totalScore < 0) {
					gameStateID = 2;
				}
			}
		}
		if (time > randomDropDownTim()) {
			for (int i = 0; i < 20; i++) {
				if (fruArr[i].speed == 0) {

					randomDropDown(fruArr[i].info);//random generate the fruits' information

					fruArr[i].posX = randomDropDownPos();
					fruArr[i].posY = 0;
					fruArr[i].tempY = 0;
					fruArr[i].speed = rand() % 8 + 7;
					fruArr[i].info.f_img.draw(fruArr[i].posX, fruArr[i].posY, 150, 150);
					break;
				}
			}
			time = 0;
		}
		else {
			time++;
		}
	}
	
	// game over
	if (gameStateID == 2) {
		putText(vidFrameMat, "press q to quit to the initial page", Point(50, 450), FONT_HERSHEY_SIMPLEX, 1, Scalar(25, 130, 10), 2, 4);
		drawMat(vidFrameMat, 0, 0, 1280, 960);
		imggameover.draw(0, 0, 1280, 960);
		for (int i = 0; i < 20; i++) {
			fruArr[i].speed = 0;
		}
		totalScore = 0;
	}

	// pause
	if (gameStateID == 3) {
		putText(vidFrameMat, "press s to continue the game", Point(50, 450), FONT_HERSHEY_SIMPLEX, 1, Scalar(25, 130, 10), 2, 4);
		drawMat(vidFrameMat, 0, 0, 1280, 960);
		for (int i = 0; i < 20; i++) {
			if (fruArr[i].speed != 0) {
				fruArr[i].info.f_img.draw(fruArr[i].posX, fruArr[i].posY, 150, 150);
			}
		}
		imgpause.draw(640 - imgpause.getWidth() / 2, 480 - imgpause.getHeight() / 2);
	}

	//draw the gui
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (gameStateID == 0 || gameStateID == 3) {
		if (key == 's') {
			gameStateID = 1;
		}
	}
	
	if (key == 'q') {
		gameStateID = 0;
	}

	if (gameStateID == 1) {
		if (key == 'p') {
			gameStateID = 3;
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::randomDropDown(f_info& fruit) {
	int fruit_generate_set[18] = { 0, 1, 1, 2, 2, 6, 6, 6, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5 };//represents fruit ID

	/*f_name		f_ID
	  "bomb"		0
	  "durian"		1
	  "grapes"		2
	  "watermelon"	3
	  "mango"		4
	  "orange"		5
	  "peach"		6;*/

	int k = rand() % 18;
	fruit.set(fruit_generate_set[k]);
}

//--------------------------------------------------------------
int ofApp::randomDropDownPos() {
	int k = (rand() % 35 + 2) * 30;
	return k;
}


//--------------------------------------------------------------
int ofApp::randomDropDownTim() {
	int k = rand() % 30 + 20;
	return k;
}