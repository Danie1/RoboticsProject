/*
 * Map.h
 *
 *  Created on: Mar 22, 2016
 *      Author: Or Gafni and Daniel Saad
 */

#ifndef CELL_MATRIX_H_
#define CELL_MATRIX_H_

#include <vector>
#include "CommonStructs.h"

using namespace std;

#include "typedefs.h"

class CellMatrix
{
private:
	dword m_width;
	dword m_height;
	vector<vector<ECellState> > m_matrix;
public:
	CellMatrix();
    ~CellMatrix();

	virtual void SetCell(int row, int col, ECellState eType);
	virtual ECellState GetCell(int row, int col);
	virtual bool IsCellClear(dword row, dword col);

	bool IsNearObstacle(dword row, dword col);

	void Resize(unsigned int height, unsigned int width);
	void CopyMatrix(CellMatrix &matrix);

	dword GetHeight();
	dword GetWidth();

	void LoadFromFile(const char* mapFileName);
	void SaveToFile(const char* mapFileName);

	bool IsThereObstacleBetweenPoints(Point firstPoint, Point secondPoint);

	template<typename Lambda>
	bool ActionInSubMatrix(dword beg_row, dword beg_col, dword end_row, dword end_col, Lambda ActionFunc, bool fQuitOnFailure = false)
	{
		auto lValidateRow = [&](dword row) {return row > GetHeight() || row < 0;};
		auto lValidateCol = [&](dword col) {return col > GetWidth() || col < 0;};

		if (lValidateRow(beg_row) || lValidateRow(end_row) || lValidateCol(beg_col) || lValidateCol(end_col))
		{
			return false;
		}

		bool fSuccess = true;

		// Go over the whole submap
		for (dword row = beg_row; row < end_row; row++)
		{
			for (dword col = beg_col; col < end_col; col++)
			{
				if (!ActionFunc(*this, row, col))
				{
					fSuccess = false;

					if (fQuitOnFailure)
					{
						return fSuccess;
					}
				}
			}
		}

		return fSuccess;
	}

	// Debug Methods
	void printMatrix();
};

#endif // CELL_MATRIX_H_
