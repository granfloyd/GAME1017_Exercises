#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include <iostream>


using namespace std;

Missile::Missile(int x, int y)
{
	m_dst = { x,y,29,7 };
}

void Missile::Update(int moving)
{
	m_dst.x += MOVESPEED * moving;
}

Enemy::Enemy(int x, int y) :m_enemySrc({ 0,0,230,200 })
{
	m_enemyDst = { x, rand() % 555 + 0,m_enemySrc.w,m_enemySrc.h };
}

void Enemy::Update()
{
	m_enemyDst.x -= EMOVESPEED;
	frames++;
}

void Enemy::resetFrames()
{
	frames = 0;
}



void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

TitleState::TitleState(){}


void TitleState::Enter()
{	
		cout << "enter titlestate" << endl;	
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
	m_pShipTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Ship.png");
	m_pBGTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "bg1.png");
	m_pEnemyTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Enemy.png");

	m_pShoot = Mix_LoadWAV("aud/shoot.wav");
	m_peShoot = Mix_LoadWAV("aud/eshoot.wav");
	//m_pMaintheme = Mix_LoadMUS("aud/MainThemeGAME1017.mp3");//title theme
	m_pMaintheme = Mix_LoadMUS("aud/GAMETHEME.mp3");//gametheme
	Mix_PlayMusic(m_pMaintheme, -1);
	Mix_VolumeMusic(24); //0-128
	Mix_Volume(-1, 32); //-1 means all channels

	m_src = { 0, 0, 250, 270 }; // Clips out entire image.
	m_dst = { WIDTH / 2, HEIGHT / 2, 154, 221 }; // On screen location/appearance.

	m_missile.reserve(4); // Pre-allocates 4 elements of the vector array. Capacity = 4.
	m_playerpew.reserve(4); // Pre-allocates 4 elements of the vector array. Capacity = 4.
	m_enemy.reserve(4);

	m_enemySrc = { 0,0,280,243 };
	m_pew = { 0,0,29,7 };//sdl rect	

	g_bg1 = { 0,0,1024, 768 };
	g_bg2 = { 1024,0,1024, 768 };

	
	m_enemyTimer = 0;
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

		if (Engine::Instance().KeyDown(SDL_SCANCODE_SPACE)) // spacebar
		{
			if (m_isAlive == true)
			{
				// Fire dynamic Missile.
				m_playerpew.push_back(new Missile(m_dst.x + 130, m_dst.y + 105));
				m_playerpew.shrink_to_fit();
				Mix_PlayChannel(-1, m_pShoot, 0);
				cout << "pew " << endl;
			}

		}
		if (m_isAlive == true)
		{
			if (Engine::Instance().KeyDown(SDL_SCANCODE_S) && m_dst.y < (HEIGHT - m_dst.h))
				m_dst.y += SPEED;
			if (Engine::Instance().KeyDown(SDL_SCANCODE_W) && m_dst.y > 0)
				m_dst.y -= SPEED;

			if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && m_dst.x < (WIDTH - m_dst.w))
				m_dst.x += SPEED;
			if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && m_dst.x > 0)
				m_dst.x -= SPEED;
		}
		//scroll background
		g_bg1.x -= SPEED;
		g_bg2.x -= SPEED;
		//check
		if (g_bg1.x <= -g_bg1.w)
		{
			g_bg1.x = 0;
			g_bg2.x = 1024;
		}

		//spawn enemy
		m_enemyTimer++;
		if (m_enemyTimer >= FPS * ESPAWN)
		{
			m_enemyTimer = 0;
			srand(time(NULL));
			m_enemy.push_back(new Enemy(WIDTH + m_enemySrc.w, rand() % (HEIGHT - m_enemySrc.h)));
			m_enemy.shrink_to_fit();
			cout << "spawningcral" << endl;
		}
		//enemy fire
		for (int i = 0; i < m_enemy.size(); i++)
		{
			if (m_enemy[i]->frames >= FPS * EFIRERATE)
			{
				m_enemy[i]->resetFrames();
				cout << "are you gonna finsih that croissant" << endl;
				Mix_PlayChannel(-1, m_peShoot, 0);
				m_missile.push_back(new Missile(m_enemy[i]->m_enemyDst.x + 22, m_enemy[i]->m_enemyDst.y + 100));
				m_missile.shrink_to_fit();
			}
		}

		//move enemies
		for (unsigned i = 0; i < m_enemy.size(); i++)
		{
			m_enemy[i]->Update();
		}
		// Deallocate enemies that go off screen.
		for (unsigned i = 0; i < m_enemy.size(); i++)
		{
			if (m_enemy[i]->m_enemyDst.x <= -m_enemy[i]->m_enemyDst.w)
			{
				delete m_enemy[i];
				m_enemy[i] = nullptr;
				m_enemy.erase(m_enemy.begin() + i);
				m_enemy.shrink_to_fit();


				break;
			}
		}

		// Move enemy missile.
		for (unsigned i = 0; i < m_missile.size(); i++)
		{
			m_missile[i]->Update(-1);
		}
		// Deallocate enemy missile that go off-screen.
		for (unsigned i = 0; i < m_missile.size(); i++)
		{
			if (m_missile[i]->m_dst.x <= -m_missile[i]->m_dst.w)
			{
				delete m_missile[i]; // Deallocates missile through pointer.
				m_missile[i] = nullptr; // Ensures no dangling pointer.
				m_missile.erase(m_missile.begin() + i); // Erase element and resize array.
				m_missile.shrink_to_fit();
				break;
			}
		}

		// Move player missile.
		if (m_isAlive == true)
		{
			for (unsigned i = 0; i < m_playerpew.size(); i++)
			{
				m_playerpew[i]->Update(4);
			}
			// Deallocate player missile that go off-screen.
			for (unsigned i = 0; i < m_playerpew.size(); i++)
			{
				if (m_playerpew[i]->m_dst.x >= WIDTH - m_playerpew[i]->m_dst.w)
				{
					delete m_playerpew[i]; // Deallocates missile through pointer.
					m_playerpew[i] = nullptr; // Ensures no dangling pointer.
					m_playerpew.erase(m_playerpew.begin() + i); // Erase element and resize array.
					m_playerpew.shrink_to_fit();
					break;
				}
			}

		}


		//Collision test for all playerpew to enemy
		for (unsigned i = 0; i < m_playerpew.size(); i++)
		{
			//For all enemies
			for (unsigned j = 0; j < m_enemy.size(); j++)
			{
				if (SDL_HasIntersection(&m_playerpew[i]->m_dst, &m_enemy[j]->m_enemyDst)) //AABB Check
				{
					cout << "ns" << endl;
					//Mix_PlayChannel(-1, m_pFree, 0);//deadsoundfx needed
					//Deallcoate bullet that hits enemy
					delete m_playerpew[i]; // Deallocates bullet through pointer.
					m_playerpew[i] = nullptr; // Ensures no dangling pointer.
					m_playerpew.erase(m_playerpew.begin() + i); // Erase element and resize array.
					m_playerpew.shrink_to_fit();
					//deallocate enemy for 1 hit kill
					delete m_enemy[j];
					m_enemy[j] = nullptr;
					m_enemy.erase(m_enemy.begin() + j);
					m_enemy.shrink_to_fit();
					break;
				}
			}
		}

		//Collision enemy missile to player
		for (unsigned i = 0; i < m_missile.size(); i++)
		{
			if (SDL_HasIntersection(&m_missile[i]->m_dst, &m_dst))
			{

				//destoyer 
				m_isAlive = false;
				m_dst = { WIDTH / 2, HEIGHT / 2, 0, 0 };
				//deallocate enemy bullet
				delete m_missile[i]; // Deallocates missile through pointer.
				m_missile[i] = nullptr; // Ensures no dangling pointer.
				m_missile.erase(m_missile.begin() + i); // Erase element and resize array.
				m_missile.shrink_to_fit();
				break;
			}
		}

		//Collision for player vs enemies
		for (unsigned i = 0; i < m_enemy.size(); i++)
		{
			if (SDL_HasIntersection(&m_enemy[i]->m_enemyDst, &m_dst))
			{

				m_isAlive = false;
				m_dst = { WIDTH / 2, HEIGHT / 2, 0, 0 };
			}
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
		{

			cout << "changing to gamestate" << endl;
			STMA::ChangeState(new EndState());
		}

		
}

