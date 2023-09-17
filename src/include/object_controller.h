//
// Created by Глеб Писарев on 27.05.2023.
//

#ifndef INC_3DVIEWER_SRC_INCLUDE_OBJ_CONVERTER_H_
#define INC_3DVIEWER_SRC_INCLUDE_OBJ_CONVERTER_H_

/**
 * Структура хранит данные о вершинах и полигонах модели в формате Wavefront
 * .obj.
 *
 * @param count_of_vertexes количество координат вершин (тип данных: unsigned).
 * @param count_of_facets количество вершин, которые необходимо соединить (тип
 * данных: unsigned).
 * @param vertexes массив вершин вида x1, y1, z1, x2, y2, z2, ... ,xn, yn, zn
 * (тип данных: float*).
 * @param polygons массив номеров вершин, которые необходимо соединить (тип
 * данных: unsigned*).
 */

typedef struct data {
  unsigned count_of_vertexes;
  unsigned count_of_facets;
  float* vertexes;
  unsigned* polygons;
} object_t;

/**
 * Это функция парсинга объекта из .obj файла в структуру object_t.
 *
 * @param filename путь к файлу .obj, из которого будет прочитана информация о
 * модели (тип данных: const char*).
 * @param dst объект, в который будет записана информация о модели (тип данных:
 * object_t*).
 * @return Код ошибки: OK = 0, ERROR = 1.
 */

int get_object(const char* filename, object_t* dst);

/**
 * Это функция очищает память выделенную под структуру object_t.
 *
 * @param dst объект который необходимо очистить.
 */

void free_object(object_t* obj);

/**
 * Это функция перемещения модели в трехмерном пространстве.
 *
 * @param obj объект, который необходимо переместить (тип данных: object_t*).
 * @param x сдвиг модели по оси x (тип данных: float).
 * @param y сдвиг модели по оси y (тип данных: float).
 * @param z сдвиг модели по оси z (тип данных: float).
 */

void move_object(object_t* obj, float x, float y, float z);

/**
 * Это функция поворота модели в трехмерном пространстве.
 *
 * @param obj объект, который необходимо повернуть (тип данных: object_t*).
 * @param alpha угол поворота модели по оси x (тип данных: float).
 * @param beta угол поворота модели по оси y (тип данных: float).
 * @param gamma угол поворота модели по оси z (тип данных: float).
 */

void rotate_object(object_t* obj, float alpha, float beta, float gama);

/**
 * Это функция масштабирования модели в трехмерном пространстве.
 *
 * @param obj объект, который необходимо масштабировать (тип данных: object_t*).
 * @param k коэффициент масштабирования (тип данных: float).
 */

void scale_object(object_t* obj, float k);

#endif  // INC_3DVIEWER_SRC_INCLUDE_OBJ_CONVERTER_H_
