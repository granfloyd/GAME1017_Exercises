#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Engine.h"
#include "GameObject.h"
#include "TiledLevel.h"
#define SPEED 5
#define MOVESPEED 7
#define ESPAWN 3
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
//an abstract class is one that cannot be instantiated
//why because theyd be a base class most likely
class State//this is the abstract base class for all state subclasses
{

public:
	SDL_Texture* Title;
	
	
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume();
	virtual ~State() {} // or = default;
	GameObject* GetGo(const std::string& s);
	auto GetIt(const std::string& s);

protected: //priv but inherited

	State() {}//or... State(){};
	vector<std::pair<std::string, GameObject*>> m_objects;
};



class TitleState : public State
{
private:
	Mix_Music* m_pTitletheme;
	SDL_Texture* Title2;
	SDL_Rect src, dst, dst2;
	SDL_Rect end;//when title goes offscreen
	SDL_Rect newdst;
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	
};




class PauseState : public State
{
private:
	
	
public:
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();

};


class GameState : public State
{
private:	
	vector<Missile*> m_missile;
	vector<Missile*> m_playerpew;
	vector<Enemy*> m_enemy;
	//double m_angle = 0.0;
    //bool m_running = false;
	//bool m_running = false;
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


   //Audio
	Mix_Chunk* hithurt;
	Mix_Chunk* m_pShoot; // myMap["Free.mp3"]
	Mix_Chunk* m_peShoot; // myMap["Short.mp3"]
	Mix_Chunk* m_pLaser;
	Mix_Music* m_pMaintheme;
	

	SDL_Rect m_player; // For primitive rectangle.d
	SDL_Rect m_src, m_dst; // For the ship sprite.
	SDL_Rect g_bg1, g_bg2;
	SDL_Point m_mousePos;

	SDL_Rect m_pew;
	
	SDL_Rect m_enemySrc, m_enemyDst;
	Uint16 m_enemyTimer;

	
	bool g_paused; // If music is paused or not
public:
	//vector<Missile*>& GetMissile() { return m_playerpew; }
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();

};
class EndState : public State
{
private:
	SDL_Texture* TitleL;
	Mix_Chunk* hurt2;

public:
	EndState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();


};
class WinState : public State
{
private:
	Mix_Music* wintheme;
	SDL_Texture* TitleW;
public:
	WinState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};
//add endstatehere
//myarray[0].Render();
#endif