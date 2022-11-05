#include "view/mainwindow.h"

#include "ui_leadswidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), controller_(new Controller) {
  ui->setupUi(this);

  SetupMenu();
  sizeHint();
  controller_->Init();
}

MainWindow::~MainWindow() {
  delete menu_bar_;
  delete controller_;
  delete ui;
}

auto MainWindow::SetupMenu() -> void {
  menu_bar_ = new QMenuBar;
  setMenuBar(menu_bar_);
  QMenu *file_menu = menu_bar_->addMenu("School");
  QMenu *leads_menu = menu_bar_->addMenu("Leads");

  QAction *load_offices = new QAction("Load offices");
  connect(load_offices, &QAction::triggered, [this]() {
    QUrlQuery params{};
    controller_->GetDataFromApi("GetOffices", params);
  });
  file_menu->addAction(load_offices);

  QAction *quit_app = new QAction("Quit application");
  connect(quit_app, &QAction::triggered, [=]() { QApplication::quit(); });
  file_menu->addAction(quit_app);

  QAction *load_leads = new QAction("Load leads");
  connect(load_leads, &QAction::triggered, [this]() { SetupLeadsWidget(); });
  leads_menu->addAction(load_leads);

  QAction *load_history_modify_lead_status =
      new QAction("Load History Modify Lead Status");
  connect(load_history_modify_lead_status, &QAction::triggered, []() {});
  leads_menu->addAction(load_history_modify_lead_status);
}

void MainWindow::SetupLeadsWidget() {
  QMdiSubWindow *new_window = new QMdiSubWindow(this);
  child_widget_ = new LeadsWidget(controller_, this);
  new_window->setWidget(child_widget_);
  new_window->setAttribute(Qt::WA_DeleteOnClose);
  ui->mdiArea->addSubWindow(new_window);
  new_window->showMaximized();
}

auto MainWindow::GetLeads() -> void {
  //  controller->
}
