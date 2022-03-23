#pragma once

#include "GameObject.h"
#define GRAVITY 2.0//pulling down, no shit
#define JUMPFORCE 25.0//upward Y force


enum PlayerState{STATE_IDLING,STATE_RUNNING,STATE_JUMPING};

class PlatformPlayer : public AnimatedSpriteObject
{
public:
	PlatformPlayer(SDL_Rect s, SDL_FRect d);
	void Update();
	void Render();
	//getters and setters
	void Stop();
	void StopX();
	void StopY();
	void SetAcclX(double a);
	void SetAcclY(double a);
	void SetX(float x);
	void SetY(float y);
	void SetGrounded(bool g);
	bool IsGrounded();
	double GetVelX();
	double GetVelY();

private:
	bool isGrounded, isFacingLeft;
	double accelX, accelY,
		velX, velY,
		maxVelX, maxVelY,
		drag, grav;
	PlayerState state;

};

