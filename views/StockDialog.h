#pragma once
#include <QDialog>
#include <QSpinBox>
#include <QRadioButton>
#include <QLabel>
#include "../models/Laptop.h"

// =============================================================
//  StockDialog — Dialog update stok masuk / terjual
// =============================================================
class StockDialog : public QDialog {
    Q_OBJECT
public:
    explicit StockDialog(Laptop *laptop, QWidget *parent = nullptr);

    QString getTipe()   const; // "in" atau "out"
    int     getJumlah() const;

private slots:
    void onKonfirmasi();
    void onTipeChanged();

private:
    Laptop       *m_laptop;
    QRadioButton *m_rbMasuk;
    QRadioButton *m_rbTerjual;
    QSpinBox     *m_spinJumlah;
    QLabel       *m_lblStokInfo;
    QLabel       *m_lblError;
};
