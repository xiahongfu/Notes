#pragma once
#include <GLFW/glfw3.h>
class MouseInput
{
public:
    static double xoffset;
    static double yoffset;
    static double lastX;
    static double lastY;
    static void mouse_callback(double xpos, double ypos);
};