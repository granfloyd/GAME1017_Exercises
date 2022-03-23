#pragma once
#ifndef _TILEDLEVEL_H_
#define _TILEDLEVEL_H_

#include <fstream>
#include <map>
#include <vector>
#include "Engine.h"
#include "GameObject.h"
#include "TextureManager.h"

class Tile : public SpriteObject
{
private:
	bool obstacle, hazard;

public:
	Tile(SDL_Rect s,SDL_FRect d,bool o = false,bool h = false)
		:SpriteObject(s,d),obstacle(o),hazard(h){}
	Tile* Clone() { return new Tile(m_src,m_dst, obstacle, hazard); }
	void Update() {}
	void Render() {}//tiles are rendered in tilelevel
	bool IsObstacle() { return obstacle; }
	void SetXY(float x, float y) { m_dst.x = x; m_dst.y = y; }
};

class TiledLevel : public GameObject//like a manager of tiles
{
public:
	TiledLevel(const unsigned short r,const unsigned short c,const int w,const int h,
		const char* tileData,const char* levelData,const char* tileKey);
	~TiledLevel();//destuctor because tiles are created within this class dynamically
	void Update() {}
	void Render();
	vector<Tile*>& GetObstacles() { return obstacles; }
private:
	const char* tileKey;//for txtermanager
	int rows, cols;
	map<char, Tile*>tiles;//16 prototype tile objects
	vector<vector<Tile*>> level;//a 2d vector//outer vec for rows //innner vector for cols
	vector < Tile*> obstacles;//a 1d vector for collision 
};

#endif

