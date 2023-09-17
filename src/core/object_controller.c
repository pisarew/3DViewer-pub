//
// Created by Глеб Писарев on 27.05.2023.
//

#include "object_controller.h"

#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 0
#define ERROR 1
#define NORMALIZE_K 0.6

unsigned factorial(unsigned x) {
  unsigned result = x == 0 ? 1 : x;
  while (x > 1) result = result * --x;
  return result;
}

unsigned get_count_of_numbers(const char *str, size_t len) {
  unsigned count = 0;
  char buf[len];
  strcpy(buf, str);
  char *token;
  strtok(buf, " ");
  while (token = strtok(NULL, " "), token) ++count;
  return count;
}

int get_count_of_v_f(const char *filename, unsigned *vertexes,
                     unsigned *facets) {
  *vertexes = 0;
  *facets = 0;
  FILE *f = fopen(filename, "r");
  size_t len = 0;
  int error = f ? OK : ERROR;
  if (error == OK) {
    char *str = NULL;
    while (getline(&str, &len, f) != -1) {
      if (str[0] == 'v' && str[1] == ' ')
        ++(*vertexes);
      else if (str[0] == 'f' && str[1] == ' ') {
        unsigned count = get_count_of_numbers(str, len);
        *facets += factorial(count) / (factorial(count - 2));
      }
    }
    *vertexes *= 3;
    free(str);
    fclose(f);
  }
  return error;
}

void scale_object(object_t *obj, float k) {
  if (obj) {
    for (unsigned i = 0; i < obj->count_of_vertexes; ++i) obj->vertexes[i] *= k;
  }
}

void normalize(object_t *object, float max_coordinate) {
  if (max_coordinate != 0.0) scale_object(object, NORMALIZE_K / max_coordinate);
}

int write_vertex_to_array(char *str, unsigned *array, unsigned len) {
  int error = OK;
  char *token;
  unsigned buf_array[len];
  strtok(str, " ");
  unsigned i = 0;
  while (token = strtok(NULL, " "), error == OK && token != NULL) {
    char *slash = strchr(token, '/');
    if (slash) *slash = '\0';
    char *point;
    unsigned number_of_vertex = (abs)((int)strtol(token, &point, 10) - 1);
    buf_array[i] = number_of_vertex;
    if (token == point) error = ERROR;
    ++i;
  }
  i = 0;
  for (unsigned j = 0; j < len + 1; ++j) {
    for (unsigned k = j + 1; k < len; ++k) {
      array[i] = buf_array[j];
      array[i + 1] = buf_array[k];
      i += 2;
    }
  }
  return error;
}

int write_v_f_to_object(const char *filename, object_t *dst) {
  FILE *f = fopen(filename, "r");
  int error = f ? OK : ERROR;
  if (error == OK) {
    float max_coordinate = 0.0f;
    size_t len = 0;
    char *str = NULL;
    unsigned i = 0;
    unsigned j = 0;
    setlocale(LC_NUMERIC, "C");
    while (error == OK && getline(&str, &len, f) != -1) {
      if (str[0] == 'v' && str[1] == ' ') {
        char *token;
        strtok(str, " ");
        while (token = strtok(NULL, " "), j < 3 && token != NULL) {
          char *end_point;
          float current_vertex_coord = strtof(token, &end_point);
          dst->vertexes[i] = current_vertex_coord;
          if (max_coordinate < fabsf(current_vertex_coord))
            max_coordinate = fabsf(current_vertex_coord);
          if (token == end_point) error = ERROR;
          ++i;
        }
      } else if (str[0] == 'f' && str[1] == ' ') {
        unsigned size = get_count_of_numbers(str, len);
        write_vertex_to_array(str, dst->polygons + j, size);
        j += factorial(size) / (factorial(size - 2));
      }
    }
    normalize(dst, max_coordinate);
    free(str);
    fclose(f);
  }
  return error;
}

void free_object(object_t *obj) {
  if (obj) {
    free(obj->vertexes);
    free(obj->polygons);
  }
}

int get_object(const char *filename, object_t *dst) {
  int error = get_count_of_v_f(filename, &dst->count_of_vertexes,
                               &dst->count_of_facets);
  if (error == OK) {
    dst->vertexes = (float *)malloc(sizeof(float) * dst->count_of_vertexes);
    dst->polygons = (unsigned *)malloc(sizeof(unsigned) * dst->count_of_facets);
    write_v_f_to_object(filename, dst);
  }
  return error;
}

void move_object(object_t *obj, float dx, float dy, float dz) {
  if (obj) {
    for (unsigned i = 0; i < obj->count_of_vertexes; i += 3) {
      obj->vertexes[i] += dx;
      obj->vertexes[i + 1] += dy;
      obj->vertexes[i + 2] += dz;
    }
  }
}

void rotate_object(object_t *obj, float alpha, float beta, float gama) {
  if (obj) {
    alpha *= M_PI_2 / 180;
    beta *= M_PI_2 / 180;
    gama *= M_PI_2 / 180;
    for (unsigned i = 0; i < obj->count_of_vertexes; i += 3) {
      float x = obj->vertexes[i];
      float y = obj->vertexes[i + 1];
      float z = obj->vertexes[i + 2];
      obj->vertexes[i] = cosf(beta) * cosf(gama) * x -
                         sinf(gama) * cosf(beta) * y + sinf(beta) * z;
      obj->vertexes[i + 1] =
          (sinf(alpha) * sinf(beta) * cosf(gama) + sinf(gama) * cosf(alpha)) *
              x +
          (-sinf(alpha) * sinf(beta) * sinf(gama) + cosf(alpha) * cosf(gama)) *
              y -
          sinf(alpha) * cosf(beta) * z;
      obj->vertexes[i + 2] =
          (sinf(alpha) * sinf(gama) - sinf(beta) * cosf(alpha) * cosf(gama)) *
              x +
          (sinf(alpha) * cosf(gama) + sinf(beta) * sinf(gama) * cosf(alpha)) *
              y +
          cosf(alpha) * cosf(beta) * z;
    }
  }
}
