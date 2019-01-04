#include "group_identical_files.h"

#include <unordered_map>
#include <QDir>
#include <fstream>
#include <bits/stl_pair.h>
#include <iostream>
#include <QCryptographicHash>
#include <QDebug>

QString appendPath(const QString& path1, const QString& path2) {
    return QDir::cleanPath(path1 + QDir::separator() + path2);
}

void check_file(QString dir) {
    QFileInfo file(dir);

    if (file.exists() && file.isDir()) {
        //No operations.
    } else {
        throw my::not_directory_exception(file.fileName() + " isn't directoty");
    }
}

void find_size(QString const& dir_, std::pair<std::unordered_map<long long, std::vector<QString>>,
                                std::vector<QString>> &group_identical_files_and_symlinks) {
    try {
        check_file(dir_);
    } catch(my::not_directory_exception e) {
        //todo
    }

    QDir dir(dir_);
    QFileInfoList list = dir.entryInfoList();

    std::vector<QString> dirs;

    for (QFileInfo file_info : list) {
        if (file_info.isDir()) {
            QString cannonic_path = file_info.canonicalFilePath();

            if (cannonic_path == dir.path() || cannonic_path.size() < dir.path().size()) { continue; }

            dirs.push_back(file_info.filePath());
        } if (file_info.isSymLink()) {
            group_identical_files_and_symlinks.second.push_back(file_info.filePath());
        } else if (file_info.isFile()) {
            group_identical_files_and_symlinks.first[file_info.size()].push_back(file_info.filePath());
        }
    }

    for (QString cur_dir : dirs) {
        find_size(cur_dir, group_identical_files_and_symlinks);
    }
}

const size_t MAX_BLOCK = 1 << 12;

bool equals_two_files(QString const &path1, QString const &path2) {
    std::ifstream file1(path1.toStdString(), std::ios::binary);
    std::ifstream file2(path2.toStdString(), std::ios::binary);

    bool can_read = false;

    if(file1.is_open() && file2.is_open()) {
        std::string buffer1;
        buffer1.resize(MAX_BLOCK);

        std::string buffer2;
        buffer2.resize(MAX_BLOCK);
        do{
            file1.read(&(buffer1[0]), MAX_BLOCK);
            buffer1.resize(size_t(file1.gcount()));

            file2.read(&(buffer2[0]), MAX_BLOCK);
            buffer2.resize(size_t(file2.gcount()));

            if (buffer1.size() > 0 && buffer2.size() > 0) {
                can_read = true;
            }

            if (buffer1 != buffer2) {
                return false;
            }
        }while(!buffer1.empty() && !buffer2.empty());

        if (buffer1.size() != buffer2.size()) {
            return false;
        }

        if (!can_read && file1.tellg() > 0 && file2.tellg() > 0) {
            return false;
        }

        return true;
    } else {
        return false;
    }
}

using namespace std;

pair<long long, bool> get_hash(QString path_file) {
    std::ifstream file(path_file.toStdString(), std::ios::binary);

    long long hash = 0;
    //QCryptographicHash my_hash(QCryptographicHash::Sha256);
    //QCryptographicHash my_hash(QCryptographicHash::Md5);
    if(file.is_open()) {
        std::string buffer;
        buffer.resize(MAX_BLOCK);

        bool can_read = false;

        do{
            file.read(&buffer[0], MAX_BLOCK);
            buffer.resize(size_t(file.gcount()));

            if (buffer.size() > 0) {
                can_read = true;
            }

            //my_hash.addData(buffer.c_str(), buffer.size());
            hash += std::hash<std::string>{}(buffer);
        } while(!buffer.empty());

        return {hash, true};
        //return {my_hash.result().toLongLong(), true};
    } else {
        return {hash, false};
        //return {my_hash.result().toLongLong(), false};
    }
}

void find_equals(std::vector<QString> &group, std::vector<std::vector<QString>> &group_equals_files) {
    std::vector<char> used(group.size(), false);

    if (group.size() == 2) {
        if (equals_two_files(group[0], group[1])) {
            group_equals_files.push_back({group[0], group[1]});
        }
    } else {
        std::unordered_map<long long, std::vector<QString>> equals_files_in_map;

        for (size_t i = 0; i < group.size(); ++i) {
            auto cur_hash = get_hash(group[i]);
            if (cur_hash.second) {
                equals_files_in_map[cur_hash.first].push_back(group[i]);
            }
        }

        for (auto& item : equals_files_in_map) {
            if (item.second.size() > 1) {
                group_equals_files.push_back(item.second);
            }
        }
    }
}

std::vector<std::vector<QString>> same_files(std::unordered_map<long long, std::vector<QString>> &group_identical_files) {
    std::vector<std::vector<QString>> group_equals_files;
    for (auto& group : group_identical_files) {
        find_equals(group.second, group_equals_files);
    }
    return group_equals_files;
}

std::pair<std::vector<std::vector<QString>>, std::vector<QString>> my::group_identical_files(QString const &dir) {
    std::pair<std::unordered_map<long long, std::vector<QString>>, std::vector<QString>> group_identical_files_and_symlinks;
    find_size(dir, group_identical_files_and_symlinks);
    return {same_files(group_identical_files_and_symlinks.first), group_identical_files_and_symlinks.second};
}
















