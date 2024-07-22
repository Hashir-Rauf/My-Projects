#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include "ImageProcessor.h"
using namespace std;
Image::Image(const char * location)
{
	strcpy(loc,location);
	fstream File;
	File.open(location);
	File.getline(format,4);
	File.ignore();
	File.getline(comment,100);
	File >> Col >> Row >> Depth;
	Data = new int *[Row];
	for(int i = 0 ; i < Row; i++)
	Data[i] = new int [Col];
	for(int i = 0 ; i < Row ; i++)
	for(int j = 0 ; j < Col ; j++)
	File >> Data[i][j];
	cout << "Image loaded" <<endl;
}
Image::	~Image()
{
	for(int i = 0 ; i < Row ; i++)	
		delete[] Data[i];
	delete[] Data;
	cout << "Image unloaded" << endl;
}
void Image::savechanges(const char * location)
{
	ofstream Saveas(location);
    if (!Saveas.is_open()) 
	{
    	cout << "Error saving file: " << location << endl;
        // Handle the error, maybe throw an exception or exit the program
   	}	
	else
	{
		Saveas << "P2\n#Created by Hitler" << endl;
       	Saveas << Col << " " << Row << endl << Depth << endl;
        for (int r = 0; r < Row; r++)
        	{
            	for (int c = 0; c < Col; c++)
           			{
                		Saveas << Data[r][c] << " ";
            		}
            		Saveas << endl;
        	}
       	Saveas.close();
		cout << "Recreated at " << location << endl;	
	}	
}
void Image::VerticalFlip() 
		{ // it do nothing else taking the center and swapping below and above part
        int temp;
        for (int i = 0; i < Row / 2; i++) {
            for (int j = 0; j < Col; j++) {
                temp = Data[i][j];
                Data[i][j] = Data[Row - 1 - i][j];
                Data[Row - 1 - i][j] = temp;
            }
        }
        cout << "Flipped" << endl;
    }
    void Image:: RHorizontalFlip() { // it do nothing else taking the center and swapping left and right part
        int temp;
        for (int i = 0; i < Row; i++)
        {
            for (int j = 0; j < Col / 2; j++) {
                temp = Data[i][j];
                Data[i][j] = Data[i][Col - 1 - j];
                Data[i][Col - 1 - j] = temp;

            }
        }
    };
    void Image::NegativeEffect() {
        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < Col; ++j) {
                // Inverting pixel values by subtracting them from the maximum value
                Data[i][j] = ((Depth)-Data[i][j]); // in negative actually the rgb value is inverted
                // 
            }
        }
    }
    void Image::Blackpoint() {
        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < Col; ++j) {
                // Inverting pixel values by subtracting them from the maximum value
                Data[i][j] = 12 + (Data[i][j]); // in negative actually the rgb value is inverted
                // 
            }
        }

    }
    void Image::LinearCont() {
        int min = 255; // white
        int max = 0;// black
        for (int i = 0; i < Row; i++)
        {
            for (int j = 0; j < Col; j++) {
                if (Data[i][j] < min) {
                    min = Data[i][j];//simple if anything more brighter
                    // checking if any thing less than assigned min
                }
                if (Data[i][j] > max) {
                    // same as above anything more darker than dark 0
                    max = Data[i][j];
                }

            }
        }
        for (int i = 0; i < Row; i++)
        {
            for (int j = 0; j < Col; j++)
            {
                Data[i][j] = (Data[i][j] - min) * 255;
                //replaced the new values by subtracting white color any
                // multiplying max depth

            }

        }

    }
    void Image::Derivative() {
        int minVal = Data[0][0];
        int maxVal = Data[0][0];

        // Csb se phle derivatives along x jitna difference hoga wo derivative hoga

        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < Col - 1; ++j) {
                // derivative calculate krta hua

                int derivative = Data[i][j + 1] - Data[i][j];

                //  min and max values ka check 
                if (derivative < minVal) {
                    minVal = derivative;
                }
                if (derivative > maxVal) {
                    maxVal = derivative;
                }

                // Store the derivative value
                Data[i][j] = derivative;
            }
            // akhri column zero krta hua ... 
            Data[i][Col - 1] = 0;
        }

        // tasweer ko grey krna k lya thora sa makeup
        // upr min max k zariya aik customized range tk ka derivative
        // simple to fit pixels in between 0-255
        int range = maxVal - minVal;
        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < Col; ++j) {
                Data[i][j] = ((Data[i][j] - minVal) * 255) / range;
            }                // min se differ         // 0-255 k lya scale 
        }
        // Now, the Data array holds the adjusted derivative values within 0-255 range
    }
    void Image::IncreaseBrightness(int factor) {
        for (int i = 0; i < Row; i++) {
            for (int j = 0; j < Col; j++) {
                int newValue = Data[i][j] * factor;
                Data[i][j] = (newValue > 255) ? 255 : newValue;
            }
        }
    }

    void Image::Mean(int ratio) {
    	ratio = 3;
        int matrix_size = ratio;
        int square = matrix_size * matrix_size;

        for (int i = 0; i < Row; i++)
        {
            for (int j = 0; j < Col; j++) {
                //taking median 
                int mean = 0;
                for (int row = -matrix_size / 2; row < matrix_size / 2; row++)
                {
                    for (int col = -matrix_size / 2; col < matrix_size / 2; col++) {

                        // find width and height
                        int height = i + row;
                        int width = j + col;//height check and width check
                        if ((height >= 0 && width >= 0) && (height < Row && width < Col)) {
                            mean += Data[height][width];//replacing sum at every index
                        }

                    }
                }
                Data[i][j] = mean / square;// mean filtering sum to mean by dividing
            }
        }
    }
    void Image::Median() {
        int matrix_size = 3;

        for (int i = 0; i < Row; i++) {
            for (int j = 0; j < Col; j++) {
                int value[9];//array for later sorting
                int index = 0;//index for incrementation

                for (int row = -matrix_size / 2; row <= matrix_size / 2; row++) {
                    for (int col = -matrix_size / 2; col <= matrix_size / 2; col++) {
                        int height = i + row;// same boundary check not excede the matrix
                        int width = j + col;
                        // width height check here
                        if ((height >= 0 && width >= 0) && (height < Row && width < Col)) {
                            value[index++] = Data[height][width];//placement
                        }
                    }
                }

                // Sort the values to find the median using a more efficient sorting algorithm
                for (int k = 0; k < index - 1; k++) {
                    for (int l = 0; l < index - k - 1; l++) {
                        if (value[l] > value[l + 1]) {
                            int temp = value[l];//bubble sort
                            value[l] = value[l + 1];
                            value[l + 1] = temp;
                        }
                    }
                }

                int median;
                if (index % 2 == 0) {
                    //for evens
                    median = (value[index / 2] + value[index / 2 - 1]) / 2; // If even number of elements
                }
                else {
                    //for odds
                    median = value[index / 2]; // If odd number of elements
                }

                Data[i][j] = median;
            }
        }
    }
    void Image:: RotateImageClockwise() {
        //vector<vector<int>> temp(Col, vector<int>(Row, 0)); // Create a temporary vector with swapped dimensions
		int ** temp = new int *[Col];
		for(int  i = 0 ; i < Col ; i++)
			temp[i] = new int [Row];
        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < Col; ++j) {
                temp[j][Row - i - 1] = Data[i][j]; // Rotate clockwise
                //transpose
            }
        }

        // Update Data with the rotated image
        Data = temp;
        for(int i = 0 ; i < Col ; i++)
        	delete[] temp[i];
        delete[] temp;
    }

    void Image::RotateImageAntiClockwise()
    {
       int ** temp = new int *[Col];
		for(int  i = 0 ; i < Col ; i++)
			temp[i] = new int [Row];
			
        for (int i = 0; i < Row; ++i) {
            for (int j = 0; j < Col; ++j) {
                temp[Col - j - 1][i] = Data[i][j]; // Rotate anticlockwise
                //transpose along cols
            }
        }

        // Update Data with the rotated image
        Data = temp;
        for(int i = 0 ; i < Col ; i++)
        	delete[] temp[i];
        delete[] temp;
    }


    void Image::RotateImageByAngle(double angle) {


        const double PI = 3.141592;
        double Radians = angle * PI / 180.0;// changing angle into radians for cmath

        double cosTheta = cos(Radians);// horizontal angle
        double sinTheta = sin(Radians);//vertical angle

        double centerX = static_cast<double>(Col) / 2.0; // finding center along X
        double center_alongY = static_cast<double>(Row) / 2.0;// finding center along Y

        // Calculating the dimensions of the rotated image 
        int rotatedCols = abs(cosTheta) * Col + abs(sinTheta) * Row;
        int rotatedRows = abs(sinTheta) * Col + abs(cosTheta) * Row;
        // new vector to get rotated image
        //vector<vector<int>> rotatedData(rotatedRows, vector<int>(rotatedCols, 0));
		int ** rotatedData = new int * [rotatedRows];
		for(int i = 0 ; i < rotatedRows; i++)
			rotatedData[i] = new int[rotatedCols];
        //loop to rotate
        for (int r = 0; r < rotatedRows; ++r) {
            for (int c = 0; c < rotatedCols; ++c) {
                double rotx = c - centerX - (rotatedCols - Col) / 2.0; // difference from center of onPoint index along x
                double roty = r - center_alongY - (rotatedRows - Row) / 2.0;// difference from center of onPoint index along Y

                double changedX = rotx * cosTheta + roty * sinTheta + centerX;// new location difference from center
                double changedY = -rotx * sinTheta + roty * cosTheta + center_alongY;

                int x = changedX;//house 
                int y = changedY;
                int x1 = x + 1;//house neighbour you got it
                int y1 = y + 1;

                if (x >= 0 && x1 < Col && y >= 0 && y1 < Row) {
                    double locX1 = changedX - x;// toget new values of rotated image
                    double locX0 = 1.0 - locX1;
                    double locY1 = changedY - y;
                    double locY0 = 1.0 - locY1;

                    rotatedData[r][c] = locY0 * (locX0 * Data[y][x] + locX1 * Data[y][x1])
                        + locY1 * (locX0 * Data[y1][x] + locX1 * Data[y1][x1]);
                }
            }
        }


        Col = rotatedCols;// changing orignal Cols
        Row = rotatedRows;// changing orignal Rows
        Data = rotatedData;


    }


    // Apply a general linear filter to the image
    void Image::LinearFilter(int input) {

        double Filter_Kernel[3][3];// filter Filter_Kernel 3x3

        if (input == 1)
        {
            //reading sharpen kernel 
            // 0 -1 0 -1 5 -1 0 -1 0 from file

            ifstream Sharpen("D:/designs/sharpen.txt");
            if (Sharpen.is_open()) {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (!(Sharpen >> Filter_Kernel[i][j])) {
                            // error accordingly
                        }
                    }
                }
            }
            Sharpen.close(); // Close the file after reading
        }
        else if (input == 2)
        {
            //reading blur kernel from file 
            //  0.1 0.1 0.1
            //  0.1 0.1 0.1
            //  0.1 0.1 0.1 
            ifstream Blur("D:/designs/blur.txt");
            if (Blur.is_open()) {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (!(Blur >> Filter_Kernel[i][j])) {
                            //ngl
                        }
                    }
                }
            }
            Blur.close(); // Close the file after reading
        }
        else if (input == 3)
        {
            //reading edge detection filter from file
            // -1 -1 -1
            //  -1 8 -1
            //  -1 -1 -1 
            ifstream Edge("D:/designs/edge.txt");
            if (Edge.is_open()) {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (!(Edge >> Filter_Kernel[i][j])) {
                            // brb
                        }
                    }
                }
            }
            Edge.close(); // Close the file after reading
        }
        else if (input == 4)
        {
            // negative filter reading from file
            // -1 -1 -1 
            // 0 0 0 
            // 1 1 1 
            ifstream Neg("D:/designs/negative.txt");
            if (Neg.is_open()) {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (!(Neg >> Filter_Kernel[i][j])) {
                            //
                        }
                    }
                }
            }
            Neg.close(); // Close the file after reading
        }



        //vector<vector<int>> temporary;
        //temporary.clear();
        //temporary.resize(Row, vector<int>(Col, 0));// outline assigning of size
		int ** temporary = new int * [Row];
		for(int i = 0 ; i < Row ;i++)
			temporary[i] = new int[Col];
        for (int y = 1; y < Row - 1; y++) {
            for (int x = 1; x < Col - 1; x++) {
                float sum = 0.0;

                // Convolution: Multiply and accumulate pixel values based on the filter Filter_Kernel ----- for hashir
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        sum += Data[y + i][x + j] * Filter_Kernel[i + 1][j + 1];// getting kernel fitted into image
                    }
                }
                temporary[y][x] = sum;

            }
        }
        for (int y = 1; y < Row - 1; ++y) {
            for (int x = 1; x < Col - 1; ++x) {
                Data[y][x] = temporary[y][x];// replacing
            }
        }

    }

    void Image::Tobinary() 
	{
        // thresholding technique
        //check if any value if greater than that thres val then make the pixel 1 
        //else 0
        int threshold = 130;
        for (int i = 0; i < Row; i++)
        {
            for (int j = 0; j < Col; j++) {
                Data[i][j] = (Data[i][j] >= threshold) ? Depth : 0;
            }
        }
    }
    