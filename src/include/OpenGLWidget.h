//
// Created by Глеб Писарев on 30.06.2023.
//

#ifndef VIEWER_SRC_GUI_OPENGLWIDGET_H_
#define VIEWER_SRC_GUI_OPENGLWIDGET_H_

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <string>

#include "ObjectManager.h"

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

 public:
  explicit OpenGLWidget(QWidget* parent = nullptr);
  ~OpenGLWidget() override;
  bool isLines = true;
  bool isPoints = false;
  ObjectManager* object = nullptr;
  void loadDataToBuffers();
  void initModelMatrix();

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void mousePressEvent(QMouseEvent* mouse) override;
  void mouseMoveEvent(QMouseEvent* mouse) override;
  void wheelEvent(QWheelEvent* event) override;

 private:
  GLuint shaderProgram;
  GLuint VAO, VBO, EBO;
  bool isDataLoad = false;
  QPoint mousePosition;
  QMatrix4x4 modelMatrix;
  static std::optional<std::string> getShaderSource(
      const std::string& filename);
  void initBuffers();
  void initShaderProgram();
  void rotateCoordinateSystem(float angle, const QVector3D& axis);
};

#endif  // VIEWER_SRC_GUI_OPENGLWIDGET_H_
