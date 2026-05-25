#pragma once
#include <QDialog>
#include <QVector>
#include "../models/Laptop.h"

// =============================================================
//  CompareDialog — Tabel 3 kolom perbandingan laptop
// =============================================================
class CompareDialog : public QDialog {
    Q_OBJECT
public:
    explicit CompareDialog(const QVector<Laptop*> &laptops, QWidget *parent = nullptr);
};
