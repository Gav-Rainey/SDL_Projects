#include <SDL.h>
#undef main

class Pong {

public:
	Pong() {
		mTicksCount = 0;

		mPlayer1Pos.y = 200;
		mPlayer1Pos.x = 15;

		mPlayer2Pos.y = 205;
		mPlayer2Pos.x = 615;

		mBallPos.x = 320 - thickness;
		mBallPos.y = 240 - thickness;

	}

	bool Initialize() {
		int result = SDL_Init(SDL_INIT_VIDEO);

		if (result != 0) {
			SDL_Log("Video Not Initializing: %s", SDL_GetError());
			return false;
		}

		mWindow = SDL_CreateWindow(
			"Pong",
			100,
			100,
			640, 480,
			0
		);

		if (!mWindow) {
			SDL_Log("Failed to create Window: %s", SDL_GetError());
			return false;
		}

		mRenderer = SDL_CreateRenderer(
			mWindow,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
		);

		return true;
	}

	void RunLoop() {

		while (mIsRunning) {
			ProcessInput();
			UpdateGame();
			GenerateOutput();
		}
	}

	void Shutdown() {
		SDL_DestroyWindow(mWindow);
		SDL_DestroyRenderer(mRenderer);
		SDL_Quit();
	}

private:
	void ProcessInput() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {

			const Uint8* state = SDL_GetKeyboardState(NULL);

			if (state[SDL_SCANCODE_ESCAPE]) {
				mIsRunning = false;
			}

			switch (event.type) {
			case SDL_QUIT:
				mIsRunning = false;
				break;
			}


			if (state[SDL_SCANCODE_W]) {
				mPlayer1Dir = -1;
			}
			else if (state[SDL_SCANCODE_S]) {
				mPlayer1Dir = 1;
			}
			else {
				mPlayer1Dir = 0;
			}

			if (state[SDL_SCANCODE_UP]) {
				mPlayer2Dir = -1;
			}
			else if (state[SDL_SCANCODE_DOWN]) {
				mPlayer2Dir = 1;
			}
			else {
				mPlayer2Dir = 0;
			}
		}
	}

	void UpdateGame() {

		while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)); // Limits Framerate to 60fps or 16ms per Frame

		float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f; // Converts deltaTime to seconds
		mTicksCount = SDL_GetTicks(); // Update tick count for next frame

		if (deltaTime > 0.05f) { // Limits deltaTime to 0.05
			deltaTime = 0.05f;
		}

		if (mPlayer1Dir != 0) {
			mPlayer1Pos.y += mPlayer1Dir * 300.0f * deltaTime;

			if (mPlayer1Pos.y < paddleH / 2.0f - 15) {
				mPlayer1Pos.y = paddleH / 2.0f - 15;
			}
			else if (mPlayer1Pos.y > (480.0f - paddleH) - 30) {
				mPlayer1Pos.y = 480.0f - paddleH - 30;
			}
		}

		if (mPlayer2Dir != 0) {
			mPlayer2Pos.y += mPlayer2Dir * 300.0f * deltaTime;

			if (mPlayer2Pos.y < paddleH / 2.0f - 15) {
				mPlayer2Pos.y = paddleH / 2.0f - 15;
			}
			else if (mPlayer2Pos.y > (480.0f - paddleH) - 30) {
				mPlayer2Pos.y = 480.0f - paddleH - 30;
			}
		}

		if (mBallPos.y <= thickness && mBallVel.y < 0.0f) { // Top Wall Collision
			mBallVel.y *= -1;
		}
		else if (mBallPos.y >= 480.0f - (thickness * 2) && mBallVel.y > 0.0f) { // Bottom Wall Collision
			mBallVel.y *= -1;
		}

		if (mBallPos.x <= mPlayer1Pos.x + thickness && mBallVel.x < 0.0f) {

			if (mBallPos.y <= mPlayer1Pos.y + paddleH && mBallPos.y >= mPlayer1Pos.y - paddleH) {
				mBallVel.x *= -1;
				SDL_Log("%.2f", mBallPos.x);
			}
			else {
				mBallVel.x *= 1;
			}
		}
		else if (mBallPos.x >= mPlayer2Pos.x - thickness && mBallVel.x > 0.0f) {

			if (mBallPos.y <= mPlayer2Pos.y + paddleH && mBallPos.y >= mPlayer2Pos.y - paddleH) {
				mBallVel.x *= -1;
			}
			else {
				mBallVel.x *= 1;
			}
		}

		if (mBallPos.x <= -30 || mBallPos.x >= 680) {
			mIsRunning = false;
		}

		mBallPos.x += mBallVel.x * deltaTime;
		mBallPos.y += mBallVel.y * deltaTime;

	}

	void GenerateOutput() {
		SDL_SetRenderDrawColor( // Background Colour
			mRenderer,
			30, // Red
			0, // Green
			30, // Blue
			255 // Alpha
		);

		SDL_RenderClear(mRenderer);

		SDL_Rect wallTop{
			0,
			0,
			640,
			thickness
		};

		SDL_Rect wallBottom{
			0, // X
			480 - thickness, // Y
			640, // Width
			thickness // Thickness
		};

		SDL_SetRenderDrawColor( // Wall Colour
			mRenderer,
			15,
			0,
			15,
			255
		);

		SDL_RenderFillRect(mRenderer, &wallTop); // Draws Rectangle

		SDL_RenderFillRect(mRenderer, &wallBottom);

		SDL_Rect ball{
			static_cast<int>(mBallPos.x),
			static_cast<int>(mBallPos.y),
			thickness - 5,
			thickness - 5
		};

		SDL_Rect Player1{
			static_cast<int>(mPlayer1Pos.x),
			static_cast<int>(mPlayer1Pos.y),
			10,
			paddleH
		};

		SDL_Rect Player2{
			static_cast<int>(mPlayer2Pos.x),
			static_cast<int>(mPlayer2Pos.y),
			10,
			paddleH
		};

		SDL_SetRenderDrawColor( // Ball & Player1 Colour
			mRenderer,
			255,
			255,
			255,
			255
		);

		SDL_RenderFillRect(mRenderer, &Player1);

		SDL_RenderFillRect(mRenderer, &Player2);

		SDL_RenderFillRect(mRenderer, &ball);

		SDL_RenderPresent(mRenderer); // Displays the renderer which is otherwise in the background
	}

	// Member variables

	SDL_Window* mWindow = NULL;

	bool mIsRunning = true;

	SDL_Renderer* mRenderer = NULL;

	struct Vector2 {
		float x;
		float y;
	};

	const int paddleH = 75;

	const int thickness = 15;

	Vector2 mBallPos;

	Vector2 mPlayer1Pos;

	Vector2 mPlayer2Pos;

	Vector2 mBallVel{
		200.0f,
		-200.0f
	};

	int mPlayer1Dir;

	int mPlayer2Dir;

	Uint32 mTicksCount;
};