#pragma once
#include <QDialog>
#include <QVector>
#include "../models/Laptop.h"

class CompareDialog : public QDialog {
    Q_OBJECT
public:
    explicit CompareDialog(const QVector<Laptop*> &laptops, bool isDark, QWidget *parent = nullptr);
};
