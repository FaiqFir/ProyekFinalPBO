#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QApplication>
#include "../models/ManajerKatalog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool isDarkMode() const { return m_isDark; }

signals:
    void themeChanged(bool isDark);

public slots:
    Q_INVOKABLE void toggleDarkMode();

private slots:
    void keHalamanKatalog();
    void keHalamanAdmin();
    void keMenuAwal();

private:
    void buildMenuAwal();
    void applyTheme();

    ManajerKatalog  *m_katalog;
    QStackedWidget  *m_stack;
    QPushButton     *m_btnTheme = nullptr;
    bool             m_isDark   = false;
};
