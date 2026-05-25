#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QApplication>
#include "../models/ManajerKatalog.h"

// =============================================================
//  MainWindow — Jendela utama aplikasi
//  Mengelola navigasi antar halaman:
//  0 = Halaman Menu Awal
//  1 = CatalogPage
//  2 = AdminPage
// =============================================================
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool isDarkMode() const { return m_isDark; }

signals:
    void themeChanged(bool isDark);

private slots:
    void keHalamanKatalog();
    void keHalamanAdmin();
    void keMenuAwal();
    void toggleDarkMode();

private:
    void buildMenuAwal();
    void applyTheme();

    ManajerKatalog  *m_katalog;
    QStackedWidget  *m_stack;
    QPushButton     *m_btnTheme = nullptr;
    bool             m_isDark   = false;
};
