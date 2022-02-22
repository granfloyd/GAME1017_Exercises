#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_
#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <ctime>
#include <map>
#include <vector> 
#include <string>
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 5
#define MOVESPEED 7
#define ESPAWN 4
#define EFIRERATE 2
#define EMOVESPEED 4


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
	 bool m_running = false;
	 Uint32 m_start, m_end, m_delta, m_fps;
	 const Uint8* m_keystates;
	 SDL_Window* m_pWindow;
	 SDL_Renderer* m_pRenderer;

	
 private:
	 Engine()
	 {
		 cout << "creating instance of engine..." << endl;
	 }
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	//bool KeyDown(SDL_Scancode c);
	void Update();
	void Render();
	void Sleep();
	
 public:	
	int Run();
	// Add static method for singleton here
	static Engine& Instance();
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	bool& Running() { return m_running; }
	
};

#endif

// Reminder: you can ONLY have declarations (of variables and functions) in headers. What we have here is fine.
