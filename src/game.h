#ifndef _GAME_
#define _GAME_

#include "grid.h"
#include "blocks.cpp"
#include "kickTable.h"
#include <memory>

class Game {
private:
	std::vector<Block> blocks;
	Block currentBlock;
	Block nextBlock;
	Grid grid;
	KickTable* kickTable;
	double gravity;
	double lastGravityTime;
	double lastTouchGround;
	double stepResetted;

	// This is for handling block behavior
	Block getRandomBlock();
	std::vector<Block> refillBlocks();
	void lockBlock();
	bool isBlockOutside(Block block);
	bool blockFits(Block block);

	// Core game function
	void updateScore(int linesCleared, int moveDownPoints);
	bool isTouchingGround();

	// Reset functions
	void reset();
	void resetLockDelay();
	void resetStepReset();
	
	// This is for input related game function
	void rotateBlockCW();
	void rotateBlockCCW();
	void moveBlockLeft();
	void moveBlockRight();
	void moveBlockDown();
	void hardDrop();

	// Kick related functions
	std::vector<std::vector<int>> getActualKickTableCW(std::vector<std::vector<int>> offsetCurrent, std::vector<std::vector<int>> offsetAfter);
	std::vector<std::vector<int>> getActualKickTableCCW(std::vector<std::vector<int>> offsetCurrent, std::vector<std::vector<int>> offsetAfter);
	void setKickTable(KickTable* newKickTable);

public:
	Game();
	bool gameOver;
	int score;
	// This is for handling inputs
	void handleInput();
	// More game-related functions
	void triggerGravity();
	void triggerLockDelay();
	// Draw function
	void draw();
};

#endif