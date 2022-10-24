#include "view/mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  SetupMenu();
  setMenuBar(menu_bar);
}

MainWindow::~MainWindow() {
  delete menu_bar;
  delete ui;
}

auto MainWindow::SetupMenu() -> void {
  QAction *quit_app = new QAction("Quit application");
  connect(quit_app, &QAction::triggered, [=]() { QApplication::quit(); });

  menu_bar = new QMenuBar;
  QMenu *file_menu = menu_bar->addMenu("File");
  file_menu->addAction(quit_app);
  menu_bar->addMenu("Leads");
  menu_bar->addMenu("Students");
}
