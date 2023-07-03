#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void drawSierpinskiTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int depth, unsigned int shader);
void setUniform(unsigned int shader);

glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
glm::mat4 rotationMatrix = glm::mat4(1.0f);
glm::vec3 translationVector(0.0f, 0.0f, 0.0f);


const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    uniform mat4 scaleMatrix;
    uniform mat4 rotationMatrix;
    uniform vec3 translationVector;

    void main()
    {
        gl_Position = scaleMatrix * rotationMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0) + vec4(translationVector, 1.0);
    }
)";

const char* backgroundShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(.762f, 0.81310f, 01.0f, 1.0f);
    }
)";
const char* fragmentShaderSource1 = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(.2f, 0.310f, 0.610f, 1.0f);
    }
)"; 

const char* fragmentShaderSource2 = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(.2f, 0.310f, 0.2610f, 1.0f);;
    }
)";


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Sierpinski's Triangle", NULL, NULL);
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    unsigned int backgroundShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(backgroundShader, 1, &backgroundShaderSource, NULL);
    glCompileShader(backgroundShader);

    unsigned int backgroundProgram = glCreateProgram();
    glAttachShader(backgroundProgram, vertexShader);
    glAttachShader(backgroundProgram, backgroundShader);
    glLinkProgram(backgroundProgram);



    unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);

    unsigned int shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShader1);
    glLinkProgram(shaderProgram1);

    unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);

    unsigned int shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    glLinkProgram(shaderProgram2);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
    

    float vertices[] = {
       -1.0f, -1.0f , 0.0f , // bottom left  
        01.0f , -01.0f, 0.0f, // bottom right 
        01.0f ,  01.0f , 0.0f,   // top right
        -01.0f ,  01.0f , 0.0f   // top left
    };

    unsigned int indices[] = {
        0,1,2,
        0,2,3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //u
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    float verticesTri[] = {
       -1.0f, 1.0f , 0.0f , // bottom left  
        01.0f , 01.0f, 0.0f, // bottom right 
        0.0f ,  -01.0f , 0.0f,   // top right
        
    };

    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);

    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTri), verticesTri, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    

    while (!glfwWindowShouldClose(window))
    {
        scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
        rotationMatrix = glm::mat4(1.0f);
        translationVector = glm::vec3(0.0f, 0.0f, 0.0f);


        processInput(window);

        //glClearColor(0.12f, 0.3f, 0.53f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        //Background
        glUseProgram(backgroundProgram);

        setUniform(backgroundProgram);
        

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        

        //Base Triangle
        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO3);
        

        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));

        setUniform(shaderProgram1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //First Division
        rotationMatrix = glm::mat4(1.0f);

        glUseProgram(shaderProgram1);


        scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.50f));
        translationVector = glm::vec3(0.0f, -1.0f, 0.0f);
        setUniform(shaderProgram2);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //Loop to make more tiers


        //Loop to make bottom left tiers
        
   
        glfwSwapBuffers(window);
        glfwPollEvents();
        glfwWaitEventsTimeout(5.0);
        
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void setUniform(unsigned int shader) {

    glUniformMatrix4fv(glGetUniformLocation(shader, "scaleMatrix"), 1, GL_FALSE, glm::value_ptr(scaleMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader, "rotationMatrix"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
    glUniform3fv(glGetUniformLocation(shader, "translationVector"), 1, glm::value_ptr(translationVector));


}


void drawSierpinskiTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int depth, unsigned int shader)
{
    if (depth == 0)
    {
        
    }
    else
    {
        float x12 = (x1 + x2) / 2.0f;
        float y12 = (y1 + y2) / 2.0f;
        float x23 = (x2 + x3) / 2.0f;
        float y23 = (y2 + y3) / 2.0f;
        float x31 = (x3 + x1) / 2.0f;
        float y31 = (y3 + y1) / 2.0f;

        float yShift = -0.5f;
        float xShift = -1.0f;

        
        scaleMatrix = glm::scale(scaleMatrix, glm::vec3(0.50f));
        yShift -= pow(0.25, depth);
        xShift *= pow(1.5, depth);
        translationVector = glm::vec3(xShift, yShift, 0.0f);

        setUniform(shader);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        



        drawSierpinskiTriangle(x1, y1, x12, y12, x31, y31, depth - 1, shader);
        drawSierpinskiTriangle(x12, y12, x2, y2, x23, y23, depth - 1, shader);
        drawSierpinskiTriangle(x31, y31, x23, y23, x3, y3, depth - 1, shader);
    }
}
