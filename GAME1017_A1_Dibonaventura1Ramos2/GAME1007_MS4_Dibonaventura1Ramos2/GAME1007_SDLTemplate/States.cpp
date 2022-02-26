#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "MathManager.h"
#include <ctime>
#include <iostream>
using namespace std;

Missile::Missile(int x, int y)
{
	m_dst = { x,y,29,7 };
	ebulletDst = { x,y,29,7 };
}

void Missile::Update(int moving)
{
	ebulletDst.x += MOVESPEED * moving;
	m_dst.x += MOVESPEED * moving;
}

Enemy::Enemy(int x, int y) :m_enemySrc({ 0,0,200,121 })
{
	m_enemyDst = { x, rand() % 300 + 100,110,77 };	

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

void State::Resume(){}


TitleState::TitleState(){}


void TitleState::Enter()
{	
		cout << "enter titlestate\nwow cool moving title" << endl;	
		m_pTitletheme = Mix_LoadMUS("aud/Titletheme.mp3");//gametheme
		Title2 = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/cosmicswagtitle.png");
		Title = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/cosmicswag.jpg");
		play = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/play.png");
		playSrc = { 0,0,350,125 };
		playDst = { 350,600,350,125 };

		 src = { 0,0,700,74 };
		 dst = { 0,150,700,74 };
		 dst2 = { 800,150,700,74 };
		 end = { 1024,150,700,74 };
		 newdst = { -700,150,700,74 };

		Mix_PlayMusic(m_pTitletheme, -1);
		Mix_VolumeMusic(17); //0-128
}

void TitleState::Update()
{
	// Move object.
	dst.x += MTITLE;
	dst2.x += MTITLE;
	if (dst.x == end.x)  
	{
		dst = newdst;		
	}
	if (dst2.x == end.x)
	{
		dst2 = newdst;
	}
	if (SDL_PointInRect(&EVMA::GetMousePos(), &playDst))
	{
		if (EVMA::MousePressed(SDL_BUTTON(1)))
		{
			playDst.x = EVMA::GetMousePos().x - playDst.w / 2;
			playDst.y = EVMA::GetMousePos().y - playDst.h / 2;
			STMA::ChangeState(new GameState());
			return;
		}
	}
	
	
	if (EVMA::KeyPressed(SDL_SCANCODE_N))
	{
		cout << "changing to gamestate" << endl;
		STMA::ChangeState(new GameState());
		return;
	}
	
}

void TitleState::Render()
{
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(),0,0,255,255);
	SDL_RenderClear(Engine::Instance().GetRenderer());	
	SDL_RenderCopy(Engine::Instance().GetRenderer(), Title,NULL,NULL);	
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Title2, NULL, &dst, 00.0, NULL, SDL_FLIP_NONE);
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Title2, NULL, &dst2, 00.0, NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), play, NULL, &playDst);
	if (dynamic_cast<TitleState*>(STMA::GetStates().back()))//if current state is gamestate

	State::Render();
}

void TitleState::Exit()
{
	cout << "exiting titlestate" << endl;
	Mix_FreeMusic(m_pTitletheme);
	SDL_DestroyTexture(Title);
	SDL_DestroyTexture(Title2);
	SDL_DestroyTexture(play);
	
}



PauseState::PauseState(){}



void PauseState::Enter()
{
	
	cout << "entering pausestate" << endl;
	
}

void PauseState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_R))
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
	cout << "entering gamestate\nPRESS P TO PAUSE" << endl;
	m_pShipTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/Ship.png");
	m_pBGTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/bg1.png");
	m_pEnemyTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/1017enemy.png");
	ebullet = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/ebullet.png");
	

	hithurt = Mix_LoadWAV("aud/hitHurt.wav");
	m_pShoot = Mix_LoadWAV("aud/shoot.wav");
	m_peShoot = Mix_LoadWAV("aud/eshoot.wav");
	m_pMaintheme = Mix_LoadMUS("aud/GAMETHEME.mp3");//gametheme
	Mix_PlayMusic(m_pMaintheme, -1);
	Mix_VolumeMusic(15); //0-128
	Mix_Volume(-1, 28); //-1 means all channels

	m_src = { 0, 0, 130, 190 }; // Clips out entire image.
	m_dst = { WIDTH / 2, HEIGHT / 2, 130, 190 }; // On screen location/appearance.

	m_pew = { 0,0,29,7 };//sdl rect	

	g_bg1 = { 0,0,1024, 768 };
	g_bg2 = { 1024,0,1024, 768 };
	//ebulletSrc = { 0,0,29,7 };
	
	m_enemyTimer = 0;
}

