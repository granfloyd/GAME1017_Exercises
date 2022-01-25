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
using namespace std;
//an abstract class is one that cannot be instantiated
//why because theyd be a base class most likely
class State//this is the abstract base class for all state subclasses
{
protected: //priv but inherited

	State() = default;//or... State(){};
	Mix_Music* m_DMCA;
	Mix_Music* m_Gasoline1;
	Mix_Chunk* m_Superpartypc;
	Mix_Chunk* m_Free;
	
	
	
	
public:
	
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume() {};
};



class TitleState : public State
{
private:
	//map for music track goes here	
	map<string, Mix_Music*> m_sounds;
	
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
	
	//map for music track goes here
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
	map<string, Mix_Music*>m_sounds;
	map<string, Mix_Chunk*>m_sfx;
	//map for music track goes here
	//map for sfx goes here
public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();

};
class EndState : public State
{

public:
	EndState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();

};
//add endstatehere
//myarray[0].Render();
#endif