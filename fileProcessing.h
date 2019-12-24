#ifndef FILEPROCESSING_H
#define FILEPROCESSING_H

#include "mainwindow.h"

#include <QMainWindow>
#include <QFile>
#include <QtSql>

void MainWindow::processFile(QString filePath)
{
	QSqlDatabase db;
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("database.db3");

	if (!db.open()) {
			qDebug() << "Что-то не так с соединением!";
		}

	QSqlQuery query;
	query.exec("CREATE TABLE `First` (\
			   `ID`	INTEGER PRIMARY KEY AUTOINCREMENT,\
			   `Number` INTEGER,\
			   `Name`	TEXT NOT NULL,\
			   `Group`	TEXT NOT NULL,\
			   `URL` TEXT NOT NULL\
			   )");

	QFile myFile (filePath);
	myFile.open(QIODevice::ReadOnly);

	int strNumber = 0; //Line number;                         x
	int chanNumber = 0; //Channel number;                     y
	int strChanNumber = 0; //Number of line in channel block; z
	ui->plainTextEdit.plainText("Hi");
	while (!myFile.atEnd()) {

	QString str = myFile.readLine();

	if (str.contains("#EXTM3U")) //Если есть первая строка
	{
		QString startStr = str.remove("#EXTM3U");
		startStr = startStr.remove("\n");
		strNumber++;
	}

	if (str.contains("#EXTINF")) //Узнаем имя
	{
		int *pos = new int;
		*pos = str.indexOf(',');
		QString chName = str.remove(0, *pos);
//		chName = chName.remove('\n');
		delete pos;
		strNumber++;
		chanNumber++;
		strChanNumber++;
	}
	else if (str.contains("#EXTGRP:"))
	{
		QString chGroup = str.remove("#EXTGRP:");
		strNumber++;
		strChanNumber++;
	}
	else if (str.contains("http"))
	{
	QString urlStr = str;
	strNumber++;
	strChanNumber = 0;
	}

	if (strChanNumber == 0 && chanNumber != 0)
	{
		QSqlQuery query;
		query.exec("INSERT INTO First (Number, Name, Group, URL) VALUES(chanNumber, chName, chGroup, urlStr);");
	}
	}
	myFile.close();
}

#endif // FILEPROCESSING_H

