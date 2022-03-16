#include "Box.h"
#include "Engine.h"
#define SCROLLSPEED 2
//note can chnage default values to the parameters and not need a default constructor
Sprite::Sprite() :dst({ 0,0,0,0 }),color({255,255,255,255}){}
Sprite::Sprite(const SDL_Rect r, const SDL_Color c):dst(r),color(c){}

void Sprite::Render()
{
	//for this initial lab we are just drawing rectangles 
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &dst);
}

Box::Box(SDL_Point p, bool makeSprite, const SDL_Rect r, const SDL_Color c): pos(p),pSprite(nullptr)//note r and c are for Sprite
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
	return nullptr;//week 10 crap
}

void Box::Update()
{
	pos.x -= SCROLLSPEED;
	if (pSprite != nullptr)
	{		
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
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(),156,230,200,255);
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &dst);
}
