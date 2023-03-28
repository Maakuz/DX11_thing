#pragma once
#include <map>
#include <string>
class Monitor 
{
public:
    Monitor() {};

    void addString(std::string lable, std::string* value);

    void addDragFloat(std::string lable, float* value, float speed = 1, float min = 0.0f, float max = 0.0f);

    void update();
private:
    struct DragFloat
    {
        float* val = nullptr;
        float speed = 1;
        float minVal = 0.f;
        float maxVal = 0.f;

        DragFloat(float* value, float speed = 1, float min = 0.0f, float max = 0.0f) :
            val(value), speed(speed), minVal(min), maxVal(max) {}
    };
    
    std::map<std::string, std::string*> m_monitoredStrings;
    std::map<std::string, DragFloat> m_monitoredDragFloats;
};