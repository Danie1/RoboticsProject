#include "CellMatrix.h"

#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"

#define CLEAR_CELL_CHAR 	(" ")
#define OBSTACLE_CELL_CHAR 	("*")
#define START_CELL_CHAR 	("S")
#define ROUTE_CELL_CHAR 	("R")
#define FINISH_CELL_CHAR 	("F")
#define DEBUG_CELL_CHAR		("#")
#define WAYPOINT_CELL_CHAR	("W")
const char* CELL_SYMBOLS[eCellState_numOfStates] = {CLEAR_CELL_CHAR,
												   OBSTACLE_CELL_CHAR,
												   START_CELL_CHAR,
												   ROUTE_CELL_CHAR,
												   FINISH_CELL_CHAR,
												   DEBUG_CELL_CHAR,
												   WAYPOINT_CELL_CHAR};


const int CLEAR_PIXEL_COLOR[4] = {255,255,255,255};
const int OBSTACLE_PIXEL_COLOR[4] = {0,0,0,255};
const int START_PIXEL_COLOR[4] = {0,255,0,255};
const int ROUTE_PIXEL_COLOR[4] = {255,0,0,255};
const int FINISH_PIXEL_COLOR[4] = {0,0,255,255};
const int DEBUG_PIXEL_COLOR[4] = {150,150,150,255};
const int WAYPOINT_PIXEL_COLOR[4] = {150,150,150,255};

const int* PIXEL_COLOR[eCellState_numOfStates] = {CLEAR_PIXEL_COLOR,
												  OBSTACLE_PIXEL_COLOR,
												  START_PIXEL_COLOR,
												  ROUTE_PIXEL_COLOR,
												  FINISH_PIXEL_COLOR,
												  DEBUG_PIXEL_COLOR,
												  WAYPOINT_PIXEL_COLOR};


CellMatrix::CellMatrix() : m_width(0), m_height(0)
{
}

CellMatrix::~CellMatrix(){}

void CellMatrix::CopyMatrix(CellMatrix &matrix)
{
	for (int i = 0; i < m_height; i++)
		for (int j = 0; j < m_width; j++)
			SetCell(i, j, matrix.GetCell(i, j));
}

void CellMatrix::SetCell(int row, int col, ECellState eType)
{
	if (row < m_height && row >= 0 && col < m_width && col >= 0)
		m_matrix[row][col] = eType;
	else
		printf("Tried to set a cell that doesn't exist! - (%d, %d) \r\n", row, col);
}

bool CellMatrix::IsNearObstacle(dword row, dword col)
{
	int const nearRadious = 1;
	for (int i = row - nearRadious; i <= row + nearRadious; i++)
	{
		for (int j = col - nearRadious; j <= col + nearRadious; j++)
		{
			if ((i < m_height && i >= 0 && j < m_width && j >= 0) &&
					(m_matrix[i][j] == eCellState_obstacle))
			{
				return true;
			}
		}
	}

	return false;
}

ECellState CellMatrix::GetCell(int row, int col)
{
	return m_matrix[row][col];
}

void CellMatrix::Resize(unsigned int height, unsigned int width)
{
	m_height = height;
	m_width = width;

	m_matrix.resize(m_height);
	for (dword i = 0; i<m_height; i++)
	{
		m_matrix[i].resize(m_width, eCellState_clear);
	}
}

dword CellMatrix::GetHeight()
{
	return m_height;
}

dword CellMatrix::GetWidth()
{
	return m_width;
}

bool CellMatrix::IsCellClear(dword row, dword col)
{
	if (row > m_height || col > m_width)
	{
		printf("row or call are out of bounds\n");
		printf("row = %d, col = %d, m_height = %d, m_width = %d\n", row, col, m_height, m_width);
		exit(0);
	}
	return m_matrix[row][col] == eCellState_clear;
}

