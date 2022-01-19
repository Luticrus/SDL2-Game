#include "game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
SDL_Rect Game::camera = { 0,0,1600,1272 };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& Enemy(manager.addEntity());
auto& label(manager.addEntity());


Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "ERROR" << std::endl;
	}

	assets->AddTexture("terrain", "Assets/terain_ss.png");
	assets->AddTexture("player", "Assets/CassiAnimations.png");
	assets->AddTexture("Enemy", "Assets/Enemy.png");
	assets->AddTexture("projectile", "Assets/proj.png");
	
	assets->AddFont("GIGI", "Assets/GIGI.ttf", 24);

	map = new Map("terrain", 2, 64);///scaleul hartii + nr de pixeli al unui tile
	//ECS implementation
	
	map->LoadMap("Assets/map.map",25 , 17);///marimea hartii (tilex,tiley)

	player.addComponent<TransformComponent>(800.0f, 450.0f, 64, 64, 2);///locul de spawn al charcterului(x,y), nr de pixeli ai charcterului(x,y), scaleul
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	Enemy.addComponent<TransformComponent>(900.0f, 550.0f, 64, 64, 2);///locul de spawn al charcterului(x,y), nr de pixeli ai charcterului(x,y), scaleul
	Enemy.addComponent<SpriteComponent>("Enemy", false);
	Enemy.addComponent<ColliderComponent>("Enemy");
	Enemy.addGroup(groupEnemy);

	SDL_Color black = { 0,0,0,0 };
	label.addComponent<UILable>(10, 10, "Test", "GIGI", black);

	//nivel 1
	assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 400, 2, "projectile");
	assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 600, 2, "projectile");
	assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");

	//nivel 2
	//assets->CreateProjectile(Vector2D(800, 632), Vector2D(2, 0), 200, 2, "projectile");
	//assets->CreateProjectile(Vector2D(900, 620), Vector2D(2, 0), 400, 2, "projectile");
	//assets->CreateProjectile(Vector2D(300, 600), Vector2D(2, 1), 600, 2, "projectile");
	//assets->CreateProjectile(Vector2D(900, 900), Vector2D(2, -1), 200, 2, "projectile");

	//nivel 3
	//assets->CreateProjectile(Vector2D(740, 300), Vector2D(2, 0), 800, 2, "projectile");
	//assets->CreateProjectile(Vector2D(900, 620), Vector2D(2, 0), 400, 2, "projectile");
	//assets->CreateProjectile(Vector2D(500, 400), Vector2D(2, 1), 600, 2, "projectile");
	//assets->CreateProjectile(Vector2D(300, 200), Vector2D(2, -1), 1200, 2, "projectile");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& Enemiss(manager.getGroup(Game::groupEnemy));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}

}


void Game::update()
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	std::stringstream ss;
	ss << "Player position:" << playerPos;
	label.getComponent<UILable>().SetLableText(ss.str(), "GIGI");

	manager.refresh();
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			std::cout << "hit player" << std::endl;
			p->destroy();
			int flags = 0;

			/*SDL_DestroyRenderer(renderer);
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			assets->AddTexture("terrain", "Assets/metin2_ss.png");
			//assets->AddTexture("player", "Assets/CassiAnimations.png");
			//assets->AddTexture("projectile", "Assets/proj.png");

			//assets->AddFont("GIGI", "Assets/GIGI.ttf", 24);

			map = new Map("terrain", 2, 64);///scaleul hartii + nr de pixeli al unui tile
			//ECS implementation

			map->LoadMap("Assets/metin2.txt", 25, 17);///marimea hartii (tilex,tiley)

			//player.addComponent<TransformComponent>(800.0f, 450.0f, 64, 64, 2);///locul de spawn al charcterului(x,y), nr de pixeli ai charcterului(x,y), scaleul
			//player.addComponent<SpriteComponent>("player", true);
			//player.addComponent<KeyboardController>();
			//player.addComponent<ColliderComponent>("player");
			//player.addGroup(groupPlayers);

			//SDL_Color black = { 0,0,0,0 };
			//label.addComponent<UILable>(10, 10, "Test", "GIGI", black);*/
		}
	}

	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 730);
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 400);

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
}



void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& c : colliders)
	{
		c->draw();
	}

	for (auto& iiiiiiii : Enemiss)
	{
		iiiiiiii->draw();
	}


	for (auto& p: players)
	{
		p->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
	}

	label.draw();

	SDL_RenderPresent(renderer);

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
