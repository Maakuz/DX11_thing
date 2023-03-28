#include "Monitor.h"
#include "Graphics/ImGui/imgui.h"
#include "Graphics/ImGui/imgui_stdlib.h"

void Monitor::addString(std::string lable, std::string* value)
{
    m_monitoredStrings.insert_or_assign(lable, value);
}

void Monitor::addDragFloat(std::string lable, float* value, float speed, float min, float max)
{
    m_monitoredDragFloats.insert_or_assign(lable, DragFloat(value, speed, min, max));
}

void Monitor::update()
{
    for (auto& item : m_monitoredStrings)
    {
        ImGui::InputText(item.first.c_str(), item.second);
    }

    for (auto& item : m_monitoredDragFloats)
    {
        ImGui::DragFloat(item.first.c_str(), item.second.val, item.second.speed, item.second.minVal, item.second.maxVal);
    }
}
