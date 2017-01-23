#pragma once
#include "Windows.h"
#include <conio.h>
#include <list>
#include <vector>
#include <iostream>

using namespace std;

class Command
{
public:
	~Command() {}
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual void redo() = 0;
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
	MacroCommand() { 
		vector<Command*> ch;
		commandHistory.push_back(ch);
	}
	virtual ~MacroCommand() {}
	vector<Command*> commands;
	vector<vector<Command*>> commandHistory;
	
	//list<list<Command*>>::iterator historyIt;
	vector<vector<Command*>>::iterator it;
	int iter;
	virtual void add(Command *c)
	{
		commands.push_back(c);

		/*vector<Command*>::iterator i;
		for (i = commands.begin(); i != commands.end(); ++i)
		{
			Command *c = *i;
			c->execute();
		}*/

		commandHistory.push_back(commands);
		iter = commandHistory.size() - 1;
		it = commandHistory.end();
	}
	virtual void remove(Command *c)
	{
		auto i = find(commands.begin(), commands.end(), c);
		if (i != commands.end())
		{
			commands.erase(i);
		}
		commandHistory.push_back(commands);
		iter = commandHistory.size() - 1;
		it = commandHistory.end();
	}
	virtual void undo()
	{
		if (!commandHistory.empty())
		{
			if (it != commandHistory.begin())
			{
				--it;
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
	bool running = true;
	bool waiting = true;
	bool mapping = false;
	bool remapping = false;
	bool macroCommands = false;
	bool keyIsPressed = true;
	bool removing = false;
	MacroCommand mc;
	char mapKey;

	void Return()
	{
		remapping = false;
		waiting = true;
		std::cout << "Macro commands = 0; Map Buttons = 1; Exit = Esc" << std::endl;
	}
	void Waiting()
	{
		if (GetAsyncKeyState(0x30) < 0) // 0-key
		{
			std::cout << "Enter macros. Finish = Enter; Undo = X; Redo = Y; Remove = Backspace" << std::endl;
			macroCommands = true;
			waiting = false;
		}
		if (GetAsyncKeyState(0x31) < 0) // 1-key
		{
			std::cout << "Enter key you want to re-map:" << std::endl;
			mapping = true;
			waiting = false;
		}
		if (GetAsyncKeyState(0x41) < 0) // A-key
		{
			buttonA->execute();
		}
		if (GetAsyncKeyState(0x53) < 0) // S-key
		{
			buttonS->execute();
		}
		if (GetAsyncKeyState(0x44) < 0) // D-key
		{
			buttonD->execute();
		}
		if (GetAsyncKeyState(0x57) < 0) // W-key
		{
			buttonW->execute();
		}

		if (GetAsyncKeyState(0x26) < 0) // Up-key
		{
			buttonUp->execute();
		}
		if (GetAsyncKeyState(0x28) < 0) // Down-key
		{
			buttonDown->execute();
		}
		if (GetAsyncKeyState(0x25) < 0) // Left-key
		{
			buttonLeft->execute();
		}
		if (GetAsyncKeyState(0x27) < 0) // Right-key
		{
			buttonRight->execute();
		}
		if (GetAsyncKeyState(0x20) < 0) // Space-key
		{
			buttonSpace->execute();
		}
	}
	void Mapping()
	{
		if (GetAsyncKeyState(0x41) < 0) // A-key
		{
			mapKey = 'a';
			std::cout << "Re-map 'A' to which key?" << std::endl;
			mapping = false;
			remapping = true;
		}
		if (GetAsyncKeyState(0x53) < 0) // S-key
		{
			mapKey = 's';
			std::cout << "Re-map 'S' to which key?" << std::endl;
			mapping = false;
			remapping = true;
		}
		if (GetAsyncKeyState(0x44) < 0) // D-key
		{
			mapKey = 'd';
			std::cout << "Re-map 'D' to which key?" << std::endl;
			mapping = false;
			remapping = true;
		}
		if (GetAsyncKeyState(0x57) < 0) // W-key
		{
			mapKey = 'w';
			std::cout << "Re-map 'W' to which key?" << std::endl;
			mapping = false;
			remapping = true;
		}
	}
	void Remapping()
	{
		if (GetAsyncKeyState(0x41) < 0) // A-key
		{
			if (mapKey == 'a')
			{
				buttonA = new ACommand();
				std::cout << "Remapping 'A' to 'A'" << std::endl;
				Return();
			}
			if (mapKey == 's')
			{
				buttonS = new ACommand();
				std::cout << "Remapping 'S' to 'A'" << std::endl;
				Return();
			}
			if (mapKey == 'd')
			{
				buttonD = new ACommand();
				std::cout << "Remapping 'D' to 'A'" << std::endl;
				Return();
			}
			if (mapKey == 'w')
			{
				buttonW = new ACommand();
				std::cout << "Remapping 'W' to 'A'" << std::endl;
				Return();
			}
		}
		if (GetAsyncKeyState(0x53) < 0) // S-key
		{
			if (mapKey == 'a')
			{
				buttonA = new SCommand();
				std::cout << "Remapping 'A' to 'S'" << std::endl;
				Return();
			}
			if (mapKey == 's')
			{
				buttonS = new SCommand();
				std::cout << "Remapping 'S' to 'S'" << std::endl;
				Return();
			}
			if (mapKey == 'd')
			{
				buttonD = new SCommand();
				std::cout << "Remapping 'D' to 'S'" << std::endl;
				Return();
			}
			if (mapKey == 'w')
			{
				buttonW = new SCommand();
				std::cout << "Remapping 'W' to 'S'" << std::endl;
				Return();
			}
		}
		if (GetAsyncKeyState(0x44) < 0) // D-key
		{
			if (mapKey == 'a')
			{
				buttonA = new DCommand();
				std::cout << "Remapping 'A' to 'D'" << std::endl;
				Return();
			}
			if (mapKey == 's')
			{
				buttonS = new DCommand();
				std::cout << "Remapping 'S' to 'D'" << std::endl;
				Return();
			}
			if (mapKey == 'd')
			{
				buttonD = new DCommand();
				std::cout << "Remapping 'D' to 'D'" << std::endl;
				Return();
			}
			if (mapKey == 'w')
			{
				buttonW = new DCommand();
				std::cout << "Remapping 'W' to 'D'" << std::endl;
				Return();
			}
		}
		if (GetAsyncKeyState(0x57) < 0) // W-key
		{
			if (mapKey == 'a')
			{
				buttonA = new WCommand();
				std::cout << "Remapping 'A' to 'W'" << std::endl;
				Return();
			}
			if (mapKey == 's')
			{
				buttonS = new WCommand();
				std::cout << "Remapping 'S' to 'W'" << std::endl;
				Return();
			}
			if (mapKey == 'd')
			{
				buttonD = new WCommand();
				std::cout << "Remapping 'D' to 'W'" << std::endl;
				Return();
			}
			if (mapKey == 'w')
			{
				buttonW = new WCommand();
				std::cout << "Remapping 'W' to 'W'" << std::endl;
				Return();
			}
		}
	}
	void MacroCommands()
	{
		if (GetAsyncKeyState(0x0D)) // Enter-key
		{
			mc.execute();
			mc.commands.clear();
			mc.commandHistory.clear();
			vector<Command*> ch;
			mc.commandHistory.push_back(ch);
			Return();
		}
		if (GetAsyncKeyState(0x08)) // Backspace-key
		{
			std::cout << "Remove which commands" << std::endl;
			removing = true;
		}

		if (GetAsyncKeyState(0x58)) // X-key
		{
			mc.undo();
			std::cout << "Undo" << std::endl;
		}
		if (GetAsyncKeyState(0x59)) // Y-key
		{
			mc.redo();
			std::cout << "Redo" << std::endl;
		}

		if (GetAsyncKeyState(0x41) < 0) // A-key
		{
			mc.add(buttonA);
			std::cout << "Added A" << std::endl;
		}
		if (GetAsyncKeyState(0x53) < 0) // S-key
		{
			mc.add(buttonS);
			std::cout << "Added S" << std::endl;
		}
		if (GetAsyncKeyState(0x44) < 0) // D-key
		{
			mc.add(buttonD);
			std::cout << "Added D" << std::endl;
		}
		if (GetAsyncKeyState(0x57) < 0) // W-key
		{
			mc.add(buttonW);
			std::cout << "Added W" << std::endl;
		}

		if (GetAsyncKeyState(0x26) < 0) // Up-key
		{
			mc.add(buttonUp);
			std::cout << "Added UP" << std::endl;
		}
		if (GetAsyncKeyState(0x28) < 0) // Down-key
		{
			mc.add(buttonDown);
			std::cout << "Added DOWN" << std::endl;
		}
		if (GetAsyncKeyState(0x25) < 0) // Left-key
		{
			mc.add(buttonLeft);
			std::cout << "Added LEFT" << std::endl;
		}
		if (GetAsyncKeyState(0x27) < 0) // Right-key
		{
			mc.add(buttonRight);
			std::cout << "Added RIGHT" << std::endl;
		}
		if (GetAsyncKeyState(0x20) < 0) // Space-key
		{
			mc.add(buttonSpace);
			std::cout << "Added SPACE" << std::endl;
		}
	}
	void Remove()
	{
		if (GetAsyncKeyState(0x41) < 0) // A-key
		{
			mc.remove(buttonA);
			removing = false;
			macroCommands = true;
			std::cout << "Remove A" << std::endl;
		}
		if (GetAsyncKeyState(0x53) < 0) // S-key
		{
			mc.remove(buttonS);
			removing = false;
			macroCommands = true;
			std::cout << "Remove S" << std::endl;
		}
		if (GetAsyncKeyState(0x44) < 0) // D-key
		{
			mc.remove(buttonD);
			removing = false;
			macroCommands = true;
			std::cout << "Remove D" << std::endl;
		}
		if (GetAsyncKeyState(0x57) < 0) // W-key
		{
			mc.remove(buttonW);
			removing = false;
			macroCommands = true;
			std::cout << "Remove W" << std::endl;
		}

		if (GetAsyncKeyState(0x26) < 0) // Up-key
		{
			mc.remove(buttonUp);
			removing = false;
			macroCommands = true;
			std::cout << "Remove UP" << std::endl;
		}
		if (GetAsyncKeyState(0x28) < 0) // Down-key
		{
			mc.remove(buttonDown);
			removing = false;
			macroCommands = true;
			std::cout << "Remove DOWN" << std::endl;
		}
		if (GetAsyncKeyState(0x25) < 0) // Left-key
		{
			mc.remove(buttonLeft);
			removing = false;
			macroCommands = true;
			std::cout << "Remove LEFT" << std::endl;
		}
		if (GetAsyncKeyState(0x27) < 0) // Right-key
		{
			mc.remove(buttonRight);
			removing = false;
			macroCommands = true;
			std::cout << "Remove RIGHT" << std::endl;
		}
		if (GetAsyncKeyState(0x20) < 0) // Space-key
		{
			mc.remove(buttonSpace);
			removing = false;
			macroCommands = true;
			std::cout << "Remove SPACE" << std::endl;
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
			if (waiting)
			{
				Waiting();
			}
			if (macroCommands)
			{
				MacroCommands();
			}
			if (remapping)
			{
				Remapping();
			}
			if (mapping)
			{
				Mapping();
			}
		}
		_getch();
	}

private:
	Command* buttonW = new WCommand();
	Command* buttonA = new ACommand();
	Command* buttonS = new SCommand();
	Command* buttonD = new DCommand();

	Command* buttonUp = new UPCommand();
	Command* buttonDown = new DOWNCommand();
	Command* buttonLeft = new LEFTCommand();
	Command* buttonRight = new RIGHTCommand();

	Command* buttonSpace = new SPACECommand();

};
