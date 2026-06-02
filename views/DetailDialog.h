#pragma once
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include "../models/Laptop.h"

// =============================================================
//  DetailDialog — Pop-up spesifikasi lengkap satu laptop
//  Menampilkan galeri gambar + spesifikasi via tampilkanInfo()
//  Memanggil tampilkanInfo() → Polymorphism bekerja di sini
//
//  Mode:
//   - Katalog (default): tombol "Tutup" saja
//   - Stok: tombol "Tutup" + "Edit" (sinyal editRequested dipancarkan)
// =============================================================
class DetailDialog : public QDialog {
    Q_OBJECT
public:
    enum class Mode { Katalog, Stok };

    explicit DetailDialog(Laptop *laptop, bool isDark = false,
                          Mode mode = Mode::Katalog, QWidget *parent = nullptr);

signals:
    void editRequested();

private slots:
    void showPrev();
    void showNext();

private:
    void updateImage();

    QStringList  m_images;
    int          m_currentIndex = 0;

    QLabel      *m_imgLabel    = nullptr;
    QLabel      *m_imgCounter  = nullptr;
    QPushButton *m_btnPrev     = nullptr;
    QPushButton *m_btnNext     = nullptr;
};
