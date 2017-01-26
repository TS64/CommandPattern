#pragma once
#include "Windows.h"
#include <conio.h>
#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <typeinfo>

using namespace std;

class Command
{
public:
	~Command() {}
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual void redo() = 0;
	vector<Command*> commands;
protected:
	Command() {}
};

class ACommand : public Command
{
public:
	virtual void execute() { std::cout << "A" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class SCommand : public Command
{
public:
	virtual void execute() { std::cout << "S" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class DCommand : public Command
{
public:
	virtual void execute() { std::cout << "D" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class WCommand : public Command
{
public:
	virtual void execute() { std::cout << "W" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class UPCommand : public Command
{
public:
	virtual void execute() { std::cout << "UP" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class DOWNCommand : public Command
{
public:
	virtual void execute() { std::cout << "DOWN" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class LEFTCommand : public Command
{
public:
	virtual void execute() { std::cout << "LEFT" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class RIGHTCommand : public Command
{
public:
	virtual void execute() { std::cout << "RIGHT" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class SPACECommand : public Command
{
public:
	virtual void execute() { std::cout << "SPACE" << endl; }
	virtual void undo() {}
	virtual void redo() {}
};

class MacroCommand : public Command {
public:
	MacroCommand()
	{
		resetHistory();
	}
	void resetHistory()
	{
		vector<Command*> empty;
		commandHistory.clear();
		commandHistory.push_back(empty);
	}
	virtual ~MacroCommand() {}
	vector<vector<Command*>> commandHistory;
	vector<vector<Command*>>::iterator it;

	virtual void add(Command *c)
	{
		commands.push_back(c);
		commandHistory.push_back(commands);
		it = commandHistory.end() - 1;
	}
	virtual void remove(Command *c)
	{
		vector<Command*>::iterator i = commands.begin();
		while (i != commands.end())
		{
			i = find(commands.begin(), commands.end(), c);
			if (i != commands.end())
			{
				commands.erase(i);
				i = commands.begin();
			}
		}
		
		commandHistory.push_back(commands);
		it = commandHistory.end() - 1;
	}
	virtual void undo()
	{
		if (!commandHistory.empty())
		{
			if (it != commandHistory.begin())
			{
				it--;
			}
			commands = *it;
			
			vector<Command*>::iterator i;
			for (i = commands.begin(); i != commands.end(); ++i)
			{
				Command *c = *i;
				c->execute();
			}
		}
	}
	virtual void redo()
	{
		if (!commandHistory.empty())
		{
			if (it != commandHistory.end() - 1)
			{
				++it;
			}
			commands = *it;

			vector<Command*>::iterator i;
			for (i = commands.begin(); i != commands.end(); ++i)
			{
				Command *c = *i;
				c->execute();
			}
		}
	}
	virtual void execute()
	{
		vector<Command*>::iterator it;
		for (it = commands.begin(); it != commands.end(); ++it)
		{
			Command *c = *it;
			c->execute();
		}
	}
};

class InputHandler
{
public:
	InputHandler()
	{
		buttons = 
		{
			buttonA, buttonS, buttonD, buttonW,
			buttonLeft, buttonUp, buttonRight, buttonDown,
			buttonSpace
		};

		defaultButtons =
		{
			defaultButtonA, defaultButtonS, defaultButtonD, defaultButtonW,
			defaultButtonLeft, defaultButtonUp, defaultButtonRight, defaultButtonDown,
			defaultButtonSpace
		};
	}
	vector<int> keys = 
	{
		0x41, 0x53, 0x44, 0x57, // A, S, D, W
		0x25, 0x26, 0x27, 0x28, // Left, Up, Right, Down
		0x20					// Space
	};
	vector<Command*> buttons;
	vector<Command*> defaultButtons;
	vector<string> keyNames =
	{
		"A", "S", "D", "W",
		"LEFT", "UP", "RIGHT", "DOWN",
		"SPACE"
	};
	
	bool running = true;
	MacroCommand* mc = new MacroCommand();
	char mapKey;

	int keySelected;

	void Return()
	{
		menu = WAITING;
		std::cout << "Macro commands = 0; Exit = Esc" << std::endl;
	}
	void Waiting()
	{
		if (GetAsyncKeyState(0x30) < 0) // 0-key
		{
			std::cout << "Enter macros. Finish = Enter; Undo = X; Redo = Y; Remove = Backspace" << std::endl;
			menu = ENTER_MACROS;
		}
		for (int i = 0; i < keys.size(); i++)
		{
			if (GetAsyncKeyState(keys[i]) < 0)
			{
				buttons[i]->execute();
			}
		}
	}
	void MacroCommands()
	{
		if (GetAsyncKeyState(0x0D)) // Enter-key
		{
			mc->execute();
			cout << "Map this macro to a key? Press the key to map or Enter to skip" << endl;
			menu = MAP_MACROS;
		}
		if (GetAsyncKeyState(0x08)) // Backspace-key
		{
			std::cout << "Remove which commands" << std::endl;
			menu = REMOVING;
		}
		if (GetAsyncKeyState(0x58)) // X-key
		{
			mc->undo();
			std::cout << "Undo" << std::endl;
		}
		if (GetAsyncKeyState(0x59)) // Y-key
		{
			mc->redo();
			std::cout << "Redo" << std::endl;
		}
		for (int i = 0; i < keys.size(); i++)
		{
			if (GetAsyncKeyState(keys[i]) < 0)
			{
				mc->add(defaultButtons[i]);
				std::cout << "Added " << keyNames[i] << std::endl;
			}
		}
	}
	void MapMacros()
	{
		for (int i = 0; i < keys.size(); i++)
		{
			if (GetAsyncKeyState(keys[i]) < 0)
			{
				buttons[i] = new MacroCommand();
				buttons[i]->commands = mc->commands;
				mc->commands.clear();
				mc->resetHistory();
				Return();
			}
		}

		if (GetAsyncKeyState(0x0D) < 0) // Enter-key
		{
			mc->commands.clear();
			mc->resetHistory();
			Return();
		}
	}
	void Remove()
	{
		for (int r = 0; r < keys.size(); r++)
		{
			if (GetAsyncKeyState(keys[r]) < 0)
			{
				mc->remove(defaultButtons[r]);
				menu = ENTER_MACROS;
				std::cout << "Remove " << keyNames[r] << std::endl;
			}
		}
	}
	void handleInput()
	{
		if (GetAsyncKeyState(VK_ESCAPE) < 0)
		{
			running = false;
		}
		
		if (_kbhit() != 1)
		{
			switch (menu) {
			case WAITING:
				Waiting();
				break;
			case ENTER_MACROS:
				MacroCommands();
				break;
			case REMOVING:
				Remove();
				break;
			case MAP_MACROS:
				MapMacros();
				break;
			}
		}
		_getch();
	}

private:
	///// Can be changed into macro commands /////
	Command* buttonW = new WCommand();
	Command* buttonA = new ACommand();
	Command* buttonS = new SCommand();
	Command* buttonD = new DCommand();
	Command* buttonUp = new UPCommand();
	Command* buttonDown = new DOWNCommand();
	Command* buttonLeft = new LEFTCommand();
	Command* buttonRight = new RIGHTCommand();
	Command* buttonSpace = new SPACECommand();

	////// Will not change into macro commands /////
	Command* defaultButtonW = new WCommand();
	Command* defaultButtonA = new ACommand();
	Command* defaultButtonS = new SCommand();
	Command* defaultButtonD = new DCommand();
	Command* defaultButtonUp = new UPCommand();
	Command* defaultButtonDown = new DOWNCommand();
	Command* defaultButtonLeft = new LEFTCommand();
	Command* defaultButtonRight = new RIGHTCommand();
	Command* defaultButtonSpace = new SPACECommand();

	enum Menu { WAITING, ENTER_MACROS, REMOVING, MAP_MACROS };
	Menu menu = WAITING;

};
