#ifndef LOGIN_HPP
#define LOGIN_HPP

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

namespace Ui
{
    class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login( QWidget *parent = NULL );
    ~Login();

private:
    Ui::Login *ui;

protected:
    void resizeEvent( QResizeEvent* );

private slots:
    void lineEditChanged( QString );
    void start();
    bool validateFields();
    void showRegister();
    void showLogin( bool ok );
    void slot_mostrarInstrucciones();
};

#endif // LOGIN_HPP
