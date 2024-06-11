#include <stdio.h>
#include <iostream>
#include <string>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../imgui/imgui.h" 
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <util.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void LoadShaderSource(const char* shaderText, GLuint shader);
GLuint createShaderProgram();

const unsigned int width = 800; 
const unsigned int height = 600;

static void glfw_error_callback(int error, const char* description){
  fprintf(stderr, "GLFW Error %d: %s\n", error, description);
};

int main() {

  // glfw initialization and configuration
  // -------------------------------------
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    std::cout << "Failed initialization\n";
    return -1;
  }

  // window initialization and configuration
  // --------------------------------------
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // window creation
  GLFWwindow* window = glfwCreateWindow(width, height, "DisplayThings", NULL, NULL);
  if (window == NULL) {
    std::cout << "Error: Failed to create window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSwapInterval(1); // Enable vsync

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Error: Failed to initialize GLAD");
    return -1;
  }

  // Setup ImGui Context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  // Setup OpenGL state machine
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  GLuint program = createShaderProgram();
  unsigned int vertexPositionLocation = glGetAttribLocation(program, "aPos");
  unsigned int vertexColorLocation = glGetAttribLocation(program, "aColor");
  unsigned int transformLocation = glGetUniformLocation(program, "transform");

  std::cout << "[" << vertexPositionLocation << ", " << vertexColorLocation << "]" << std::endl;

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
    // FRONT
    -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f
  };


  unsigned int positionVBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &positionVBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {

    // user input
    // ----------
    processInput(window);

    // rendering commands
    // ------------------
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}


// shader compilation: deals with the creation, compilation, and linking of shader object files to a shader program
// ----------------------------------------------------------------------------------------------------------------
void LoadShaderSource(const char* shaderText, GLuint shaderObject){

  const GLchar* p[1];
  p[0] = shaderText;

  GLint Lengths[1];
  Lengths[0] = (GLint)strlen(shaderText);

  glShaderSource(shaderObject, 1, p, Lengths);
}


// current path is C:\Dev\DisplayThings\DisplayThings
const char* pVSFileName = "src/Window/Shaders/basic.vert";
const char* pFSFileName = "src/Window/Shaders/basic.frag";

// shader compilation and error handling 
// -------------------------------------
GLuint createShaderProgram(){

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  GLint isCompiled = 0;


  std::string vertexSource, fragmentSource;
  if(!ReadFile(pVSFileName, vertexSource)){
    std::cout << "File was not read.\n";
    std::cout << vertexSource;
    exit(1);
  }

  if(!ReadFile(pFSFileName, fragmentSource)){
    std::cout << "File was not read.\n";
    std::cout << fragmentSource;
    exit(1);
  }


  LoadShaderSource(vertexSource.c_str(), vertexShader);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
    // The maxLength includes the NULL character
    char temp[256] = ""; 
    glGetShaderInfoLog(vertexShader, 256, NULL, temp);
    fprintf( stderr, "Compile failed:\n%s\n", temp);

    glDeleteShader(vertexShader);

    exit(1);
  }


  LoadShaderSource(fragmentSource.c_str(), fragmentShader);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
    // The maxLength includes the NULL character
    char temp[256] = ""; 
    glGetShaderInfoLog(fragmentShader, 256, NULL, temp);
    fprintf( stderr, "Compile failed:\n%s\n", temp);

    glDeleteShader(fragmentShader);

    exit(1);
  }



  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);

  glLinkProgram(program);

  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
  if (isLinked == GL_FALSE)
  {
    GLsizei maxLength = 4096;
    GLchar infoLog[4096];
    GLsizei actualLength;

    glGetProgramInfoLog(program, maxLength, &actualLength, infoLog);
    printf("Warning/Error in GLSL shader:\n");
    printf("%s\n",infoLog);

    // We don't need the program anymore.
    glDeleteProgram(program);
    // Don't leak shaders either.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    exit(1); 
  }

  int numAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);
  std::cout << "Maximum number of vertex attributes supported: " << numAttributes << std::endl;

  // Always detach shaders after a successful link.
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);

  return program;
}
