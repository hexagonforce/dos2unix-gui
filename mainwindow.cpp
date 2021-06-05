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
    ui->file_list->setModel(list_of_files);
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
        this->add_files_to_list(dialog.selectedFiles());
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
