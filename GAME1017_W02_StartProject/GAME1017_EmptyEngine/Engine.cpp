#include "Engine.h"
#include "StateManager.h"
#include "States.h"

int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay...
	{
		// Create the SDL window...
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{
			// Create the SDL renderer...(back buffer)
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, NULL);
			if (m_pRenderer != nullptr)
			{
				// Initialize subsystems...
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
				{
					// Do something here.add mixer
				}
				else return false; // Image init failed.
				if (Mix_Init(MIX_INIT_MP3) != 0)
				{
					//configure mixer
					Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
					Mix_AllocateChannels(16);
					//add sound pointers to map
					//m_sounds.emplace("", new Mix_Chunk);
					//m_sounds.emplace("", new Mix_Chunk);
					//m_sounds.emplace("", new Mix_Chunk);
					//m_sounds.emplace("", new Mix_Chunk);
					//load sounds
					//m_sounds[""] = Mix_LoadWAV("Aud/");
					//m_sounds[""] = Mix_LoadWAV("Aud/");
					//m_sounds[""] = Mix_LoadWAV("Aud/");
					//m_sounds[""] = Mix_LoadWAV("Aud/");
					//m_mask = Mix_loadMUS("Aud/);
				}
				else return false; //mixer init failed

			}
			else return false; // Renderer creation failed.
		}
		else return false; // Window creation failed.
	}
	else return false; // initalization failed.
	m_fps = (Uint32)round(1.0 / (double)FPS * 1000); // Converts FPS into milliseconds, e.g. 16.67
	m_keystates = SDL_GetKeyboardState(nullptr);
	STMA::ChangeState(new TitleState() );
	cout << "Initialization successful!" << endl;
	m_running = true;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;
		}
	}
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_keystates != nullptr)
	{
		if (m_keystates[c] == 1)
			return true;
	}
	return false;
}

void Engine::Update()
{
	STMA::Update();
}

void Engine::Render()
{
	STMA::Render();
	//StateMAnager.Render()->CurrentState.Render()
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks(); // GetTicks return time in ms since initialization
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
	if (Init("GAME1017 LE1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
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
	cout << "Cleaning engine..." << endl;
	STMA::Quit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}