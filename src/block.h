#ifndef _BLOCK_
#define _BLOCK_

#include <vector>
#include <map>
#include "position.h"
#include "color.h"
#include "grid.h"
#include "kickTable.h"

class Block {
private:
	int cellSize;
	std::vector<Color> colors;
	int rowOffset;
	int colOffset;
public:
	int id;
	int rotationState;
	std::map<int, std::vector<Position>> cells;
	
	Block();
	void draw(int offsetX, int offsetY);
	void move(int rows, int cols);
	void reset();
	std::vector<Position> getCellsPositions();
	void rotateCW();
	void rotateCCW();
	bool isBlockOutside(Grid grid);
	bool isFit(Grid grid);
};

#endif