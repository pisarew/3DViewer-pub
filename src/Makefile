CFLAGS := -Wall -Wextra -Werror -I./include
CC := gcc
OS := $(shell uname -s)

HOME_DIR := $(shell pwd)
LIB_SRC_FILES := $(HOME_DIR)/core
QT_SRC_FILES := $(HOME_DIR)/gui
INCLUDE_FILES := $(HOME_DIR)/include
QT_UI_FILES := $(HOME_DIR)/gui/ui

SOURCES = $(LIB_SRC_FILES)/object_controller.c
LIBRARY = object_controller.a

CMAKE := "cmake_minimum_required(VERSION 3.22) \n\
          project(viewer C CXX) \n\
          include_directories($(INCLUDE_FILES)) \n\
          set(SHADER_PATH "$(HOME_DIR)/gui/shaders/") \n\
          configure_file($(INCLUDE_FILES)/config.h.in $(INCLUDE_FILES)/config.h) \n\
          set(CMAKE_C_STANDARD 11) \n\
          set(CMAKE_CXX_STANDARD 17) \n\
          set(CMAKE_AUTOMOC ON) \n\
          set(CMAKE_AUTOUIC ON) \n\
          set(CMAKE_AUTORCC ON) \n\
          set(CMAKE_AUTOUIC_SEARCH_PATHS $(QT_UI_FILES)) \n\
          find_package(Qt6 REQUIRED COMPONENTS Core  Widgets OpenGLWidgets) \n\
          add_library(controller \n\
                  $(LIB_SRC_FILES)/object_controller.c \n\
                  $(INCLUDE_FILES)/object_controller.h) \n\
          qt_add_executable(viewer \n\
                  $(QT_SRC_FILES)/main.cpp \n\
                  $(QT_SRC_FILES)/viewer.cpp \n\
                  $(INCLUDE_FILES)/viewer.h \n\
                  $(QT_UI_FILES)/viewer.ui \n\
                  $(QT_SRC_FILES)/OpenGLWidget.cpp \n\
                  $(INCLUDE_FILES)/OpenGLWidget.h \n\
                  $(QT_SRC_FILES)/ObjectManager.cpp \n\
                  $(INCLUDE_FILES)/ObjectManager.h) \n\
          set_target_properties(viewer PROPERTIES \n\
                  MACOSX_BUNDLE TRUE \n\
                  ) \n\
          target_link_libraries(viewer  PUBLIC \n\
                  controller \n\
                  Qt::Core \n\
                  Qt::Widgets \n\
                  Qt6::OpenGLWidgets \n\
                  )"

ifeq ($(OS), Darwin)
TEST_FLAGS = -lcheck
else
TEST_FLAGS = -lcheck -lsubunit -lm
endif

all: install tests

install: install_qt
ifeq ($(OS), Darwin)
	open ./bin/viewer.app
else
	./bin/viewer
endif

uninstall:
	rm -rf bin/

install_qt:
	mkdir -p bin
	echo $(CMAKE) > ./bin/CMakeLists.txt
	cd bin; cmake .; make;

tests: build
	./build/test

build: object_controller.a $(LIB_SRC_FILES)/tests/test.c
	$(CC) $(CFLAGS) $(LIB_SRC_FILES)/tests/test.c ./build/object_controller.a -o ./build/test $(TEST_FLAGS)

%.o: %.c
	mkdir ./build
	$(CC) -c $(CFLAGS) $< -o ./build/object_controller.o

object_controller.a:  $(SOURCES:.c=.o)
	ar rcs ./build/$@ ./build/object_controller.o

clean:
	rm -rf ./build

dist:
	mkdir -p "3DViewer"
	cp -r Makefile Doxyfile core gui include 3DViewer/
	tar -zcf 3DViewer.tar.gz 3DViewer
	rm -rf 3DViewer

linter:
	clang-format -n core/*.c include/*.h gui/*.cpp

leaks: build
	leaks --atExit -- ./build/test

leaksvg:
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./build/test

gcov_report: clean
	mkdir -p ./build
	$(CC) $(CFLAGS) --coverage -o ./build/test $(LIB_SRC_FILES)/object_controller.c $(LIB_SRC_FILES)/tests/test.c $(TEST_FLAGS)
	./build/test
	gcovr -r . --object-directory ./build --html --html-details -o build/coverage_report.html
	open build/coverage_report.html

dvi:
	mkdir -p "dvi"
	doxygen Doxyfile

