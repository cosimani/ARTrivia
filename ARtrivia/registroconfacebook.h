#ifndef REGISTROCONFACEBOOK_H
#define REGISTROCONFACEBOOK_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>


namespace Ui {
class RegistroConFacebook;
}

class RegistroConFacebook : public QWidget
{
    Q_OBJECT

public:
    explicit RegistroConFacebook(QWidget *parent = 0);
    ~RegistroConFacebook();

private:
    Ui::RegistroConFacebook *ui;

    QTimer * timerEnlace;

private slots:
    void slot_autorizar();

    void slot_codigoParaAutorizar(QString code);

    void slot_mostrarEnlace();
};

#endif // REGISTROCONFACEBOOK_H
