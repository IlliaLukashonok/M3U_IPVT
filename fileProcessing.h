#ifndef FILEPROCESSING_H
#define FILEPROCESSING_H

#include "mainwindow.h"

#include <QMainWindow>
#include <QFile>
#include <QtSql>
#include <QMessageBox>

void MainWindow::processFile_Intodb(QString filePath)
{
	QSqlDatabase db;
	db = QSqlDatabase::addDatabase("QSQLITE");

	if (!db.isOpen())
	{
		db.setDatabaseName("database.db3");
	}
	else
	{
		db.close();
		db.setDatabaseName("database.db3");
	}

	if (!db.open())
	{
			qDebug() << "Что-то не так с соединением!";
	}

	QSqlQuery query;

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, QString::fromUtf8("Сообщение"),
						  QString::fromUtf8("Drop table?"),
						  QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
	{
		query.exec("DROP TABLE First");
		query.exec("CREATE TABLE `First` (\
											   `ID`	INTEGER PRIMARY KEY AUTOINCREMENT,\
											   `Number` INTEGER,\
											   `Name`	TEXT NOT NULL,\
											   `Gr`	TEXT NOT NULL,\
											   `URL` TEXT NOT NULL\
											   )");
	}
	else
	{

	}


	QFile myFile (filePath);
	myFile.open(QIODevice::ReadOnly);

	int strNumber = 0; //Line number;                         x
	int chanNumber = 0; //Channel number;                     y
	int strChanNumber = 0; //Number of line in channel block; z

	ui->plainTextEdit->setPlainText("Start");

	QString str = myFile.readLine();
	QString startStr = nullptr;
	QString chName = nullptr;
	QString chGroup = nullptr;
	QString chUrl = nullptr;
	QString	strDb = nullptr;


	while (!myFile.atEnd())

	{
	str = str.simplified();
	if (str.contains("#EXTM3U")) //Если есть первая строка
	{
		startStr = str.remove("#EXTM3U");
		startStr = startStr.remove("\r\n");
		strNumber++;
	}

	else if (str.contains("#EXTINF")) //Узнаем имя
	{
		int *pos = new int;
		*pos = str.indexOf(',');
		chName = str.remove(0, *pos + 1);
		chName = chName.remove('\'');
		delete pos;
		strNumber++;
		chanNumber++;
		strChanNumber++;
	}
	else if (str.contains("#EXTGRP:"))
	{
		chGroup = str.remove("#EXTGRP:");
		chGroup = chGroup.remove("\r\n");
		strNumber++;
		strChanNumber++;
	}
	else if (str.contains("http"))
	{
	chUrl = str;
	chUrl = chUrl.remove("\r\n");
	strNumber++;
	strChanNumber = 0;
	}

	if (strChanNumber == 0 && chanNumber != 0)
	{
		QString strF =
				  "INSERT INTO  First (Number, Name, Gr, URL) "
				  "VALUES(%1, '%2', '%3', '%4');";

			strDb = strF.arg(chanNumber)
					  .arg(chName)
					  .arg(chGroup)
					  .arg(chUrl);
			if (!query.exec(strDb)) {
				qDebug() << "Unable to do insert opeation";
			}
	}
	str = myFile.readLine();
	}
	myFile.close();
	ui->plainTextEdit->setPlainText("Finish");
}

#endif // FILEPROCESSING_H

