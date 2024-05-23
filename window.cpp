#include <iostream>
#include <string>
#include <vector>

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


  createShaderProgram();

	// render loop
  // -----------
	while (!glfwWindowShouldClose(window)) {

		// user input
    // ----------
		processInput(window);

		// rendering commands
    // ------------------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
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


// shader compilation and error handling 
// -------------------------------------
const char* pVSFileName = "src/basic.vert";
const char* pFSFileName = "src/basic.frag";
void createShaderProgram(){

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  GLint isCompiled = 0;


  std::string vertexSource, fragmentSource;
  if(!ReadFile(pVSFileName, vertexSource)){
    std::cout << "File was not read.\n";
    exit(1);
  }

  if(!ReadFile(pVSFileName, vertexSource)){
    std::cout << "File was not read.\n";
    exit(1);
  }


  LoadShaderSource(vertexSource.c_str(), vertexShader);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

    std::cout << "Vertex shader compiled unsuccessfully.\n";

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

    glDeleteShader(vertexShader);

    exit(1);
  }


  LoadShaderSource(fragmentSource.c_str(), fragmentShader);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

    std::cout << "Fragment shader compiled unsuccessfully.\n";

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

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
    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

    std::cout << "Shaders linked unsuccessfully.\n";

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
