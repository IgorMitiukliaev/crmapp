#include <QApplication>
#include <QDebug>

#include "./view/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.showMaximized();
  return a.exec();
}
