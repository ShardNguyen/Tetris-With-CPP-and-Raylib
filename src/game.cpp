#include "coreGameConfig.h"
#include "game.h"
#include "graphicConfig.h"
#include <iostream>
#include <random>
#include <vector>


// ----- INITIALIZATION -----
Game::Game()
{
	grid = Grid();
	blocks = refillBlocks();
	currentBlock = getRandomBlock();
	nextBlock = getRandomBlock();
	playerHandling = PlayersHandling(0.1, 0.0/60.0, 0.0/60.0);
	gameOver = false;
	score = 0;
	lastGravityTime = 0;
	lastTouchGround = 0;
	stepResetted = 0;
	gravity = 1;
	setKickTable(new AkiraSRSTable());
	DASTimer = 0;
	ARRTimer = 0;
	SDARRTimer = 0;
	directionHolding = 0;
}

// ----- HANDLING BLOCK BEHAVIOR (PRIVATE) -----

Block Game::getRandomBlock() {
	// Refill the bag if it is empty
	if (blocks.empty()) {
		blocks = refillBlocks();
	}

	// Implementing 7-Bag randomizer
	int randomIndex = rand() % blocks.size();
	Block block = blocks[randomIndex];
	blocks.erase(blocks.begin() + randomIndex);
	return block;
}

