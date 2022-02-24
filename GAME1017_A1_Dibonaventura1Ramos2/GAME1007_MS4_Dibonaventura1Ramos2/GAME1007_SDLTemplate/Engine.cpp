#include "Engine.h"
#include "StateManager.h"
#include "States.h"
#include "EventManager.h"
#include <ctime>

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
						EVMA::Init();
						if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
						{
					
						}
						else return false; // Image init failed.
						if (Mix_Init(MIX_INIT_MP3) != 0)
						{
							Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);
							Mix_AllocateChannels(16);
							
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
			STMA::ChangeState(new TitleState());
			cout << "Initialization successful!" << endl;
			m_running = true;
			return true;
		}
		
	}

	void Engine::Wake()
	{
		m_start = SDL_GetTicks();
	}


void Engine::HandleEvents()
{
	EVMA::HandleEvents();
}


//bool Engine :: KeyDown(SDL_Scancode c)
//{
//	if (m_keystates != nullptr)
//	{
//		if (m_keystates[c] == 1) // Key we're testing for is down.
//			return true;
//	}
//	return false;
//}

void Engine::Update()
{
	STMA::Update();	
}

	
void Engine::Render()
{
	STMA::Render();
	//StateManager.Render()->CurrentState.Render()
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
	if (Init("CosmicSwag6900", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
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
Engine& Engine::Instance()//no static keyword required
{
	// TODO: insert return statement here
	static Engine instance;//creating an object of Engine. magic statics
	return instance;

}
void Engine::Clean()
{
	cout << "cleaning up..." << endl;
	STMA::Quit();	
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
//bool& Engine::Running() { return m_running; } // Getter.
//void Engine::SetRunning(const bool b) { m_running = b; }











