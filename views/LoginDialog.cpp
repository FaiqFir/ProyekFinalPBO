#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

LoginDialog::LoginDialog(bool isDark, QWidget *parent) : QDialog(parent) {
    setWindowTitle("Login Admin");
    setFixedSize(360, 230);
    setModal(true);

    // ── Warna sesuai tema ─────────────────────────────────────────────────
    const QString bgDialog  = isDark ? "#1E293B" : "#FFFFFF";
    const QString textMain  = isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub   = isDark ? "#94A3B8" : "#64748B";
    const QString textHint  = isDark ? "#475569"  : "#94A3B8";
    const QString inputBg   = isDark ? "#0F172A"  : "#FFFFFF";
    const QString inputBrd  = isDark ? "#334155"  : "#CBD5E1";
    const QString inputFocus= isDark ? "#8B5CF6"  : "#8B5CF6";
    const QString inputTxt  = isDark ? "#F1F5F9"  : "#1E293B";
    const QString btnCancelBg  = isDark ? "#1E293B" : "#FFFFFF";
    const QString btnCancelBrd = isDark ? "#334155" : "#CBD5E1";
    const QString btnCancelTxt = isDark ? "#94A3B8" : "#475569";
    const QString btnCancelHov = isDark ? "#334155" : "#F1F5F9";
    const QString accentColor  = "#8B5CF6";
    const QString accentHover  = "#7C3AED";

    setStyleSheet(QString("QDialog { background:%1; }").arg(bgDialog));

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(28,24,28,24);

    auto *title = new QLabel("⚿ Area Admin", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(QString("font-size:18px; font-weight:bold; color:%1;").arg(accentColor));
    root->addWidget(title);

    auto *sub = new QLabel("Masukkan password untuk melanjutkan", this);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet(QString("color:%1; font-size:13px;").arg(textSub));
    root->addWidget(sub);

    root->addSpacing(4);

    m_pwdEdit = new QLineEdit(this);
    m_pwdEdit->setEchoMode(QLineEdit::Password);
    m_pwdEdit->setPlaceholderText("Password...");
    m_pwdEdit->setFixedHeight(40);
    m_pwdEdit->setStyleSheet(QString(
        "QLineEdit { border:1px solid %1; border-radius:8px; padding:0 12px;"
        " font-size:14px; background:%2; color:%3; }"
        "QLineEdit:focus { border-color:%4; }")
        .arg(inputBrd, inputBg, inputTxt, inputFocus));
    root->addWidget(m_pwdEdit);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color:#EF4444; font-size:12px;");
    m_errorLabel->hide();
    root->addWidget(m_errorLabel);

    auto *btnRow  = new QHBoxLayout;
    auto *btnBatal = new QPushButton("Batal", this);
    btnBatal->setFixedHeight(38);
    btnBatal->setStyleSheet(QString(
        "QPushButton { border:1px solid %1; border-radius:8px; background:%2;"
        " color:%3; font-size:14px; }"
        "QPushButton:hover { background:%4; }")
        .arg(btnCancelBrd, btnCancelBg, btnCancelTxt, btnCancelHov));

    auto *btnLogin = new QPushButton("Login", this);
    btnLogin->setFixedHeight(38);
    btnLogin->setStyleSheet(QString(
        "QPushButton { background:%1; color:white; border-radius:8px;"
        " font-size:14px; font-weight:bold; border:none; }"
        "QPushButton:hover { background:%2; }")
        .arg(accentColor, accentHover));

    btnRow->addWidget(btnBatal);
    btnRow->addWidget(btnLogin);
    root->addLayout(btnRow);



    connect(btnBatal, &QPushButton::clicked, this, &QDialog::reject);
    connect(btnLogin, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(m_pwdEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLogin);
}

void LoginDialog::onLogin() {
    if (m_pwdEdit->text() == PASSWORD) {
        m_berhasil = true;
        accept();
    } else {
        m_errorLabel->setText("❌ Password salah. Coba lagi.");
        m_errorLabel->show();
        m_pwdEdit->clear();
        m_pwdEdit->setFocus();
    }
}
