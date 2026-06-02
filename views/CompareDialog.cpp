#include "CompareDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

CompareDialog::CompareDialog(const QVector<Laptop*> &laptops, bool isDark, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Perbandingan Produk");
    setMinimumSize(720, 500);
    setModal(true);

    // ── Warna tema ────────────────────────────────────────────────────────
    const QString bgDialog  = isDark ? "#1E293B" : "#FFFFFF";
    const QString textMain  = isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub   = isDark ? "#94A3B8" : "#475569";
    const QString border    = isDark ? "#334155" : "#E2E8F0";
    const QString rowBg     = isDark ? "#1E293B" : "#FFFFFF";
    const QString rowAlt    = isDark ? "#0F172A" : "#F8FAFC";
    const QString labelBg   = isDark ? "#0F172A" : "#F1F5F9";
    const QString headerBg  = "#3B82F6";
    const QString btnHover  = "#2563EB";

    setStyleSheet(QString("QDialog { background:%1; }").arg(bgDialog));

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(20,16,20,16);

    auto *title = new QLabel("⚖️ Perbandingan Spesifikasi", this);
    title->setStyleSheet(QString("font-size:17px; font-weight:bold; color:%1;").arg(textMain));
    root->addWidget(title);

    struct Row { QString label; QStringList values; };
    QVector<Row> rows;
    auto cap = [](int c) { return c >= 1024 ? QString("%1TB").arg(c/1024) : QString("%1GB").arg(c); };

    for (const QString &lbl : {"Gambar","Nama","Brand","Jenis","CPU","GPU","RAM",
                                "Storage","Display","sRGB","Wireless","Port",
                                "Battery","Harga","Stok","Garansi"}) {
        Row r; r.label = lbl;
        for (Laptop *l : laptops) {
            if      (lbl == "Gambar")   r.values << "";  // diisi via setCellWidget
            else if (lbl == "Nama")     r.values << l->getNama();
            else if (lbl == "Brand")    r.values << l->getBrand();
            else if (lbl == "Jenis")    r.values << l->getTipePerangkat();
            else if (lbl == "CPU")      r.values << l->getCPU().toDisplayString();
            else if (lbl == "GPU")      r.values << l->getGpuBrand()+" "+l->getGpuModel();
            else if (lbl == "RAM")      r.values << QString("%1GB %2").arg(l->getRAM().getKapasitas()).arg(l->getRAM().getTipe());
            else if (lbl == "Storage")  r.values << cap(l->getStorage().getKapasitas())+" "+l->getStorage().getTipe();
            else if (lbl == "Display")  r.values << l->getDisplay().toDisplayString();
            else if (lbl == "sRGB")     r.values << QString("%1%").arg(l->getDisplay().getSrgb());
            else if (lbl == "Wireless") r.values << l->getWireless().toDisplayString();
            else if (lbl == "Port")     r.values << l->portsString();
            else if (lbl == "Battery")  r.values << QString("%1 Wh").arg(l->getBatteryWh());
            else if (lbl == "Harga")    r.values << l->formatHarga();
            else if (lbl == "Stok")     r.values << QString("%1 unit").arg(l->getStok());
            else if (lbl == "Garansi")  r.values << l->getGaransi();
        }
        rows.append(r);
    }

    int cols = laptops.size() + 1;
    auto *tbl = new QTableWidget(rows.size(), cols, this);

    QStringList headers = {"Spesifikasi"};
    for (int i = 0; i < laptops.size(); ++i)
        headers << laptops[i]->getNama();
    tbl->setHorizontalHeaderLabels(headers);

    tbl->setStyleSheet(QString(
        "QTableWidget { border:1px solid %1; font-size:13px; background:%2; color:%3; }"
        "QTableWidget::item { padding:7px; color:%3; background:%2; }"
        "QTableWidget::item:alternate { background:%4; }"
        "QHeaderView::section { background:%5; color:white; font-weight:bold;"
        " padding:8px; border:none; border-right:1px solid #2563EB; }")
        .arg(border, rowBg, textMain, rowAlt, headerBg));

    tbl->verticalHeader()->setVisible(false);
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbl->setAlternatingRowColors(true);
    tbl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    for (int c = 1; c < cols; ++c)
        tbl->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);

    for (int r = 0; r < rows.size(); ++r) {
        auto *item = new QTableWidgetItem(rows[r].label);
        item->setFont(QFont("", -1, QFont::Bold));
        item->setBackground(QColor(labelBg));
        item->setForeground(QColor(textSub));
        tbl->setItem(r, 0, item);

        for (int c = 0; c < rows[r].values.size(); ++c) {
            auto *cell = new QTableWidgetItem(rows[r].values[c]);
            cell->setForeground(QColor(textMain));
            tbl->setItem(r, c+1, cell);
        }

        // Baris Gambar: isi dengan QLabel berisi pixmap
        if (rows[r].label == "Gambar") {
            const int rowH = 140;
            tbl->setRowHeight(r, rowH);
            for (int c = 0; c < laptops.size(); ++c) {
                auto *imgLbl = new QLabel;
                imgLbl->setAlignment(Qt::AlignCenter);
                imgLbl->setStyleSheet(QString("background:%1;").arg(rowBg));
                if (!laptops[c]->getImagePaths().isEmpty()) {
                    QPixmap px(laptops[c]->getImagePaths().first());
                    if (!px.isNull()) {
                        // Skala tinggi gambar = tinggi baris (sisi atas/bawah pas
                        // menempel border baris), lebar menyesuaikan rasio asli.
                        // Tidak ada crop, tidak ada stretch.
                        QPixmap scaled = px.scaledToHeight(rowH, Qt::SmoothTransformation);
                        imgLbl->setPixmap(scaled);
                    } else {
                        imgLbl->setText("📷");
                    }
                } else {
                    imgLbl->setText("📷");
                }
                tbl->setCellWidget(r, c+1, imgLbl);
            }
        }
    }

    root->addWidget(tbl);

    auto *btnTutup = new QPushButton("Tutup", this);
    btnTutup->setFixedHeight(40);
    btnTutup->setStyleSheet(QString(
        "QPushButton { background:%1; color:white; border-radius:8px;"
        " font-size:14px; border:none; }"
        "QPushButton:hover { background:%2; }")
        .arg(headerBg, btnHover));
    root->addWidget(btnTutup);
    connect(btnTutup, &QPushButton::clicked, this, &QDialog::accept);
}
