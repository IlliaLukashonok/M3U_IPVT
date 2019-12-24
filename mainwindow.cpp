#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileProcessing.h"
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
	QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"),
													   "e:\\iptv",
													   tr("m3u (*.m3u*)"));
	processFile(filePath);

}