void GameState::Render()
{
	//SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);//fills window with solid colour
	SDL_RenderClear(Engine::Instance().GetRenderer());
	// Any drawing goes here...
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_player); // & is the address-of operator.
	//SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, NULL); // For the bg.
	// moving bg
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGTexture, NULL, &g_bg1);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGTexture, NULL, &g_bg2);

	//Render enemies.
	for (unsigned i = 0; i < m_enemy.size(); i++)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pEnemyTexture, &m_enemy[i]->m_enemySrc, &m_enemy[i]->m_enemyDst, 00.0, NULL, SDL_FLIP_NONE);
	}

	// Render missiles. 
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 255, 255);
	for (unsigned i = 0; i < m_playerpew.size(); i++)
	{
		SDL_RenderFillRect(Engine::Instance().GetRenderer(), &(m_playerpew[i]->m_dst));
	}

	//enemy missile
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 225, 0, 0, 255);
	for (unsigned i = 0; i < m_missile.size(); i++)
	{
		SDL_RenderFillRect(Engine::Instance().GetRenderer(), &(m_missile[i]->m_dst));
	}

	// Render sprite.		
	// For the ship.
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pShipTexture, NULL, &m_dst, 90.00, NULL, SDL_FLIP_NONE);
	if (m_turret == true)
	{
		SDL_Point origin = { m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 };
		SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 255, 0);
		SDL_RenderDrawLine(Engine::Instance().GetRenderer(), origin.x, origin.y, m_mousePos.x, m_mousePos.y);
	}

	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);
	//SDL_RenderClear(Engine::Instance().GetRenderer());
	//SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.
	//this code below prevents SDL_RenderPresent form running twice in one frame
	if(dynamic_cast<GameState*>(STMA::GetStates().back() ) )//if current state is gamestate
	
	State::Render();

}

void GameState::Exit()
{
	cout << "exiting gamestate" << endl;
	// Clean up vector.
	for (unsigned i = 0; i < m_enemy.size(); i++)
	{
		delete m_enemy[i];
		m_enemy[i] = nullptr;
	}
	for (unsigned i = 0; i < m_missile.size(); i++)
	{
		delete m_missile[i]; // Deallocates Missile through pointer.
		m_missile[i] = nullptr; // Ensures no dangling pointer.
	}
	for (unsigned i = 0; i < m_playerpew.size(); i++)
	{
		delete m_playerpew[i];
		m_playerpew[i] = nullptr;
	}
	m_playerpew.clear(); // Removes all elements. Size = 0.
	m_playerpew.shrink_to_fit();// Sets capacity to size.
	m_missile.clear();
	m_missile.shrink_to_fit();
	SDL_DestroyTexture(m_pShipTexture);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pEnemyTexture);
	Mix_FreeChunk(m_pShoot);
	Mix_FreeChunk(m_peShoot);
	Mix_FreeMusic(m_pMaintheme);
	
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
