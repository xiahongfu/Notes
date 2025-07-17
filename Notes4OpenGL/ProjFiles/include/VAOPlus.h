#pragma once
#include <vector>
#include <glad/glad.h>

class VAOPlus
{
public:
    /*
     * stepSize 是一行中，不同Attribute的步长
     */
    VAOPlus(const float* vertices, int size, std::vector<int> stepSize)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

        int total = 0;
        for (int i : stepSize)
        {
            total += i;
        }
        auto stride = sizeof(float) * total;
        int preSize = 0;
        for (int i = 0; i < stepSize.size(); i++)
        {
            glVertexAttribPointer(i, stepSize[i], GL_FLOAT, GL_FALSE, static_cast<int>(stride), (void*)preSize);
            glEnableVertexAttribArray(i);
            preSize += sizeof(float) * stepSize[i];
        }
    }
    void DrawArrays(int count, int first=0, GLenum mode=GL_TRIANGLES)
    {
        glBindVertexArray(VAO);
        glDrawArrays(mode, first, count);
    }

    ~VAOPlus()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
private:
    unsigned int VAO, VBO;
};
