#ifndef ABANDONAR_H
#define ABANDONAR_H

#include <QWidget>

namespace Ui {
class Abandonar;
}

class Abandonar : public QWidget
{
    Q_OBJECT

public:
    explicit Abandonar(QWidget *parent = 0);
    ~Abandonar();

private:
    Ui::Abandonar *ui;

private slots:
    void slot_abandonar();
};

#endif // ABANDONAR_H
