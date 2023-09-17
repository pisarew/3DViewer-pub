//
// Created by Глеб Писарев on 30.06.2023.
//

#ifndef VIEWER_SRC_GUI_VIEWER_H_
#define VIEWER_SRC_GUI_VIEWER_H_

#include <QMainWindow>

#include "ObjectManager.h"

namespace Ui {
class viewer;
}

class viewer : public QMainWindow {
  Q_OBJECT

 public:
  explicit viewer(QWidget* parent = nullptr);
  ~viewer();

 private slots:
  void on_open_file_clicked();
  void on_rotateButton_clicked();
  void on_moveButton_clicked();
  void on_scaleButton_clicked();
  void on_plusButton_clicked();
  void on_minusButton_clicked();
  void setPointsState(int state);
  void setLinesState(int state);

 private:
  Ui::viewer* ui;
  ObjectManager* object;
  void openFile(QString filename);
};

#endif  // VIEWER_SRC_GUI_VIEWER_H_
