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

// =============================================================
//  CatalogPage — Halaman katalog dengan filter, sort, komparasi
// =============================================================
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
    void refreshFilterCounts();
    void updateCompareBadge();

    ManajerKatalog *m_katalog;
    bool            m_isDark = false;

    // Filter state
    QString    m_filterCpuSeri;
    QString    m_filterGpuBrand;
    QString    m_filterRamTipe;
    int        m_filterRamKap    = 0;
    QString    m_filterStorTipe;
    int        m_filterStorKap   = 0;
    QString    m_filterDispTipe;
    QStringList m_filterPorts;

    ManajerKatalog::SortMode m_sortMode = ManajerKatalog::SortMode::None;

    // Komparasi
    bool        m_compareMode = false;
    QStringList m_selectedIds;

    // Widgets
    QWidget      *m_filterWidget;   // panel kiri
    QWidget      *m_tableWidget;    // panel kanan
    QTableWidget *m_table;
    QLabel       *m_lblJumlah;
    QLabel       *m_lblCompare;
    QPushButton  *m_btnCompareMode;
    QPushButton  *m_btnLihat;
    QPushButton  *m_btnBack;
    QComboBox    *m_cmbSort;
    QLabel       *m_lblFilterTitle;
    QPushButton  *m_btnReset;

    // Filter tree items & checkbox map
    QTreeWidget  *m_filterTree;

    // Map id laptop → checkbox (untuk compare mode)
    QMap<QString, QCheckBox*> m_compareChecks;

    // Cache hasil filter terakhir
    QVector<Laptop*> m_displayedLaptops;
};
