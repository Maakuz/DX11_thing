#include "ConsoleWindow.h"
#include <stdarg.h>
#include <algorithm>
#include <sstream>
#include "Graphics/ImGui/imgui_stdlib.h"

ConsoleWindow::ConsoleWindow()
{
    m_historyPos = -1;
    m_open = false;
    m_justOpened = false;

    addCommand("clearLog", [&](Arguments args)->RetVal
        {
            this->clearLog();
            return RetVal(true, "Log cleared!");
        });

    addCommand("listAll", [&](Arguments args)->RetVal
        {
            listAllCommands();
            return RetVal(true, "Commands listed.");
        });
    
    addLog("Use command \"listAll\" for a list of all available commands.");
}

void ConsoleWindow::updateInputBox()
{
    ImGuiInputTextCallback callback = [](ImGuiInputTextCallbackData* data)->int
    {
        ConsoleWindow* wind = (ConsoleWindow*)data->UserData;
        return wind->textEditCallback(data);
    };

    static std::string input = "";
    if (ImGui::InputText("command", &input,
        ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue |
        ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackHistory |
        ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackCompletion |
        ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackAlways
        , callback, (void*)this))
    {
        if (input[0])
            runCommand(input);

        input = "";
        m_historyPos = -1;
        ImGui::SetKeyboardFocusHere(-1);
    }
}

int ConsoleWindow::textEditCallback(ImGuiInputTextCallbackData* data)
{
    switch (data->EventFlag)
    {
    case ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackHistory:
        textEditCallbackHistory(data);
        break;

    case ImGuiInputTextFlags_::ImGuiInputTextFlags_CallbackCompletion:
        textEditCallbackComplete(data);
        break;
    }

    return 0;
}

int ConsoleWindow::textEditCallbackHistory(ImGuiInputTextCallbackData* data)
{
    int l_prevPos = m_historyPos;

    if (data->EventKey == ImGuiKey_UpArrow)
    {
        m_historyPos++;
        if (m_historyPos >= m_history.size())
            m_historyPos--;
    }

    else
    {
        m_historyPos--;
        if (m_historyPos < 0)
            m_historyPos = -1;
    }

    if (l_prevPos != m_historyPos)
    {
        int l_length = 0;
        if (m_historyPos == -1)
            l_length = snprintf(data->Buf, size_t(data->BufSize), "");
        
        else
            l_length = snprintf(data->Buf, size_t(m_history[m_historyPos].capacity() + 1), "%s", m_history[m_historyPos].c_str());

        data->BufTextLen = l_length;
        data->SelectionEnd = l_length;
        data->SelectionStart = l_length;
        data->CursorPos = l_length;
        data->BufDirty = true;
    }

    return 0;
}

int ConsoleWindow::textEditCallbackComplete(ImGuiInputTextCallbackData* data)
{
    std::string l_compare = data->Buf;

    if (l_compare.empty())
    {
        this->addLog("Too many results.");
        addLog("Use command \"listAll\" for a list of all available commands.");
        return 0;
    }

    std::vector<std::string> l_possibilities;

    for (const Command& commandLine : m_commands)
    {
        if (lowercaseCompare(commandLine.commandLine, l_compare))
            l_possibilities.push_back(commandLine.commandLine);
    }

    if (l_possibilities.empty())
    {
        this->addLog("No matches found :(");
        return 0;
    }

    std::sort(l_possibilities.begin(), l_possibilities.end(), sortByLength);

    this->addLog("Possibilities: ");
    for (std::string& possibility : l_possibilities)
        this->addLog("- " + possibility);

    bool l_branching = false;
    std::string l_completion = "";
    for (size_t i = 0; i < l_possibilities[0].size() && !l_branching; i++)
    {
        l_completion += l_possibilities[0][i];
        for (size_t j = 0; j < l_possibilities.size() && !l_branching; j++)
        {
            if (!lowercaseCompare(l_possibilities[j], l_completion))
                l_branching = true;
        }
    }

    if (l_branching)
        l_completion.erase(l_completion.end() -1);

    int l_length = snprintf(data->Buf, l_completion.capacity() + 1, "%s", l_completion.c_str());

    data->BufTextLen = l_length;
    data->SelectionEnd = l_length;
    data->SelectionStart = l_length;
    data->CursorPos = l_length;
    data->BufDirty = true;

    return 0;
}

void ConsoleWindow::addLog(std::string string, ImVec4 color)
{
    if (m_log.size() >= LOG_MAX)
        m_log.erase(m_log.begin());

    m_log.push_back(LogEntry(string, color));
}

void ConsoleWindow::clearLog()
{
    m_log.clear();
}

