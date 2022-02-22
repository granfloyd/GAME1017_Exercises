#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <ctime>
#include <vector> 
#include <string>
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 5
#define MOVESPEED 7
#define ESPAWN 5
#define EFIRERATE 3
#define EMOVESPEED 3


using namespace std;

class Missile
{
public:
	SDL_Rect m_src, m_dst;
	
	Missile(int = 0, int = 0); 
	void Update(int moving);
	
	
};
	
class Enemy
{
	public:
		SDL_Rect m_enemySrc, m_enemyDst;
		Uint16 frames = 0;
		Enemy(int = 0, int = 0);
		void Update();
		void resetFrames();
	
};




class Engine
{
 private:
	//double m_angle = 0.0;
	//bool m_running = false;
	bool m_running = false;
	bool m_fire = false; // Fire toggle.
	bool m_turret = false;// Another fire toggle for turret.
	bool m_isAlive = true;
	SDL_Point g_mousePos;
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	SDL_Texture* m_pBGTexture;//background
    SDL_Texture* m_pEnemyTexture;//enemy/carl
	SDL_Texture* m_pShipTexture; // lilspaceship
	
	SDL_Rect m_player; // For primitive rectangle.d
	SDL_Rect m_src, m_dst; // For the ship sprite.
	SDL_Rect g_bg1, g_bg2;
	SDL_Point m_mousePos;

	SDL_Rect m_pew;
	vector<Missile*> m_missile;
	vector<Missile*> m_playerpew;
	vector<Enemy*> m_enemy;
	SDL_Rect m_enemySrc, m_enemyDst;
	Uint16 m_enemyTimer;
	
	//Audio
	Mix_Chunk* m_pShoot; // myMap["Free.mp3"]
	Mix_Chunk* m_peShoot; // myMap["Short.mp3"]
	Mix_Chunk* m_pLaser;
	Mix_Music* m_pMaintheme;
	Mix_Music* m_pTitletheme;
	bool g_paused; // If music is paused or not
	
	

 private:
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	bool KeyDown(SDL_Scancode c);
	void Update();
	void Render();
	void Sleep();
	
 public:
	int Run();
	
};

#endif

// Reminder: you can ONLY have declarations (of variables and functions) in headers. What we have here is fine.
