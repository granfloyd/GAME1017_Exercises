#include "PlatformPlayer.h"
#include "EventMAnager.h"
#include "TextureManager.h"
#include <cmath>
//45-50mins in video week7

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d) :AnimatedSpriteObject(s, d),
state(STATE_JUMPING),isGrounded(false),isFacingLeft(false),
maxVelX(9.0),maxVelY(JUMPFORCE),grav(GRAVITY),drag(0.85)
{
	accelX = accelY = velX = velY = 0.0;
	//setanimation(?,?,?);inti jump ani
}

void PlatformPlayer::Update()
{
	switch (state)
	{
	case STATE_IDLING:
		//trans to run
		if(EVMA::KeyPressed(SDL_SCANCODE_A)|| EVMA::KeyPressed(SDL_SCANCODE_D))
		{
			state = STATE_RUNNING;
			//SetAnimation(?,?,?,?)
		}
		//trans to jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE)&& isGrounded)
		{
			accelY = -JUMPFORCE;
			isGrounded = false;
			state = STATE_JUMPING;
			//SetAnimation(?,?,?,?)
		}
		break;
	case STATE_RUNNING:
		//move on ground
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			accelX = -1.5;
			if (!isFacingLeft)
				isFacingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			accelX = 1.5;
			if (isFacingLeft)
				isFacingLeft = false;
		}
		//trans to idle
		if (!EVMA::KeyHeld(SDL_SCANCODE_A) && !EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			state = STATE_IDLING;
		}
		//trans to jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && isGrounded)
		{
			accelY = -JUMPFORCE;
			isGrounded = false;
			state = STATE_JUMPING;
			//SetAnimation(?,?,?,?)
		}
		break;
	case STATE_JUMPING:
		if (EVMA::KeyHeld(SDL_SCANCODE_A))//move in mid air
		{
			accelX = -1.5;
			if(!isFacingLeft)
			isFacingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			accelX = 1.5;
			if (isFacingLeft)
			isFacingLeft = false;
		}
		if (isGrounded)//transition to run
		{
			state = STATE_RUNNING;//could also be idle
			//SetAnimation(?,?,?,?)
		}
		break;

	}
	//player movement universal for all states x-axis first
	velX += accelX;///add acceleration to velocity
	velX *= (isGrounded ? drag : 1.0);
	//if (std::abs(velX) < 0.1)velX = 0.0;
	velX = std::min(std::max(velX, -maxVelX), maxVelX);
	m_dst.x += (float)velX;
	//wrap the player on screen here
	
	//y-axis 
	velY += accelY + grav;///add acceleration to velocity
	velY = std::min(std::max(velY, -maxVelY), maxVelY);
	m_dst.y += (float)velY;

	accelX = accelY = 0.0;
	//animate();
}

void PlatformPlayer::Render()
{
	//to animate the sprite use SDL_RENDERCOPYExF() and you will have to acess the texturemanager and pass in a key such as "player"
	//for this part were just gonna use a colored square
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0,255);
	SDL_RenderFillRectF(Engine::Instance().GetRenderer(), &m_dst);
}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX() { velX = 0.0; }

void PlatformPlayer::StopY() { velY = 0.0; }

void PlatformPlayer::SetAcclX(double a) { accelX = a; }

void PlatformPlayer::SetAcclY(double a) { accelY = a; }

void PlatformPlayer::SetX(float x) { m_dst.x = x; }

void PlatformPlayer::SetY(float y) { m_dst.y = y; }

void PlatformPlayer::SetGrounded(bool g) { isGrounded = g; }

bool PlatformPlayer::IsGrounded(){	return isGrounded;}

double PlatformPlayer::GetVelX(){	return velX;}

double PlatformPlayer::GetVelY(){	return velY;}
