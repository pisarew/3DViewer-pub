//
// Created by Глеб Писарев on 30.06.2023.
//

#include "OpenGLWidget.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "config.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

OpenGLWidget::~OpenGLWidget() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  modelMatrix.setToIdentity();
  initBuffers();
  initShaderProgram();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.784f, 0.823f, 0.819f, 1.0f);
}
void OpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT);
  if (isDataLoad) {
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_TRUE,
                       modelMatrix.constData());
    glBindVertexArray(VAO);
    if (isLines)
      glDrawElements(GL_LINES, (int)object->countOfPolygons, GL_UNSIGNED_INT,
                     nullptr);
    if (isPoints) glDrawArrays(GL_POINTS, 0, (int)object->countOfVertexes);
    glBindVertexArray(0);
  }
}
std::optional<std::string> OpenGLWidget::getShaderSource(
    const std::string &filename) {
  std::ifstream file(SHADER_PATH + filename);
  if (!file.is_open()) return std::nullopt;
  std::stringstream buf;
  buf << file.rdbuf();
  std::string fileData = buf.str();
  file.close();
  return fileData;
}

void OpenGLWidget::initBuffers() {
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3,
                        (void *)nullptr);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}
void OpenGLWidget::loadDataToBuffers() {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER,
               (int)(sizeof(GLfloat) * object->countOfVertexes),
               object->vertexes, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               (int)(sizeof(unsigned) * object->countOfPolygons),
               object->polygons, GL_STATIC_DRAW);
  isDataLoad = true;
}
void OpenGLWidget::initShaderProgram() {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  std::optional<std::string> stringShaderSourcesCode =
      getShaderSource("point_vertex");
  if (!stringShaderSourcesCode) {
    std::cout << "SHADERS DON'T OPENED" << std::endl;
    return;
  }
  const char *charShaderSourceCode = stringShaderSourcesCode->c_str();
  glShaderSource(vertexShader, 1, &charShaderSourceCode, nullptr);
  glCompileShader(vertexShader);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  stringShaderSourcesCode = getShaderSource("point_fragment");
  if (!stringShaderSourcesCode) {
    return;
  }
  charShaderSourceCode = stringShaderSourcesCode->c_str();
  glShaderSource(fragmentShader, 1, &charShaderSourceCode, nullptr);
  glCompileShader(fragmentShader);
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mouse) {
  mousePosition = mouse->pos();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *mouse) {
  if (mouse->buttons() & Qt::LeftButton) {
    float alpha = float(mouse->pos().y() - mousePosition.y());
    float beta = float(mouse->pos().x() - mousePosition.x());
    rotateCoordinateSystem(alpha, QVector3D(1.0f, 0.0f, 0.0f));
    rotateCoordinateSystem(beta, QVector3D(0.0f, 1.0f, 0.0f));
  } else if (mouse->buttons() & Qt::RightButton) {
    float alpha = float(mouse->pos().x() - mousePosition.x()) * 0.001f;
    float beta = -float(mouse->pos().y() - mousePosition.y()) * 0.001f;
    object->move(alpha, beta, 0.0f);
    loadDataToBuffers();
    update();
  }
  mousePosition = mouse->pos();
}

void OpenGLWidget::rotateCoordinateSystem(float angle, const QVector3D &axis) {
  modelMatrix.rotate(angle, axis);
  update();
}
void OpenGLWidget::wheelEvent(QWheelEvent *event) {
  if (isDataLoad) {
    if (event->angleDelta().y() > 0)
      object->scale(1.1);
    else
      object->scale(0.9);
    loadDataToBuffers();
    update();
  }
  QWidget::wheelEvent(event);
}
void OpenGLWidget::initModelMatrix() { modelMatrix.setToIdentity(); }
