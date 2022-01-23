#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#include <iostream>
#include <map>
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
//an abstract class is one that cannot be instantiated
//why because theyd be a base class most likely
class State//this is the abstract base class for all state subclasses
{
protected: //priv but inherited
	State() = default;//or... State(){};
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
	
	//Mix_Chunk* m_free;
	//Mix_Music* m_dmca;
	
	//map<string, Mix_Chunk*> m_sfx;
	//map<string, Mix_Music*> m_music;
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
//add endstatehere
//myarray[0].Render();
#endif