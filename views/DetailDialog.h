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
// =============================================================
class DetailDialog : public QDialog {
    Q_OBJECT
public:
    explicit DetailDialog(Laptop *laptop, QWidget *parent = nullptr);

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
