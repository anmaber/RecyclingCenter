#ifndef LOGINWINDOW_HH
#define LOGINWINDOW_HH

#include <QDialog>
#include <QString>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    bool LoginData(QString);
    void QWERTY(QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_HH
