#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void pixelRGBtoLMS(int& red, int& green, int& blue);
void pixelHumanToDog(int& red, int& green, int& blue);
void pixelLMStoRGB(int& red, int& green, int& blue);
int intFromBinary(fstream& stream, int offset);

int main(int argc, char *argv[]) {
	//Make sure that the command line uses 2 inputs
	if(argc != 2) {
		cout << "Input error! Please reformat your entry.\n";
	}
	
	else {
		//Get the filepath and store it as a string
		string filePath = argv[1];
		
		//Initiate a filestream object
		fstream stream;
		stream.open(filePath.c_str(), ios::in | ios::out | ios::binary);
		
		//Get the integers from the binary file
		int fileSize = intFromBinary(stream, 2);
		int start = intFromBinary(stream, 10);
		int width = intFromBinary(stream, 18);
		int height = intFromBinary(stream, 22);
		
		//Prepare for processing the file
		int scanLineSize = width * 3;
		int padding = 0;
		if(scanLineSize % 4 != 0) {
			padding = 4 - scanLineSize % 4;
		}
		if(fileSize != start + (scanLineSize + padding) * height) {
			cout << "Invalid image: not a 24-bit true-color image!.\n";
			return 1;
		}
		
		//Head to the start of the stream object
		stream.seekg(start);
		
		//Prompt user that conversion is starting
		cout << "Inverting image...";
		
		//Take each pixel of a bitmap and translate it to dog vision
		for(int row = 0; row < height; row++) {
			for(int col = 0; col < width; col++) {
				int pos = stream.tellg();
				
				//Grab the pixel color values from the file
				int blue = stream.get();
				int green = stream.get();
				int red = stream.get();
				
				//Pass the pixel into a function to change it from RGB to LMS
				pixelRGBtoLMS(red, green, blue);
				//Pass the pixel into a function to change it from human to dog LMS
				pixelHumanToDog(red, green, blue);
				//Pass the pixel into a function to change it back to RGB
				pixelLMStoRGB(red, green, blue);
			}
			stream.seekg(padding, ios::cur);
		}
		
		//Prompt user that the conversion is finished
		cout << "\nInversion complete!";
	}
	
	return 0;
}

//Pass the pixel into a function to change it from RGB to LMS
void pixelRGBtoLMS(int& red, int& green, int& blue) {
	//Store the values of the pixels before modifying
	int origRed = red;
	int origGrn = green;
	int origBlu = blue;
	
	//Create conversion matrix for this step (from internet)
	float conversionMatrix[3][3] = {
		{17.8824, 43.5161, 4.1193},
		{3.4557, 27.1554, 3.8671},
		{0.02996, 0.18431, 1.4670},
	};
	
	//Do the conversion
	red = origRed * conversionMatrix[0][0] + origGrn * conversionMatrix[0][1] + origBlu * conversionMatrix[0][2];
	green = origRed * conversionMatrix[1][0] + origGrn * conversionMatrix[1][1] + origBlu * conversionMatrix[1][2];
	blue = origRed * conversionMatrix[2][0] + origGrn * conversionMatrix[2][1] + origBlu * conversionMatrix[2][2];
}

//Pass the pixel into a function to change it from human to dog LMS
void pixelHumanToDog(int& red, int& green, int& blue) {
	//Store the values of the pixels before modifying
	int origRed = red;
	int origGrn = green;
	int origBlu = blue;
	
	//Create conversion matrix for this step (from internet)
	float conversionMatrix[3][3] = {
		{0, 2.02344, -2.52581},
		{0, 1, 0},
		{0, 0, 1},
	};
	
	//Do the conversion
	red = origRed * conversionMatrix[0][0] + origGrn * conversionMatrix[0][1] + origBlu * conversionMatrix[0][2];
	green = origRed * conversionMatrix[1][0] + origGrn * conversionMatrix[1][1] + origBlu * conversionMatrix[1][2];
	blue = origRed * conversionMatrix[2][0] + origGrn * conversionMatrix[2][1] + origBlu * conversionMatrix[2][2];	
}

//Pass the pixel into a function to change it back to RGB
void pixelLMStoRGB(int& red, int& green, int& blue) {
	//Store the values of the pixels before modifying
	int origRed = red;
	int origGrn = green;
	int origBlu = blue;
	
	//Create conversion matrix for this step (from internet)
	float conversionMatrix[3][3] = {
		{0.0809, -0.1305, 0.1167},
		{-0.0102, 0.0540, -0.1136},
		{-0.0003, -0.0041, 0.6935},
	};
	
	//Do the conversion
	red = origRed * conversionMatrix[0][0] + origGrn * conversionMatrix[0][1] + origBlu * conversionMatrix[0][2];
	green = origRed * conversionMatrix[1][0] + origGrn * conversionMatrix[1][1] + origBlu * conversionMatrix[1][2];
	blue = origRed * conversionMatrix[2][0] + origGrn * conversionMatrix[2][1] + origBlu * conversionMatrix[2][2];
}

//Extract an integer from a binary number
int intFromBinary (fstream& stream, int offset) {
	stream.seekg(offset);
	
	int result = 0;
	int base = 1;
	
	for(int i = 0; i < 4; i++) {
		result = result + stream.get() * base;
		base = base * 256;
	}
	
	return result;
}