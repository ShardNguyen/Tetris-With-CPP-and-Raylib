// Task:
// Improve the gravity function: DONE
// Implement Lock Delay: DONE
// Improve Lock Delay (with limited step reset): DONE
// Implement Hard Drop: DONE
// Implement Kick Tables: DONE (And expandable)
// DAS and ARR

#include "C:\raylib\raylib\src\raylib.h"
#include "game.h"
#include "color.h"
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
		DrawRectangle(320, 215, 170, 170, lightBlue);
		DrawTextEx(scoreFont, scoreText, {320 + (170 - textSize.x) / 2, 65}, 32, 2, WHITE);
		DrawTextEx(scoreFont, "NEXT", {370, 175}, 32, 2, WHITE);

		game.draw();
		if (game.gameOver) {
			DrawTextEx(scoreFont, "GAME OVER", {80, 300}, 32, 4, WHITE);
		}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}