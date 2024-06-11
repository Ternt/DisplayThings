#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <util.h>



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
GLuint createShaderProgram(const char* pVSFileName, const char* pFSFileName){

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
