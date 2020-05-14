#ifndef LOGINMW_HH
#define LOGINMW_HH

#include <QMainWindow>

namespace Ui {
class LoginMW;
}

class LoginMW : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginMW(QWidget *parent = nullptr);
    ~LoginMW();

private:
    Ui::LoginMW *ui;
};

#endif // LOGINMW_HH