void ConsoleWindow::listAllCommands()
{
    this->addLog("Listing all commands:", ConsoleColors::info);

    for (Command& command : m_commands)
        this->addLog(command.commandLine, ConsoleColors::info);
}

void ConsoleWindow::addCommand(std::string commandLine, std::function<RetVal(Arguments args)> func)
{
    bool l_found = false;

    for (Command& command : m_commands)
    {
        if (command.commandLine == commandLine)
        {
            l_found = true;
        }
    }
    if (l_found)
        this->addLog("Command \"" + commandLine + "\" already exists.");

    else
        m_commands.push_back(Command(commandLine, func));
}

void ConsoleWindow::addStaticCommand(std::string commandLine, std::function<RetVal(Arguments args)> func)
{
    for (Command& command : m_commands)
        if (command.commandLine == commandLine)
            return;

    m_commands.push_back(Command(commandLine, func));
    this->addLog("Static command \"" + commandLine + "\" added!");   
}

void ConsoleWindow::print(std::string text)
{
    addLog(text, ConsoleColors::foreign);
}

void ConsoleWindow::print(const char* format, ...)
{
    va_list l_listPtr;
    va_start(l_listPtr, format);
    std::string l_str = format;
    std::string l_finalStr;

    for (int i = 0; i < l_str.size(); i++)
    {
        if (l_str[i] == '%')
        {
            char l_type = l_str[++i];

            switch (l_type)
            {
            case 'f':
                l_finalStr += std::to_string(va_arg(l_listPtr, double));
                    break;

            case 'd':
                l_finalStr += std::to_string(va_arg(l_listPtr, int));
                break;

            case 'b':
                l_finalStr += std::to_string(va_arg(l_listPtr, bool));
                break;

            case 's':
                l_finalStr += va_arg(l_listPtr, const char*);
                break;

            default:
                break;
            }
        }

        else
            l_finalStr += l_str[i];
    }

    va_end(l_listPtr);
    print(l_finalStr);
}

void ConsoleWindow::update()
{
    const float FOOTER_HEIGHT_TO_RESERVE = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

    ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_WindowBg, ConsoleColors::window);
    ImGui::Begin("Conesole", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize);
    ImGui::SetWindowSize(ImVec2(500, 600));

    ImVec2 l_windowPos = ImGui::GetWindowPos();

    if (ImGui::BeginTabBar("TurboConsole"))
    {
        if (ImGui::BeginTabItem("Console"))
        {
            //LOG
            ImGui::BeginChild("Log", ImVec2(0, -FOOTER_HEIGHT_TO_RESERVE), false);
            for (int i = 0; i < m_log.size(); i++)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, m_log[i].color);
                ImGui::TextWrapped("%d: %s\n", i, m_log[i].text.c_str());
                ImGui::PopStyleColor();
            }

            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.f);
            
            ImGui::EndChild();
            ImGui::Separator();

            if (m_justOpened)
            {
                m_justOpened = false;
                ImGui::SetKeyboardFocusHere();
            }

            this->updateInputBox();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Monitor"))
        {
            m_monitor.update();

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Color"))
        {
            static float col[4] = { 0, 0, 0, 200 };
            if (ImGui::ColorEdit4("Window", col))
            {
                //colors.windowColor = ImVec4(col[0] * 255, col[1] * 255, col[2] * 255, col[3] * 255);
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }



    ImGui::End();
    ImGui::PopStyleColor();
}

void ConsoleWindow::toggleOpen()
{
    if (!m_open)
    {
        m_justOpened = true;
    }

    m_open = !m_open;
}

void ConsoleWindow::runCommand(std::string command)
{
    std::string l_actualCommand;
    std::vector<std::string> l_args;
    std::stringstream l_stream(command);

    l_stream >> l_actualCommand;

    int l_commandIndex = -1;
    for (size_t i = 0; i < m_commands.size() && l_commandIndex == -1; i++)
    {
        if (l_actualCommand == m_commands[i].commandLine)
            l_commandIndex = i;
    }

    if (l_commandIndex != -1)
    {
        while (!l_stream.eof())
        {
            std::string l_temp = "";
            l_stream >> l_temp;
            l_args.push_back(l_temp);
        }

        auto l_iterator = m_history.begin();
        while (l_iterator != m_history.end())
        {
            if (l_iterator->compare(command) == 0)
                l_iterator = m_history.erase(l_iterator);

            else
                ++l_iterator;
        }

        //Todo: limit when it becomes an issue
        m_history.insert(m_history.begin(), command);

        RetVal l_ret = m_commands[l_commandIndex](l_args);

        addLog(command + " - " + l_ret.text, l_ret.success ? ConsoleColors::command : ConsoleColors::error);
    }

    else
    {
        addLog("Command \"" + l_actualCommand + "\" not found.", ConsoleColors::error);
    }
}