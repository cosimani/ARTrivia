#ifndef ERROR_HPP
#define ERROR_HPP

#include <QWidget>

namespace Ui
{
    class Error;
}

class Error : public QWidget
{
    Q_OBJECT

private:
    Ui::Error *ui;
    static Error *instance;

public:
    explicit Error( QWidget *parent = NULL );
    static Error *getInstance();

    void setDescription( QString description );
    ~Error();

private slots:
    void slotExit();
};

#endif // ERROR_HPP
