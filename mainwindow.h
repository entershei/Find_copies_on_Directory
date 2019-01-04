#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <memory>

namespace Ui {
class MainWindow;
}

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = nullptr);
    ~main_window();

private slots:
    void select_directory();
    void scan_directory(QString const& dir);
    void delete_selected_files();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QString dir_path;
};

#endif // MAINWINDOW_H
