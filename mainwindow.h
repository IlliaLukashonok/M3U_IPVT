#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <dos.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void processFile_Intodb(QString filePath);
	void fromdbToFile(QString filePath);
    void baseToTable();
    //void statBarState();

private slots:

	void on_actionOpen_File_triggered();

	void on_actionSave_As_triggered();

	void on_actionAbout_triggered();

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
