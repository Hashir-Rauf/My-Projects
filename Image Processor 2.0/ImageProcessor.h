class Image
{
	private:
		int Row;
		int Col;
		int Depth;
		char format[5];
		char comment[100];
		int ** Data;
		char loc[200];
	public:
		Image(const char * location);
		void savechanges(const char * location);
		~Image();
		void VerticalFlip();
		void NegativeEffect();
		void RHorizontalFlip();
		void Blackpoint();
		void LinearCont();
		void Derivative();
		void IncreaseBrightness(int factor);
		void Mean(int);
		void Median();
		void RotateImageClockwise();
		void RotateImageAntiClockwise();
		void RotateImageByAngle(double angle);
		void LinearFilter(int input);
		void Tobinary();
};