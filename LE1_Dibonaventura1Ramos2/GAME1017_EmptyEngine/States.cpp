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
		//load music track add it to map and play it
		m_DMCA = Mix_LoadMUS("Aud/DMCA.mp3");
		m_sounds.emplace("DMCA", m_DMCA);
		Mix_PlayMusic(m_DMCA, -1);
		Mix_VolumeMusic(32);

		/*m_Gasoline1 = Mix_LoadMUS("Aud/Gasoline1.mp3");
		m_sounds.emplace("Gasoline1", m_Gasoline1);
		Mix_PlayMusic(m_Gasoline1, -1);
		Mix_VolumeMusic(32);*/
	
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
	Mix_FreeMusic(m_DMCA);
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
	m_Gasoline1 = Mix_LoadMUS("Aud/Gasoline1.mp3");
	m_sounds.emplace("Gasoline1", m_Gasoline1);
	Mix_PlayMusic(m_Gasoline1, -1);
	Mix_VolumeMusic(32);

	m_Superpartypc = Mix_LoadWAV("Aud/superpartypcvoice.mp3");
	m_sfx.emplace("Superpartypc", m_Superpartypc);

	m_Free = Mix_LoadWAV("Aud/Free.mp3");
	m_sfx.emplace("Free", m_Free);

	//superpartypc 
	Mix_Volume(2, 12);
	
	//free
	Mix_Volume(3, 39);
}

void GameState::Update()
{
	
		if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
		{
			cout << "Changing to PauseState" << endl;
			//pause the music track
			STMA::PushState(new PauseState());
			Mix_PauseMusic();
		}
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_1))
		{
			Mix_PlayChannel(2, m_sfx["Superpartypc"], 0);

		}
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_2))
		{
			Mix_PlayChannel(3, m_sfx["Free"], 0);
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
		{

			cout << "changing to gamestate" << endl;
			STMA::ChangeState(new EndState());
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
	Mix_FreeMusic(m_Gasoline1);
	Mix_FreeMusic(m_DMCA);
	Mix_FreeChunk(m_Superpartypc);
	Mix_FreeChunk(m_Free);
	
}

void GameState::Resume()
{
	cout << "resuming gamestate" << endl;
	if (Mix_PausedMusic() == true)
		Mix_ResumeMusic();
	//resume music track
	
}

EndState::EndState(){}

void EndState::Enter()
{
	cout << "entering endstate" << endl;
}

void EndState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{

		cout << "changing to gamestate" << endl;
		STMA::ChangeState(new TitleState());
	}

}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 69, 100, 200, 230);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();

}

void EndState::Exit()
{
	cout << "exitinggamestate" << endl;
}
