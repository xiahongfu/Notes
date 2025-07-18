#if false
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "camera.h"
#include "MyTime.h"
#include "MouseInput.h"
#include "Shader.h"
#include "VAOPlus.h"
using namespace std;

GLFWwindow* window;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
MouseInput mouse;
float vertices[] = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == nullptr)
    {
        cout << ">>Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << ">>Failed to initialize GLAD" << endl;
        return -1;
    }
    
    Shader shader("./src/illumination/01Color/shader.vs", "./src/illumination/01Color/shader.fs");
    Shader lightShader("./src/illumination/01Color/light.vs", "./src/illumination/01Color/light.fs");
    shader.use();

    VAOPlus VAO{vertices, sizeof(vertices), vector<int>{3}};

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        MyTime::UpdateTime();
        camera.processCameraInput(window);
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 绘制物体
        {
            glm::mat4 model, view, projection;
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            view = camera.GetViewMatrix();
            projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

            shader.use();
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
            glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
            shader.setVec3("objectColor", objectColor);
            shader.setVec3("lightColor", lightColor);
            VAO.DrawArrays(36);
        }

        // 绘制灯
        {
            glm::mat4 model, view, projection;
            model = glm::translate(model, glm::vec3(3.0f, 3.0f, -3.0f));
            model = glm::scale(model, glm::vec3(0.2f));
            view = camera.GetViewMatrix();
            projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

            lightShader.use();
            lightShader.setMat4("model", model);
            lightShader.setMat4("view", view);
            lightShader.setMat4("projection", projection);
            glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
            glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
            lightShader.setVec3("objectColor", objectColor);
            lightShader.setVec3("lightColor", lightColor);
            VAO.DrawArrays(36);
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    shader.deleteProgram();
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool firstFrame = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    MouseInput::mouse_callback(xpos, ypos);
    if (!firstFrame) camera.ProcessMouseMovement(mouse.xoffset, mouse.yoffset);
    firstFrame = false;
}
#endif
