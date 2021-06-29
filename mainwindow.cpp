#include "mainwindow.h"
#include "treemodel.h"
#include <QDebug>
#include <QFileDialog>
//#include "treeitem.h"

TreeModel *model;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    const QStringList headers({tr("Группы")});

    QFile file(":/default.txt");
    file.open(QIODevice::ReadOnly);
    model = new TreeModel(headers, file.readAll());
    file.close();

    view->setModel(model);
    for (int column = 0; column < model->columnCount(); ++column)
        view->resizeColumnToContents(column);

    //connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    connect(view->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::updateActions);

    //connect(actionsMenu, &QMenu::aboutToShow, this, &MainWindow::updateActions);
    connect(action, &QAction::triggered, this, &MainWindow::insertRow);
    //connect(insertColumnAction, &QAction::triggered, this, &MainWindow::insertColumn);
    connect(action_5, &QAction::triggered, this, &MainWindow::removeRow);
    //connect(removeColumnAction, &QAction::triggered, this, &MainWindow::removeColumn);
    connect(action_2, &QAction::triggered, this, &MainWindow::insertChild);

    updateActions();
}

void MainWindow::prov_child()
{
  const QModelIndex index = view->selectionModel()->currentIndex();
  QAbstractItemModel *model = view->model();
  QStringList task;
  task=model->data(index).toString().split(" ");
  //qDebug() << task;

}

void MainWindow::insertChild()
{
    const QModelIndex index = view->selectionModel()->currentIndex();
    QModelIndex index_prov;
       QAbstractItemModel *model = view->model();
        bool flag=false;
    for(int i=0;i<model->rowCount();i++)
    {
      index_prov = model->index(i,0);
      if(index == index_prov)
          flag=true;
    }
    if(flag == false)
        return;

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;
    //prov_child();
    for (int column = 0; column < model->columnCount(); ++column)
    {
        const QModelIndex child = model->index(0, column, index);

        model->setData(child, QVariant(tr("Имя человека")), Qt::EditRole);

        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant(tr("[No header]")), Qt::EditRole);
        //qDebug() << child << "CHILD";
    }

    view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    //open_file();
    updateActions();
}

bool MainWindow::insertColumn()
{
    QAbstractItemModel *model = view->model();
    int column = view->selectionModel()->currentIndex().column();

    // Insert a column in the parent item.
    bool changed = model->insertColumn(column + 1);
    if (changed)
        model->setHeaderData(column + 1, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);

    updateActions();

    return changed;
}

void MainWindow::insertRow()
{
    const QModelIndex index = view->selectionModel()->currentIndex();
    QModelIndex index_prov, index_child;
    QAbstractItemModel *model = view->model();
    bool flag=true;
    for(int i=0;i<model->rowCount();i++)
    {
     index_prov = model->index(i,0);
     for(int j=0;j<100;j++)
     {
     index_child= model->index(j,0,index_prov);
     qDebug() << index_child;
     if(index==index_child)
         flag=false;
     }
    }
    if(flag == false)
     return;
    if (!model->insertRow(index.row()+1, index.parent()))
        return;

    updateActions();

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        const QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant(tr("Название группы")), Qt::EditRole);
    }
}

bool MainWindow::removeColumn()
{
    QAbstractItemModel *model = view->model();
    const int column = view->selectionModel()->currentIndex().column();

    // Insert columns in each child of the parent item.
    const bool changed = model->removeColumn(column);
    if (changed)
        updateActions();

    return changed;
}

void MainWindow::removeRow()
{
    const QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}

void MainWindow::updateActions()
{
    const bool hasSelection = !view->selectionModel()->selection().isEmpty();
    //removeRowAction->setEnabled(hasSelection);
    //removeColumnAction->setEnabled(hasSelection);

    const bool hasCurrent = view->selectionModel()->currentIndex().isValid();
    //insertRowAction->setEnabled(hasCurrent);
    //insertColumnAction->setEnabled(hasCurrent);

    if (hasCurrent) {
        view->closePersistentEditor(view->selectionModel()->currentIndex());

        const int row = view->selectionModel()->currentIndex().row();
        const int column = view->selectionModel()->currentIndex().column();
        if (view->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}

void MainWindow::on_pushButton_1_clicked()
{
    const QStringList headers({tr("Title")});
    QString fileName_DATA = QFileDialog::getOpenFileName(this, tr("Save File"),"/home",tr("DATA (*.txt)"));
    QFile fileOut(fileName_DATA);
    fileOut.open(QIODevice::WriteOnly);
    QModelIndex papa = model->index(0,0);
    QString st="hello";
    QTextStream stream(&fileOut);
    for (int i = 0 ;i < model->rowCount();i++)
    {
        QModelIndex temp = model->index(i,0);
        stream<<QVariant(model->data(temp,Qt::EditRole)).toString()<<"\n";
        //temp = model->index(i,0,papa);
        draw(temp,fileOut, model, stream);
        qDebug() << model->rowCount(papa) << "ASDSDADS";
    }
    fileOut.close();
}

void MainWindow::on_pushButton_2_clicked()
{
     model->~TreeModel();
     const QStringList headers({tr("Title")});
     QString fileName_DATA = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("DATA (*.txt)"));
     QFile file(fileName_DATA);
     //QFile file(":/default.txt");
     file.open(QIODevice::ReadOnly);
     model = new TreeModel(headers, file.readAll());
     file.close();
     view->setModel(model);
     for (int column = 0; column < model->columnCount(); ++column)
         view->resizeColumnToContents(column);
     updateActions();
     file.close();
}

void MainWindow::draw(QModelIndex &index, QFile &fileOut, QAbstractItemModel *modal,QTextStream &stream)
{
    QModelIndex temp ;
    if (modal->rowCount(index) == 0)
     return;
    for (int i = 0 ;i < modal->rowCount(index);i++)
    {
        temp = modal->index(i,0,index);
        stream<<" "<<QVariant(model->data(temp,Qt::EditRole)).toString()<<"\n";
        /*if ( i < modal->rowCount(index))
        {
            temp = modal->index(i,0,index);
            draw(temp,fileOut, model, stream);
        }*/
    }
}