void CellMatrix::LoadFromFile(const char* mapFileName)
{
	auto IsCellColorClear = [](byte cellVal) {return cellVal == 255;};

	vector<byte> pixels;
	unsigned error = lodepng::decode(pixels, m_width, m_height, mapFileName);
	if (error)
	{
		printf("decoder error %d : %s\n", error, lodepng_error_text(error));
		exit(-1);
	}

	Resize(GetHeight(), GetWidth());

	int pixelIter = 0;
	for (dword row = 0; row < GetHeight(); row++)
	{
		for (dword col = 0; col < GetWidth(); col++)
		{
			SetCell(row, col, IsCellColorClear(pixels[pixelIter]) ? eCellState_clear : eCellState_obstacle);

			pixelIter+= 4;
		}
	}
}

void CellMatrix::SaveToFile(const char* mapFileName)
{
	auto ColorCell = [&](vector<unsigned char>& pixels_new, int row, int col, int* pixelsIter)
				    	{
							pixels_new[(*pixelsIter)++] = PIXEL_COLOR[GetCell(row, col)][0];
							pixels_new[(*pixelsIter)++] = PIXEL_COLOR[GetCell(row, col)][1];
							pixels_new[(*pixelsIter)++] = PIXEL_COLOR[GetCell(row, col)][2];
							pixels_new[(*pixelsIter)++] = PIXEL_COLOR[GetCell(row, col)][3];
					    };

	vector<unsigned char> pixels_new;
	pixels_new.resize(GetHeight()*GetWidth()*4);

	int pixelsIter = 0;
	for (dword i = 0; i < GetHeight(); ++i)
	{
		for (dword j = 0; j < GetWidth(); ++j)
		{

			ColorCell(pixels_new, i, j, &pixelsIter);
		}

	}

	unsigned error = lodepng::encode(mapFileName, pixels_new, GetWidth(), GetHeight());
	if (error)
	{
		printf("encoder error %d : %s\n", error, lodepng_error_text(error));
		exit(-1);
	}
}

void CellMatrix::printMatrix()
{
	for (dword row = 0; row < m_height; row++)
	{
		for (dword col = 0; col < m_width; col++)
		{
			printf("%s",CELL_SYMBOLS[m_matrix[row][col]]);
		}
		printf("\n");
	}
}


bool CellMatrix::IsThereObstacleBetweenPoints(Point firstPoint, Point secondPoint)
{
	if (firstPoint.GetX() >= m_width || firstPoint.GetX() <= 0 ||
		firstPoint.GetY() >= m_height|| firstPoint.GetY() <= 0 ||
		secondPoint.GetX() >= m_width || secondPoint.GetX() <= 0 ||
		secondPoint.GetY() >= m_height || secondPoint.GetY() <= 0)
	{
		printf("IsThereObstacleBetweenPoints:: out of bounds\n");
		exit(-1);
	}

	double m = (secondPoint.GetY() - firstPoint.GetY()) / (secondPoint.GetX() - firstPoint.GetX());

	printf("line between points\n");

	if (firstPoint.GetX() < secondPoint.GetX())
	{
		for (int xVal = firstPoint.GetX(); xVal <= secondPoint.GetX(); xVal++)
		{
			int yVal = m*xVal -m*firstPoint.GetX() + firstPoint.GetY();
			printf("(%d , %d)\n", xVal,yVal);
			if (m_matrix[yVal][xVal] == eCellState_obstacle)
			{
				return true;
			}
//			m_matrix[yVal][xVal] = ecellState_debug;
		}
	}
	else
	{
		for (int xVal = secondPoint.GetX(); xVal <= firstPoint.GetX(); xVal++)
		{
			int yVal = m*xVal -m*firstPoint.GetX() + firstPoint.GetY();
			printf("(%d , %d)\n", xVal,yVal);
			if (m_matrix[yVal][xVal] == eCellState_obstacle)
			{
				return true;
			}
//			m_matrix[yVal][xVal] = ecellState_debug;
		}
	}

	return false;

}




