#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    list_of_files = new QStringListModel();
    ui->files_listview->setModel(list_of_files);
    ui->files_listview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->directoryPicker->setEnabled(false);
    ui->directory_textbox->setEnabled(false);
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
    auto output_directory = QFileDialog::getExistingDirectory(0, ("목적 폴더 지정"), QDir::currentPath());
    this->ui->directory_textbox->setText(output_directory);
}

void MainWindow::add_files_to_list(QStringList &fileNames){
    for (auto fileName : fileNames){
        QFileInfo file_info(fileName);
        if (file_info.isDir()){
            showMessageBox(this, "폴더 변환은 지원하지 않습니다.");
            return;
        }
    }
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
        //set color
        QPalette highlight = palette();
        highlight.setColor(QPalette::Background, QColor(65,35,95,255));
        ui->files_listview->setAutoFillBackground(true);
        ui->files_listview->setPalette(highlight);
        ui->files_listview->show();

        e->acceptProposedAction();
    }   
}

void MainWindow::dropEvent(QDropEvent *e){
    //set color
    QPalette default_widget_color = QApplication::palette("QListView");
    ui->files_listview->setAutoFillBackground(true);
    ui->files_listview->setPalette(default_widget_color);
    ui->files_listview->show();

    //actual processing
    QStringList fileNames;
    foreach (const QUrl &url, e->mimeData()->urls()){
        QString fileName = url.toLocalFile();
        QFileInfo file_info(fileName);
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

void MainWindow::on_convert_clicked()
{
    QProcess process;
    QStringList args;

    // if no files, exit gracefully
    if (list_of_files->rowCount() == 0){
        showMessageBox(this, "파일을 선택하십시오.");
        return;
    }

    // conversion mode
    int filetype_descriptor = ui->filetype_selector->currentIndex();
    const QString convmode[4] = {"ascii", "7bit", "iso", "mac"};
    args << "-c" << convmode[filetype_descriptor];


    if (ui->overwrite_checker->isChecked()){ // newfile mode

        QFileInfo target_directory(ui->directory_textbox->text());
        // if no directory specified, exit gracefully
        if (not target_directory.exists() or not target_directory.isWritable()){
            showMessageBox(this, "다른 폴더를 선택해주십시오.");
            return;
        }
        args << "-n";

        for (auto path: list_of_files->stringList()){
            QFileInfo curr = QFileInfo(path);
            QString filename = curr.fileName();
            QString curr_folder = curr.absolutePath();
            if (curr_folder == target_directory.absoluteFilePath()){
                filename += "_unix";
            }
            args << path;
            args << target_directory.absoluteFilePath() + "/" + filename;
        }
    }
    else{
        for (auto path: list_of_files->stringList()){
            args << path;
        }
    }

//    for (auto arg : args){
//        std::cerr << arg.toStdString() << " ";
//    }
//    std::cerr << std::endl;

    int result = system("dos2unix");
    if (result != 0){
        showMessageBox(this, "dos2unix를 설치해주십시오.");
        return;
    }
    process.execute("dos2unix", args);

    if (process.exitCode() == 0){
        list_of_files->removeRows(0, list_of_files->rowCount());
        ui->directory_textbox->setText("");
    }
}

void MainWindow::on_overwrite_checker_stateChanged(int arg1)
{
    bool enable_directory_select = ui->overwrite_checker->isChecked();
    if (not enable_directory_select){
        ui->directory_textbox->setText("");
    }
    ui->directoryPicker->setEnabled(enable_directory_select);
    ui->directory_textbox->setEnabled(enable_directory_select);
}

void MainWindow::showMessageBox(QMainWindow* window, QString msg)
{
    QMessageBox msgBox(window);
    msgBox.setText(msg);
    msgBox.exec();
}
