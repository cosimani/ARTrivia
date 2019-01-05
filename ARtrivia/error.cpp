#include "error.hpp"
#include "ui_error.h"
#include <QPalette>

Error *Error::instance = NULL;

Error::Error( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::Error )
{
    ui->setupUi( this );

    QPalette palette = this->palette();
    palette.setColor( QPalette::WindowText, QColor( 255, 255, 255 ) );
//    palette.setColor( QPalette::Background, QColor( "#A90230" ) );
    palette.setColor( QPalette::Background, QColor( "#ACB353" ) );
    this->setPalette( palette );

    connect( ui->okButton, SIGNAL( pressed() ), SLOT( slotExit() ) );
    this->show();
}

Error *Error::getInstance()
{
    if( !instance )
    {
        instance = new Error();
    }
    return instance;
}

Error::~Error()
{
    delete ui;
    delete instance;
}

void Error::setDescription( QString description )
{
    ui->descriptionLabel->setText( description );
}

void Error::slotExit()
{
    qApp->quit();
}
