/*******************************************************************************
* File Name   :		 1017LE2
* Description :	add saving for turret pos,add collision for bullets , delete stuff that go off screen
* Author      :	Michael Dibonaventura
* Created     :	Feb 8 2022
* Modified    :	//19mins in video for delete pos enemy bullet 38min in vid
*******************************************************************************/

// SDL includes pasted for convenience. Move/copy to relevant files.
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Engine.h"
#include <iostream>

int main(int argc, char* argv[])
{
	return Engine::Instance().Run();
}