#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <treemodel.h>
#include "ui_mainwindow.h"
#include <QFile>

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void updateActions();

private slots:
    void insertChild();
    bool insertColumn();
    void insertRow();
    bool removeColumn();
    void removeRow();
    void prov_child();
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void draw(QModelIndex &index, QFile &fileOut, QAbstractItemModel *model,QTextStream &stream);
};

#endif // MAINWINDOW_H
