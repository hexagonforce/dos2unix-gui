#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QStringList>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showMessageBox();

private slots:
    //void on_pushButton_clicked();

    void on_addFiles_clicked();

    void on_directoryPicker_clicked();

    void add_files_to_list(QStringList& fileNames);

    void dragEnterEvent(QDragEnterEvent *e);

    void dropEvent (QDropEvent *e);

    void on_deleteFiles_clicked();

    void on_convert_clicked();

    void on_overwrite_checker_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QStringListModel *list_of_files;
    void showMessageBox(QMainWindow* w, QString msg);
};

#endif // MAINWINDOW_H
