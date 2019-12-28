/*
Библиотека в которой происходит взаимодействие с базой
*/
#ifndef DBUSING_H
#define DBUSING_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <QFile>
#include <QtSql>
#include <QMessageBox>

static QSqlDatabase db;//Глобальный обьект базы

void openDB();//Открываем базу

// processFile_Intodb & fromdbToFile ищи в mainwin. Ну спришивай почему. Надо

#endif // DBUSING_H
