#pragma once
#ifndef __InputHandler__
#define __InputHandler__

#include <vector>
#include <utility>

#include "SDL.h"
class InputHandler
{
public:
	static InputHandler* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new InputHandler();
		}

		return s_pInstance;
	}

	void update();
	void clean() {}
	bool isKeyDown(SDL_Scancode key);

private:
	InputHandler() {}
	~InputHandler() { }

	static InputHandler* s_pInstance;
	// handle keyboard events
	void onKeyDown();
	void onKeyUp();
	const Uint8* m_keystates;
};
typedef InputHandler TheInputHandler;

#endif