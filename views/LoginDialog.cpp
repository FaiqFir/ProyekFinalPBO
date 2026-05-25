#include "LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Login Admin");
    setFixedSize(360, 220);
    setModal(true);

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(28, 24, 28, 24);

    // Judul
    auto *title = new QLabel("🔒 Area Admin", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:18px; font-weight:bold; color:#7C3AED;");
    root->addWidget(title);

    auto *sub = new QLabel("Masukkan password untuk melanjutkan", this);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color:#64748B; font-size:13px;");
    root->addWidget(sub);

    root->addSpacing(4);

    // Input password
    m_pwdEdit = new QLineEdit(this);
    m_pwdEdit->setEchoMode(QLineEdit::Password);
    m_pwdEdit->setPlaceholderText("Password...");
    m_pwdEdit->setFixedHeight(40);
    m_pwdEdit->setStyleSheet(
        "QLineEdit { border:1px solid #CBD5E1; border-radius:8px;"
        " padding:0 12px; font-size:14px; }"
        "QLineEdit:focus { border-color:#8B5CF6; }");
    root->addWidget(m_pwdEdit);

    // Label error
    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color:#DC2626; font-size:12px;");
    m_errorLabel->hide();
    root->addWidget(m_errorLabel);

    // Tombol
    auto *btnRow = new QHBoxLayout;
    auto *btnBatal = new QPushButton("Batal", this);
    btnBatal->setFixedHeight(38);
    btnBatal->setStyleSheet(
        "QPushButton { border:1px solid #CBD5E1; border-radius:8px;"
        " background:white; color:#475569; font-size:14px; }"
        "QPushButton:hover { background:#F1F5F9; }");

    auto *btnLogin = new QPushButton("Login", this);
    btnLogin->setFixedHeight(38);
    btnLogin->setStyleSheet(
        "QPushButton { background:#8B5CF6; color:white; border-radius:8px;"
        " font-size:14px; font-weight:bold; border:none; }"
        "QPushButton:hover { background:#7C3AED; }");

    btnRow->addWidget(btnBatal);
    btnRow->addWidget(btnLogin);
    root->addLayout(btnRow);

    // Hint
    auto *hint = new QLabel("Password default: admin123", this);
    hint->setAlignment(Qt::AlignCenter);
    hint->setStyleSheet("color:#94A3B8; font-size:11px;");
    root->addWidget(hint);

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
