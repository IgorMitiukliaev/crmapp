#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>

#include "./widgets/leadswidget.h"
#include "controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
//  QMdiSubWindow *NewWindow();
  QMenuBar *menu_bar_;
  QWidget *child_widget_;
  Controller *controller_;
  auto SetupMenu() -> void;
  auto SetupLeadsWidget() -> void;
  auto GetLeads() -> void;
};
#endif  // MAINWINDOW_H