void GameState::Update()
{
	
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		cout << "Changing to PauseState\nPRESS R TO RESUME" << endl;
		//pause the music track
		STMA::PushState(new PauseState());
		Mix_PauseMusic();
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_SPACE))
	{// Fire dynamic Missile.
		m_playerpew.push_back(new Missile(m_dst.x + 130, m_dst.y + 90));
		m_playerpew.shrink_to_fit();
		Mix_PlayChannel(-1, m_pShoot, 0);
		cout << "pew " << endl;
	}

	if (EVMA::KeyHeld(SDL_SCANCODE_S) && m_dst.y < (HEIGHT - m_dst.h))
		m_dst.y += SPEED;
	if (EVMA::KeyHeld(SDL_SCANCODE_W) && m_dst.y > 0)
		m_dst.y -= SPEED;

	if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x < (WIDTH - m_dst.w))
		m_dst.x += SPEED;
	if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 0)
		m_dst.x -= SPEED;
		
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
			cout << "spawning enemy" << endl;
		}
		//enemy fire
		for (unsigned i = 0; i < m_enemy.size(); i++)
		{
			if (m_enemy[i]->frames >= FPS * EFIRERATE / 2)
			{
				m_enemy[i]->resetFrames();
				cout << "e fire" << endl;
				Mix_PlayChannel(-1, m_peShoot, 0);
				m_missile.push_back(new Missile(m_enemy[i]->m_enemyDst.x, m_enemy[i]->m_enemyDst.y + 45));
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
				cout << "bye";
				STMA::ChangeState(new WinState());
				return;
				
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
			if (m_missile[i]->ebulletDst.x <= -m_missile[i]->ebulletDst.w)
			{
				delete m_missile[i]; // Deallocates missile through pointer.
				m_missile[i] = nullptr; // Ensures no dangling pointer.
				m_missile.erase(m_missile.begin() + i); // Erase element and resize array.
				m_missile.shrink_to_fit();
				break;
			}
		}

		// Move player missile.
		
		
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

		


		//Collision test for all playerpew to enemy
		for (unsigned i = 0; i < m_playerpew.size(); i++)
		{
			//For all enemies
			for (unsigned j = 0; j < m_enemy.size(); j++)
			{
				if (SDL_HasIntersection(&m_playerpew[i]->m_dst, &m_enemy[j]->m_enemyDst)) //AABB Check
				{
					Mix_PlayChannel(-1, hithurt, 0);
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
			if (SDL_HasIntersection(&m_missile[i]->ebulletDst, &m_dst))
			{
				cout << "died to enemy bullet" << endl;
				//destoyer 
				//m_dst = { WIDTH / 2, HEIGHT / 2, 0, 0 };
				//deallocate enemy bullet
				delete m_missile[i]; // Deallocates missile through pointer.
				m_missile[i] = nullptr; // Ensures no dangling pointer.
				m_missile.erase(m_missile.begin() + i); // Erase element and resize array.
				m_missile.shrink_to_fit();
				STMA::ChangeState(new EndState());
				return;					
			}
			
		}

		//Collision for player vs enemies
		
		for (unsigned i = 0; i < m_enemy.size(); i++)
		{
			if (SDL_HasIntersection(&m_enemy[i]->m_enemyDst, &m_dst))
			{
				cout << "carl " << endl;
				//m_dst = { WIDTH / 2, HEIGHT / 2, 0, 0 };
				STMA::ChangeState(new EndState());
				return;
			}
			
		}

		if (EVMA::KeyPressed(SDL_SCANCODE_X))
		{

			cout << "changing to endstate" << endl;
			STMA::ChangeState(new EndState());
			return;
			
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
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 111,111, 0, 255);
	for (unsigned i = 0; i < m_missile.size(); i++)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), ebullet,NULL, &(m_missile[i]->ebulletDst),00.0, NULL, SDL_FLIP_NONE);
	}

	// Render sprite.		
	// For the ship.
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pShipTexture, NULL, &m_dst, 90.00, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.
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
	SDL_DestroyTexture(ebullet);
	Mix_FreeChunk(hithurt);
	Mix_FreeChunk(m_pShoot);
	Mix_FreeChunk(m_peShoot);
	Mix_FreeMusic(m_pMaintheme);
	
}

