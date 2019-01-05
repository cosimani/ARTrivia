#ifndef REGISTROCONMAIL_H
#define REGISTROCONMAIL_H

#include <QWidget>

namespace Ui {
class RegistroConMail;
}

class RegistroConMail : public QWidget
{
    Q_OBJECT

public:
    explicit RegistroConMail(QWidget *parent = 0);
    ~RegistroConMail();

private:
    Ui::RegistroConMail *ui;

private slots:
    bool matchPasswords( QString );
    void lineEditChanged( QString );
    void start();
    bool validateFields();
    void changePasswordEchoMode( bool checked );
    void showData( bool available );

};

#endif // REGISTROCONMAIL_H
