#include "abandonar.h"
#include "ui_abandonar.h"

#include "database.hpp"
#include <QMessageBox>
#include <QApplication>

Abandonar::Abandonar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Abandonar)
{
    ui->setupUi(this);

    ui->logo->soyLogo();  // Emi
    ui->background->setImage( "background.jpg", FILL );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
    this->setPalette( palette );

    this->setAttribute(Qt::WA_DeleteOnClose);  // Para que se destruya si se llama a close(), no se destruye con hide()

    ui->pbBorrar->setFixedHeight( 2 * ui->pbBorrar->sizeHint().height() );

    connect( ui->pbBorrar, SIGNAL( pressed() ), this, SLOT( slot_abandonar() ) );
}

Abandonar::~Abandonar()
{
    delete ui;
}

void Abandonar::slot_abandonar()
{
    Database::getInstance()->borrarTablas();

    QMessageBox::information( this, "Información", "Todos los datos tuyos han sido borrados de esta App." );

    qApp->quit();
}
