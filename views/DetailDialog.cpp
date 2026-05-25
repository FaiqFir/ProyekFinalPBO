#include "DetailDialog.h"
#include "../models/LaptopGaming.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QFrame>
#include <QPixmap>
#include <QLocale>

DetailDialog::DetailDialog(Laptop *laptop, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Detail Produk \u2014 " + laptop->getNama());
    setMinimumSize(500, 580);
    setModal(true);

    m_images = laptop->getImagePaths();

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(24, 20, 24, 20);

    // ── Header nama & badge ───────────────────────────────────────────────
    bool isGaming = dynamic_cast<LaptopGaming*>(laptop) != nullptr;
    QString badge = isGaming ? "\U0001F3AE Laptop Gaming" : "\U0001F4BB Laptop";

    auto *lblNama = new QLabel(laptop->getNama(), this);
    lblNama->setStyleSheet("font-size:18px; font-weight:bold; color:#1E293B;");
    root->addWidget(lblNama);

    auto *lblBadge = new QLabel(badge, this);
    lblBadge->setStyleSheet(
        isGaming
        ? "color:white; background:#EF4444; border-radius:6px;"
          " padding:2px 10px; font-size:12px; font-weight:bold;"
        : "color:white; background:#3B82F6; border-radius:6px;"
          " padding:2px 10px; font-size:12px; font-weight:bold;");
    lblBadge->setFixedWidth(130);
    root->addWidget(lblBadge);

    // ── Galeri Gambar (hanya tampil jika ada gambar) ──────────────────────
    if (!m_images.isEmpty()) {
        auto *galleryWidget = new QWidget(this);
        galleryWidget->setStyleSheet(
            "QWidget { background:#F1F5F9; border-radius:10px; }");
        auto *galleryLayout = new QVBoxLayout(galleryWidget);
        galleryLayout->setContentsMargins(8, 8, 8, 8);
        galleryLayout->setSpacing(6);

        // Label gambar utama
        m_imgLabel = new QLabel(galleryWidget);
        m_imgLabel->setAlignment(Qt::AlignCenter);
        m_imgLabel->setFixedHeight(210);
        m_imgLabel->setStyleSheet("background:transparent;");
        galleryLayout->addWidget(m_imgLabel);

        // Row navigasi: < 1/4 >
        auto *navRow = new QHBoxLayout;
        navRow->setSpacing(8);

        QString navStyle =
            "QPushButton { background:white; border:1px solid #CBD5E1;"
            " border-radius:16px; font-size:14px; color:#475569; }"
            "QPushButton:hover { background:#E2E8F0; }"
            "QPushButton:disabled { color:#CBD5E1; border-color:#E2E8F0; }";

        m_btnPrev = new QPushButton("\u276E", galleryWidget);
        m_btnPrev->setFixedSize(32, 32);
        m_btnPrev->setStyleSheet(navStyle);

        m_btnNext = new QPushButton("\u276F", galleryWidget);
        m_btnNext->setFixedSize(32, 32);
        m_btnNext->setStyleSheet(navStyle);

        m_imgCounter = new QLabel(galleryWidget);
        m_imgCounter->setAlignment(Qt::AlignCenter);
        m_imgCounter->setStyleSheet("font-size:12px; color:#64748B; min-width:40px;");

        navRow->addStretch();
        navRow->addWidget(m_btnPrev);
        navRow->addWidget(m_imgCounter);
        navRow->addWidget(m_btnNext);
        navRow->addStretch();
        galleryLayout->addLayout(navRow);

        root->addWidget(galleryWidget);

        // Tampilkan gambar pertama
        updateImage();

        // Sembunyikan nav jika hanya 1 gambar
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
    line->setStyleSheet("color:#E2E8F0;");
    root->addWidget(line);

    // ── Info spesifikasi (Polymorphism) ───────────────────────────────────
    auto *txtInfo = new QTextEdit(this);
    txtInfo->setReadOnly(true);
    txtInfo->setPlainText(laptop->tampilkanInfo());
    txtInfo->setStyleSheet(
        "QTextEdit { border:1px solid #E2E8F0; border-radius:8px;"
        " background:#F8FAFC; padding:12px; font-size:13px;"
        " font-family: Consolas, monospace; color:#334155; }");
    root->addWidget(txtInfo);

    // ── Harga & stok ──────────────────────────────────────────────────────
    auto *hargaRow = new QHBoxLayout;
    auto *lblHarga = new QLabel("\U0001F4B0 " + laptop->formatHarga(), this);
    lblHarga->setStyleSheet("font-size:16px; font-weight:bold; color:#059669;");

    auto *lblStok = new QLabel(
        QString("\U0001F4E6 Stok: %1 unit").arg(laptop->getStok()), this);
    lblStok->setStyleSheet("font-size:13px; color:#475569;");

    hargaRow->addWidget(lblHarga);
    hargaRow->addStretch();
    hargaRow->addWidget(lblStok);
    root->addLayout(hargaRow);

    // ── Tombol tutup ──────────────────────────────────────────────────────
    auto *btnTutup = new QPushButton("Tutup", this);
    btnTutup->setFixedHeight(40);
    btnTutup->setStyleSheet(
        "QPushButton { background:#3B82F6; color:white; border-radius:8px;"
        " font-size:14px; border:none; }"
        "QPushButton:hover { background:#2563EB; }");
    root->addWidget(btnTutup);
    connect(btnTutup, &QPushButton::clicked, this, &QDialog::accept);
}

// ── Navigasi gambar ───────────────────────────────────────────────────────────
void DetailDialog::showPrev()
{
    if (m_currentIndex > 0) {
        m_currentIndex--;
        updateImage();
    }
}

void DetailDialog::showNext()
{
    if (m_currentIndex < m_images.size() - 1) {
        m_currentIndex++;
        updateImage();
    }
}

void DetailDialog::updateImage()
{
    QPixmap px(m_images[m_currentIndex]);
    if (px.isNull()) {
        m_imgLabel->setText("\U0001F4F7  Gambar belum tersedia");
        m_imgLabel->setStyleSheet("color:#94A3B8; font-size:13px; background:transparent;");
    } else {
        m_imgLabel->setPixmap(
            px.scaled(m_imgLabel->width() > 0 ? m_imgLabel->width() : 460,
                      210,
                      Qt::KeepAspectRatio,
                      Qt::SmoothTransformation));
        m_imgLabel->setStyleSheet("background:transparent;");
    }

    // Update counter "1 / 4"
    if (m_imgCounter)
        m_imgCounter->setText(
            QString("%1 / %2").arg(m_currentIndex + 1).arg(m_images.size()));

    // Nonaktifkan tombol di ujung
    if (m_btnPrev) m_btnPrev->setEnabled(m_currentIndex > 0);
    if (m_btnNext) m_btnNext->setEnabled(m_currentIndex < m_images.size() - 1);
}
