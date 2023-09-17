//
// Created by Глеб Писарев on 25.07.2023.
//

#ifndef VIEWER_OBJECTMANAGER_H
#define VIEWER_OBJECTMANAGER_H

#include <QtCore>
#include <QtOpenGL>
#include <optional>

extern "C" {
#include "object_controller.h"
}

class ObjectManager {
 public:
  ~ObjectManager();
  bool isEmpty = true;

 public:
  GLfloat* vertexes = nullptr;
  GLuint* polygons = nullptr;
  size_t countOfVertexes = 0;
  size_t countOfPolygons = 0;
  void load(QString filePath);
  void scale(GLfloat k);
  void rotate(GLfloat alpha, GLfloat beta, GLfloat gamma);
  void move(GLfloat dx, GLfloat dy, GLfloat dz);

 private:
  std::optional<object_t> object;
  void arrayCopy(const float* src);
  void arrayCopy(const unsigned* src);
};

#endif  // VIEWER_OBJECTMANAGER_H
