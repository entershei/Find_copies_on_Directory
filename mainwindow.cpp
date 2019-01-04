#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCommonStyle>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <group_identical_files.h>
#include <iostream>
#include <ctime>
#include <QDebug>
#include <QCheckBox>
#include <algorithm>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->treeWidget->header()->setSectionResizeMode(3, QHeaderView::Stretch);

    QCommonStyle style;
    ui->actionScan_Directory->setIcon(style.standardIcon(QCommonStyle::SP_DialogOpenButton));
    ui->actionExit->setIcon(style.standardIcon(QCommonStyle::SP_DialogCloseButton));

    connect(ui->actionScan_Directory, &QAction::triggered, this, &main_window::select_directory);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionDelete_selected_files, &QAction::triggered, this, &main_window::delete_selected_files);
}

main_window::~main_window() {}

void main_window::select_directory() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory for Scanning",
                                                    QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    dir_path = dir;
    scan_directory(dir);
}

bool size_cmp(std::vector<QString> &a, std::vector<QString> b) {
    long long a_size = QFileInfo(a[0]).size();
    long long b_size = QFileInfo(b[0]).size();
    return a_size > b_size;
}

using namespace std;

void main_window::scan_directory(QString const& dir) {
    if (dir.isEmpty()) {
        return;
    }

    clock_t stast_time = clock();
    ui->treeWidget->clear();

    setWindowTitle(QString("Directory Content - %1").arg(dir));

    auto groups_and_symlinks = my::group_identical_files(dir);
    //cerr << "finded" << endl;

    QDir base(dir);

    for (size_t i = 0; i < groups_and_symlinks.second.size(); ++i) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);       
        item->setText(0, "(symlink) " + QString::number(i + 1));
        item->setText(1, base.relativeFilePath(groups_and_symlinks.second[i]));

        QFileInfo cur = QFileInfo(groups_and_symlinks.second[i]);
        item->setText(2, QString::number(cur.size() / 1000.0));
        item->setText(3, "1");

        ui->treeWidget->addTopLevelItem(item);
    }

    size_t add = groups_and_symlinks.second.size();

    //std::sort(groups_and_symlinks.first.begin(), groups_and_symlinks.first.end(), size_cmp);

    for (size_t i = 0; i < groups_and_symlinks.first.size(); ++i) {
        QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
        item->setText(0, QString::number(i + 1 + add));
        item->setText(1, base.relativeFilePath(groups_and_symlinks.first[i][0]));

        QFileInfo cur = QFileInfo(groups_and_symlinks.first[i][0]);
        item->setText(2, QString::number(cur.size() / 1000.0));
        item->setText(3, QString::number(groups_and_symlinks.first[i].size()));

        ui->treeWidget->addTopLevelItem(item);

        for (size_t j = 0; j < groups_and_symlinks.first[i].size(); ++j) {
            QTreeWidgetItem* child = new QTreeWidgetItem();      

            child->setText(0, base.relativeFilePath(groups_and_symlinks.first[i][j]));
            item->addChild(child);
         }
    }
    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->treeWidget->header()->setSectionResizeMode(2, QHeaderView::Interactive);

    qDebug() << QString::number((clock() - stast_time) / 1000000.0) << "s.";
}

void main_window::delete_selected_files() {
    auto selected_files = ui->treeWidget->selectedItems();

    QString all_path;

    for (auto& item : selected_files) {
        if (item->parent() != nullptr) {
            all_path = QDir(dir_path).filePath(item->text(0));
            if (QFile::remove(all_path)) {
                delete item;
            }
        } else if (item->text(0)[0] == '(') {
            qDebug() << "symlink";
            all_path = QDir(dir_path).filePath(item->text(1));
            qDebug() << all_path;
            if (QFile::remove(all_path)) {
                delete item;
            }
        }
    }
}

