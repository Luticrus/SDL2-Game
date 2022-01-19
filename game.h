#pragma once

#include "SDL.H"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class AssetManager;
class ColliderComponent;

class Game
{
	public:
		Game();
		~Game();

		void init(const char* title, int width, int height, bool fullscreen);
	
		void handleEvents();
		void update();
		void render();
		void clean();

	
		static SDL_Renderer* renderer;
		static SDL_Event event;
		static SDL_Rect camera;
		static AssetManager* assets;
		bool running()
		{
			return isRunning;
		}
		static bool isRunning;
		enum groupLables : std::size_t
		{
			groupMap,
			groupPlayers,
			groupEnemy,
			groupColliders,
			groupProjectiles
		};

	private:
		int cnt = 0;
		SDL_Window* window;


};




