#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include <iostream>


using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

TitleState::TitleState(){}


void TitleState::Enter()
{
	cout << "enter titlestate" << endl;
	
	//hint load music track add it to map and play it

	
	
	
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
	{
		cout << "changing to gamestate" << endl;
		STMA::ChangeState(new GameState() );
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(),0,0,255,255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
}

void TitleState::Exit()
{
	cout << "exiting titlestate" << endl;
	
//call Mix_FreeMusic on your music track
}



PauseState::PauseState(){}



void PauseState::Enter()
{
	cout << "entering pausestate" << endl;
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
		STMA::PopState();
}

void PauseState::Render()
{
	//1st render the gamestate
	STMA::GetStates().front()->Render();
	//now render the rest of pausestate
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 128);
	SDL_Rect rect = { 255,128,512,512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	State::Render();
}

void PauseState::Exit()
{
	cout << "exiting pausestate" << endl;
}



GameState::GameState(){}

void GameState::Enter()
{
	cout << "entering gamestate" << endl;

	//load music sfx add them to map
	//load music track add it to map and play it
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		cout << "Changing to PauseState" << endl;
		//pause the music track
		STMA::PushState(new PauseState());
	}
	//parse 'X' key and change state to new endstate
	//parse 1 key and play first sfx
	//parse 2 key and play second sfx
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	//this code below prevents SDL_RenderPresent form running twice in one frame
	if(dynamic_cast<GameState*>(STMA::GetStates().back() ) )//if current state is gamestate
	State::Render();

}

void GameState::Exit()
{
	cout << "exiting gamestate" << endl;
	
}

void GameState::Resume()
{
	cout << "resuming gamestate" << endl;
	//resume music track
}
