/*
Библиотека в которой происходит взаимодействие с базой
*/
#ifndef DBUSING_H
#define DBUSING_H

#include "mainwindow.h"     //Подключаем заголовочные
#include "ui_mainwindow.h"

#include <QMainWindow>      //Пошли либы
#include <QFile>
#include <QtSql>
#include <QMessageBox>

static QSqlDatabase db;//Глобальный обьект базы

void openDB();//Открываем базу

// processFile_Intodb & fromdbToFile ищи в mainwin. Ну спрашивай почему. Надо

#endif // DBUSING_H
