#include <stdio.h>
#include <iostream>
#include <string>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <util.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void LoadShaderSource(const char* shaderText, GLuint shader);
void createShaderProgram();

const unsigned int width = 800; 
const unsigned int height = 600;

int main() {
	// glfw initialization and configuration
	// -------------------------------------
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

  // glad: load all OpenGL function pointers
  // ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Error: Failed to initialize GLAD");
		return -1;
	}


	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

  createShaderProgram();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {

		// user input
    // ----------
		processInput(window);

		// rendering commands
    // ------------------
		glClear(GL_COLOR_BUFFER_BIT);

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



const char* pVSFileName = "basic.vert";
const char* pFSFileName = "basic.frag";

// shader compilation and error handling 
// -------------------------------------
void createShaderProgram(){

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

  // Always detach shaders after a successful link.
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);

  glUseProgram(program);
}
