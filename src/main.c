#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

const int WIDTH = 1152;
const int HEIGHT = 648;

float vertices[] = {
   1.0f,  1.0f,
   1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f,  1.0f,
};
unsigned int indices[] = {
  0, 1, 3,
  1, 2, 3,
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int load_shaders(const char *vertex_path, const char *fragment_path);

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "bg-gen", NULL, NULL);
  if (window == NULL)
  {
    fprintf(stderr, "Could not create window window.\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);

  glViewport(0, 0, WIDTH, HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  unsigned int vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  int shader = load_shaders("shaders/background.vertex.glsl", "shaders/background.fragment.glsl");
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    glfwPollEvents();

    glUseProgram(shader);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

int load_shaders(const char *vertex_path, const char *fragment_path)
{
  int success;
  char info_log[512];

  FILE *vertex_file = fopen(vertex_path, "r");
  fseek(vertex_file, 0, SEEK_END);
  long vertex_file_size = ftell(vertex_file);
  fseek(vertex_file, 0, SEEK_SET);

  char *vertex_str = calloc(vertex_file_size + 1, sizeof(char));
  fread(vertex_str, vertex_file_size, 1, vertex_file);
  fclose(vertex_file);

  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_str, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
    fprintf(stderr, "Could not compile vertex shader\n%s", info_log);
    return 0;
  }

  FILE *fragment_file = fopen(fragment_path, "r");
  fseek(fragment_file, 0, SEEK_END);
  long fragment_file_size = ftell(fragment_file);
  fseek(fragment_file, 0, SEEK_SET);

  char *fragment_str = calloc(fragment_file_size + 1, sizeof(char));
  fread(fragment_str, fragment_file_size, 1, fragment_file);
  fclose(fragment_file);

  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_str, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
    fprintf(stderr, "Could not compile fragment shader\n%s", info_log);
    return 0;
  }

  unsigned int id = glCreateProgram();
  glAttachShader(id, vertex_shader);
  glAttachShader(id, fragment_shader);
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(id, 512, NULL, info_log);
    fprintf(stderr, "Could not link shader\n%s", info_log);
    return 0;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  free(vertex_str);
  free(fragment_str);

  return id;
}
