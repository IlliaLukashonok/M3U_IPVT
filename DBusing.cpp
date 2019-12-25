#include "DBusing.h"

static QString startStr = nullptr;

void openDB()//Как открывем базу
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("database.db3");

	if (!db.open())
	{
			qDebug() << "Что-то не так с соединением!1";
	}

	QSqlQuery query;

	query.exec("CREATE TABLE IF NOT EXISTS `First` (\
										   `ID`	INTEGER PRIMARY KEY AUTOINCREMENT,\
										   `Number` INTEGER,\
										   `Name`	TEXT NOT NULL,\
										   `Gr`	TEXT NOT NULL,\
										   `URL` TEXT NOT NULL\
										   )");
}

void MainWindow::processFile_Intodb(QString filePath)
{

	QFile myFile (filePath);
	myFile.open(QIODevice::ReadOnly);

	QSqlQuery query;

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, QString::fromUtf8("Вопрос"),
						  QString::fromUtf8("Очистить таблицу?"),
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

	int strNumber = 0; //Line number;                         x
	int chanNumber = 0; //Channel number;                     y
	int strChanNumber = 0; //Number of line in channel block; z

	ui->plainTextEdit->setPlainText("Start");

//	QString str = myFile.readLine();
	QString str = nullptr;
	QString chName = nullptr;
	QString chGroup = nullptr;
	QString chUrl = nullptr;
	QString	strDb = nullptr;


	while (!myFile.atEnd())
	{
	str = myFile.readLine();
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

	}
	myFile.close();
	ui->plainTextEdit->setPlainText("Finish");
}

void MainWindow::fromdbToFile(QString filePath)
{
	QSqlQuery query;

	QFile myFile (filePath);
	QTextStream stream;

	myFile.open(QIODevice::WriteOnly);

	stream.setDevice(&myFile); // Подключение потока к файлу

	stream << "#EXTM3U" << startStr << "\r\n";

	if (!query.exec("SELECT * FROM First;")) {
			qDebug() << "Unable to execute query - exiting";
		}

	//Reading of the data
	QSqlRecord rec     = query.record();
	QString    chNumber = nullptr;
	QString    chName = nullptr;
	QString    chGroup = nullptr;
	QString    chUrl = nullptr;

	while (query.next())
	{
		chNumber  = query.value(rec.indexOf("Number")).toString();
		chName  = query.value(rec.indexOf("Name")).toString();
		chGroup = query.value(rec.indexOf("Gr")).toString();
		chUrl = query.value(rec.indexOf("URL")).toString();
		stream << "#EXTINF:0," << chName << "\n"
			   << "#EXTGRP:" << chGroup << "\r\n"
			   <<  chUrl << "\r\n";
	}
	myFile.close();
}