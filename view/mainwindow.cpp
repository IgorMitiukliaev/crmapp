#include "view/mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller(new Controller) {
  ui->setupUi(this);

  SetupMenu();
  setMenuBar(menu_bar);
  sizeHint();
  controller->Init();
}

MainWindow::~MainWindow() {
  delete menu_bar;
  delete controller;
  delete ui;
}

auto MainWindow::SetupMenu() -> void {
  QAction *quit_app = new QAction("Quit application");
  QAction *load_leads = new QAction("Load leads");
  QAction *load_offices = new QAction("Load offices");
  connect(quit_app, &QAction::triggered, [=]() { QApplication::quit(); });
  connect(load_leads, &QAction::triggered,
          [this]() { controller->GetLeads(); });
  connect(load_offices, &QAction::triggered,
          [this]() { controller->GetOffices(); });

  menu_bar = new QMenuBar;
  QMenu *file_menu = menu_bar->addMenu("School");
  QMenu *leads_menu = menu_bar->addMenu("Leads");
  //  QMenu *students_menu = menu_bar->addMenu("Students");
  file_menu->addAction(load_offices);
  file_menu->addAction(quit_app);

  leads_menu->addAction(load_leads);
}

auto MainWindow::GetLeads() -> void {
  //  controller->
}
