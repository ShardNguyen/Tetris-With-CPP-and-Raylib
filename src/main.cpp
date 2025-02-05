// Task:
// Improve the gravity function: DONE
// Implement Lock Delay: DONE
// Improve Lock Delay (with limited step reset): DONE
// Implement Hard Drop: DONE
// Implement Kick Tables: DONE (And expandable)
// Implement DAS and ARR: DONE
// Implement Hold Box: Currently In Progress

#include "C:\raylib\raylib\src\raylib.h"
#include "game.h"
#include "color.h"
#include "graphicConfig.h"
#include <iostream>

int main()
{
	InitWindow(500, 620, "Tetris CPP");
	SetTargetFPS(60); // Aka setting the cap FPS
	Font scoreFont = LoadFontEx("D:\\Code\\CPP Tetris\\font\\Aptos-Display-Bold.ttf", 64, 0, 0);
	Game game = Game();

	while(WindowShouldClose() == false) {
		game.handleInput();

		if (!game.gameOver) {
			game.triggerGravity();
			game.triggerDASAndARR();
			game.triggerLockDelay();
		}

		// This is for drawing the game scene
		BeginDrawing();
		ClearBackground(darkBlue);
		DrawTextEx(scoreFont, "Score", {365, 15}, 32, 2, WHITE);

		char scoreText[10];
		sprintf(scoreText, "%d", game.score);
		Vector2 textSize = MeasureTextEx(scoreFont, scoreText, 32, 2);

		DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);
		DrawRectangle(320, NEXT_BOX_Y, 170, 170, lightBlue); // Pretty sure this is for drawing the next box
		DrawRectangle(320, NEXT_BOX_Y + 205, 170, 170, lightBlue); // This is for drawing the hold box
		DrawTextEx(scoreFont, scoreText, {320 + (170 - textSize.x) / 2, 65}, 32, 2, WHITE);
		DrawTextEx(scoreFont, "NEXT", {370, NEXT_BOX_Y - 30}, 32, 2, WHITE);
		DrawTextEx(scoreFont, "HOLD", {370, NEXT_BOX_Y + 175}, 32, 2, WHITE);
		game.draw();
		if (game.gameOver) {
			DrawTextEx(scoreFont, "GAME OVER", {80, 300}, 32, 4, WHITE);
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}