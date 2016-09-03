//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <cmath>

#define PI 3.1415926

// Constructor and Desctructors
MyImage::MyImage()
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if (Data)
		delete Data;
}


// Copy constructor
MyImage::MyImage(MyImage *otherImage)
{
	Height = otherImage->Height;
	Width = otherImage->Width;
	Data = new char[Width*Height * 3];
	strcpy(otherImage->ImagePath, ImagePath);

	for (int i = 0; i < (Height*Width * 3); i++)
	{
		Data[i] = otherImage->Data[i];
	}
}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width = otherImage.Width;
	Data = new char[Width*Height * 3];
	strcpy((char *)otherImage.ImagePath, ImagePath);

	for (int i = 0; i < (Height*Width * 3); i++)
	{
		Data[i] = otherImage.Data[i];
	}

	return *this;

}

// MyImage::CreatImageCanv
// Function to create white image with two dots connected
bool MyImage::CreatImageCanv(int n)
{
	// Allocate Data structure and copy
	Data = new char[Width*Height * 3]; //BGR Order
	for (int i = 0; i < Height*Width; i++)
	{
		Data[3 * i] = 255;
		Data[3 * i + 1] = 255;
		Data[3 * i + 2] = 255;
	}

	// vertices of the square
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = Width-1;
	int x3 = Width - 1, y3 = Height - 1;
	int x4 = Width - 1, y4 = 0;
	DrawLine(x1, y1, x2, y2);
	DrawLine(x2, y2, x3, y3);
	DrawLine(x3, y3, x4, y4);
	DrawLine(x4, y4, x1, y1);

	double exrad = 2 * PI / n;
	double rad = 0;
	int sx = Width/2-1 , sy = Height/2-1;
	int ex = Width/2-1, ey = 0;
	DrawLine(sx, sy, ex, ey);
	while (n > 0)
	{
		if ((rad <= (0.25 * PI)) || (rad >= (1.75 * PI)))
		{
			ex = sx + Height / 2 * tan(rad);
			ey = 0;
		}
		else if (rad <= (0.75 * PI))
		{
			ex = Width - 1;
			ey = sy - Width / 2 * tan(0.5*PI - rad);
		}
		else if (rad <= (1.25 * PI))
		{
			ex = sx - Height / 2 * tan(rad);
			ey = Height - 1;
		}
		else if (rad <= (1.75 * PI))
		{
			ex = 0;
			ey = sy + Width / 2 * tan(1.5*PI - rad);
		}
		DrawLine(sx, sy, ex, ey);
		rad += exrad;
		n--;
	}

	return true;
}

bool MyImage::DrawLine(int x1, int y1, int x2, int y2)
{
	int start_x = x1, start_y = y1, end_x = x2, end_y = y2;
	int dx = x2 - x1;
	int dy = y2 - y1;
	double slope = 0;

	bool anchor_x;		//True if -1<=slope<=1, False if slope >1 or slope<-1
	if (dx != 0)
		slope = dy / (double)dx;

	if (abs(slope) <= 1 && dx != 0)
	{
		anchor_x = true;
		if (dx < 0)		//make dx always be positive
		{
			start_x = x2;
			start_y = y2;
			end_x = x1;
			end_y = y1;
		}
		slope = (end_y - start_y) / (double)(end_x - start_x);
	}
	else 
	{
		anchor_x = false;
		if (dy < 0)		//make dy always be positive
		{
			start_x = x2;
			start_y = y2;
			end_x = x1;
			end_y = y1;
		}
		if (dx == 0) 
		{ 
			slope = 0; 
		}
		else 
		{ 
			slope = (end_x - start_x) / (double)(end_y - start_y); 
		}
	}

	if (anchor_x) 
	{
		/*if (start_x < 0)
		{
			if (slope != 0)
				start_y -= start_x * slope;				
			start_x = 0;
		}
		if (end_x > Width-1)
			end_x = Width;*/
		double y = start_y + 0.5;
		for (int x = start_x + 1; x <= end_x; x++) 
		{
			y = y + slope;
			int int_y = (int)floor(y);
			Data[int_y*Width * 3 + x * 3] = 0;
			Data[int_y*Width * 3 + x * 3 + 1] = 0;
			Data[int_y*Width * 3 + x * 3 + 2] = 0;
		}
	}
	else 
	{
		/*if (start_y < 0)
		{
			if (slope != 0)
				start_x -= start_y * slope;
			start_y = 0;
		}
		if (end_y > 511)
			end_y = 511*/;
		double x = start_x + 0.5;
		for (int y = start_y + 1; y <= end_y; y++) 
		{
			x = x + slope;
			int int_x = (int)floor(x);
			Data[y*Width * 3 + int_x * 3] = 0;
			Data[y*Width * 3 + int_x * 3 + 1] = 0;
			Data[y*Width * 3 + int_x * 3 + 2] = 0;
		}
	}

	// set starting point to zero first
	Data[start_y*Width * 3 + start_x * 3] = 0;
	Data[start_y*Width * 3 + start_x * 3 + 1] = 0;
	Data[start_y*Width * 3 + start_x * 3 + 2] = 0;

	return true;
}

// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}

	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if (IN_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width];
	char *Gbuf = new char[Height*Width];
	char *Bbuf = new char[Height*Width];

	for (i = 0; i < Width*Height; i++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}

	// Allocate Data structure and copy
	Data = new char[Width*Height * 3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3 * i] = Bbuf[i];
		Data[3 * i + 1] = Gbuf[i];
		Data[3 * i + 2] = Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0)
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}

	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if (OUT_FILE == NULL)
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width];
	char *Gbuf = new char[Height*Width];
	char *Bbuf = new char[Height*Width];

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3 * i];
		Gbuf[i] = Data[3 * i + 1];
		Rbuf[i] = Data[3 * i + 2];
	}


	// Write data to file
	for (i = 0; i < Width*Height; i++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}




// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.
bool MyImage::Modify(const MyImage &img, double scale)
{
	/*int oriw = Width;
	int orih = Height;
	double scale = 2.0;
	Width = Width / scale;
	Height = Height / scale;*/
	int oriw = img.Width;
	int orih = img.Height;
	Width = oriw / scale;
	Height = orih / scale;
	Data = new char[Width*Height * 3];

	//int s = (Height / 2 - 1)*Width*scale + (Width / 2 - 1);
	int i = 0, s = 0, cl = 0, ch = 0;
	while (s < Width*Height)
	{
		Data[3 * s] = img.Data[3*i];
		Data[3 * s + 1] = img.Data[3*i+1];
		Data[3 * s + 2] = img.Data[3*i+2];
		s++;
		cl++;
		if (cl >= Width)
		{
			ch += scale;
			cl = 0;
			i = ch * oriw;
		}
		else
			i += scale;		
	}

	return false;
}