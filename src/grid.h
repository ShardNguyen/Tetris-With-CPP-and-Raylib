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
	std::vector<std::vector<int>> grid;
	Grid();
	void initialize();
	void emptyBoard();
	void setSize(int newNumRows, int newNumCols);
	bool isCellOutside(int row, int col);
	bool isCellEmpty(int row, int col);
	int clearFullRows();
	void print();
	void draw();
};

#endif