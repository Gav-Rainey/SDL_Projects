#include <SDL.h>
#include "Pong.cpp"

	int main(int argc, char** argv) {


		Pong newGame;

		bool success = newGame.Initialize();

		if (success) {
			newGame.RunLoop();
		}
		newGame.Shutdown();
	}