void GameState::Resume()
{
	cout << "resuming gamestate\nPRESS P TO PAUSE" << endl;
	if (Mix_PausedMusic() == true)
		Mix_ResumeMusic();
	//resume music track
	
}

EndState::EndState(){}

void EndState::Enter()
{
	cout << "entering endstate" << endl;
	hurt2 = Mix_LoadWAV("aud/hurt2.mp3");//endsfx
	sage = Mix_LoadMUS("aud/sage.mp3");
	TitleL = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/cosmicswagL.png");
	menyou = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/mainmenu.jfif");
	Mix_PlayMusic(sage,-1);
	Mix_PlayChannel(-1,hurt2, 0);
	Mix_VolumeMusic(44); //0-128
	Mix_Volume(-1, 40); //0-128
	menyouSrc = { 0,0,350,125 };
	menyouDst = { 350,600,350,125 };
	
}

void EndState::Update()
{
	if (SDL_PointInRect(&EVMA::GetMousePos(), &menyouDst))
	{
		if (EVMA::MousePressed(SDL_BUTTON(1)))
		{
			menyouDst.x = EVMA::GetMousePos().x - menyouDst.w / 2;
			menyouDst.y = EVMA::GetMousePos().y - menyouDst.h / 2;
			STMA::ChangeState(new TitleState());
			return;
		}
	}

	if (EVMA::KeyPressed(SDL_SCANCODE_R))
	{
		cout << "changing to gamestate" << endl;
		STMA::ChangeState(new TitleState());
		return;
	}

}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 69, 100, 200, 230);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TitleL, NULL, NULL);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), menyou, NULL, &menyouDst);
	if (dynamic_cast<EndState*>(STMA::GetStates().back()))//if current state is endstate
	
	State::Render();

}

void EndState::Exit()
{
	cout << "exitinggamestate" << endl;
	SDL_DestroyTexture(TitleL);
	SDL_DestroyTexture(menyou);
	Mix_FreeChunk(hurt2);
	Mix_FreeMusic(sage);
	
	
}

WinState::WinState(){}

void WinState::Enter()
{
	cout << "entering winstate" << endl;
	wintheme = Mix_LoadMUS("aud/win.mp3");//endtheme
	TitleW = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/cosmicswagW.png");
	menyou = IMG_LoadTexture(Engine::Instance().GetRenderer(), "stuff/mainmenu.jfif");
	menyouSrc = { 0,0,350,125 };
	menyouDst = { 350,600,350,125 };
    Mix_PlayMusic(wintheme, -1);
	Mix_VolumeMusic(33); //0-128
	
}

void WinState::Update()
{
	if (SDL_PointInRect(&EVMA::GetMousePos(), &menyouDst))
	{
		if (EVMA::MousePressed(SDL_BUTTON(1)))
		{
			menyouDst.x = EVMA::GetMousePos().x - menyouDst.w / 2;
			menyouDst.y = EVMA::GetMousePos().y - menyouDst.h / 2;
			STMA::ChangeState(new TitleState());
			return;
		}
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_R))
	{
		cout << "changing to gamestate" << endl;
		STMA::ChangeState(new TitleState());
		return;
	}
	
}

void WinState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 123, 123, 123, 123);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	SDL_RenderCopy(Engine::Instance().GetRenderer(), TitleW, NULL, NULL);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), menyou, NULL, &menyouDst);
	if (dynamic_cast<WinState*>(STMA::GetStates().back()))//if current state is winstate
	State::Render();
}

void WinState::Exit()
{
	cout << "exitinggamestate" << endl;
	SDL_DestroyTexture(TitleW);
	SDL_DestroyTexture(menyou);
	Mix_FreeMusic(wintheme);

}
