#ifndef INDEXEDBUTTON_HPP
#define INDEXEDBUTTON_HPP

#include <QPushButton>

class IndexedButton : public QPushButton
{
    Q_OBJECT

private:

    int index;

public:

    explicit IndexedButton( QString text = "", int index = -1, QWidget *parent = NULL );
    ~IndexedButton();

    int getIndex() const;
    void setIndex( int value );

private slots:

    void slotPressed();

signals:

    void pressedWithIndex( int index );
};

#endif // INDEXEDBUTTON_HPP
