#include "Engine.h"


Missile::Missile( int x, int y )
{
	m_dst = { x,y,10,10 };
}

void Missile::Update(int moving)
{
	m_dst.x += MOVESPEED * moving;
}

Enemy::Enemy(int x, int y) :m_enemySrc({ 0,0,230,200 })
{
	m_enemyDst = { x, rand() % 100 + -100,m_enemySrc.w,m_enemySrc.h };
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

	int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
	{
		{
			cout << "Initializing game..." << endl;
			if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay.
			{
				// Try to create the SDL_Window.
				m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
				if (m_pWindow != nullptr) // If window creation passes.
				{
					// Try to create the SDL_Renderer. (Back buffer)
					m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
					if (m_pRenderer != nullptr)
					{
						if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
						{
							m_pShipTexture = IMG_LoadTexture(m_pRenderer, "Ship.png");
							m_pBGTexture = IMG_LoadTexture(m_pRenderer, "bg1.png");
							//m_pMissileTexture = IMG_LoadTexture(m_pRenderer, "Missile.png");
							m_pEnemyTexture = IMG_LoadTexture(m_pRenderer, "Enemy.png");
														// Add check of texture pointer later...
						}
						else return false; // Image init failed.
						if (Mix_Init(MIX_INIT_MP3) != 0)
						{
							Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);
							Mix_AllocateChannels(16);
							//load sound
							m_pFree = Mix_LoadWAV("aud/Free.mp3");
							m_pShort = Mix_LoadWAV("aud/Blaster.mp3");
							m_pLaser = Mix_LoadWAV("aud/carlpew.mp3");
							m_pMusic = Mix_LoadMUS("aud/MainTheme.mp3");
						}
						else return false; // Mixer init failed
					}
					else return false; // Renderer creation failed.
				}
				else return false; // Window creation failed.
			}
			else return false; // Initialization has failed.
			// If everything is okay, we are here...
			m_fps = (Uint32)round(1 / (double)FPS * 1000);
			m_keystates = SDL_GetKeyboardState(nullptr);
			//m_missile = { WIDTH / 2, HEIGHT / 2, 99, 99 };

			m_src = { 0, 0, 250, 270 }; // Clips out entire image.
			m_dst = { WIDTH / 2, HEIGHT / 2, 154, 221 }; // On screen location/appearance.

			m_missile.reserve(4); // Pre-allocates 4 elements of the vector array. Capacity = 4.
			m_playerpew.reserve(4); // Pre-allocates 4 elements of the vector array. Capacity = 4.
			m_enemy.reserve(4);

			m_enemySrc = { 0,0,280,243 };						
			m_pew = { 0,0,10,10 };//sdl rect	
			
			g_bg1 = { 0,0,1024, 768 };
			g_bg2 = { 1024,0,1024, 768 };

			Mix_PlayMusic(m_pMusic, -1);
			Mix_VolumeMusic(60); //0-128
			Mix_Volume(-1, 32); //-1 means all channels
			

			m_enemyTimer = 0;
			cout << "Initialization successful!" << endl;
			m_running = true;
			return true;
		}
		
	}
	//Missile::Missile(int x, int y) :m_src({ 0,0,10,10})
		// Body considered assignment not initialization.
	 
		//m_dst = { x,y,m_src.w * 2,m_src.h * 2 };
	

	void Engine::Clean()
	{
		cout << "cleaning up..." << endl;
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
		SDL_DestroyRenderer(m_pRenderer);
		SDL_DestroyWindow(m_pWindow);
		SDL_DestroyTexture(m_pShipTexture);
		SDL_DestroyTexture(m_pBGTexture);
		SDL_DestroyTexture(m_pEnemyTexture);
		Mix_FreeChunk(m_pLaser);
		Mix_FreeChunk(m_pShort);
		Mix_FreeChunk(m_pFree);
		Mix_FreeMusic(m_pMusic);
		Mix_CloseAudio();
		Mix_Quit();

		IMG_Quit();
		SDL_Quit();
	}


	void Engine::Wake()
	{
		m_start = SDL_GetTicks();
	}


void Engine::HandleEvents()
{
	//cout << "Getting input..." << endl;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // Pressing 'X' button of window.
			m_running = false;
			break;
		case SDL_KEYDOWN: // Essential parses every frame. Just like keystates.
		//	if (event.key.keysym.sym == SDLK_SPAC) // Pressing spacebar.
		//	{

		//		
		//		if (m_isAlive == true)
		//		{
		//			m_fire = true;
		//			if (!Mix_Playing(8))
		//			{
		//				Mix_SetPanning(8, 255, 128);//all left ,half right
		//				//Mix_PlayChannel(8, m_pShort, -1);
		//			    cout << "Firin' mah lazor!" << endl;
		//			}
		//			cout << "Firin' mah !" << endl;
		//		}
		//			
		//		

		//	}
		//	break;
		//case SDL_KEYUP: // One-shot
		//	if (event.key.keysym.sym == ' ') // Releasing spacebar.
		//	{
		//		m_fire = false;
		//		Mix_HaltChannel(8);
		//		cout << "Done shootin" << endl;
		//		
		//	}
			if (event.key.keysym.sym == 32) // Enter.
			{
				if (m_isAlive == true)
				{
					// Fire dynamic Missile.
					m_playerpew.push_back(new Missile(m_dst.x +130, m_dst.y +105 ));
					m_playerpew.shrink_to_fit();
					Mix_PlayChannel(-1, m_pShort, 0);
					cout << "pew " << endl;
				}
				
				
			}
			break;
			
		//	break;
		//case SDL_MOUSEMOTION:
		//	SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
		//	// cout << '(' << g_mousePos.x << ',' << g_mousePos.y << ')' << endl;
		//	break;
		//case SDL_MOUSEBUTTONDOWN:
		//	if (event.button.button == SDL_BUTTON_LEFT)
		//	{
		//		if (m_isAlive == true)
		//		{ 
		//			m_turret = true;
		//			if (!Mix_Playing(9))
		//			{
		//				Mix_SetPanning(9, 128, 255);//all right ,half left
		//				Mix_PlayChannel(9, m_pShort, -1);
		//			}
		//		}
		//		
		//	}
		//	break;

		//case SDL_MOUSEBUTTONUP:
		//	if (event.button.button == SDL_BUTTON_LEFT)
		//	{
		//		m_turret = false;
		//		Mix_HaltChannel(9);
		//	}

		//	break;
		}
	}
}


