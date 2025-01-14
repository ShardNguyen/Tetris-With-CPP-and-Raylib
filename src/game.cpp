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
	gameOver = false;
	score = 0;
	lastGravityTime = 0;
	lastTouchGround = 0;
	stepResetted = 0;
	gravity = 0.1;
	setKickTable(new AkiraSRSTable());
}

// 
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
	if (!blockFits(currentBlock)) {
		gameOver = true;
	}
	nextBlock = getRandomBlock();
	int rowsCleared = grid.clearFullRows();
	updateScore(rowsCleared, 0);
	resetLockDelay();
	resetStepReset();
}

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
	if (isBlockOutside(checkBlock) || !blockFits(checkBlock)) {
		return true;
	}
	return false;
}

// ----- RESET FUNCTIONS (PRIVATE) -----

void Game::reset() {
	grid.initialize();
	blocks = refillBlocks();
	currentBlock = getRandomBlock();
	nextBlock = getRandomBlock();
	score = 0;
	lastGravityTime = GetTime();
}

void Game::resetLockDelay() {
	lastTouchGround = GetTime();
}

void Game::resetStepReset()
{
	stepResetted = 0;
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
			std::cout << "Kick valid";
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
			std::cout << "Kick valid";
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
	if (isBlockOutside(currentBlock) || !blockFits(currentBlock)) {
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
	if (isBlockOutside(currentBlock) || !blockFits(currentBlock)) {
		currentBlock.move(0, -1);
	} else {
		if (isTouchingGround() && stepResetted < STEP_RESET) {
			resetLockDelay();
			++stepResetted;
			// std::cout << stepResetted << "\n";
		}
	}
}

void Game::moveBlockDown() {
	currentBlock.move(1, 0);
	if (isBlockOutside(currentBlock) || !blockFits(currentBlock)) {
		currentBlock.move(-1, 0);
	} else {
		resetLockDelay();
	}
}

void Game::hardDrop() {
	while (!isTouchingGround()) {
		moveBlockDown();
		updateScore(0, 2);
	}
	lockBlock();
}

// Kick related function
std::vector<std::vector<int>> Game::getActualKickTableCW(std::vector<std::vector<int>> offsetCurrent, std::vector<std::vector<int>> offsetAfter) {
	std::vector<std::vector<int>> actual = {};
	for (long long unsigned int i = 0; i < offsetCurrent.size(); i++) {
		actual.push_back({offsetCurrent[i][0] - offsetAfter[i][0], offsetCurrent[i][1] - offsetAfter[i][1]});
	}
	return actual;
}

std::vector<std::vector<int>> Game::getActualKickTableCCW(std::vector<std::vector<int>> offsetCurrent, std::vector<std::vector<int>> offsetAfter) {
	std::vector<std::vector<int>> actual = {};
	for (long long unsigned int i = 0; i < offsetCurrent.size(); i++) {
		actual.push_back({offsetCurrent[i][0] + offsetAfter[i][0], offsetCurrent[i][1] + offsetAfter[i][1]});
	}
	return actual;
}

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
			break;
		
		case KEY_RIGHT:
			moveBlockRight();
			break;

		case KEY_DOWN:
			moveBlockDown();
			updateScore(0, 1);
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