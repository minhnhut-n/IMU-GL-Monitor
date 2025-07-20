#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include "main.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

//For close window by escape button
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void) {

    //Init openGL frameWork
    if(!glfwInit()) {
        cout << "frame work not be inited" << endl;
        return -1;
    }

    //Create a window 
    GLFWwindow* wd = glfwCreateWindow(800, 600, "IMU SERVICES", nullptr, nullptr);
    if(!wd) {
        cout << "Create window is failed!" <<endl;
        return -1;
    }

    glfwMakeContextCurrent(wd);
    glewExperimental = GL_TRUE;
    glViewport(0, 0, 800, 600);
    //Init GL Extension WWrangler (this is lib for working with modern GL fw)
    if(glewInit() != GLEW_OK) {
        cout << "Failed to init GLEW!!" <<endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    /* Lưu trữ dữ liệu đỉnh (vertex data) như tọa độ, màu sắc, vector pháp tuyến...
    trực tiếp trong bộ nhớ của GPU thay vì CPU*/
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Infinitive loop ( Main render loop )
    while(!glfwWindowShouldClose(wd)) {
        /* FOR ESCAPE WINDOW BY "ESC" BUTTON */
        processInput(wd);



        /* BODY CODE */
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader
        glUseProgram(shaderProgram);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 mvp = proj * view * model;

        int mvpLoc = glGetUniformLocation(shaderProgram, "mvp");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        /* END BODY CODE */




        /*DO NOT CHANGE ANYTHING BELOW CODE*/
        //Background finished load data into buffer => Swap back to front to \
        show data for User
        glfwSwapBuffers(wd);
        //Poll event is used to take input event from User such as terminate \
        window
        glfwPollEvents();
    }

    return 0;
}