bool Engine :: KeyDown(SDL_Scancode c)
{
	if (m_keystates != nullptr)
	{
		if (m_keystates[c] == 1) // Key we're testing for is down.
			return true;
	}
	return false;
}

void Engine::Update()
{
	if (m_isAlive == true)
	{
		//cout << "Updating game..." << endl;
		if (KeyDown(SDL_SCANCODE_S) && m_dst.y < (HEIGHT - m_dst.h))
			m_dst.y += SPEED;
		if (KeyDown(SDL_SCANCODE_W) && m_dst.y > 0)
			m_dst.y -= SPEED;

		if (KeyDown(SDL_SCANCODE_D) && m_dst.x < (WIDTH - m_dst.w))
			m_dst.x += SPEED;
		if (KeyDown(SDL_SCANCODE_A) && m_dst.x > 0)
			m_dst.x -= SPEED;
	}
	
	/*if (KeyDown(SDL_SCANCODE_A))
		g_dst.x -= SPEED;
	if (KeyDown(SDL_SCANCODE_D))
		g_dst.x += SPEED;*/

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
			Mix_PlayChannel(-1, m_pLaser, 0);
			m_missile.push_back(new Missile(m_enemy[i]->m_enemyDst.x +35, m_enemy[i]->m_enemyDst.y +100));
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
				cout << "noice" << endl;
				Mix_PlayChannel(-1, m_pFree, 0);
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

	
}

	
void Engine::Render()
{
	
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);//fills window with solid colour
	SDL_RenderClear(m_pRenderer);
	// Any drawing goes here...
	SDL_RenderFillRect(m_pRenderer, &m_player); // & is the address-of operator.
	//SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, NULL); // For the bg.
	// moving bg
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, &g_bg1);
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, &g_bg2);
	
	//spacebar thing
	if (m_fire == true)
	{
		SDL_Point origin = { m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 };
		SDL_Point end = { m_dst.x + m_dst.w / 2, m_dst.y - 300 };
		SDL_SetRenderDrawColor(m_pRenderer, 222, 255, 255, 255);//colour
		SDL_RenderDrawLine(m_pRenderer, origin.x, origin.y, end.x, end.y);//where thing will spawn
	}

	//Render enemies.
    for (unsigned i = 0; i < m_enemy.size(); i++)
    {
			SDL_RenderCopyEx(m_pRenderer, m_pEnemyTexture, &m_enemy[i]->m_enemySrc, &m_enemy[i]->m_enemyDst, 00.0, NULL, SDL_FLIP_NONE);
	}
	// Render missiles. 
	SDL_SetRenderDrawColor(m_pRenderer, 0, 255, 255, 255);
	for (unsigned i = 0; i < m_playerpew.size(); i++)
	{
		SDL_RenderFillRect(m_pRenderer, &(m_playerpew[i]->m_dst));
	}
	//enemy missile
	SDL_SetRenderDrawColor(m_pRenderer, 225, 0, 0, 255);
	for (unsigned i = 0; i < m_missile.size(); i++)
	{
		SDL_RenderFillRect(m_pRenderer, &(m_missile[i]->m_dst));
	}
	
	// Render sprite.		
	// For the ship.
	SDL_RenderCopyEx(m_pRenderer, m_pShipTexture,NULL , &m_dst, 90.00, NULL, SDL_FLIP_NONE);
	if (m_turret == true)
	{
		SDL_Point origin = { m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 };
		SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, 0);
		SDL_RenderDrawLine(m_pRenderer, origin.x, origin.y, m_mousePos.x, m_mousePos.y);//mousePos can work with g?
	}
	
	SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.


}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start; // 1055 - 1050 = 5ms
	if (m_delta < m_fps)
		SDL_Delay(m_fps - m_delta);
}

int Engine::Run()

	{
		if (m_running == true)
		{
			return 1;
		}
		// Start and run the "engine"
		if (Init("GAME1007 M2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
		{
			return 2;
		}
		// We passed our initial checks, start the loop!
		while (m_running == true)
		{
			Wake();
			HandleEvents(); // Input
			Update();       // Processing
			Render();       // Output
			if (m_running == true)
				Sleep();
		}
		Clean();


		return 0;
	}










