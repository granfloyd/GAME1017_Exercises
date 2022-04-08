#pragma once
#ifndef _BOX_H_
#define _BOX_H_

#include <SDL.h>

class Sprite //Could be renamed Obstacle for A2
{
public:
	Sprite();
	Sprite(const SDL_Rect r, const SDL_Color c);
	//void Update(); //May need for A2, ex. Spinning Blades
	void Render();
	friend class Box; //You can also make a setter for m_dst

private:
	//SDL_Rect m_src; //For A2 or use GameObject.h
	SDL_Rect m_dst;
	SDL_Color m_color; //Don't need this for A2
};

class Box //Proxy for the obstacle/sprite
{
public:
	Box(const SDL_Point p, bool makeSprite = false, const SDL_Rect r = { 0, 0, 0, 0 }, const SDL_Color c = { 255, 255, 255, 255 });
	~Box();
	Box* Clone(); //We're making Box also a prototype
	void Update();
	void Render();
	const SDL_Point GetPos() { return m_pos; }

private:
	SDL_Point m_pos; //Position on screen. Y is optional.
	Sprite* m_pSprite; //Pointer to a dynamic Sprite object
	int m_numSprites; //For bonus of A2
};

#endif

