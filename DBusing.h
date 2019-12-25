/*
Библиотека в которой происходит взаимодействие с базой
*/
#ifndef DBUSING_H
#define DBUSING_H

#include "mainwindow.h"

#include <QMainWindow>
#include <QFile>
#include <QtSql>
#include <QMessageBox>

static QSqlDatabase db;//Глобальный обьект базы

void openDB();//Открываем базу

void MainWindow::processFile_Intodb(QString filePath);

#endif // DBUSING_H
