#include "MouseInput.h"

#include <iostream>

double MouseInput::xoffset = 0;
double MouseInput::yoffset = 0;
double MouseInput::lastX = 0;
double MouseInput::lastY = 0;

void MouseInput::mouse_callback(double xpos, double ypos)
{
    xoffset = xpos - lastX;
    yoffset = lastY - ypos;
    // std::cout<<"("<<xoffset<<","<<yoffset<<") ("<<xpos<<","<<ypos<<")"<<std::endl;
    lastX = xpos;
    lastY = ypos;
}

