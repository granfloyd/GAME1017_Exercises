#include "TiledLevel.h"

TiledLevel::TiledLevel(const unsigned short r, const unsigned short c, const int w, const int h, const char*
	tileData, const char* levelData, const char* tileKey):rows(r),cols(c),tileKey(tileKey)
{
	ifstream inFile(tileData);//create the 16 protypes tiles
	if (inFile.is_open())
	{
		char key;
		int x, y;
		bool obs, haz;
		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> obs >> haz;
			tiles.emplace(key, new Tile({ x * w,y * h,w,h }, {0.0f,0.0f,(float)w,(float)h},obs,haz));
		}
	}
	inFile.close();
	inFile.open(levelData);//create the whole tiled level now from text file
	if (inFile.is_open())
	{
		char key;
		level.resize(rows);//sets the size of the fist dimension.we can only use [] after a size is set
		for (unsigned short row = 0; row < rows;row++) 
		{
			level[row].resize(cols);
			for (unsigned short col = 0; col < cols;col++)
			{
				inFile >> key;
				level[row][col] = tiles[key]->Clone();//common prototype method
				level[row][col]->SetXY((float)col * w, (float)row * h);
				if (level[row][col]->IsObstacle())
					obstacles.push_back(level[row][col]);
			}
			
		}
	}
	inFile.close();
}

TiledLevel::~TiledLevel()
{
	//clear the tiled level of clones. this also would account for those in obstacles
	for (unsigned short row = 0; row < rows; row++)
	{
		for (unsigned short col = 0; col < cols; col++)
		{
			delete level[row][col];
			level[row][col] = nullptr;
		}
	}
	level.clear();
	obstacles.clear();
	//clear the original tile prototypes
	for (map<char, Tile*>::iterator i = tiles.begin(); i != tiles.end(); i++)
	{
		delete i->second;
		i->second = nullptr;
	}
	tiles.clear();
}

void TiledLevel::Render()
{
	for (unsigned short row = 0; row < rows; row++)
	{
		for (unsigned short col = 0; col < cols; col++)
		{
			SDL_RenderCopyF(Engine::Instance().GetRenderer(), TEMA::GetTexture(tileKey),
				level[row][col]->GetSrc(), level[row][col]->GetDst());
		}
	}
}