std::vector<Block> Game::refillBlocks() {
	return {LBlock(), JBlock(), IBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

void Game::lockBlock() {
	std::vector<Position> tiles = currentBlock.getCellsPositions();
	for (Position item: tiles) {
		grid.grid[item.row][item.col] = currentBlock.id;
	}
	currentBlock = nextBlock;
	// Check if the block is blocked out to trigger game over
	if (!currentBlock.isFit(grid)) {
		gameOver = true;
	}
	nextBlock = getRandomBlock();
	int rowsCleared = grid.clearFullRows();
	updateScore(rowsCleared, 0);
	resetLockDelay();
	resetStepReset();
}
/*
bool Game::isBlockOutside(Block block)
{
	std::vector<Position> tiles = block.getCellsPositions();
	for (Position item: tiles) {
		if (grid.isCellOutside(item.row, item.col)) {
			return true;
		}
	}
	return false;
}

bool Game::blockFits(Block block)
{
	std::vector<Position> tiles = block.getCellsPositions();
	for (Position item: tiles) {
		if (!grid.isCellEmpty(item.row, item.col)) {
			return false;
		}
	}
	return true;
}
*/
// ----- CORE GAME FUNCTIONS (PRIVATE) -----

void Game::updateScore(int linesCleared, int moveDownPoints) {
	switch (linesCleared) {
		case 1:
			score += 40;
			break;
		case 2:
			score += 100;
			break;
		case 3:
			score += 300;
			break;
		case 4:
			score += 1200;
			break;
		default:
			score += 0;
			break;
	}

	score += moveDownPoints;
}

bool Game::isTouchingGround()
{
	Block checkBlock = currentBlock;
	checkBlock.move(1, 0);
	if (checkBlock.isBlockOutside(grid) || !checkBlock.isFit(grid)) {
		return true;
	}
	return false;
}

// ----- RESET FUNCTIONS (PRIVATE) -----

void Game::reset() {
	grid.emptyBoard();
	blocks = refillBlocks();
	currentBlock = getRandomBlock();
	nextBlock = getRandomBlock();
	score = 0;
	lastGravityTime = GetTime();
	resetLockDelay();
	resetStepReset();
	resetDASandARR();
	resetSDARR();
}

void Game::resetLockDelay() {
	lastTouchGround = GetTime();
}

void Game::resetStepReset()
{
	stepResetted = 0;
}

void Game::resetDASandARR()
{
	DASTimer = 0;
	ARRTimer = 0;
}

void Game::resetSDARR() {
	SDARRTimer = 0;
}

// ----- INPUT FUNCTIONS (PRIVATE) -----

void Game::rotateBlockCCW() {
	Block checkBlock = currentBlock;
	checkBlock.rotateCCW();

	// get kick table
	std::vector<std::vector<int>> kickOffsetCheck;
	kickTable->getKickOffset(kickOffsetCheck, checkBlock.id, checkBlock.rotationState * 2 + 1);
	
	// Checking kicks
	for (long long unsigned int i = 0; i < kickOffsetCheck.size(); i++) {
		checkBlock.move(-kickOffsetCheck[i][1], kickOffsetCheck[i][0]);
		if (!checkBlock.isBlockOutside(grid) && checkBlock.isFit(grid)) {
			// std::cout << "Kick valid";
			currentBlock = checkBlock;
			break;
		}
		checkBlock.move(kickOffsetCheck[i][1], -kickOffsetCheck[i][0]);
	}

	if (isTouchingGround() && stepResetted < STEP_RESET) {
		resetLockDelay();
		++stepResetted;
	}
}

void Game::rotateBlockCW() {
	Block checkBlock = currentBlock;
	checkBlock.rotateCW();

	// get kick table
	std::vector<std::vector<int>> kickOffsetCheck;
	kickTable->getKickOffset(kickOffsetCheck, checkBlock.id, currentBlock.rotationState * 2);
	
	// Checking kicks
	for (long long unsigned int i = 0; i < kickOffsetCheck.size(); i++) {
		checkBlock.move(-kickOffsetCheck[i][1], kickOffsetCheck[i][0]);
		if (!checkBlock.isBlockOutside(grid) && checkBlock.isFit(grid)) {
			// std::cout << "Kick valid";
			currentBlock = checkBlock;
			break;
		}
		checkBlock.move(kickOffsetCheck[i][1], -kickOffsetCheck[i][0]);
	}

	if (isTouchingGround() && stepResetted < STEP_RESET) {
		resetLockDelay();
		++stepResetted;
	}
}

void Game::moveBlockLeft() {
	currentBlock.move(0, -1);
	if (currentBlock.isBlockOutside(grid) || !currentBlock.isFit(grid)) {
		currentBlock.move(0, 1);
	} else {
		if (isTouchingGround() && stepResetted < STEP_RESET) {
			resetLockDelay();
			++stepResetted;
			// std::cout << stepResetted << "\n";
		}
	}
}

void Game::moveBlockRight() {
	currentBlock.move(0, 1);
	if (currentBlock.isBlockOutside(grid) || !currentBlock.isFit(grid)) {
		currentBlock.move(0, -1);
	} else {
		if (isTouchingGround() && stepResetted < STEP_RESET) {
			resetLockDelay();
			++stepResetted;
			// std::cout << stepResetted << "\n";
		}
	}
}
// This is bool to check for the soft drop score
bool Game::moveBlockDown() {
	currentBlock.move(1, 0);
	if (currentBlock.isBlockOutside(grid) || !currentBlock.isFit(grid)) {
		currentBlock.move(-1, 0);
		return false;
	} else {
		resetLockDelay();
		return true;
	}
}

void Game::hardDrop() {
	while (!isTouchingGround()) {
		moveBlockDown();
		updateScore(0, 2);
	}
	lockBlock();
}

void Game::triggerSDARR() {
	float deltaTime = GetFrameTime();
	if (IsKeyDown(KEY_DOWN)) {
		SDARRTimer += deltaTime;
	}
	if (playerHandling.reachSDARR(SDARRTimer)) {
		while (SDARRTimer > playerHandling.getSDARR()) {
			if (moveBlockDown()) {
				updateScore(0, 1);
			}
			SDARRTimer -= playerHandling.getSDARR();
		}
	}
}

// ----- DAS AND ARR RELATED FUNCTIONS -----

void Game::ARRZeroSpecialLeft() {
	Block checkBlock = currentBlock;
	while (!checkBlock.isBlockOutside(grid) && checkBlock.isFit(grid)) {
		checkBlock.move(0, -1);
	}
	checkBlock.move(0, 1);
	currentBlock = checkBlock;
}

void Game::ARRZeroSpecialRight() {
	Block checkBlock = currentBlock;
	while (!checkBlock.isBlockOutside(grid) && checkBlock.isFit(grid)) {
		checkBlock.move(0, 1);
	}
	checkBlock.move(0, -1);
	currentBlock = checkBlock;
}

void Game::SDARRZeroSpecial() {
	Block checkBlock = currentBlock;
	while (!checkBlock.isBlockOutside(grid) && checkBlock.isFit(grid)) {
		checkBlock.move(1, 0);
	}
	checkBlock.move(-1, 0);
	currentBlock = checkBlock;
	resetLockDelay();
}

// ----- KICK RELATED FUNCTION -----

void Game::setKickTable(KickTable* newKickTable)
{
	kickTable = newKickTable;
}

// ----- HANDLE INPUTS (PUBLIC) -----

void Game::handleInput() {
	int keyPressed = GetKeyPressed();
	if (gameOver && keyPressed != 0) {
		gameOver = false;
		reset();
	}

	switch (keyPressed) {
		case KEY_LEFT:
			moveBlockLeft();
			resetDASandARR();
			directionHolding = KEY_LEFT;
			break;
		
		case KEY_RIGHT:
			moveBlockRight();
			resetDASandARR();
			directionHolding = KEY_RIGHT;
			break;

		case KEY_DOWN:
			if (playerHandling.getSDARR() == 0) {
				SDARRZeroSpecial();
			}
			else {
				resetSDARR();
				if (moveBlockDown()) {
					updateScore(0, 1);
				}
			}
			break;

		case KEY_UP:
			rotateBlockCW();
			break;

		case KEY_Z:
			rotateBlockCCW();
			break;

		case KEY_SPACE:
			hardDrop();
			break;
			
		default:
			break;
	}
	
	// Handling holding direction 
	if (IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_RIGHT)) {
		resetDASandARR();
	}
	if (playerHandling.getSDARR() != 0 && IsKeyDown(KEY_DOWN)) {
		triggerSDARR();
	}
}


// ----- CORE GAME FUNCTIONS (PUBLIC) -----

void Game::triggerGravity()
{
	double currentTime = GetTime();
	while (currentTime - lastGravityTime >= gravity) {
		lastGravityTime += gravity;
		moveBlockDown();
	}
}

void Game::triggerLockDelay() {
	double currentTime = GetTime();
	if (isTouchingGround() && currentTime - lastTouchGround >= LOCK_DELAY) {
		lockBlock();
	}
}

void Game::triggerDASAndARR() {
	float deltaTime = GetFrameTime();
	// Check for DAS
	// If DAS is reached, check for ARR
	if (!playerHandling.reachDAS(DASTimer)) {
		if (IsKeyDown(directionHolding)) {
			DASTimer += deltaTime;
		}
	} else {
		// Special case (ARR = 0)
		if (playerHandling.getARR() == 0) {
			switch (directionHolding) {
				case KEY_LEFT:
					ARRZeroSpecialLeft();
					break;
				case KEY_RIGHT:
					ARRZeroSpecialRight();
					break;
				default:
					break;
			}
		}
		else {
			if (IsKeyDown(directionHolding)) {
				ARRTimer += deltaTime;
			}
			if (playerHandling.reachARR(ARRTimer)) {
				while (ARRTimer > playerHandling.getARR()) {
					switch (directionHolding) {
					case KEY_LEFT:
						moveBlockLeft();
						break;
					case KEY_RIGHT:
						moveBlockRight();
						break;
					default:
						break;
					}
					ARRTimer -= playerHandling.getARR();
				}
			}
		}
	}
}

// ----- DRAW FUNCTION (PUBLIC) -----

void Game::draw() {
	grid.draw();
	currentBlock.draw(LEFT_OFFSET, UP_OFFSET);
	switch (nextBlock.id) {
		case 3:
			nextBlock.draw(255, 290);
			break;
		case 4:
			nextBlock.draw(255, 280);
			break;
		default:
			nextBlock.draw(270, 270);
			break;
	}
}