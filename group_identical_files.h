#ifndef GROUP_IDENTICAL_FILES_H
#define GROUP_IDENTICAL_FILES_H

#include<vector>
#include<QString>

namespace my {
    struct not_directory_exception {
        not_directory_exception(QString message) : message(message) {}

        QString getMessage() {
            return message;
        }
    private:
        QString message;
    };


    std::pair<std::vector<std::vector<QString>>, std::vector<QString>> group_identical_files(QString const &p);
}

#endif // GROUP_IDENTICAL_FILES_H
