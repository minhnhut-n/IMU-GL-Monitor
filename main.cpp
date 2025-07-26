/***
 * Maker: minhnhut.n
 * Date: 26/7/2025
 * Name: Cube static displat with openGL
 * Docs: https://stepby2step.blogspot.com/2025/07/opengl-static-cube-display.html
 */


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

void init_data_for_image(GLuint *vS, GLuint *fS, GLuint *fSoutline, \
                         GLuint *sP, GLuint *sPoutline,\
                         uint *_vao, uint *_vbo) {
    glEnable(GL_DEPTH_TEST);
    // Only need when dynamic motion / have many layout
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_MULTISAMPLE);

    //Create source
        *vS = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(*vS, 1, &vertexShaderSource, nullptr);
        glCompileShader(*vS);

        *fS = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(*fS, 1, &fragmentShaderSource, nullptr);
        glCompileShader(*fS);

        *fSoutline = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(*fSoutline, 1, &fragnentOutlining, nullptr);
        glCompileShader(*fSoutline);
    
    //Using source to create program
        *sP = glCreateProgram();
        glAttachShader(*sP, *vS);
        glAttachShader(*sP, *fS);
        glLinkProgram(*sP);

        *sPoutline = glCreateProgram();
        glAttachShader(*sPoutline, *vS);
        glAttachShader(*sPoutline, *fSoutline);
        glLinkProgram(*sPoutline);

    //Delete source set
        glDeleteShader(*vS);
        glDeleteShader(*fS);
        glDeleteShader(*fSoutline);

    /* Lưu trữ dữ liệu đỉnh (vertex data) như tọa độ, màu sắc, vector pháp tuyến...
    trực tiếp trong bộ nhớ của GPU thay vì CPU*/
        glGenVertexArrays(1, _vao); // Save the ways to draw
        glGenBuffers(1, _vbo);      // Store information about object (color, coordinate,..) into system

        glBindVertexArray(*_vao);               //binding (bond (n)) rang buoc
        glBindBuffer(GL_ARRAY_BUFFER, *_vbo);   //binding (bond (n)) rang buoc
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
}

void draw_solid(GLuint *shaderP, GLuint* __vao) {
    /* BODY CODE */
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use shader
        glUseProgram(*shaderP);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 mvp = proj * view * model;

        int mvpLoc = glGetUniformLocation(*shaderP, "mvp");

        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(*__vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    /* END BODY CODE */
}
void draw_outline(GLuint *shaderPoutline, GLuint* __vao) {
    glUseProgram(*shaderPoutline);

    // Dùng cùng ma trận MVP
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 mvp = proj * view * model;

        int mvpLoc = glGetUniformLocation(*shaderPoutline, "mvp");
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

        glEnable(GL_POLYGON_OFFSET_LINE);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);
        glPolygonOffset(-1.0f, -1.0f);

    //Jump to line mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth(2.0f);

        glBindVertexArray(*__vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    //Back to normal (fill mode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
    
        GLuint vertexShader, fragmentShader, fragmentShaderOutline,\
            shaderProgram, shaderProgramOutline;
        unsigned int VAO, VBO;
        init_data_for_image(&vertexShader, &fragmentShader, &fragmentShaderOutline,\
                            &shaderProgram, &shaderProgramOutline,\
                            &VAO, &VBO);

    //Infinitive loop ( Main render loop )
        while(!glfwWindowShouldClose(wd)) {
        /* FOR ESCAPE WINDOW BY "ESC" BUTTON */
            processInput(wd);



        /* Main function */
        draw_solid(&shaderProgram, &VAO);
        draw_outline(&shaderProgramOutline, &VAO);



        /*DO NOT CHANGE ANYTHING BELOW CODE*/
            //Reproduce geometry image from data have passed to VBO and VAO with purpose\
            - Faster in performance\
            - Multiple draw only with 1 command\
            - Take advance on data store in VAO, VBO
            glDrawArrays(GL_TRIANGLES, 0, 36);

            //Background finished load data into buffer => Swap back to front to \
            show data for User
            glfwSwapBuffers(wd);
            //Poll event is used to take input event from User such as terminate \
            window
            glfwPollEvents();
    }

    return 0;
}