#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{	
	delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
													  "/home",
													  tr("All (*.*)"));
}
