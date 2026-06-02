#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QStringList>
#include "../models/ManajerKatalog.h"

class CatalogPage : public QWidget {
    Q_OBJECT
public:
    explicit CatalogPage(ManajerKatalog *katalog, QWidget *parent = nullptr);

signals:
    void kembali();

public slots:
    void applyTheme(bool isDark);

private slots:
    void onFilterChanged();
    void onSortChanged(int index);
    void onCompareModeToggled();
    void onLihatPerbandingan();
    void onItemDoubleClicked(int row, int col);
    void onCheckboxCompareChanged(const QString &id, bool checked);

private:
    void buildFilterPanel(QWidget *panel);
    void buildTablePanel(QWidget *panel);
    void refreshTable();
    void updateCompareBadge();

    ManajerKatalog *m_katalog;
    bool            m_isDark = false;

    // Filter state
    QStringList m_filterJenis;
    QString     m_filterCpuSeri;
    QString     m_filterGpuBrand;
    QString     m_filterRamTipe;
    int         m_filterRamKap    = 0;
    QString     m_filterStorTipe;
    int         m_filterStorKap   = 0;
    QString     m_filterDispTipe;
    QString     m_filterWifi;
    QString     m_filterBt;
    QStringList m_filterPorts;

    ManajerKatalog::SortMode m_sortMode = ManajerKatalog::SortMode::None;

    bool        m_compareMode = false;
    QStringList m_selectedIds;

    // Widgets
    QWidget      *m_filterWidget;
    QWidget      *m_tableWidget;
    QTableWidget *m_table;
    QLabel       *m_lblJumlah;
    QLabel       *m_lblCompare;
    QPushButton  *m_btnCompareMode;
    QPushButton  *m_btnLihat;
    QPushButton  *m_btnBack;
    QPushButton  *m_btnTheme;
    QComboBox    *m_cmbSort;
    QLabel       *m_lblFilterTitle;
    QPushButton  *m_btnReset;
    QTreeWidget  *m_filterTree;

    QMap<QString, QCheckBox*> m_compareChecks;
    QVector<Laptop*>          m_displayedLaptops;
};
