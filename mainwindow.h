#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>

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

private slots:

	void on_actionOpen_File_triggered();

	void on_actionSave_As_triggered();

	void on_actionAbout_triggered();

    void on_refreshBtn_clicked();

private:
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
