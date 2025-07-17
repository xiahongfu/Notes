#pragma once

class MyTime
{
public:
    static void UpdateTime();
    static float GetDeltaTime();
private:
    static float lastFrame;
    static float deltaTime;
};
