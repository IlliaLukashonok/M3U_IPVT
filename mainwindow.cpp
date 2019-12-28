#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DBusing.h"
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
													   "c:\\",
													   tr("m3u (*.m3u*)"));
	processFile_Intodb(filePath);

}

void MainWindow::on_actionSave_As_triggered()
{
	QString savePath = QFileDialog::getSaveFileName(this, tr("Save File"),
													"c:\\untitled.m3u",
													tr("m3u (*.m3u*)"));
	fromdbToFile(savePath);
}

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox::about(this, "About", "\
Bielarus,   ____           ____    ________\n\
Miensk,   /   /           /   /   /  _ ____/\n\
12.2019, /   /          /   /   |  (___\n\
Happy /   /          /   /    \\___ \\\n\
New  /   /______    /   /    ______)  |\n\
Year/__________/   /___/    /________/\n");
}
