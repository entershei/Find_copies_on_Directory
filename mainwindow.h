#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <memory>
#include <QFutureWatcher>
#include <QPair>
#include <QElapsedTimer>

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
    void cancel();
    void close();
    void find_finished();

private:
    void stop_find(std::atomic_bool & find_run);

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QString dir_path;
    bool want_to_close;
    QFutureWatcher<QPair<bool, std::pair<std::vector<std::vector<QString>>, std::vector<QString>> > > watcher;
    std::atomic_bool find_run;
    QElapsedTimer timer_find;
};

#endif // MAINWINDOW_H
