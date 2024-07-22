#include <iostream>
#include "ImageProcessor.cpp"
using namespace std;
int main()
{
	char location[200];
	cout << "Enter the directory of the Image" << endl;
	//cin.ignore();
	cin.getline(location,200);
	Image image(location);	
	char choice = 'y';
	do
	{
		int c;
		cout << "Enter your choice\n1:Save an Image" << endl
		<< "2:Vertical Flip" << endl
		<< "3:Horizontal Flip" << endl
		<< "4:Blackpoint Effect" << endl
		<<"5:Derivative  Effect" << endl
		<<"6:Exposure" << endl
		<<"7:Constrast" << endl
		<<"8:Linear Filter" << endl
		<<"9:Mean Effect" << endl
		<<"10:Median Effect" << endl
		<<"11:Invert" << endl
		<<"12:Rotate AntiClockwise" << endl
		<<"13:Rotate" << endl
		<<"14:Rotate Clockwise" << endl
		<<"15:Convert to binary" << endl;
		cin >> c;
		switch(c)
		{
			case 1:
				{
					char destination[200];
					cout << "Enter the path where you want to save: ";
					cin.ignore();
					cin.getline(destination,200);
					image.savechanges(destination);
					break;
				}
			case 2:
				{
					image.VerticalFlip();
					break;
				}
			case 3:
				{
					image.RHorizontalFlip();
					break;
				}	
			case 4:
				{
					image.Blackpoint();
					break;
				}
			case 5:
				{
					image.Derivative();
					break;
				}
			case 6:
				{
					int factor = 0;
					cout << "Enter the factor:";
					cin >> factor;					
					image.IncreaseBrightness(factor);
					break;
				}
			case 7:
				{
					image.LinearCont();
					break;
				}
			case 8:
				{
					int factor = 0;
					cout << "Enter the factor:";
					cin >> factor;	
					image.LinearFilter(factor);
					break;
				}
			case 9:
				{	int factor = 0;
					cout << "Enter the factor:";
					cin >> factor;	
					image.Mean(factor);
					break;
				}	
			case 10:
				{
					image.Median();
					break;
				}
			case 11:
				{
					image.NegativeEffect();
					break;
				}
			case 12:
				{
					image.RotateImageAntiClockwise();
					break;
				}
			case 13:
				{
					double angle = 0.0;
					cout << "input the angle:";
					cin >> angle;
					image.RotateImageByAngle(angle);
					break;
				}
			case 14:
				{
					image.RotateImageClockwise();
					break;
				}
			case 15:
				{
					image.Tobinary();
					break;
				}									
			default:
				{
					cout << "Illegal Command" << endl;
					break;
				}
		}
		cout << "Do  you wish to continue editing? (y/n)";
		cin >> choice;
		cin.ignore();
	}
	while(choice !='n' && choice !='N');
	return 0;
}