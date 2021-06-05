#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

#include <iostream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    list_of_files = new QStringListModel();
    ui->files_listview->setModel(list_of_files);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addFiles_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    QStringList fileNames;
    if (dialog.exec()){
        fileNames = dialog.selectedFiles();
        this->add_files_to_list(fileNames);
    }
}

void MainWindow::on_directoryPicker_clicked()
{
    auto output_directory = QFileDialog::getExistingDirectory(0, ("Select Output Folder"), QDir::currentPath());
    this->ui->directory_textbox->setText(output_directory);
}

void MainWindow::add_files_to_list(QStringList &fileNames){
    for (auto fileName : fileNames){
        if (not list_of_files->stringList().contains(fileName)){
            if (list_of_files->insertRow(list_of_files->rowCount())){
                QModelIndex myindex = list_of_files->index(list_of_files->rowCount() - 1, 0);
                list_of_files->setData(myindex, fileName);
            }
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e){
    if (e->mimeData()->hasUrls()){
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e){
    QStringList fileNames;
    foreach (const QUrl &url, e->mimeData()->urls()){
        QString fileName = url.toLocalFile();
        fileNames << fileName;
    }
    add_files_to_list(fileNames);
}

void MainWindow::on_deleteFiles_clicked()
{
    ui->files_listview->setUpdatesEnabled(false);
    auto selectedIndexes = ui->files_listview->selectionModel()->selectedIndexes();

    qSort(selectedIndexes.begin(), selectedIndexes.end());
    for (QModelIndexList::const_iterator it = selectedIndexes.constEnd() - 1;
         it >= selectedIndexes.constBegin(); --it){
        list_of_files->removeRow(it->row());
    }
    ui->files_listview->setUpdatesEnabled(true);
}
