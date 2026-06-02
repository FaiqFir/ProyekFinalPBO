#pragma once
#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QLabel>
#include "../models/Laptop.h"

class StockDialog : public QDialog {
    Q_OBJECT
public:
    explicit StockDialog(Laptop *laptop, bool isDark, QWidget *parent = nullptr);

    QString getTipe()   const;
    int     getJumlah() const;

private slots:
    void onKonfirmasi();
    void onTipeChanged();

private:
    Laptop    *m_laptop;
    QCheckBox *m_chkMasuk;
    QCheckBox *m_chkTerjual;
    QSpinBox  *m_spinJumlah;
    QLabel    *m_lblStokInfo;
    QLabel    *m_lblError;
};
