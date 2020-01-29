#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * BELOW IS A BUNCH OF HELPER CODE
 * You do not need to understand what is going on with it, but if you want to
 * know, let me know and I can walk you through it.
 */

class Vector5 {
public:
    float x;
    float y;
    float r;
    float b;
    float g;

    Vector5() : x(0), y(0), r(0), g(0), b(0) {
        //initializes a Vector5 set to 0,0,0
        //std::cout << "in Vector5 default constructor" << std::endl;
    }

    // Constructor
    Vector5(float xx, float yy, float rr, float gg, float bb) : x(xx), y(yy), r(rr), g(gg), b(bb) {
        // nothing to do here as we've already initialized x, y, r, g, and b above
        //std::cout << "in Vector5 constructor" << std::endl;
    }

    // Destructor - called when an object goes out of scope or is destroyed
    ~Vector5() {
        // this is where you would release resources such as memory or file descriptors
        // in this case we don't need to do anything
        //std::cout << "in Vector5 destructor" << std::endl;
    }

};

//overloads the << operator to print out a Vector3 object
std::ostream& operator<<(std::ostream& stream, const Vector5& v) {
    stream << v.x << ", " << v.y << ": " << v.r << ", " << v.g << ", " << v.b;
    return stream;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* initWindow() {
    GLFWwindow* window;
    if (!glfwInit()) {
        return NULL;
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    window = glfwCreateWindow(640, 480, "Lab 2", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    if (!gladLoadGL()) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return NULL;
    }

    return window;
}

std::string shaderTypeName(GLenum shaderType) {
    switch(shaderType) {
        case GL_VERTEX_SHADER: return "VERTEX";
        case GL_FRAGMENT_SHADER: return "FRAGMENT";
        default: return "UNKNOWN";
    }
}

std::string readFile(const std::string& fileName) {
    std::ifstream stream(fileName);
    std::stringstream buffer;
    buffer << stream.rdbuf();

    std::string source = buffer.str();
    std::cout << "Source:" << std::endl;
    std::cout << source << std::endl;

    return source;
}

/** END OF CODE THAT YOU DON'T NEED TO WORRY ABOUT */

GLuint createShader(const std::string& fileName, GLenum shaderType) {
    std::string source = readFile(fileName);
    const char* src_ptr = source.c_str();

    /** YOU WILL ADD CODE STARTING HERE */
    // create the shader using
    // glCreateShader, glShaderSource, and glCompileShader
    GLuint shader = 0;
    //checks what type of shader that needs to be made
    if(shaderType == GL_VERTEX_SHADER) {
        shader = glCreateShader(GL_VERTEX_SHADER);
    } else {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    glShaderSource(shader, 1, &src_ptr, NULL);
    glCompileShader(shader);
    /** END CODE HERE */

    // Perform some simple error handling on the shader
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::" << shaderTypeName(shaderType)
            <<"::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    return shader;
}

GLuint createShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
    /** YOU WILL ADD CODE STARTING HERE */
    // create the program using glCreateProgram, glAttachShader, glLinkProgram
    GLuint program = 0;
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    /** END CODE HERE */

    // Perform some simple error handling
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::COMPILATION_FAILED\n"
            << infoLog << std::endl;
    }

    return program;
}

void w2nd(Vector5& vector) {
    //overwrites the x and y values to the normalized versions
    //assumes that the height and width don't change
    vector.x = -1 + (vector.x*((2.0)/640.0));
    vector.y = 1 - (vector.y*((2.0)/480.0));
}

int main(void) {
    GLFWwindow* window = initWindow();
    if (!window) {
        std::cout << "There was an error setting up the window" << std::endl;
        return 1;
    }

    /** YOU WILL ADD DATA INITIALIZATION CODE STARTING HERE */
    /* PART1: ask the user for coordinates and colors, and convert to normalized
     * device coordinates */
    // convert the triangle to an array of floats containing
    // normalized device coordinates and color components.
    // float triangle[] = ...
    Vector5* v_array = new Vector5[3];  //
    std::cout << "Enter 3 points (enter a point as x,y:r,g,b):" << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cin >> v_array[i].x >> v_array[i].y >> v_array[i].r >> v_array[i].g >> v_array[i].b;
        w2nd(v_array[i]);   //sends the vectors to be normalized
    }
    //sets up the array that contains the normalized vertex points and the rgb values
    float triangle[] = {v_array[0].x, v_array[0].y, v_array[0].r, v_array[0].g, v_array[0].b,
                        v_array[1].x, v_array[1].y, v_array[1].r, v_array[1].g, v_array[1].b,
                        v_array[2].x, v_array[2].y, v_array[2].r, v_array[2].g, v_array[2].b,};

    delete [] v_array; //deletes the input vectors
    /** PART2: map the data
    // create vertex and array buffer objects using
    // glGenBuffers, glGenVertexArrays
    GLuint VBO[1], VAO[1];
    // setup triangle using glBindVertexArray, glBindBuffer, GlBufferData
    // setup the attribute pointer for the coordinates
    // setup the attribute pointer for the colors
    // both will use glVertexAttribPointer and glEnableVertexAttribArray;
    */
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(2, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    /** PART3: create the shader program */
    // create the shaders
    // YOU WILL HAVE TO ADD CODE TO THE createShader FUNCTION ABOVE
    GLuint vertexShader = createShader("../vert.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader("../frag.glsl", GL_FRAGMENT_SHADER);
    // create the shader program
    // YOU WILL HAVE TO ADD CODE TO THE createShaderProgram FUNCTION ABOVE
    GLuint shaderProgram = createShaderProgram(vertexShader, fragmentShader);
    // cleanup the vertex and fragment shaders using glDeleteShader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    /** END INITIALIZATION CODE */

    while (!glfwWindowShouldClose(window)) {
        // you don't need to worry about processInput, all it does is listen
        // for the escape character and terminate when escape is pressed.
        processInput(window);

        /** YOU WILL ADD RENDERING CODE STARTING HERE */
        /** PART4: Implemting the rendering loop */
        // clear the screen with your favorite color using glClearColor
        glClearColor(0.2, 0.6, 0.7, 1.0);   //a light bluish color
        glClear(GL_COLOR_BUFFER_BIT);
        // set the shader program using glUseProgram
        glUseProgram(shaderProgram);
        // bind the vertex array using glBindVertexArray
        glBindVertexArray(VAO);
        // draw the triangles using glDrawArrays
        glDrawArrays(GL_TRIANGLES, 0, 3);
        /** END RENDERING CODE */

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
