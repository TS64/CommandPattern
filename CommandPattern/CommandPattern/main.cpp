#include "Command.h"
#include "Windows.h"
#include <iostream>

int main()
{
	MacroCommand mc;
	InputHandler input;
	bool running = true;
	std::cout << "starting" << std::endl;
	std::cout << "Macro commands = 0; Map Buttons = 1; Exit = Esc" << std::endl;
	while (input.running)
	{
		input.handleInput();
	}
}