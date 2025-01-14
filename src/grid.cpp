#include "grid.h"
#include <iostream>
#include "color.h"
#include "graphicConfig.h"

Grid::Grid()
{
	numRows = 20;
	numCols = 10;
	cellSize = 30;
	initialize();
	colors = getCellColors();
}

void Grid::initialize() {
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			grid[row][col] = 0;
		}
	}
}

bool Grid::isCellOutside(int row, int col)
{
	if (row >= 0 && row < numRows && col >= 0 && col < numCols) {
		return false;
	} else {
		return true;
	}
}

bool Grid::isCellEmpty(int row, int col)
{
	if (grid[row][col] == 0) {
		return true;
	}
	return false;
}

bool Grid::isRowFull(int row)
{
	for (int col = 0; col < numCols; col++) {
		if (grid[row][col] == 0) {
			return false;
		}
	}
	return true;
}

void Grid::clearRow(int row)
{
	for (int col = 0; col < numCols; col++) {
		grid[row][col] = 0;
	}
}

void Grid::moveRowDown(int row, int numRows)
{
	for (int col = 0; col < numCols; col++) {
		grid[row + numRows][col] = grid[row][col];
		grid[row][col] = 0;
	}
}

void Grid::print() {
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			std::cout << grid[row][col] << " ";
		}
		std::cout << "\n";
	}
}

int Grid::clearFullRows()
{
	int completed = 0;
	for (int row = numRows - 1; row >= 0; row--) {
		if (isRowFull(row)) {
			clearRow(row);
			++completed;
		} else if (completed > 0) {
			moveRowDown(row, completed);
		}
	}
	return completed;
}

void Grid::draw() {
	for (int row = 0; row < numRows; row++) {
		for (int col = 0; col < numCols; col++) {
			int cellValue = grid[row][col];
			DrawRectangle(col * cellSize + GRID_SIZE + LEFT_OFFSET, row * cellSize + GRID_SIZE + UP_OFFSET, cellSize - GRID_SIZE, cellSize - GRID_SIZE, colors[cellValue]);
		}
	}
}