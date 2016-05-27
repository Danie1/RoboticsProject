#include "Graph.h"
#include <chrono>
#include <functional>
#include <stdio.h>
#include <stdlib.h>

using namespace std::chrono;

Graph::Graph(Map& map, int Ratio) : m_ratio(Ratio)
{
	FromMapToGraph(map, Ratio);
}

void Graph::FromMapToGraph(Map& map, dword ratio)
{
	/**
	* Lamda definitions
	*/
	auto ClearMap = [](CellMatrix& matrix, int height, int width) {
															for (int i = 0; i < height; i++) 
																for (int j = 0; j < width; j++) 
																	matrix.SetCell(i, j, eCellState_clear);
														};

	// Checks if there is an obstacle in specified cell
	auto IsNoObstacleInCell = [&](CellMatrix& matrix, int row, int col) {return matrix.GetCell(row, col) != eCellState_obstacle;};

	// Puts an obstacle in cell
	auto setObstacleInCell = [&](CellMatrix& matrix, int row, int col) {matrix.SetCell(row, col, eCellState_obstacle); return true;};

	/**
	* Logic
	*/
	// It's ok if it doesn't divide perfectly. The map isn't relevant in the bottom corner anyway.
	Resize((map.GetHeight() / ratio) + ratio, (map.GetWidth() / ratio) + ratio);

	// Initialize the new map.
	ClearMap(*this, (map.GetHeight() / ratio) + ratio, (map.GetWidth() / ratio) + ratio);

	for (dword row = 0, new_row = 0; row < map.GetHeight(); row+=ratio, new_row++)
	{
		for (dword col = 0, new_col = 0; col < map.GetWidth(); col+=ratio, new_col++)
		{
			int end_row, end_col;

			if (row + ratio > map.GetHeight()) 
				end_row = map.GetHeight() - 1;
			else 
				end_row = row + ratio;

			if (col + ratio > map.GetWidth()) 
				end_col = map.GetWidth() - 1;
			else
				end_col = col + ratio;

			// Check if there is at least one obstacle in submap
			if (!map.ActionInSubMatrix(row, col, end_row, end_col, IsNoObstacleInCell))
			{
				// Color whole submap as an obstacle
				map.ActionInSubMatrix(row, col, end_row, end_col, setObstacleInCell);
				SetCell(new_row, new_col, eCellState_obstacle);
			}
		}
	}
}

void Graph::ConvertToMap(Map& map)
{
	using namespace std::placeholders;

	/**
	* Lamda definitions
	*/
	auto ClearMap = [](CellMatrix& matrix, int height, int width) {
															for (int i = 0; i < height; i++) 
																for (int j = 0; j < width; j++) 
																	matrix.SetCell(i, j, eCellState_clear);
														};

	// Checks if there is an obstacle in specified cell
	auto IsNoObstacleInCell = [&](CellMatrix& matrix, int row, int col) {return matrix.GetCell(row, col) != eCellState_obstacle;};

	// Puts an obstacle in cell
	auto setOrginalTypeInCell = [&](CellMatrix& matrix, int row, int col, int org_row, int org_col) 
								{matrix.SetCell(row, col, this->GetCell(org_row, org_col)); return true;};

	/**
	* Logic
	*/
	// It's ok if it doesn't divide perfectly. The map isn't relevant in the bottom corner anyway.
	map.Resize((GetHeight() * m_ratio), (GetWidth() * m_ratio));

	// Initialize the new map.
	ClearMap(map, (GetHeight() * m_ratio), (GetWidth() * m_ratio));

	for (dword row = 0, new_row = 0; row < GetHeight(); row++, new_row+=m_ratio)
	{
		for (dword col = 0, new_col = 0; col < GetWidth(); col++, new_col+=m_ratio)
		{
			int end_row, end_col;

			if (row + m_ratio > GetHeight()) 
				end_row = GetHeight() - 1;
			else 
				end_row = new_row + m_ratio;

			if (col + m_ratio > GetWidth()) 
				end_col = GetWidth() - 1;
			else
				end_col = new_col + m_ratio;

			auto setTypeInCell = bind<bool>(setOrginalTypeInCell, _1, _2, _3, row, col);

			// Color whole submap as an obstacle
			map.ActionInSubMatrix(new_row, new_col, end_row, end_col, setTypeInCell);
		}
	}
}


void Graph::SetOriginalCell(int row, int col, ECellState eType)
{
	if (row < GetHeight() && row > 0 && col < GetWidth() && col > 0)
		CellMatrix::SetCell(row / m_ratio, col / m_ratio,  eType);
}

ECellState Graph::GetOriginalCell(int row, int col)
{
	return CellMatrix::GetCell(row / m_ratio, col / m_ratio);
}

void Graph::SetRouteOnGraph(vector<vector<int> >& intMap)
{
	for (dword row = 0; row < GetHeight(); ++row)
	{
		for (dword col = 0; col < GetWidth(); ++col)
		{
			SetCell(row, col, ECellState(intMap[col][row]));
		}
	}
}

bool Graph::IsOriginalCellClear(dword row, dword col)
{
	if (row / m_ratio > GetHeight() || col / m_ratio > GetWidth() )
	{
		printf("row or call are out of bounds\n");
		printf("row = %d, col = %d, m_height = %d, m_width = %d\n", row / m_ratio, col / m_ratio, GetHeight(), GetWidth());
		exit(0);
	}
	return GetCell(row, col) == eCellState_clear;
}

bool Graph::GetRandomStartAndFinishPoints(dword& xStart, dword& yStart, dword& xFinish, dword& yFinish)
{
	vector<pair<int, int>> FreeCell;

	for (dword row = 0; row < GetHeight(); ++row)
	{
		for (dword col = 0; col < GetWidth(); ++col)
		{
			if (IsCellClear(row, col))
			{
				FreeCell.push_back(make_pair(row,col));
			}
		}
	}

	milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

	srand (ms.count());
	
	int randNum = rand()%(FreeCell.size() + 1);

	xStart = FreeCell.at(randNum).first;
	yStart = FreeCell.at(randNum).second;

	randNum = rand()%(FreeCell.size() + 1);

	xFinish = FreeCell.at(randNum).first;
	yFinish = FreeCell.at(randNum).second;

	printf("Start: (%d, %d) || Finish: (%d, %d) \r\n", xStart, yStart, xFinish, yFinish);
}
