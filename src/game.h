#ifndef _GAME_
#define _GAME_

#include "grid.h"
#include "blocks.cpp"
#include "kickTable.h"
#include "playerHandling.h"
#include <memory>

class Game {
private:
	std::vector<Block> blocks;
	Block currentBlock;
	Block nextBlock;
	Grid grid;
	KickTable* kickTable;
	PlayersHandling playerHandling;
	double gravity;
	double lastGravityTime;
	double lastTouchGround;
	double stepResetted;

	// DAS and ARR behavior
	double DASTimer;
	double ARRTimer;
	double SDARRTimer;
	int directionHolding;

	// This is for handling block behavior
	Block getRandomBlock();
	std::vector<Block> refillBlocks();
	void lockBlock();
	// bool isBlockOutside(Block block);
	// bool blockFits(Block block);

	// Core game function
	void updateScore(int linesCleared, int moveDownPoints);
	bool isTouchingGround();

	// Reset functions
	void reset();
	void resetLockDelay();
	void resetStepReset();
	void resetDASandARR();
	void resetSDARR();

	// This is for input related game function
	void rotateBlockCW();
	void rotateBlockCCW();
	void moveBlockLeft();
	void moveBlockRight();
	bool moveBlockDown();
	void hardDrop();
	void triggerSDARR();

	// DAS and ARR related function
	void ARRZeroSpecialLeft();
	void ARRZeroSpecialRight();
	void SDARRZeroSpecial();

	// Kick related functions
	void setKickTable(KickTable* newKickTable);

public:
	Game();
	bool gameOver;
	int score;
	// This is for handling inputs
	void handleInput();
	// More game-related functions
	void triggerDASAndARR();
	void triggerGravity();
	void triggerLockDelay();
	// Draw function
	void draw();
};

#endif