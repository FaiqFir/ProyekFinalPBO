#include "StockDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

StockDialog::StockDialog(Laptop *laptop, QWidget *parent)
    : QDialog(parent), m_laptop(laptop)
{
    setWindowTitle("Kelola Stok — " + laptop->getNama());
    setFixedSize(380, 300);
    setModal(true);

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(24, 20, 24, 20);

    auto *title = new QLabel("📦 " + laptop->getNama(), this);
    title->setStyleSheet("font-size:15px; font-weight:bold; color:#1E293B;");
    title->setWordWrap(true);
    root->addWidget(title);

    m_lblStokInfo = new QLabel(
        QString("Stok saat ini: <b>%1 unit</b>").arg(laptop->getStok()), this);
    m_lblStokInfo->setStyleSheet("color:#475569; font-size:13px;");
    root->addWidget(m_lblStokInfo);

    // Pilihan tipe
    auto *grpTipe = new QButtonGroup(this);
    m_rbMasuk   = new QRadioButton("📥 Stok Masuk (barang baru datang)", this);
    m_rbTerjual = new QRadioButton("📤 Stok Terjual (unit habis terjual)", this);
    m_rbMasuk->setChecked(true);
    grpTipe->addButton(m_rbMasuk);
    grpTipe->addButton(m_rbTerjual);

    root->addWidget(m_rbMasuk);
    root->addWidget(m_rbTerjual);

    // Jumlah
    auto *jumlahRow = new QHBoxLayout;
    jumlahRow->addWidget(new QLabel("Jumlah:", this));
    m_spinJumlah = new QSpinBox(this);
    m_spinJumlah->setMinimum(1);
    m_spinJumlah->setMaximum(9999);
    m_spinJumlah->setValue(1);
    m_spinJumlah->setFixedHeight(36);
    m_spinJumlah->setStyleSheet(
        "QSpinBox { border:1px solid #CBD5E1; border-radius:6px; padding:0 8px; font-size:14px; }");
    jumlahRow->addWidget(m_spinJumlah);
    jumlahRow->addStretch();
    root->addLayout(jumlahRow);

    // Label error/validasi
    m_lblError = new QLabel(this);
    m_lblError->setStyleSheet("color:#DC2626; font-size:12px;");
    m_lblError->hide();
    root->addWidget(m_lblError);

    root->addStretch();

    // Tombol
    auto *btnRow = new QHBoxLayout;
    auto *btnBatal = new QPushButton("Batal", this);
    btnBatal->setFixedHeight(38);
    btnBatal->setStyleSheet(
        "QPushButton { border:1px solid #CBD5E1; border-radius:8px;"
        " background:white; color:#475569; font-size:13px; }"
        "QPushButton:hover { background:#F1F5F9; }");

    auto *btnOk = new QPushButton("Simpan", this);
    btnOk->setFixedHeight(38);
    btnOk->setStyleSheet(
        "QPushButton { background:#059669; color:white; border-radius:8px;"
        " font-size:13px; font-weight:bold; border:none; }"
        "QPushButton:hover { background:#047857; }");

    btnRow->addWidget(btnBatal);
    btnRow->addWidget(btnOk);
    root->addLayout(btnRow);

    connect(btnBatal,  &QPushButton::clicked, this, &QDialog::reject);
    connect(btnOk,     &QPushButton::clicked, this, &StockDialog::onKonfirmasi);
    connect(m_rbMasuk,   &QRadioButton::toggled, this, &StockDialog::onTipeChanged);
    connect(m_rbTerjual, &QRadioButton::toggled, this, &StockDialog::onTipeChanged);
}

void StockDialog::onTipeChanged() {
    m_lblError->hide();
}

void StockDialog::onKonfirmasi() {
    int jumlah = m_spinJumlah->value();

    // Validasi: stok terjual tidak boleh melebihi stok tersedia
    if (m_rbTerjual->isChecked() && jumlah > m_laptop->getStok()) {
        m_lblError->setText(
            QString("❌ Jumlah terjual (%1) melebihi stok tersedia (%2)!")
            .arg(jumlah).arg(m_laptop->getStok()));
        m_lblError->show();
        return;
    }
    accept();
}

QString StockDialog::getTipe() const {
    return m_rbMasuk->isChecked() ? "in" : "out";
}

int StockDialog::getJumlah() const {
    return m_spinJumlah->value();
}
