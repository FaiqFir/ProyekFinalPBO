#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include "../models/ManajerKatalog.h"

// =============================================================
//  AdminPage — CRUD panel (hanya bisa diakses setelah login)
// =============================================================
class AdminPage : public QWidget {
    Q_OBJECT
public:
    explicit AdminPage(ManajerKatalog *katalog, QWidget *parent = nullptr);

signals:
    void kembali();

public slots:
    void applyTheme(bool isDark);

private slots:
    void onTambah();
    void onEdit();
    void onHapus();
    void onKelolStok();
    void onSelectionChanged();

private:
    void refreshTable();

    ManajerKatalog *m_katalog;
    bool            m_isDark = false;

    QTableWidget   *m_table;
    QPushButton    *m_btnEdit;
    QPushButton    *m_btnHapus;
    QPushButton    *m_btnStok;
    QPushButton    *m_btnBack;
    QLabel         *m_lblTotal;

    QVector<Laptop*> m_displayed;
};
