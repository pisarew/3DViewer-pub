//
// Created by Глеб Писарев on 30.06.2023.
//
#include "viewer.h"

#include <QFileDialog>

#include "ui_viewer.h"

viewer::viewer(QWidget* parent) : QMainWindow(parent), ui(new Ui::viewer) {
  ui->setupUi(this);
  object = new ObjectManager;
  connect(ui->pointsCheckBox, &QCheckBox::stateChanged, this,
          &viewer::setPointsState);
  connect(ui->linesCheckBox, &QCheckBox::stateChanged, this,
          &viewer::setLinesState);
}

viewer::~viewer() {
  delete object;
  delete ui;
}

void viewer::openFile(QString filename) {
  object->load(filename);
  ui->filePath->setText(filename);
  ui->vertexesLabel->setText(
      "Вершины: " + QVariant((int)object->countOfVertexes / 3).toString());
  ui->edgesLabel->setText(
      "Ребра: " + QVariant((int)object->countOfPolygons / 2).toString());
  ui->openGL->object = object;
  ui->openGL->initModelMatrix();
  ui->openGL->loadDataToBuffers();
  ui->openGL->update();
}
void viewer::on_open_file_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Выберите файл", "",
                                                  "Wavefront OBJ (*.obj)");
  if (!filename.isEmpty()) openFile(filename);
}
void viewer::on_rotateButton_clicked() {
  if (object->isEmpty) return;
  object->rotate((GLfloat)ui->xRotateSpinBox->value(),
                 (GLfloat)ui->yRotateSpinBox->value(),
                 (GLfloat)ui->zRotateSpinBox->value());
  ui->openGL->loadDataToBuffers();
  ui->openGL->update();
}
void viewer::on_moveButton_clicked() {
  if (object->isEmpty) return;
  object->move((GLfloat)ui->xMoveSpinBox->value(),
               (GLfloat)ui->yMoveSpinBox->value(),
               (GLfloat)ui->zMoveSpinBox->value());
  ui->openGL->loadDataToBuffers();
  ui->openGL->update();
}
void viewer::on_scaleButton_clicked() {
  if (object->isEmpty) return;
  object->scale((GLfloat)ui->scaleSpinBox->value());
  ui->openGL->loadDataToBuffers();
  ui->openGL->update();
}
void viewer::on_plusButton_clicked() {
  if (object->isEmpty) return;
  object->scale(0.9);
  ui->openGL->loadDataToBuffers();
  ui->openGL->update();
}
void viewer::on_minusButton_clicked() {
  if (object->isEmpty) return;
  object->scale(1.15);
  ui->openGL->loadDataToBuffers();
  ui->openGL->update();
}
void viewer::setPointsState(int state) {
  if (state == Qt::Checked)
    ui->openGL->isPoints = true;
  else
    ui->openGL->isPoints = false;
  ui->openGL->update();
}
void viewer::setLinesState(int state) {
  if (state == Qt::Checked)
    ui->openGL->isLines = true;
  else
    ui->openGL->isLines = false;
  ui->openGL->update();
}
