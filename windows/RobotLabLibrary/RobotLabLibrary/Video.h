#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>
#include <time.h>

class Video
{
public:
	Video(int numberCam);
	Video();

	void startAllDetect();
	void stopAllDetect();
	void startFaceDetect();
	void stopFaceDetect();
	void startSmileDetect();
	void stopSmileDetect();
	void startEyeDetect();
	void stopEyeDetect();
	void startCustomDetect(cv::CascadeClassifier& cascade, int precision);
	void stopCustomDetect();

	int start();

	int faceDetect(cv::Mat& img);
	int eyeDetect(cv::Mat& img, cv::Mat& principalFrame, int width, int height);
	int smileDetect(cv::Mat& img, cv::Mat& principalFrame, int width, int height);
	int customDetect(cv::Mat& img);
	void draw(cv::Rect rectangle, cv::Mat &img, cv::Scalar color);

	//print the real number of specified object detected. Some smile or eye detected aren't draw for more accuracy but count in this functions
	int getFaceNumber();
	int getSmileNumber();
	int getEyeNumber();
	int getObjectNumber();
	std::vector<bool> getVectorSmiling();
	int faceTracking(cv::Rect faceToTrack, cv::Mat& frame);
	int getTracking();

	void printVectorSmilingData();

	~Video();


private:
	bool _detectFaceOn = false, _detectSmileOn = false, _detectEyeOn = false, _detectCustomOn = false;
	int _numberCam, _faceNumber = 0, _smileNumber = 0, _eyeNumber = 0, _objectNumber = 0, _precisionForCustomObject = 3;
	int _tracking = 0;
	bool _recording = true, _timeToDraw = false;
	std::vector<bool> _faceAreSmiling;
	double _scale = 1;
	cv::CascadeClassifier _faceCascade, _smileCascade, _eyeCascade, _customCascade;
	cv::VideoCapture _capture;
	double time;
	std::vector<std::vector<cv::Rect>> _storeSmileDetected;
	std::vector<cv::Rect> _averageSmilesRect, _averageFacesRect, _averageEyesRect, _averageCustomRect;

};

#endif
