#include "mainwindow.h"
#include "DBusing.h"

#include <QApplication>
#include <QSql>

int main(int argc, char *argv[])
{    
	QApplication a(argc, argv);
	MainWindow w;
    QIcon icon("icon.ico");
    w.setWindowIcon(icon);
	w.show();
	openDB();


	return a.exec();
}
