#include "StockDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

StockDialog::StockDialog(Laptop *laptop, bool isDark, QWidget *parent)
    : QDialog(parent), m_laptop(laptop)
{
    setWindowTitle("Kelola Stok " + laptop->getNama());
    setFixedSize(400, 320);
    setModal(true);

    const QString bgDialog  = isDark ? "#1E293B" : "#FFFFFF";
    const QString textMain  = isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub   = isDark ? "#94A3B8" : "#475569";
    const QString inputBg   = isDark ? "#0F172A"  : "#FFFFFF";
    const QString inputBrd  = isDark ? "#334155"  : "#CBD5E1";
    const QString inputTxt  = isDark ? "#F1F5F9"  : "#1E293B";
    const QString btnCnlBg  = isDark ? "#1E293B"  : "#FFFFFF";
    const QString btnCnlBrd = isDark ? "#334155"  : "#CBD5E1";
    const QString btnCnlTxt = isDark ? "#94A3B8"  : "#475569";
    const QString btnCnlHov = isDark ? "#334155"  : "#F1F5F9";
    // Custom checkbox (sama dengan filter di katalog)
    const QString cbBg      = isDark ? "#1E293B"  : "#FFFFFF";
    const QString cbBorder  = isDark ? "#334155"  : "#94A3B8";
    const QString cbCheck   = "#3B82F6";

    setStyleSheet(QString(
        "QDialog { background:%1; }"
        // SpinBox tanpa tombol up/down + warna
        "QSpinBox { border:1px solid %2; border-radius:6px; padding:0 8px;"
        " font-size:14px; background:%3; color:%4; }"
        "QSpinBox::up-button, QSpinBox::down-button { width:0; height:0; border:none; }"
        // Checkbox custom dengan gambar check di tengah
        "QCheckBox { color:%5; font-size:13px; background:transparent; spacing:8px; }"
        "QCheckBox::indicator { width:18px; height:18px; border-radius:4px; }"
        "QCheckBox::indicator:unchecked { border:2px solid %6; background:%7; }"
        "QCheckBox::indicator:checked { border:2px solid %8; background:%8;"
        " image:url(:/images/check_white.png); }")
        .arg(bgDialog, inputBrd, inputBg, inputTxt,
             textMain, cbBorder, cbBg, cbCheck));

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(24, 20, 24, 20);

    auto *title = new QLabel("📦 " + laptop->getNama(), this);
    title->setStyleSheet(QString("font-size:15px; font-weight:bold; color:%1;").arg(textMain));
    title->setWordWrap(true);
    root->addWidget(title);

    m_lblStokInfo = new QLabel(
        QString("Stok saat ini: <b>%1 unit</b>").arg(laptop->getStok()), this);
    m_lblStokInfo->setStyleSheet(QString("color:%1; font-size:13px;").arg(textSub));
    root->addWidget(m_lblStokInfo);

    // Checkbox mutually exclusive (mirip pilihan filter)
    m_chkMasuk   = new QCheckBox("Stok Masuk (barang baru datang)", this);
    m_chkTerjual = new QCheckBox("Stok Terjual (unit habis terjual)", this);
    m_chkMasuk->setChecked(true);
    root->addWidget(m_chkMasuk);
    root->addWidget(m_chkTerjual);

    auto *jumlahRow = new QHBoxLayout;
    auto *lblJml    = new QLabel("Jumlah:", this);
    lblJml->setStyleSheet(QString("color:%1; font-size:13px;").arg(textMain));
    jumlahRow->addWidget(lblJml);

    m_spinJumlah = new QSpinBox(this);
    m_spinJumlah->setMinimum(1);
    m_spinJumlah->setMaximum(9999);
    m_spinJumlah->setValue(1);
    m_spinJumlah->setFixedHeight(36);
    jumlahRow->addWidget(m_spinJumlah);
    jumlahRow->addStretch();
    root->addLayout(jumlahRow);

    m_lblError = new QLabel(this);
    m_lblError->setStyleSheet("color:#EF4444; font-size:12px;");
    m_lblError->hide();
    root->addWidget(m_lblError);
    root->addStretch();

    auto *btnRow   = new QHBoxLayout;
    auto *btnBatal = new QPushButton("Batal", this);
    btnBatal->setFixedHeight(38);
    btnBatal->setStyleSheet(QString(
        "QPushButton { border:1px solid %1; border-radius:8px; background:%2;"
        " color:%3; font-size:13px; }"
        "QPushButton:hover { background:%4; }")
        .arg(btnCnlBrd, btnCnlBg, btnCnlTxt, btnCnlHov));

    auto *btnOk = new QPushButton("Simpan", this);
    btnOk->setFixedHeight(38);
    btnOk->setStyleSheet(
        "QPushButton { background:#059669; color:white; border-radius:8px;"
        " font-size:13px; font-weight:bold; border:none; }"
        "QPushButton:hover { background:#047857; }");

    btnRow->addWidget(btnBatal);
    btnRow->addWidget(btnOk);
    root->addLayout(btnRow);

    connect(btnBatal,    &QPushButton::clicked,    this, &QDialog::reject);
    connect(btnOk,       &QPushButton::clicked,    this, &StockDialog::onKonfirmasi);
    // Mutually exclusive: kalau yang satu di-check, yang lain otomatis uncheck
    connect(m_chkMasuk,   &QCheckBox::toggled, this, [this](bool on){
        if (on) m_chkTerjual->setChecked(false);
        else if (!m_chkTerjual->isChecked()) m_chkMasuk->setChecked(true); // selalu ada 1 terpilih
        onTipeChanged();
    });
    connect(m_chkTerjual, &QCheckBox::toggled, this, [this](bool on){
        if (on) m_chkMasuk->setChecked(false);
        else if (!m_chkMasuk->isChecked()) m_chkTerjual->setChecked(true);
        onTipeChanged();
    });
}

void StockDialog::onTipeChanged() { m_lblError->hide(); }

void StockDialog::onKonfirmasi() {
    int jumlah = m_spinJumlah->value();
    if (m_chkTerjual->isChecked() && jumlah > m_laptop->getStok()) {
        m_lblError->setText(
            QString("❌ Jumlah terjual (%1) melebihi stok tersedia (%2)!")
            .arg(jumlah).arg(m_laptop->getStok()));
        m_lblError->show();
        return;
    }
    accept();
}

QString StockDialog::getTipe()   const { return m_chkMasuk->isChecked() ? "in" : "out"; }
int     StockDialog::getJumlah() const { return m_spinJumlah->value(); }
