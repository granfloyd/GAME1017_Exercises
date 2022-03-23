#include "PlatformPlayer.h"
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

			break;
	case STATE_RUNNING:

			break;
	case STATE_JUMPING:

		break;

	}
}

void PlatformPlayer::Render()
{
	//to animate the sprite use SDL_RENDERCOPYEXF()
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
