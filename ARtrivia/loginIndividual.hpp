#ifndef LOGININDIVIDUAL_HPP
#define LOGININDIVIDUAL_HPP

#include <QWidget>
#include <QLineEdit>
#include <QPalette>
#include <QResizeEvent>

#include "datamanager.hpp"
#include "game.hpp"
#include "data.hpp"
#include "database.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>

class RegistroConMail;
class RegistroConFacebook;
class Abandonar;

namespace Ui
{
    class LoginIndividual;
}

class LoginIndividual : public QWidget
{
    Q_OBJECT

public:
    explicit LoginIndividual( QWidget *parent = NULL );
    ~LoginIndividual();

private:
    Ui::LoginIndividual *ui;

    QNetworkAccessManager * manager;

    RegistroConMail *registroConMail;
    RegistroConFacebook *registroConFacebook;
    Abandonar * abandonar;

protected:
    void resizeEvent( QResizeEvent* );

private slots:
    void lineEditChanged( QString );
    void start();
    bool validateFields();
    void showLogin( bool ok );
    void slot_mostrarInstrucciones();

    void slot_descargaFinalizada(QNetworkReply *reply);

    void slot_iniciaSesionConFacebook();
    void slot_crearCuentaConMail();

    void slot_abrirWidgetAbandonar();

    void slot_acondicionarInterfaz();
};

#endif // LOGININDIVIDUAL_HPP
