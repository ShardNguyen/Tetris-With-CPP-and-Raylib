#include "block.h"
#include <math.h>
#include "graphicConfig.h"

Block::Block() {
	cellSize = 30;
	rotationState = 0;
	colors = getCellColors();
	rowOffset = 0;
	colOffset = 0;
}

void Block::draw(int offsetX, int offsetY) {
	std::vector<Position> tiles = getCellsPositions();
	for (Position item: tiles) {
		DrawRectangle(item.col * cellSize + GRID_SIZE + offsetX, item.row * cellSize + GRID_SIZE + offsetY, cellSize - GRID_SIZE, cellSize - GRID_SIZE, colors[id]);
	}
}

void Block::move(int rows, int cols) {
	rowOffset += rows;
	colOffset += cols;
}

std::vector<Position> Block::getCellsPositions() {
	std::vector<Position> tiles = cells[rotationState];
	std::vector<Position> movedTiles;

	for (Position item: tiles) {
		Position newPos = Position(item.row + rowOffset, item.col + colOffset);
		movedTiles.push_back(newPos);
	}

	return movedTiles;
}

void Block::rotateCW() {
	rotationState = (rotationState + 1) % int(cells.size());
}

void Block::rotateCCW() {
	if (rotationState == 0) {
		rotationState = int(cells.size() - 1);
	} else {
		rotationState = rotationState - 1;
	}
}

bool Block::isBlockOutside(Grid grid)
{
	std::vector<Position> tiles = getCellsPositions();
	for (Position item: tiles) {
		if (grid.isCellOutside(item.row, item.col)) {
			return true;
		}
	}
	return false;
}

bool Block::isFit(Grid grid)
{
	std::vector<Position> tiles = getCellsPositions();
	for (Position item: tiles) {
		if (!grid.isCellEmpty(item.row, item.col)) {
			return false;
		}
	}
	return true;
}
