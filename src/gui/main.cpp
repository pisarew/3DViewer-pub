//
// Created by Глеб Писарев on 30.06.2023.
//
#include <QApplication>
#include <QOpenGLWidget>

#include "viewer.h"

int main(int argc, char* argv[]) {
  QSurfaceFormat format;
  format.setVersion(4, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
  QApplication app(argc, argv);
  viewer w;
  w.show();
  return QApplication::exec();
}