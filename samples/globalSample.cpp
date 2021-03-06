#include "Video.h"
#include "Voice.h"
#include "Recognizer.h"
#include "Constantes.h"

#include <iostream>

/*###############################################################################################################################
#																																#
#										Sample for all the function of the library     											#
#																																#
#################################################################################################################################*/

//std::string processOnRecognitionPersonnalized(std::string dataToProcess);
int main(int argc, const char** argv)
{
	std::thread t[1];
	std::string speech, wordRecognized;
	std::string namePersonRecognized, goodName;
	std::vector<cv::Mat> lastFacesDetected;

	cv::Mat imgNull;

	int i = 0;

	Voice myVoice;
	/* -------------------------------- !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!--------------------------------------
	You may have problem here if you don't use the right settings for pocketsphinx. Please be sur of your frequency recorder. 
	In linux is often 16000 and in Windows 44100.
	If you set frequency to 16000 set nfft to 542 else for 44100 set to 2048.
	*/
	//Voice myVoice("../data/modelPocketSphinx/roboticModel/fr-fr", "../data/modelPocketSphinx/roboticModel/roboticOrder.lm", "../data/modelPocketSphinx/roboticModel/roboticOrder.dic", "44100", "2048");
	Video myVideo(0);
	Recognizer myRecognizer;

	//Launch video in a thread
	t[0] = std::thread([&] {myVideo.start(); });

	try {
		//Read the CSV for prepare a training data model
		myRecognizer.readCsv(CSV_FACE_RECO); // see Constantes.h for CSV_FACE_RECO
	}
	catch (cv::Exception& e) {
		std::cerr << "Error opening file \"" << CSV_FACE_RECO << "\". Reason: " << e.msg << std::endl;
		getchar();
		exit(1);
	}

	while (true)
	{
		std::cout << "You can say robotlab followeb by: detection faciale, detection sourires, detection yeux, reconnaissance faciale and stop " << std::endl;
		wordRecognized = myVoice.recognizeFromMicrophoneWhileTime(7); //get all the world recognized in wordRecognized variable
		std::cout << wordRecognized << std::endl;
		speech = myVoice.processOnRecognition(wordRecognized); //get only a valide commande in all the words says. A valide commande is for example : robotlab detection faciale
		std::cout << speech << std::endl;
		if (speech == "robotlab detection faciale") //If the receive command is ...
		{
			std::cout << "Begin face detect" << std::endl;
			myVideo.startFaceDetect(); //launch face detection
		}
		if (speech == "robotlab detection sourires")
		{
			std::cout << "Begin smile detect" << std::endl;
			myVideo.startSmileDetect(); //launch smile detection
		}
		if (speech == "robotlab detection yeux")
		{
			std::cout << "Begin eyes detect" << std::endl;
			myVideo.startEyeDetect(); //launch eye detection
		}
		if (speech == "robotlab stop")
		{
			std::cout << "stop all detect" << std::endl;
			myVideo.stopAllDetect(); //stop all detection
		}
		if (speech == "robotlab reconnaissance faciale")
		{
			if (!myRecognizer.isTrained() && !myVideo.getLastFacesDetected().empty() && !myRecognizer.equalTest(myVideo.getLastFacesDetected(), lastFacesDetected)) // If recognizer is not trained, we have at least detected one face, and this face it not the same that the last one. 
			{
				lastFacesDetected = myVideo.getLastFacesDetected(); // get all the current face in video stream
				i = 0;
				myVideo.clearLabel(); // remove the old name associed to a face
				for (std::vector<cv::Mat>::iterator face = lastFacesDetected.begin(); face != lastFacesDetected.end(); face++, i++)//for each face detected
				{
					namePersonRecognized = myRecognizer.recognize(*face);//launch the recognition
					myVideo.setImgToPrint(*face); // Create the second windows that print the face detected
					myVideo.addLabel(namePersonRecognized, i); //display the name on the video stream. If you only want to display him if the reognition is sur move this line under, in the else statement.
					if (myRecognizer.getLastConfidence() > 2500 && !myRecognizer.askForAddImageInProcess())
					{
						myRecognizer.askForAddImageToCurrentTrainingAndSave(*face,"customDatabase/");//Function ask the user if he want to add his face to the database and do it if the answer is positif
					}
					else
					{
						if (!myRecognizer.askForAddImageInProcess()) //if we are not already asking for add image if we ask in an other thread
						{
							std::cout << "Good Name ? y/n" << std::endl; //Ask the user if the name given to the face detected is correct
							std::cin >> goodName;
							if (goodName == "n" || goodName == "N")//If the user said that the name isn't correct ask him if he want to add the correct name to the face in the database
							{
								myRecognizer.askForAddImageToCurrentTrainingAndSave(*face, "customDatabase/"); //Function ask the user if he want to add his face to the database and do it if the answer is positif
							}
						}
					}
					myVideo.setImgToPrint(imgNull); //Remove the second windows printing the detected face
					myVideo.clearLabel(); //remove the name recognized of the face detected
				}
			}
			else if (!myVideo.getDetectFaceOn()) //If you want to recognize a face but you have not activated the face detection
			{
				std::cout << "Face detection need to be on for face recognition" << std::endl;
			}
			else if (!myRecognizer.isTrained()) //If the model is trained
			{
				std::cout << "data is trained wait a moment and retry" << std::endl;
			}
			else
			{
				std::cout << "You need for face recognition to wait that a circle appears around your face to show that the program have detected your face" << std::endl;
			}
		}
	}
	getchar();
	return 0;
}

//If you use an other language model that mine you can change the process on recognition function for adapt it to your need.
/*std::string processOnRecognitionPersonnalized(std::string dataToProcess)
{
	std::vector<std::string> vectorString;
	std::string stringRetour;
	bool robotlabSays = false, detectionSays = false, reconnaissanceSays = false;

	std::string::size_type stTemp = dataToProcess.find(" ");

	while (stTemp != std::string::npos)
	{
		vectorString.push_back(dataToProcess.substr(0, stTemp));
		dataToProcess = dataToProcess.substr(stTemp + 1);
		stTemp = dataToProcess.find(" ");
	}
	vectorString.push_back(dataToProcess);

	//std::cout << "vector size: " << vectorString.size() << std::endl;

	for (int i = 0; i < vectorString.size(); i++)
	{
		//std::cout << "value in treatment: " << vectorString[i].data() << std::endl;
		if (vectorString[i] == "robotlab")
		{
			robotlabSays = true;
			if (!stringRetour.empty())
			{
				stringRetour.clear();
			}
			stringRetour += vectorString[i];
		}
		else if (robotlabSays)
		{
			if (detectionSays)
			{
				if (vectorString[i] == "faciale" || vectorString[i] == "sourires" || vectorString[i] == "yeux")
				{
					stringRetour += " " + vectorString[i];
				}
				detectionSays = false;
				robotlabSays = false;
			}
			else if (reconnaissanceSays)
			{
				if (vectorString[i] == "faciale")
				{
					stringRetour += " " + vectorString[i];
				}
				reconnaissanceSays = false;
				robotlabSays = false;
			}
			else if (vectorString[i] == "reconnaissance")
			{
				stringRetour += " " + vectorString[i];
				reconnaissanceSays = true;
			}
			else if (vectorString[i] == "detection")
			{
				stringRetour += " " + vectorString[i];
				detectionSays = true;
			}
			else
			{
				stringRetour += " " + vectorString[i];
				robotlabSays = false;
			}
		}
	}

	if (!stringRetour.empty())
	{
		return stringRetour;
	}
	else
	{
		return "invalide";
	}
}*/