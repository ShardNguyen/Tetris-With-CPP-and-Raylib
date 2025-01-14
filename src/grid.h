#ifndef _GRID_
#define _GRID_

#include <vector>
#include "C:\raylib\raylib\src\raylib.h"

class Grid {
private:
	int numRows;
	int numCols;
	int cellSize;
	std::vector <Color> colors;
	bool isRowFull(int row);
	void clearRow(int row);
	void moveRowDown(int row, int numRows);
public:
	Grid();
	bool isCellOutside(int row, int col);
	bool isCellEmpty(int row, int col);
	void initialize();
	void print();
	int clearFullRows();
	int grid[20][10];
	void draw();
};

#endif