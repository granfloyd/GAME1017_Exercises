#include "StateManager.h"

void StateManager::Update()
{
	if (!s_states.empty())
		s_states.back()->Update();//s_states.back represents the current state running
}

void StateManager::Render()
{
	if (!s_states.empty())
		s_states.back()->Render();
}

//used when there are 2 state in vector , i.e game and pause
void StateManager::PushState(State* pState)//going from game state to pause
{
	s_states.push_back(pState);//pState now becomes current state
	s_states.back()->Enter();
		
}
//used when there are 2 state in vector , i.e game and pause
void StateManager::PopState()//going from pause back to game
{
	if (s_states.size() <= 1) return;
	if (!s_states.empty())
	{
		s_states.back()->Exit();//cleans up curret state 
		delete s_states.back();// deallocates current state
		s_states.back() = nullptr;//wranle our dangle..... 
		s_states.pop_back();//removed nullptr to deallocated current state 
	}
	s_states.back()->Resume();
}

void StateManager::ChangeState(State* pState)
{
	if (!s_states.empty())
	{
		s_states.back()->Exit();//cleans up curret state 
		delete s_states.back();// deallocates current state
		s_states.back() = nullptr;//wranle our dangle..... 
		s_states.pop_back();//removed nullptr to deallocated current state 
	}
	s_states.push_back(pState);
	s_states.back()->Enter();
}

void StateManager::Quit()
{
	while (!s_states.empty())
	{
		s_states.back()->Exit();//cleans up curret state 
		delete s_states.back();// deallocates current state
		s_states.back() = nullptr;
		s_states.pop_back();//removed nullptr to deallocated current state 
	}
}
std::vector<State*>& StateManager::GetStates() { return s_states; }
std::vector<State*> StateManager::s_states;

bool StateManager::s_stateChange = false;
const bool StateManager::StateChanging() { return s_stateChange; }

