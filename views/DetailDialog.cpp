#include "DetailDialog.h"
#include "../models/LaptopGaming.h"
#include "../models/LaptopDesain.h"
#include "../models/LaptopWorkstation.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QFrame>
#include <QPixmap>
#include <QLocale>
#include <QScrollArea>

DetailDialog::DetailDialog(Laptop *laptop, bool isDark, Mode mode, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Detail Produk " + laptop->getNama());
    setMinimumSize(520, 600);
    setModal(true);

    m_images = laptop->getImagePaths();

    // ── Warna tema ────────────────────────────────────────────────────────
    const QString bgDialog  = isDark ? "#1E293B" : "#FFFFFF";
    const QString bgContent = isDark ? "#0F172A" : "#F8FAFC";
    const QString textMain  = isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub   = isDark ? "#94A3B8" : "#475569";
    const QString borderClr = isDark ? "#334155" : "#E2E8F0";
    const QString galleryBg = isDark ? "#0F172A" : "#F1F5F9";
    const QString navBtnBg  = isDark ? "#1E293B" : "#FFFFFF";
    const QString navBtnBrd = isDark ? "#334155" : "#CBD5E1";
    const QString navBtnTxt = isDark ? "#94A3B8" : "#475569";
    const QString navBtnHov = isDark ? "#334155" : "#E2E8F0";
    const QString infoBg    = isDark ? "#0F172A" : "#F8FAFC";
    const QString infoTxt   = isDark ? "#CBD5E1" : "#334155";
    const QString hargaTxt  = isDark ? "#34D399" : "#059669";
    const QString stokTxt   = isDark ? "#94A3B8" : "#475569";
    const QString divClr    = isDark ? "#334155" : "#E2E8F0";

    setStyleSheet(QString("QDialog { background:%1; }").arg(bgDialog));

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(24, 20, 24, 20);

    // ── Header nama & badge ───────────────────────────────────────────────
    bool isGaming      = dynamic_cast<LaptopGaming*>(laptop)      != nullptr;
    bool isDesain      = dynamic_cast<LaptopDesain*>(laptop)      != nullptr;
    bool isWorkstation = dynamic_cast<LaptopWorkstation*>(laptop) != nullptr;

    QString badge, badgeBg;
    if      (isGaming)      { badge = "\U0001F3AE Laptop Gaming";      badgeBg = "#EF4444"; }
    else if (isWorkstation) { badge = "\U0001F4BC Laptop Workstation"; badgeBg = "#7C3AED"; }
    else if (isDesain)      { badge = "\U0001F3A8 Laptop Desain";      badgeBg = "#0891B2"; }
    else                    { badge = "\U0001F4BB Laptop";             badgeBg = "#3B82F6"; }

    auto *lblNama = new QLabel(laptop->getNama(), this);
    lblNama->setStyleSheet(QString("font-size:18px; font-weight:bold; color:%1;"
                                   " background:transparent;").arg(textMain));
    lblNama->setWordWrap(true);
    root->addWidget(lblNama);

    auto *lblBadge = new QLabel(badge, this);
    lblBadge->setStyleSheet(
        QString("color:white; background:%1; border-radius:6px;"
                " padding:2px 10px; font-size:12px; font-weight:bold;").arg(badgeBg));
    lblBadge->setFixedWidth(170);
    root->addWidget(lblBadge);

    // ── Galeri Gambar ─────────────────────────────────────────────────────
    if (!m_images.isEmpty()) {
        auto *galleryWidget = new QWidget(this);
        galleryWidget->setStyleSheet(
            QString("QWidget { background:%1; border-radius:10px; }").arg(galleryBg));
        auto *galleryLayout = new QVBoxLayout(galleryWidget);
        galleryLayout->setContentsMargins(0, 0, 0, 8);
        galleryLayout->setSpacing(6);

        // Label gambar utama — mengisi penuh, sisi atas & bawah pas border
        m_imgLabel = new QLabel(galleryWidget);
        m_imgLabel->setAlignment(Qt::AlignCenter);
        m_imgLabel->setFixedHeight(220);
        m_imgLabel->setStyleSheet(QString(
            "background:%1; border-radius:10px 10px 0 0;").arg(galleryBg));
        galleryLayout->addWidget(m_imgLabel);

        // Row navigasi: < 1/4 >
        auto *navRow = new QHBoxLayout;
        navRow->setSpacing(8);
        navRow->setContentsMargins(12, 0, 12, 0);

        QString navStyle = QString(
            "QPushButton { background:%1; border:1px solid %2;"
            " border-radius:16px; font-size:14px; color:%3; }"
            "QPushButton:hover { background:%4; }"
            "QPushButton:disabled { color:%2; border-color:%2; }")
            .arg(navBtnBg, navBtnBrd, navBtnTxt, navBtnHov);

        m_btnPrev = new QPushButton("\u276E", galleryWidget);
        m_btnPrev->setFixedSize(32, 32);
        m_btnPrev->setStyleSheet(navStyle);

        m_btnNext = new QPushButton("\u276F", galleryWidget);
        m_btnNext->setFixedSize(32, 32);
        m_btnNext->setStyleSheet(navStyle);

        m_imgCounter = new QLabel(galleryWidget);
        m_imgCounter->setAlignment(Qt::AlignCenter);
        m_imgCounter->setStyleSheet(
            QString("font-size:12px; color:%1; min-width:40px;"
                    " background:transparent;").arg(textSub));

        navRow->addStretch();
        navRow->addWidget(m_btnPrev);
        navRow->addWidget(m_imgCounter);
        navRow->addWidget(m_btnNext);
        navRow->addStretch();
        galleryLayout->addLayout(navRow);

        root->addWidget(galleryWidget);

        updateImage();

        if (m_images.size() == 1) {
            m_btnPrev->hide();
            m_btnNext->hide();
            m_imgCounter->hide();
        }

        connect(m_btnPrev, &QPushButton::clicked, this, &DetailDialog::showPrev);
        connect(m_btnNext, &QPushButton::clicked, this, &DetailDialog::showNext);
    }

    // ── Garis pemisah ─────────────────────────────────────────────────────
    auto *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet(QString("color:%1;").arg(divClr));
    root->addWidget(line);

    // ── Info spesifikasi ──────────────────────────────────────────────────
    auto *txtInfo = new QTextEdit(this);
    txtInfo->setReadOnly(true);
    txtInfo->setPlainText(laptop->tampilkanInfo());
    txtInfo->setStyleSheet(QString(
        "QTextEdit { border:1px solid %1; border-radius:8px;"
        " background:%2; padding:12px; font-size:13px;"
        " font-family: Consolas, monospace; color:%3; }")
        .arg(borderClr, infoBg, infoTxt));
    root->addWidget(txtInfo);

    // ── Harga & stok ──────────────────────────────────────────────────────
    auto *hargaRow = new QHBoxLayout;
    auto *lblHarga = new QLabel("\U0001F4B0 " + laptop->formatHarga(), this);
    lblHarga->setStyleSheet(QString("font-size:16px; font-weight:bold; color:%1;"
                                    " background:transparent;").arg(hargaTxt));

    auto *lblStok = new QLabel(
        QString("\U0001F4E6 Stok: %1 unit").arg(laptop->getStok()), this);
    lblStok->setStyleSheet(QString("font-size:13px; color:%1;"
                                   " background:transparent;").arg(stokTxt));

    hargaRow->addWidget(lblHarga);
    hargaRow->addStretch();
    hargaRow->addWidget(lblStok);
    root->addLayout(hargaRow);

    // ── Tombol bawah ──────────────────────────────────────────────────────
    auto *btnRow = new QHBoxLayout;
    btnRow->setSpacing(8);

    auto *btnTutup = new QPushButton("Tutup", this);
    btnTutup->setFixedHeight(40);

    if (mode == Mode::Stok) {
        // Mode stok: Tutup di kiri (warna netral) + Edit di kanan (biru)
        const QString btnCnlBg  = isDark ? "#1E293B"  : "#FFFFFF";
        const QString btnCnlBrd = isDark ? "#334155"  : "#CBD5E1";
        const QString btnCnlTxt = isDark ? "#94A3B8"  : "#475569";
        const QString btnCnlHov = isDark ? "#334155"  : "#F1F5F9";
        btnTutup->setStyleSheet(QString(
            "QPushButton { background:%1; color:%2; border:1px solid %3; border-radius:8px;"
            " font-size:14px; }"
            "QPushButton:hover { background:%4; }")
            .arg(btnCnlBg, btnCnlTxt, btnCnlBrd, btnCnlHov));

        auto *btnEdit = new QPushButton("Edit", this);
        btnEdit->setFixedHeight(40);
        btnEdit->setStyleSheet(
            "QPushButton { background:#3B82F6; color:white; border-radius:8px;"
            " font-size:14px; font-weight:bold; border:none; }"
            "QPushButton:hover { background:#2563EB; }");

        btnRow->addWidget(btnTutup);
        btnRow->addWidget(btnEdit);
        root->addLayout(btnRow);

        connect(btnTutup, &QPushButton::clicked, this, &QDialog::reject);
        connect(btnEdit,  &QPushButton::clicked, this, [this]{
            emit editRequested();
            accept();
        });
    } else {
        // Mode katalog: hanya Tutup (biru, lebar penuh)
        btnTutup->setStyleSheet(
            "QPushButton { background:#3B82F6; color:white; border-radius:8px;"
            " font-size:14px; border:none; }"
            "QPushButton:hover { background:#2563EB; }");
        root->addWidget(btnTutup);
        connect(btnTutup, &QPushButton::clicked, this, &QDialog::accept);
    }
}

void DetailDialog::showPrev()
{
    if (m_currentIndex > 0) { m_currentIndex--; updateImage(); }
}

void DetailDialog::showNext()
{
    if (m_currentIndex < m_images.size() - 1) { m_currentIndex++; updateImage(); }
}

void DetailDialog::updateImage()
{
    QPixmap px(m_images[m_currentIndex]);
    if (px.isNull()) {
        m_imgLabel->setText("\U0001F4F7  Gambar belum tersedia");
        m_imgLabel->setStyleSheet(m_imgLabel->styleSheet()
                                  + " color:#94A3B8; font-size:13px;");
    } else {
        int h = 220;
        // Scale agar tinggi pas = 220, lebar menyesuaikan — tidak ada crop, tidak stretch
        QPixmap scaled = px.scaledToHeight(h, Qt::SmoothTransformation);
        m_imgLabel->setPixmap(scaled);
    }

    if (m_imgCounter)
        m_imgCounter->setText(
            QString("%1 / %2").arg(m_currentIndex + 1).arg(m_images.size()));
    if (m_btnPrev) m_btnPrev->setEnabled(m_currentIndex > 0);
    if (m_btnNext) m_btnNext->setEnabled(m_currentIndex < m_images.size() - 1);
}
