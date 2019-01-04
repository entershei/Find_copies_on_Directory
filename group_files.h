/*#ifndef GROUP_FILES_H
#define GROUP_FILES_H

#include <QObject>

class group_files : public QObject {
    struct not_directory_exception {
        not_directory_exception(QString message) : message(message) {}

        QString getMessage() {
            return message;
        }
    private:
        QString message;
    };

    Q_OBJECT
public:
    explicit group_files(QObject *parent = nullptr);

signals:
void send();

public slots:
void work();
};

#endif // GROUP_FILES_H
*/
