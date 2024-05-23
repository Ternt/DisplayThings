#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <util.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void girlfriendShenanigans();

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
  
  // create a shader object
  // ----------------------
  std::string vertexSource;
  const char* pVSFileName = "src/vert.glsl";

  ReadFile(pVSFileName, vertexSource);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar* source = (const GLchar*) vertexSource.c_str();

  GLint Lengths[1];
  Lengths[0] = (GLint)strlen(source);

  glShaderSource(vertexShader, 1, &source, Lengths);

  // shader compilation and error handling 
  // -------------------------------------
  glCompileShader(vertexShader);
  GLint isCompiled = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
  if(isCompiled == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

    // The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

    // We don't need the shader anymore.
    glDeleteShader(vertexShader);

    // Use the infoLog as you see fit.

    // In this simple program, we'll just leave
    return 0;
  }

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

  girlfriendShenanigans();
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

// gloglog 3000
void girlfriendShenanigans() {
  std::cout << "glog glog\n";
}

