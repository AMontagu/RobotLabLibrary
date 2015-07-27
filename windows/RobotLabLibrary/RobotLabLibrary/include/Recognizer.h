#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#ifdef _WIN32

#include <windows.h>

#endif

#ifdef __linux__ 

#include <sys/stat.h>
#include <sys/types.h>

#endif

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "../faceRecognizerContrib/include/face.hpp"


#include "Constantes.h"

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>

class Recognizer
{
public:
	Recognizer();

	~Recognizer();

	void addFrameToCurrentTraining(cv::Mat frame, int label, std::string faceName);
	void addFrameToCurrentTrainingAndSave(cv::Mat frame, int label, std::string faceName, std::string fileName, std::string folderName = "face/");
	void askForAddImageToCurrentTrainingAndSave(cv::Mat noRecognizedFace);

	double recognize(cv::Mat faceToRecognize);
	void train();

	void readCsv(const std::string& filename, char separator = ';');
	void saveCsv(std::string fileName = "customFaceCsv.txt");
	void saveImg(std::string folderName, std::string nameOfFile, cv::Mat faceToSave);
	cv::Mat processFrame(cv::Mat frameToProcess);

	bool isTrained();
	int getFrameWidth();
	int getFrameHeight();
	cv::Size getFrameSize();
	double getLastConfidence();
	int getLabelFrameSize();
	int getTrainingFramesSize();
	int getNumberOfFaceSauvegarded();
	void printConf();
	cv::Ptr<cv::face::FaceRecognizer> getModel();

	bool imageExist(const std::string name, const std::string folderName = "face/");
	void createDirectory(std::string folderName);
private :
	std::vector<cv::Mat> _trainingFrames, _trainingFramesInTrain;
	std::vector<int> _labelsFrames, _labelsFramesInTrain;
	std::map<int, std::string> _pathToFrame;
	std::map<int, cv::String> _labelsInfo; // use labelsInfo.insert(std::make_pair(label, info));

	cv::Ptr<cv::face::FaceRecognizer> _model = cv::face::createEigenFaceRecognizer();

	cv::Size _frameSize;

	bool _newData = true, _isTrained = false;
	int _frameWidth = 92, _frameHeight = 112;
	double _confidence = 0.0;
};

#endif