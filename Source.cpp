#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	
	if (!glfwInit()) {
		printf("Failed initialization");
		return -1;
	}
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	scanf_s("waiting for input: ");
	glfwTerminate();
	return 0;
}

