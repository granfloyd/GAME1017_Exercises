#pragma once
#ifndef _BOX_H_
#define _BOX_H_
#include <SDL.h>
//week9 lab 25:37
//pSprite = new Sprite[numSprites]
class Sprite//for A2
{
private:
	//SDL_Rect src; //for a2 or use GameObject.h
	SDL_Rect dst;
	SDL_Color color;//dont need colour for a2
public:
	Sprite();//use this for dynamic array of Sprite*
	Sprite(const SDL_Rect r, const SDL_Color c);
	//void Update();//may need for a2 ex spinning blade
	void Render();
	friend class Box;//Alt you can make a setter for dst
};
class Box//proxy for the sprite
{
	//dynamic array of sprites
	//every box is going to have a pointer to a sprite
	//each box needs a x position
private:
	SDL_Point pos;    // position on screen Y is optional
	Sprite* pSprite;  // pointer to the dynamic sprite object(Hint can have more then one)
	int numSprites;   // for the bonus-A2
public:
	Box(SDL_Point p, bool makeSprite = false, const SDL_Rect r = {0,0,0,0}, const SDL_Color c = { 255,255,255,255 });
	~Box();
	Box* Clone(); // were making box also a prototype
	void Update();
	void Render();
	const SDL_Point GetPos(){ return pos; }
	//adding map stuff later
};
#endif
