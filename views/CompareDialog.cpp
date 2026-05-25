#include "CompareDialog.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QLabel>

CompareDialog::CompareDialog(const QVector<Laptop*> &laptops, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Perbandingan Produk");
    setMinimumSize(700, 480);
    setModal(true);

    auto *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(20, 16, 20, 16);

    auto *title = new QLabel("⚖️ Perbandingan Spesifikasi", this);
    title->setStyleSheet("font-size:17px; font-weight:bold; color:#1E293B;");
    root->addWidget(title);

    // Baris data perbandingan
    struct Row { QString label; QStringList values; };
    QVector<Row> rows;

    // Isi nilai dari setiap laptop
    auto cap = [](int c) { return c >= 1024 ? QString("%1TB").arg(c/1024) : QString("%1GB").arg(c); };

    for (const QString &lbl : {"Nama","Brand","Tipe","CPU","GPU","RAM",
                                "Storage","Display","Port","Harga","Stok","Garansi"}) {
        Row r; r.label = lbl;
        for (Laptop *l : laptops) {
            if      (lbl == "Nama")    r.values << l->getNama();
            else if (lbl == "Brand")   r.values << l->getBrand();
            else if (lbl == "Tipe")    r.values << l->getTipePerangkat();
            else if (lbl == "CPU")     r.values << l->getCPU().toDisplayString();
            else if (lbl == "GPU")     r.values << l->getGpuBrand()+" "+l->getGpuModel();
            else if (lbl == "RAM")     r.values << QString("%1GB %2").arg(l->getRAM().getKapasitas()).arg(l->getRAM().getTipe());
            else if (lbl == "Storage") r.values << cap(l->getStorage().getKapasitas())+" "+l->getStorage().getTipe();
            else if (lbl == "Display") r.values << l->getDisplay().toDisplayString();
            else if (lbl == "Port")    r.values << l->portsString();
            else if (lbl == "Harga")   r.values << l->formatHarga();
            else if (lbl == "Stok")    r.values << QString("%1 unit").arg(l->getStok());
            else if (lbl == "Garansi") r.values << l->getGaransi();
        }
        rows.append(r);
    }

    // Buat tabel
    int cols = laptops.size() + 1;
    auto *tbl = new QTableWidget(rows.size(), cols, this);

    // Header kolom
    QStringList headers = {"Spesifikasi"};
    for (int i = 0; i < laptops.size(); ++i)
        headers << QString("Produk %1").arg(i+1);
    tbl->setHorizontalHeaderLabels(headers);

    tbl->horizontalHeader()->setStyleSheet(
        "QHeaderView::section { background:#3B82F6; color:white;"
        " font-weight:bold; padding:6px; border:none; }");
    tbl->verticalHeader()->setVisible(false);
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbl->setAlternatingRowColors(true);
    tbl->setStyleSheet("QTableWidget { border:1px solid #E2E8F0; font-size:13px; }"
                       "QTableWidget::item { padding:6px; }");
    tbl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    for (int c = 1; c < cols; ++c)
        tbl->horizontalHeader()->setSectionResizeMode(c, QHeaderView::Stretch);

    // Isi data
    for (int r = 0; r < rows.size(); ++r) {
        auto *item = new QTableWidgetItem(rows[r].label);
        item->setFont(QFont("", -1, QFont::Bold));
        item->setBackground(QColor("#F1F5F9"));
        tbl->setItem(r, 0, item);

        for (int c = 0; c < rows[r].values.size(); ++c)
            tbl->setItem(r, c+1, new QTableWidgetItem(rows[r].values[c]));
    }

    root->addWidget(tbl);

    auto *btnTutup = new QPushButton("Tutup", this);
    btnTutup->setFixedHeight(40);
    btnTutup->setStyleSheet(
        "QPushButton { background:#3B82F6; color:white; border-radius:8px;"
        " font-size:14px; border:none; }"
        "QPushButton:hover { background:#2563EB; }");
    root->addWidget(btnTutup);
    connect(btnTutup, &QPushButton::clicked, this, &QDialog::accept);
}
