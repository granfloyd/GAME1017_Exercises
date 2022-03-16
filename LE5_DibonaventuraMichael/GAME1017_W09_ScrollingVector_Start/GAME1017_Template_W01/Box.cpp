#include "Box.h"
#include "Engine.h"
#define SCROLLSPEED 2
//note can chnage default values to the parameters and not need a default constructor
Sprite::Sprite() :dst({ 0,0,0,0 }),color({255,255,255,255})
{
}

Sprite::Sprite(const SDL_Rect r, const SDL_Color c):dst(r),color(c)
{
	//for this initial lab we are just drawing rectangles 
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &dst);
}

void Sprite::Render()
{

}

Box::Box(SDL_Point p, bool makeSprite = false, const SDL_Rect r, const SDL_Color c): pos(p),pSprite(nullptr)//note r and c are for Sprite
{
	if (makeSprite)
	{
		// if making a dynamic array pSprite = new Sprite[size of stuff]
		pSprite = new Sprite( r, c );
	}
}

Box::~Box()
{
	if (pSprite != nullptr)
	{
		delete pSprite;
	}
}

Box* Box::Clone()
{
	return nullptr;
}

void Box::Update()
{
	if (pSprite != nullptr)
	{
		pos.x -= SCROLLSPEED;
		pSprite->dst.x = pos.x;
	}

}

void Box::Render()
{
	if (pSprite != nullptr)
	{
		pSprite->Render();
	}
	//to render a border around each BOx.../
	SDL_Rect dst = { pos.x, pos.y,128,128 };
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(),0,0,200,100);
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &dst);
}
