#include "indexedbutton.hpp"

IndexedButton::IndexedButton( QString text, int index, QWidget *parent ) :
    QPushButton( text, parent )
{
    this->setIndex( index );
    connect( this, SIGNAL( pressed() ), SLOT( slotPressed() ) );
}

IndexedButton::~IndexedButton()
{

}

int IndexedButton::getIndex() const
{
    return index;
}

void IndexedButton::setIndex( int value )
{
    index = value;
}

void IndexedButton::slotPressed()
{
    emit pressedWithIndex( this->getIndex() );
}
