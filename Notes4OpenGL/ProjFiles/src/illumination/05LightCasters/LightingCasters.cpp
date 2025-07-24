#if true
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include "Light.h"
#include "Material.h"
#include "MyTime.h"
#include "MouseInput.h"
#include "Shader.h"
#include "Texture.h"
#include "VAOPlus.h"
using namespace std;

GLFWwindow* window;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
MouseInput mouse;





float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
  };
// 定义光源的属性
glm::vec3 DirectionalLightDirection = glm::vec3(0.0f, -1.0f, -1.0f);
glm::vec3 DirectionalLightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};
glm::vec3 pointLightColors[] = {
    glm::vec3(0.5f, 0.0f, 0.0f),
    glm::vec3(0.5f, 0.0f, 0.0f),
    glm::vec3(0.5f, 0.0f, 0.0f),
    glm::vec3(0.5f, 0.0f, 0.0f)
};
glm::vec3 SpotlightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 SpotlightPosition = glm::vec3(1.0f, 1.0f, 1.0f);


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
    
    Shader shader("./src/illumination/05LightCasters/shader.vs", "./src/illumination/05LightCasters/shader.fs");
    Shader lightShader("./src/illumination/05LightCasters/light.vs", "./src/illumination/05LightCasters/light.fs");

    Texture diffuse("./resource/container2.png", GL_TEXTURE0);
    Texture specular("./resource/container2_specular.png", GL_TEXTURE1);
    Texture emissionMap("./resource/matrix.jpg", GL_TEXTURE2);
    Material material(diffuse, specular, emissionMap);
    // Texture specular("./resource/lighting_maps_specular_color.png", GL_TEXTURE1);
    
    VAOPlus VAO{vertices, sizeof(vertices), vector<int>{3,3,2}};
    VAOPlus VAOLight{vertices, sizeof(vertices), vector<int>{3,3,2}, vector<bool>{true,false,false}};

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    // 定向光源和点光源都不变，放在外面
    shader.use();
    material.SetValue(shader, "material");
    DirectionalLight dirLight(DirectionalLightColor*0.1f, glm::vec3(-1.0f, -1.0f, 0), glm::vec3(0.1f, 0.1f, 0.1f),
    glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));
    dirLight.SetValue(shader, "dirLight");
    for (int i = 0; i < 4; i++)
    {
        PointLight pointLight(pointLightColors[i], pointLightColors[i],
            glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 20.0f);
        pointLight.SetValue(shader, "pointLight["+to_string(i)+"]");
    }
    while (!glfwWindowShouldClose(window))
    {
        MyTime::UpdateTime();
        camera.processCameraInput(window);
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 lightPos = glm::vec3(0, 0, 3.0f);
        glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        Spotlight spotlight(lightColor, lightPos, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.1f, 0.1f, 0.1f),
        glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 10.0f, 12.0f);
        // 绘制物体
        {
            glm::mat4 view, projection;
            // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
            view = camera.GetViewMatrix();
            projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

            for(unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                shader.use();
                shader.setMat4("model", model);
                shader.setMat4("view", view);
                shader.setMat4("projection", projection);
                shader.setVec3("objectColor", objectColor);
                shader.setVec3("viewPos", camera.Position);
                spotlight.SetValue(shader, "spotLight");
                VAO.DrawArrays(36);
            }
            
        }

        // 绘制灯
        {
            for (auto pointLightPosition : pointLightPositions)
            {
                glm::mat4 model, view, projection;
                model = glm::translate(model, pointLightPosition);
                model = glm::scale(model, glm::vec3(0.05f));
                view = camera.GetViewMatrix();
                projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

                lightShader.use();
                lightShader.setMat4("model", model);
                lightShader.setMat4("view", view);
                lightShader.setMat4("projection", projection);
                lightShader.setVec3("lightColor", lightColor);
                VAOLight.DrawArrays(36);
            }
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
