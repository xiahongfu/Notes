#include "Time.h"

#include <GLFW/glfw3.h>

float MyTime::lastFrame = 0.0f;
float MyTime::deltaTime = 0.0f;


void MyTime::UpdateTime(){
    // float nowtime = static_cast<float>(glfwGetTime());
    // deltaTime = nowtime - lastFrame;
    // lastFrame = nowtime;
}
float MyTime::GetDeltaTime()
{
    return deltaTime;
}