#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QLabel>

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(bool isDark, QWidget *parent = nullptr);
    bool loginBerhasil() const { return m_berhasil; }

private slots:
    void onLogin();

private:
    QLineEdit *m_pwdEdit;
    QLabel    *m_errorLabel;
    bool       m_berhasil = false;
    static constexpr const char* PASSWORD = "admin123";
};
