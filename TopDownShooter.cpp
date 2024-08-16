#include <SDL.h>
#undef main

class TopDownShooter {

public:
	TopDownShooter() {

	}

	bool Initialize() {

		int result = SDL_Init(SDL_INIT_VIDEO);

		if (result != 0) {
			SDL_Log("Video Failed To Initialize");
			return false;
		}

		Window = SDL_CreateWindow(

			"Shooter", // Title
			100, // X
			100, // Y
			640, // Width
			480, // Height
			0 // Flags
		);

		if (!Window) {
			SDL_Log("Failed To Create Window");
			return false;
		}

		Renderer = SDL_CreateRenderer(

			Window,
			-1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

		);

		return true;
	}

	void RunLoop() {

		while (isRunning) {

		}

	}

	void Shutdown() {

		SDL_DestroyWindow(Window);
		SDL_DestroyRenderer(Renderer);
		SDL_Quit();

	}

private:
	void ProcessInput() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {

			const Uint8* state(SDL_GetKeyboardState(NULL));

			if (state[SDL_SCANCODE_ESCAPE]) {
				isRunning = false;
			}

		}

	}

	void UpdateGame() {

	}

	void GenerateOutput() {

	}


	SDL_Window* Window = NULL;

	SDL_Renderer* Renderer = NULL;

	bool isRunning = true;

};