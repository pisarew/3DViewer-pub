//
// Created by Глеб Писарев on 25.07.2023.
//

#include "ObjectManager.h"

ObjectManager::~ObjectManager() {
  if (countOfVertexes) delete[] vertexes;
  if (countOfPolygons) delete[] polygons;
  if (object) free_object(&(*object));
}

void ObjectManager::arrayCopy(const float *src) {
  for (size_t i = 0; i < countOfVertexes; ++i) {
    vertexes[i] = (GLfloat)src[i];
  }
}
void ObjectManager::arrayCopy(const unsigned *src) {
  for (size_t i = 0; i < countOfPolygons; ++i) {
    polygons[i] = (GLuint)src[i];
  }
}
void ObjectManager::load(QString filePath) {
  if (countOfVertexes) delete[] vertexes;
  if (countOfPolygons) delete[] polygons;
  if (!object)
    object.emplace();
  else
    free_object(&(*object));
  if (get_object(filePath.toStdString().c_str(), &(*object)) != 0) return;
  countOfVertexes = object->count_of_vertexes;
  vertexes = new GLfloat[countOfVertexes];
  countOfPolygons = object->count_of_facets;
  polygons = new GLuint[countOfPolygons];
  arrayCopy(object->vertexes);
  arrayCopy(object->polygons);
  isEmpty = false;
}
void ObjectManager::scale(GLfloat k) {
  scale_object(&(*object), k);
  arrayCopy(object->vertexes);
}
void ObjectManager::rotate(GLfloat alpha, GLfloat beta, GLfloat gamma) {
  rotate_object(&(*object), alpha, beta, gamma);
  arrayCopy(object->vertexes);
}
void ObjectManager::move(GLfloat dx, GLfloat dy, GLfloat dz) {
  move_object(&(*object), dx, dy, dz);
  arrayCopy(object->vertexes);
}
