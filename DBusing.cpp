#include "DBusing.h"
#include "windows.h"

static QString startStr = nullptr;

void openDB()//Как открывем базу
{
	db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db3");

	if (!db.open())
	{
			qDebug() << "Что-то не так с соединением!1";
	}

    QSqlQuery *query = new QSqlQuery;

    query->exec("CREATE TABLE IF NOT EXISTS `First` (\
										   `ID`	INTEGER PRIMARY KEY AUTOINCREMENT,\
										   `Number` INTEGER,\
										   `Name`	TEXT NOT NULL,\
										   `Gr`	TEXT NOT NULL,\
										   `URL` TEXT NOT NULL\
										   )");
    delete query;
}

void MainWindow::baseToTable()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QSqlQuery *query = new QSqlQuery(db);

    query->prepare("SELECT * FROM First");
    query->exec();

    model->setQuery(*query);

    ui->tableDb->setModel(model);

    ui->statusBar->showMessage( tr("Table ready"), 2000 ); //Shit
    //QDebug() << (model->rowCount());
}

/*Обрабатывает базу в файл*/
void MainWindow::processFile_Intodb(QString filePath)
{

    ui->statusBar->showMessage( tr("Start"), 1000 );
    QFile myFile (filePath);
	myFile.open(QIODevice::ReadOnly);

    QSqlQuery *query = new QSqlQuery;

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, QString::fromUtf8("Вопрос"),
						  QString::fromUtf8("Очистить таблицу?"),
						  QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
		{
        query->exec("DROP TABLE First");
        query->exec("CREATE TABLE `First` (\
										   `ID`	INTEGER PRIMARY KEY AUTOINCREMENT,\
										   `Number` INTEGER,\
										   `Name`	TEXT NOT NULL,\
										   `Gr`	TEXT NOT NULL,\
										   `URL` TEXT NOT NULL\
										   )");
		}
    ui->statusBar->showMessage( tr("Base ready"), 1000 );
    Sleep(1000);
    ui->statusBar->showMessage( tr("Start processing file"), 2000);
	int strNumber = 0; //Line number;                         x
	int chanNumber = 0; //Channel number;                     y
	int strChanNumber = 0; //Number of line in channel block; z

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
            if (!query->exec(strDb)) {
				qDebug() << "Unable to do insert opeation";
			}
	}

	}
    ui->statusBar->showMessage( tr("Processing end"), 1000 );
    ui->statusBar->showMessage( tr("Base is fill"), 1000);
	myFile.close();
    delete query;
    baseToTable();
    ui->statusBar->showMessage( tr("Ready"), 2000);
//	ui->plainTextEdit->setPlainText("Finish");
}

void MainWindow::fromdbToFile(QString filePath)
{
    QSqlQuery *query = new QSqlQuery;

    QFile myFile (filePath);
	QTextStream stream;


	myFile.open(QIODevice::WriteOnly);

	stream.setDevice(&myFile); // Подключение потока к файлу

	stream << "#EXTM3U" << startStr << "\r\n";

    if (!query->exec("SELECT * FROM First;")) {
			qDebug() << "Unable to execute query - exiting";
		}

	//Reading of the data
    QSqlRecord rec     = query->record();
    QString    chNumber = nullptr;
	QString    chName = nullptr;
	QString    chGroup = nullptr;
	QString    chUrl = nullptr;

    while (query->next())
	{
        chNumber = query->value(rec.indexOf("Number")).toString();
        chName  = query->value(rec.indexOf("Name")).toString();
        chGroup = query->value(rec.indexOf("Gr")).toString();
        chUrl = query->value(rec.indexOf("URL")).toString();
        stream.setCodec("UTF-8");
        stream.setGenerateByteOrderMark(false);
        stream << "#EXTINF:-1 tvg-chno=\"" <<  chNumber<< "\","
               << chName << "\n"
               << "#EXTGRP:" << chGroup << "\r\n"
               <<  chUrl << "\r\n";
	}
	myFile.close();
    delete query;
}
