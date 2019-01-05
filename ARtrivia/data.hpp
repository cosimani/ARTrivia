#ifndef DATA_HPP
#define DATA_HPP

#include <QWidget>
#include <QRegExpValidator>
#include <QRegExp>
#include <QPalette>
#include <QLineEdit>

#include "datamanager.hpp"
#include "data.hpp"
#include "game.hpp"
#include "database.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>


namespace Ui
{
    class Data;
}

class Data : public QWidget
{
    Q_OBJECT

private:
    Ui::Data *ui;

    QNetworkAccessManager * manager;
    QNetworkAccessManager * managerCada5;
    QNetworkAccessManager * managerConAccessToken;

    bool isAutorizado;  // Luego de ingresar el codigo en el browser, hay que consultar cada 5 seg para que se autorice.
                        // Cuando esto suceda, entocnes esta variable se pondra en true

    QString longCode;  // Es el codigo largo del primer paso de login.

    QTimer * timer;  // Para consultar cada 5 seg si se autorizo

public:
    explicit Data( QWidget *parent = NULL );
    ~Data();

private slots:
    bool matchPasswords( QString );
    void lineEditChanged( QString );
    void start();
    bool validateFields();
    void changePasswordEchoMode( bool checked );
    void showData( bool available );

    void slot_descargaFinalizada(QNetworkReply *reply);
    void slot_descargaFinalizadaCada5seg(QNetworkReply *reply);
    void slot_descargaFinalizadaConAccessToken(QNetworkReply *);
    void slot_inicioLoginFacebook();
    void slot_consultaCada5seg();
};

#endif // DATA_HPP
