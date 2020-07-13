#include "DBusing.h"

static QString startStr = nullptr;

void openDB()//Как открывем базу
{
    db = QSqlDatabase::addDatabase("QSQLITE");//Добавляем базу
    db.setDatabaseName("database.db3");//Название базы

    if (!db.open())//Проверка открытия
	{
            qDebug() << "Что-то не так с соединением!1";//АЛЯРМА!!!
	}

    QSqlQuery *query = new QSqlQuery;//ИЧСХ - query
    /*
    Если нямае такой таблицы в базе - создаем.
    */
    query->exec("CREATE TABLE IF NOT EXISTS `First` (\
										   `ID`	INTEGER PRIMARY KEY AUTOINCREMENT,\
										   `Number` INTEGER,\
										   `Name`	TEXT NOT NULL,\
										   `Gr`	TEXT NOT NULL,\
										   `URL` TEXT NOT NULL\
										   )");
    delete query;//ИЧСХ - удаляем query
}

void MainWindow::baseToTable()//Визуализация базы в виджет таблицы
{
    QSqlTableModel *model = new QSqlTableModel(this);//Устанавливаем модель
    model->setTable("First");//Имя тэйблы
    model->setEditStrategy(QSqlTableModel::OnFieldChange);//Метод обновления таблицы после редактирования
    model->setSort(1, Qt::AscendingOrder);
    model->select(); // Делаем выборку значений из таблиц

    ui->tableDb->setModel(model);     // Устанавливаем модель на TableView

    ui->tableDb->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Устанавливаем размер колонок по содержимому
    ui->tableDb->resizeColumnsToContents();
    ui->tableDb->horizontalHeader()->setStretchLastSection(true);

    ui->statusBar->showMessage( tr("Table ready"), 2000 ); //Все еще  не работает
}

void MainWindow::processFile_Intodb(QString filePath)//Обрабатывает файл в базу
{

    ui->statusBar->showMessage( tr("Start"), 1000 ); //№2
    QFile myFile (filePath); //Устанавливаем что за файл
    myFile.open(QIODevice::ReadOnly); //ИЧСХ - открываем файл

    QSqlQuery *query = new QSqlQuery;

	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, QString::fromUtf8("Вопрос"),
						  QString::fromUtf8("Очистить таблицу?"),
						  QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes)
		{
        query->exec("DROP TABLE First"); //Дропаем таблицу
        query->exec("CREATE TABLE `First` (\
										   `ID`	INTEGER PRIMARY KEY AUTOINCREMENT,\
										   `Number` INTEGER,\
										   `Name`	TEXT NOT NULL,\
										   `Gr`	TEXT NOT NULL,\
										   `URL` TEXT NOT NULL\
                                           )");//Пересоздаем таблицу
		}
    ui->statusBar->showMessage( tr("Base ready"), 1000 ); //N3
    ui->statusBar->showMessage( tr("Start processing file"), 2000); //N4

    int strNumber = 0; //Line number;                          x
    int chanNumber = 0; //Channel number;                      y
    int strChanNumber = 0; //Number of line in channel's block z

	QString str = nullptr;
	QString chName = nullptr;
	QString chGroup = nullptr;
	QString chUrl = nullptr;
	QString	strDb = nullptr;

    bool *flag = new bool(false);// Ну, надо (см. ниже)

    while (!myFile.atEnd())//Бежим по файлу
	{
    *flag = false; //Возвращаем в исходное положение
	str = myFile.readLine();
    str = str.simplified();//Выкидываем из линии все лишнее

        if (str.contains("#EXTM3U")) //Если есть первая строка
        {
            startStr = str.remove("#EXTM3U");
            startStr = startStr.remove("\r\n");//Удаляем перевод каретки и сдвиг строки
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

        else if ((!str.startsWith("0x0A") || !str.startsWith("\r") || !str.startsWith("\n")) && strNumber != 0)
        {
            QMessageBox *errorMessage = new QMessageBox;
            QString *errText = new QString ("Я не умею реагировать на такую информацию)\nString N" + QString::number(strNumber + 1));
            errorMessage->setText(*errText);
            errorMessage->exec();
            delete errorMessage;
            delete errText;
            QString strF =
                      "INSERT INTO  First (Number, Name, Gr, URL) "
                      "VALUES(%1, '%2', '%3', '%4');";//Загружаем ересь в базу

                strDb = strF.arg(strNumber+1)
                          .arg("Ересь")
                          .arg("Ересь")
                          .arg("Ересь");
        if (!query->exec(strDb)) //Проверочка
        {
            qDebug() << "Unable to do insert opeation";
        }
        strNumber++;
        *flag =true;
    }
    // Вот сюда смотри-----------------------------|
    //                                             V
    if (strChanNumber == 0 && chanNumber != 0 && *flag != true) //Иначе считает что 1 строка - канал и загружает его
	{
		QString strF =
				  "INSERT INTO  First (Number, Name, Gr, URL) "
                  "VALUES(%1, '%2', '%3', '%4');";//Загружаем дату о канале в базу

			strDb = strF.arg(chanNumber)
					  .arg(chName)
					  .arg(chGroup)
					  .arg(chUrl);

        if (!query->exec(strDb)) //Проверочка
        {
            qDebug() << "Unable to do insert opeation";
        }
    }

	}
    delete flag;
    ui->statusBar->showMessage( tr("Processing end"), 1000 ); //Nn
    ui->statusBar->showMessage( tr("Base is fill"), 1000); //Nn+1
	myFile.close();
    delete query;
    baseToTable();
    ui->statusBar->showMessage( tr("Ready"), 2000); //Задолбался! Все стороки со статусбаром - криво НЕработуещее дерьмо
}

void MainWindow::fromdbToFile(QString filePath) //Превод даты из базы в файл
{
    QSqlQuery *query = new QSqlQuery; // -//-

    QFile myFile (filePath);
    QTextStream stream; //Создаем поток


	myFile.open(QIODevice::WriteOnly);

	stream.setDevice(&myFile); // Подключение потока к файлу

    stream << "#EXTM3U" << startStr << "\r\n"; //Добавляем конец строки и переход

    if (!query->exec("SELECT * FROM First;")) //Проверочка
        {
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
        //В следующих 4 строках я сам не до конца разобрался
        chNumber = query->value(rec.indexOf("Number")).toString();
        chName  = query->value(rec.indexOf("Name")).toString();
        chGroup = query->value(rec.indexOf("Gr")).toString();
        chUrl = query->value(rec.indexOf("URL")).toString();
        stream.setCodec("UTF-8"); //Ставим кодировку
        stream.setGenerateByteOrderMark(false); //Не надо нам ваш BOM, или как его там
        stream << "#EXTINF:-1 tvg-chno=\"" <<  chNumber<< "\","
               << chName << "\n"
               << "#EXTGRP:" << chGroup << "\r\n"
               <<  chUrl << "\r\n"; //Выводим все разом в файл
	}
	myFile.close();
    delete query;
}
