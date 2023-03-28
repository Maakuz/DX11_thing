#pragma once
#include <functional>
#include <vector>
#include <string>
#include <DirectXColors.h>
#include <map>
#include "Graphics/ImGui/imgui.h"
#include "Monitor.h"

#define printfCon(format, ...) ConsoleWindow::get().printf(format, __VA_ARGS__)
#define monitorCon ConsoleWindow::get().getMonitor()

typedef std::vector<std::string> Arguments;

struct RetVal
{
	RetVal(bool success, std::string text) :
		success(success),
		text(text) {}

	bool success;
	std::string text;
};

namespace ConsoleColors //TODO: Load from file? 
{
	const ImVec4 error =	{ 1.f,		0.196f, 0.196f, 1.f };
	const ImVec4 command =	{ 0.274f,	1.f,	0.274f, 1.f };
	const ImVec4 info =		{ 1.f,		1.f,	1.f,	1.f };
	const ImVec4 foreign =	{ 1.f,		0.f,	1.f,	1.f };
	const ImVec4 window =	{ 0.f,		0.f,	0.18f,	1.f };
}

class ConsoleWindow
{
public:
	static ConsoleWindow& get()
	{
		static ConsoleWindow s_wind;
		return s_wind;
	}

	void addCommand(std::string commandLine, std::function<RetVal(Arguments args)> func);
	void addStaticCommand(std::string commandLine, std::function<RetVal(Arguments args)> func);
	void print(std::string text);
	void print(const char* format, ...);
	void update();
	void toggleOpen();
	void runCommand(std::string command);

	Monitor& getMonitor() { return m_monitor; }

private:
	struct Command
	{
		std::string commandLine;
		std::function<RetVal(Arguments args)> func;

		Command(std::string commandLine, std::function<RetVal(Arguments args)> func)
		{
			this->commandLine = commandLine;
			this->func = func;
		}

		RetVal operator()(Arguments args)
		{
			return func(args);
		}
	};

	struct LogEntry 
	{
		ImVec4 color;
		std::string text;

		LogEntry(std::string text, ImVec4 color = ConsoleColors::info)
		{
			this->text = text;
			this->color = color;
		}
	};

	Monitor m_monitor;

	std::vector<Command> m_commands;
	std::vector<LogEntry> m_log;
	std::vector<std::string> m_history;


	int m_historyPos;
	bool m_justOpened;
	bool m_open;

	const int LOG_MAX = 3000;



	ConsoleWindow();

	void updateInputBox();
	int textEditCallback(ImGuiInputTextCallbackData* data);
	int textEditCallbackHistory(ImGuiInputTextCallbackData* data);
	int textEditCallbackComplete(ImGuiInputTextCallbackData* data);

	void addLog(std::string string, ImVec4 color = ConsoleColors::info);
	void clearLog();
	void listAllCommands();

	static bool sortByLength(const std::string& a, const std::string& b) 
	{ 
		return a.size() < b.size(); 
	}
	
	static std::string lowercase(std::string str)
	{
		std::string ret;

		for (char& c : str)
			ret += std::tolower(c);

		return ret;
	}

	static bool lowercaseCompare(std::string a, std::string b)
	{
		if (lowercase(a).compare(0, b.size(), lowercase(b)) == 0)
			return true;

		return false;
	}
};

