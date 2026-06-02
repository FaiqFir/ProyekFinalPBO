#include "CatalogPage.h"
#include "DetailDialog.h"
#include "CompareDialog.h"
#include "../models/LaptopDesain.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSplitter>
#include <QTreeWidgetItem>
#include <QScrollArea>
#include <QMessageBox>
#include <QLocale>
#include <QFont>
#include <QFrame>

// ── Helper ────────────────────────────────────────────────────────────────────
static QTreeWidgetItem* makeCheckItem(const QString &label) {
    auto *item = new QTreeWidgetItem;
    item->setText(0, label);
    item->setCheckState(0, Qt::Unchecked);
    return item;
}
static QTreeWidgetItem* makeFolderItem(QTreeWidget *tree, const QString &label,
                                       QTreeWidgetItem *parent = nullptr) {
    QTreeWidgetItem *item = parent ? new QTreeWidgetItem(parent) : new QTreeWidgetItem(tree);
    item->setText(0, label);   // tanpa simbol panah ekstra
    item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);
    QFont f = item->font(0); f.setBold(true); item->setFont(0, f);
    return item;
}

// ── Constructor ───────────────────────────────────────────────────────────────
CatalogPage::CatalogPage(ManajerKatalog *katalog, QWidget *parent)
    : QWidget(parent), m_katalog(katalog)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    // ── Top bar ───────────────────────────────────────────────────────────
    auto *topBar = new QWidget(this);
    topBar->setObjectName("catalogTopBar");
    topBar->setFixedHeight(50);
    auto *topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(12,0,12,0);
    topBarLayout->setSpacing(10);

    m_btnBack = new QPushButton("← Menu Utama", topBar);
    m_btnBack->setObjectName("btnBack");
    m_btnBack->setFixedHeight(34);
    m_btnBack->setCursor(Qt::PointingHandCursor);

    auto *pageTitle = new QLabel("📋  Katalog Produk", topBar);
    pageTitle->setObjectName("catalogPageTitle");

    // Tombol tema di kanan atas
    m_btnTheme = new QPushButton(topBar);
    m_btnTheme->setObjectName("catalogBtnTheme");
    m_btnTheme->setFixedSize(110,34);
    m_btnTheme->setCursor(Qt::PointingHandCursor);

    topBarLayout->addWidget(m_btnBack);
    topBarLayout->addSpacing(12);
    topBarLayout->addWidget(pageTitle);
    topBarLayout->addStretch();
    topBarLayout->addWidget(m_btnTheme);
    mainLayout->addWidget(topBar);

    auto *divider = new QFrame(this);
    divider->setObjectName("catalogDivider");
    divider->setFrameShape(QFrame::HLine);
    divider->setFixedHeight(1);
    mainLayout->addWidget(divider);

    // ── Konten: splitter ──────────────────────────────────────────────────
    auto *contentArea   = new QWidget(this);
    auto *contentLayout = new QHBoxLayout(contentArea);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);

    auto *splitter = new QSplitter(Qt::Horizontal, contentArea);

    m_filterWidget = new QWidget;
    m_filterWidget->setObjectName("filterPanel");
    m_filterWidget->setMinimumWidth(240);
    m_filterWidget->setMaximumWidth(280);
    buildFilterPanel(m_filterWidget);
    splitter->addWidget(m_filterWidget);

    m_tableWidget = new QWidget;
    m_tableWidget->setObjectName("tablePanel");
    buildTablePanel(m_tableWidget);
    splitter->addWidget(m_tableWidget);

    splitter->setStretchFactor(0,0);
    splitter->setStretchFactor(1,1);
    contentLayout->addWidget(splitter);
    mainLayout->addWidget(contentArea,1);

    connect(m_btnBack, &QPushButton::clicked, this, &CatalogPage::kembali);
    // Tombol tema di CatalogPage hanya emits kembali ke MainWindow (MainWindow yang toggle)
    // tapi kita butuh sinyal ke parent untuk toggle — pakai lambda
    connect(m_btnTheme, &QPushButton::clicked, this, [this]() {
        QWidget *p = qobject_cast<QWidget*>(this->parent());
        if (p) QMetaObject::invokeMethod(p->window(), "toggleDarkMode", Qt::DirectConnection);
    });

    refreshTable();
    applyTheme(false);
}

// ── applyTheme ────────────────────────────────────────────────────────────────
void CatalogPage::applyTheme(bool isDark)
{
    m_isDark = isDark;

    const QString bg        = isDark ? "#0F172A" : "#F8FAFC";
    const QString bgPanel   = isDark ? "#1E293B" : "#F8FAFC";
    const QString bgTable   = isDark ? "#0F172A" : "#FFFFFF";
    const QString border    = isDark ? "#334155" : "#E2E8F0";
    const QString textMain  = isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub   = isDark ? "#94A3B8" : "#475569";
    const QString headerBg  = isDark ? "#1E293B" : "#F8FAFC";
    const QString rowAlt    = isDark ? "#1A2744" : "#F0F9FF";
    const QString selBg     = isDark ? "#1D4ED8" : "#EFF6FF";
    const QString selTxt    = isDark ? "#FFFFFF"  : "#1E293B";
    const QString treeBg    = isDark ? "#1E293B" : "#F8FAFC";
    const QString treeHover = isDark ? "#1E3A5F" : "#EFF6FF";
    const QString btnBg     = isDark ? "#1E293B" : "white";
    const QString btnBorder = isDark ? "#334155" : "#CBD5E1";
    const QString btnTxt    = isDark ? "#94A3B8" : "#475569";
    const QString btnHover  = isDark ? "#334155" : "#F1F5F9";
    const QString cmbBg     = isDark ? "#1E293B" : "white";
    const QString cmbBrd    = isDark ? "#334155" : "#CBD5E1";
    const QString cmbTxt    = isDark ? "#F1F5F9" : "#1E293B";
    const QString badgeBg   = isDark ? "#1E3A5F" : "#EFF6FF";
    const QString badgeBrd  = isDark ? "#1D4ED8" : "#BFDBFE";
    const QString badgeTxt  = isDark ? "#93C5FD" : "#1D4ED8";
    const QString hintTxt   = isDark ? "#64748B"  : "#94A3B8";
    const QString topBarBg  = isDark ? "#1E293B" : "#FFFFFF";
    const QString divColor  = isDark ? "#334155" : "#E2E8F0";
    // Warna checkbox compare: putih di light, gelap di dark
    const QString cbColor   = isDark ? "#1E293B" : "#FFFFFF";
    const QString cbBorder  = isDark ? "#334155" : "#94A3B8";
    const QString cbCheck   = isDark ? "#93C5FD" : "#3B82F6";
    const QString themTxt   = isDark ? "#94A3B8" : "#475569";
    const QString themBrd   = isDark ? "#334155" : "#CBD5E1";
    const QString themHov   = isDark ? "#334155" : "#F1F5F9";

    // Top bar
    if (auto *w = findChild<QWidget*>("catalogTopBar"))
        w->setStyleSheet(QString("QWidget#catalogTopBar { background:%1; }").arg(topBarBg));
    if (auto *w = findChild<QFrame*>("catalogDivider"))
        w->setStyleSheet(QString("background:%1;").arg(divColor));
    if (auto *w = findChild<QLabel*>("catalogPageTitle"))
        w->setStyleSheet(QString("font-size:15px; font-weight:bold; color:%1; background:transparent;").arg(textMain));

    m_btnBack->setStyleSheet(QString(
        "QPushButton { background:%1; border:1px solid %2; border-radius:7px;"
        " color:%3; font-size:13px; padding:0 14px; }"
        "QPushButton:hover { background:%4; }")
        .arg(btnBg, btnBorder, btnTxt, btnHover));

    if (m_btnTheme) {
        m_btnTheme->setText(isDark ? "☀ Light" : "☾ Dark");
        m_btnTheme->setStyleSheet(QString(
            "QPushButton { background:%1; border:1px solid %2; border-radius:8px;"
            " font-size:13px; color:%3; padding:0 10px; font-weight:500; }"
            "QPushButton:hover { background:%4; }")
            .arg(btnBg, themBrd, themTxt, themHov));
    }

    // Filter panel
    m_filterWidget->setStyleSheet(QString(
        "QWidget#filterPanel { background:%1; border-right:1px solid %2; }")
        .arg(bgPanel, border));
    if (m_lblFilterTitle)
        m_lblFilterTitle->setStyleSheet(QString(
            "font-weight:bold; font-size:14px; color:%1; padding:4px; background:transparent;").arg(textMain));
    if (m_btnReset)
        m_btnReset->setStyleSheet(QString(
            "QPushButton { border:1px solid %1; border-radius:6px; background:%2;"
            " color:%3; font-size:12px; padding:4px; }"
            "QPushButton:hover { background:%4; }")
            .arg(border, btnBg, textSub, btnHover));
    if (m_filterTree)
        m_filterTree->setStyleSheet(QString(
            "QTreeWidget { border:none; background:%1; font-size:12px; color:%2; }"
            "QTreeWidget::item { padding:3px; color:%2; }"
            "QTreeWidget::item:hover { background:%3; border-radius:4px; }"
            "QTreeWidget::indicator { width:15px; height:15px; }"
            "QTreeWidget::indicator:unchecked { border:2px solid %4; border-radius:3px; background:%5; }"
            "QTreeWidget::indicator:checked { border:2px solid %6; border-radius:3px; background:%6;"
            " image:url(:/images/check_white.png); }"
            "QTreeWidget::indicator:indeterminate { border:2px solid %6; border-radius:3px; background:%6; }")
            .arg(treeBg, textMain, treeHover, cbBorder, cbColor, cbCheck));

    // Table panel
    m_tableWidget->setStyleSheet(QString("QWidget#tablePanel { background:%1; }").arg(bgTable));
    if (m_lblJumlah)
        m_lblJumlah->setStyleSheet(QString("font-size:13px; color:%1; background:transparent;").arg(textSub));
    if (m_cmbSort)
        m_cmbSort->setStyleSheet(QString(
            "QComboBox { border:1px solid %1; border-radius:6px; padding:0 10px;"
            " font-size:13px; background:%2; color:%3; }"
            "QComboBox::drop-down { border:none; }"
            "QComboBox QAbstractItemView { background:%2; color:%3; border:1px solid %1; }")
            .arg(cmbBrd, cmbBg, cmbTxt));
    if (m_btnCompareMode)
        m_btnCompareMode->setStyleSheet(QString(
            "QPushButton { border:1px solid #3B82F6; border-radius:6px; background:%1;"
            " color:%2; font-size:13px; padding:0 12px; }"
            "QPushButton:checked { background:#3B82F6; color:white; }"
            "QPushButton:hover:!checked { background:%3; }")
            .arg(btnBg, isDark ? "#93C5FD" : "#3B82F6", isDark ? "#1E3A5F" : "#EFF6FF"));
    if (m_btnLihat)
        m_btnLihat->setStyleSheet(
            "QPushButton { background:#059669; color:white; border-radius:6px;"
            " font-size:13px; padding:0 12px; border:none; }"
            "QPushButton:disabled { background:#64748B; color:#94A3B8; }"
            "QPushButton:hover:!disabled { background:#047857; }");
    if (m_lblCompare)
        m_lblCompare->setStyleSheet(QString(
            "background:%1; border:1px solid %2; border-radius:6px;"
            " color:%3; font-size:12px; padding:4px 10px;")
            .arg(badgeBg, badgeBrd, badgeTxt));
    if (m_table)
        m_table->setStyleSheet(QString(
            "QTableWidget { border:1px solid %1; border-radius:8px; font-size:13px;"
            " background:%2; color:%3; gridline-color:%1; }"
            "QTableWidget::item { padding:6px; color:%3; background:%2;"
            " border-bottom:1px solid %1; }"
            "QTableWidget::item:alternate { background:%4; }"
            "QTableWidget::item:selected { background:%5; color:%6; }"
            "QHeaderView::section { background:%7; font-weight:bold; color:%8;"
            " border:none; border-bottom:2px solid %1; padding:8px; }")
            .arg(border, bgTable, textMain, rowAlt, selBg, selTxt, headerBg, textSub));

    auto hints = findChildren<QLabel*>();
    for (auto *lbl : hints)
        if (lbl->text().contains("Klik ganda"))
            lbl->setStyleSheet(QString("color:%1; font-size:11px; background:transparent;").arg(hintTxt));
}

// ── Build Filter Panel ────────────────────────────────────────────────────────
void CatalogPage::buildFilterPanel(QWidget *panel) {
    auto *vl = new QVBoxLayout(panel);
    vl->setContentsMargins(8,8,8,8);
    vl->setSpacing(6);

    m_lblFilterTitle = new QLabel("🔍 Filter Spesifikasi", panel);
    vl->addWidget(m_lblFilterTitle);

    m_btnReset = new QPushButton("Reset Filter", panel);
    vl->addWidget(m_btnReset);

    m_filterTree = new QTreeWidget(panel);
    m_filterTree->setHeaderHidden(true);

    // ── Jenis Laptop (paling atas) ─────────────────────────────────────────
    auto *jenisFolder = makeFolderItem(m_filterTree, "Jenis Laptop");
    jenisFolder->addChild(makeCheckItem(
        QString("Laptop Office (%1)").arg(m_katalog->hitungDenganJenis("Laptop Office"))));
    jenisFolder->addChild(makeCheckItem(
        QString("Laptop Gaming (%1)").arg(m_katalog->hitungDenganJenis("Laptop Gaming"))));
    jenisFolder->addChild(makeCheckItem(
        QString("Laptop Desain (%1)").arg(m_katalog->hitungDenganJenis("Laptop Desain"))));
    jenisFolder->addChild(makeCheckItem(
        QString("Laptop Workstation (%1)").arg(m_katalog->hitungDenganJenis("Laptop Workstation"))));
    jenisFolder->setExpanded(true);

    // ── CPU ────────────────────────────────────────────────────────────────
    auto *cpuFolder = makeFolderItem(m_filterTree, "CPU");
    auto *cpuIntel  = makeFolderItem(m_filterTree, "Intel", cpuFolder);
    for (const QString &s : {"Celeron","Core i3","Core i5","Core i7","Core i9"})
        cpuIntel->addChild(makeCheckItem(
            QString("%1 (%2)").arg(s).arg(m_katalog->hitungDenganCpuSeri(s))));
    auto *cpuAmd = makeFolderItem(m_filterTree, "AMD", cpuFolder);
    for (const QString &s : {"Athlon","Ryzen 3","Ryzen 5","Ryzen 7","Ryzen 9"})
        cpuAmd->addChild(makeCheckItem(
            QString("%1 (%2)").arg(s).arg(m_katalog->hitungDenganCpuSeri(s))));

    // ── GPU ────────────────────────────────────────────────────────────────
    auto *gpuFolder = makeFolderItem(m_filterTree, "GPU");
    for (const QString &b : {"Intel","AMD","Nvidia"})
        gpuFolder->addChild(makeCheckItem(
            QString("%1 (%2)").arg(b).arg(m_katalog->hitungDenganGpuBrand(b))));

    // ── RAM ────────────────────────────────────────────────────────────────
    auto *ramFolder = makeFolderItem(m_filterTree, "RAM");
    auto *ddr4 = makeFolderItem(m_filterTree, "DDR4", ramFolder);
    auto *ddr5 = makeFolderItem(m_filterTree, "DDR5", ramFolder);
    for (int kap : {4,8,16,32}) {
        ddr4->addChild(makeCheckItem(QString("%1GB (%2)").arg(kap).arg(m_katalog->hitungDenganRamKapasitas(kap))));
        ddr5->addChild(makeCheckItem(QString("%1GB (%2)").arg(kap).arg(m_katalog->hitungDenganRamKapasitas(kap))));
    }

    // ── Storage ────────────────────────────────────────────────────────────
    auto *storFolder = makeFolderItem(m_filterTree, "Storage");
    auto *ssd = makeFolderItem(m_filterTree, "SSD", storFolder);
    auto *hdd = makeFolderItem(m_filterTree, "HDD", storFolder);
    for (const QString &k : {"128GB","256GB","512GB","1TB"}) {
        ssd->addChild(makeCheckItem(k));
        hdd->addChild(makeCheckItem(k));
    }

    // ── Display ────────────────────────────────────────────────────────────
    auto *dispFolder = makeFolderItem(m_filterTree, "Display");
    for (const QString &t : {"IPS","OLED","TN","VA"})
        dispFolder->addChild(makeCheckItem(
            QString("%1 (%2)").arg(t).arg(m_katalog->hitungDenganDisplayTipe(t))));

    // ── Wireless ───────────────────────────────────────────────────────────
    auto *wirelessFolder = makeFolderItem(m_filterTree, "Wireless");
    auto *wifiFolder = makeFolderItem(m_filterTree, "Wi-Fi", wirelessFolder);
    for (const QString &v : {"Wi-Fi 5","Wi-Fi 6","Wi-Fi 6E","Wi-Fi 7"})
        wifiFolder->addChild(makeCheckItem(
            QString("%1 (%2)").arg(v).arg(m_katalog->hitungDenganWifiVersi(v))));
    auto *btFolder = makeFolderItem(m_filterTree, "Bluetooth", wirelessFolder);
    for (const QString &v : {"4.2","5.0","5.1","5.2","5.3","5.4"})
        btFolder->addChild(makeCheckItem(
            QString("%1 (%2)").arg(v).arg(m_katalog->hitungDenganBtVersi(v))));

    // ── Port ───────────────────────────────────────────────────────────────
    auto *portFolder = makeFolderItem(m_filterTree, "Port (multi)");
    for (const QString &p : {"USB-A","USB-C","USB-C PD","HDMI","Audio Jack"})
        portFolder->addChild(makeCheckItem(p));

    vl->addWidget(m_filterTree);

    connect(m_filterTree, &QTreeWidget::itemChanged, this, [this](QTreeWidgetItem *item, int) {
        if (!(item->flags() & Qt::ItemIsUserCheckable)) return;
        onFilterChanged();
    });

    connect(m_btnReset, &QPushButton::clicked, this, [this]() {
        std::function<void(QTreeWidgetItem*)> resetAll = [&](QTreeWidgetItem *node) {
            if (node->flags() & Qt::ItemIsUserCheckable)
                node->setCheckState(0, Qt::Unchecked);
            for (int i = 0; i < node->childCount(); ++i)
                resetAll(node->child(i));
        };
        for (int i = 0; i < m_filterTree->topLevelItemCount(); ++i)
            resetAll(m_filterTree->topLevelItem(i));
        m_filterJenis.clear(); m_filterCpuSeri.clear(); m_filterGpuBrand.clear();
        m_filterRamTipe.clear(); m_filterRamKap = 0;
        m_filterStorTipe.clear(); m_filterStorKap = 0;
        m_filterDispTipe.clear(); m_filterPorts.clear();
        m_filterWifi.clear(); m_filterBt.clear();
        refreshTable();
    });
}

// ── Build Table Panel ─────────────────────────────────────────────────────────
void CatalogPage::buildTablePanel(QWidget *panel) {
    auto *vl = new QVBoxLayout(panel);
    vl->setContentsMargins(12,10,12,10);
    vl->setSpacing(8);

    auto *toolbar = new QHBoxLayout;
    m_lblJumlah = new QLabel(panel);
    m_cmbSort = new QComboBox(panel);
    m_cmbSort->addItems({"Default","Harga Termurah",
                         "RAM Terbesar","Storage Terbesar","Stok Terbanyak"});
    m_cmbSort->setFixedHeight(34);

    m_btnCompareMode = new QPushButton("⚖️ Mode Bandingkan", panel);
    m_btnCompareMode->setFixedHeight(34);
    m_btnCompareMode->setCheckable(true);

    m_btnLihat = new QPushButton("Lihat Perbandingan →", panel);
    m_btnLihat->setFixedHeight(34);
    m_btnLihat->setEnabled(false);

    toolbar->addWidget(m_lblJumlah);
    toolbar->addStretch();
    toolbar->addWidget(m_cmbSort);
    toolbar->addWidget(m_btnCompareMode);
    toolbar->addWidget(m_btnLihat);
    vl->addLayout(toolbar);

    m_lblCompare = new QLabel(panel);
    m_lblCompare->hide();
    vl->addWidget(m_lblCompare);

    m_table = new QTableWidget(panel);
    m_table->setColumnCount(9);
    m_table->setHorizontalHeaderLabels({"","Nama Produk","CPU","GPU","RAM","Storage","Display","Harga","Stok"});
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->setTextElideMode(Qt::ElideRight);
    m_table->verticalHeader()->setVisible(false);
    m_table->setShowGrid(false);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_table->setColumnWidth(0,44);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    for (int c = 2; c <= 8; ++c)
        m_table->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    vl->addWidget(m_table);

    auto *hint = new QLabel("Klik ganda pada laptop untuk melihat detail spesifikasi lengkap", panel);
    vl->addWidget(hint);

    connect(m_cmbSort, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CatalogPage::onSortChanged);
    connect(m_btnCompareMode, &QPushButton::toggled, this, &CatalogPage::onCompareModeToggled);
    connect(m_btnLihat, &QPushButton::clicked, this, &CatalogPage::onLihatPerbandingan);
    connect(m_table, &QTableWidget::cellDoubleClicked, this, &CatalogPage::onItemDoubleClicked);
}

// ── Refresh Tabel ─────────────────────────────────────────────────────────────
void CatalogPage::refreshTable() {
    ManajerKatalog::FilterParam fp;
    fp.jenisLaptop     = m_filterJenis;
    if (!m_filterCpuSeri.isEmpty())   fp.cpuSeri          << m_filterCpuSeri;
    if (!m_filterGpuBrand.isEmpty())  fp.gpuBrand         << m_filterGpuBrand;
    if (!m_filterRamTipe.isEmpty())   fp.ramTipe          << m_filterRamTipe;
    if (m_filterRamKap > 0)           fp.ramKapasitas     << QString::number(m_filterRamKap);
    if (!m_filterStorTipe.isEmpty())  fp.storageTipe      << m_filterStorTipe;
    if (m_filterStorKap > 0)          fp.storageKapasitas << QString::number(m_filterStorKap);
    if (!m_filterDispTipe.isEmpty())  fp.displayTipe      << m_filterDispTipe;
    if (!m_filterWifi.isEmpty())      fp.wifiVersi        << m_filterWifi;
    if (!m_filterBt.isEmpty())        fp.btVersi          << m_filterBt;
    fp.ports = m_filterPorts;

    m_displayedLaptops = m_katalog->filterDanSort(fp, m_sortMode);
    m_lblJumlah->setText(QString("📋 <b>%1</b> produk sesuai filter").arg(m_displayedLaptops.size()));

    m_table->setRowCount(m_displayedLaptops.size());
    m_compareChecks.clear();

    for (int r = 0; r < m_displayedLaptops.size(); ++r) {
        Laptop *l = m_displayedLaptops[r];
        m_table->setRowHeight(r,44);

        if (m_compareMode) {
            auto *cb = new QCheckBox;
            bool sudahDipilih = m_selectedIds.contains(l->getId());
            cb->setChecked(sudahDipilih);
            if (m_selectedIds.size() >= 3 && !sudahDipilih) cb->setEnabled(false);
            cb->setStyleSheet(QString(
                "QCheckBox { background:transparent; }"
                "QCheckBox::indicator { width:18px; height:18px; border-radius:4px;"
                " border:2px solid %1; background:%2; }"
                "QCheckBox::indicator:checked { background:%3; border-color:%3;"
                " image:url(:/images/check_white.png); }")
                .arg(m_isDark ? "#334155" : "#94A3B8",
                     m_isDark ? "#1E293B" : "#FFFFFF",
                     "#3B82F6"));
            m_compareChecks[l->getId()] = cb;
            m_table->setCellWidget(r,0,cb);
            connect(cb, &QCheckBox::toggled, this, [this, id=l->getId()](bool chk) {
                onCheckboxCompareChanged(id, chk);
            });
        } else {
            m_table->setCellWidget(r,0,nullptr);
            m_table->setItem(r,0,new QTableWidgetItem(""));

            // Thumbnail kecil di kolom 0 (hanya saat bukan compare mode)
            if (!l->getImagePaths().isEmpty()) {
                QPixmap px(l->getImagePaths().first());
                if (!px.isNull()) {
                    auto *lblThumb = new QLabel;
                    lblThumb->setAlignment(Qt::AlignCenter);
                    lblThumb->setPixmap(px.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    lblThumb->setStyleSheet("background:transparent; padding:2px;");
                    m_table->setCellWidget(r,0,lblThumb);
                }
            }
        }

        // Kolom nama — pakai QLabel + setItem agar tidak terpotong "..."
        auto *namaItem = new QTableWidgetItem(l->getNama());
        namaItem->setData(Qt::UserRole, l->getId());
        m_table->setItem(r,1,namaItem);

        m_table->setItem(r,2,new QTableWidgetItem(l->getCPU().toShortString()));
        m_table->setItem(r,3,new QTableWidgetItem(
            l->getGpuBrand() + " " + l->getGpuModel()));
        m_table->setItem(r,4,new QTableWidgetItem(
            QString("%1GB %2").arg(l->getRAM().getKapasitas()).arg(l->getRAM().getTipe())));
        m_table->setItem(r,5,new QTableWidgetItem(
            l->getStorage().kapasitasLabel()+" "+l->getStorage().getTipe()));
        m_table->setItem(r,6,new QTableWidgetItem(
            QString("%1\" %2 %3Hz")
            .arg(l->getDisplay().getUkuran(), 0, 'f', 1)
            .arg(l->getDisplay().getTipe())
            .arg(l->getDisplay().getRefreshRate())));
        m_table->setItem(r,7,new QTableWidgetItem(l->formatHarga()));
        auto *stokItem = new QTableWidgetItem(QString("%1").arg(l->getStok()));
        stokItem->setForeground(l->getStok()<=5 ? QColor("#EF4444") : QColor(m_isDark?"#34D399":"#059669"));
        m_table->setItem(r,8,stokItem);
    }
}

// ── onFilterChanged ───────────────────────────────────────────────────────────
void CatalogPage::onFilterChanged() {
    m_filterJenis.clear();
    m_filterCpuSeri.clear(); m_filterGpuBrand.clear();
    m_filterRamTipe.clear(); m_filterRamKap = 0;
    m_filterStorTipe.clear(); m_filterStorKap = 0;
    m_filterDispTipe.clear(); m_filterPorts.clear();
    m_filterWifi.clear(); m_filterBt.clear();

    std::function<void(QTreeWidgetItem*)> scan;
    scan = [&](QTreeWidgetItem *node) {
        if ((node->flags() & Qt::ItemIsUserCheckable) && node->checkState(0) == Qt::Checked) {
            QString txt = node->text(0).split(" (").first().trimmed();
            QTreeWidgetItem *p1 = node->parent();
            QTreeWidgetItem *p2 = p1 ? p1->parent() : nullptr;
            QTreeWidgetItem *p3 = p2 ? p2->parent() : nullptr;
            QString cat1 = p1 ? p1->text(0).trimmed() : "";
            QString cat2 = p2 ? p2->text(0).trimmed() : "";
            QString cat3 = p3 ? p3->text(0).trimmed() : "";

            if (cat1 == "Jenis Laptop") {
                if (!m_filterJenis.contains(txt)) m_filterJenis << txt;
            }
            else if (cat2 == "CPU" || cat3 == "CPU") m_filterCpuSeri = txt;
            else if (cat1 == "GPU")                   m_filterGpuBrand = txt;
            else if (cat2 == "RAM" || cat3 == "RAM") {
                QString ks = txt; ks.remove("GB");
                m_filterRamKap = ks.toInt();
                if (cat1=="DDR4"||cat2=="DDR4") m_filterRamTipe="DDR4";
                if (cat1=="DDR5"||cat2=="DDR5") m_filterRamTipe="DDR5";
            }
            else if (cat2=="Storage"||cat3=="Storage") {
                QString ks = txt;
                if (ks.endsWith("TB")) m_filterStorKap = ks.remove("TB").toInt()*1024;
                else { ks.remove("GB"); m_filterStorKap = ks.toInt(); }
                if (cat1=="SSD"||cat2=="SSD") m_filterStorTipe="SSD";
                if (cat1=="HDD"||cat2=="HDD") m_filterStorTipe="HDD";
            }
            else if (cat1=="Display") m_filterDispTipe=txt;
            else if (cat1=="Wi-Fi")     m_filterWifi = txt;
            else if (cat1=="Bluetooth") m_filterBt   = txt;
            else if (cat1.contains("Port")) { if (!m_filterPorts.contains(txt)) m_filterPorts<<txt; }
        }
        for (int i = 0; i < node->childCount(); ++i) scan(node->child(i));
    };
    for (int i = 0; i < m_filterTree->topLevelItemCount(); ++i)
        scan(m_filterTree->topLevelItem(i));

    refreshTable();
}

void CatalogPage::onSortChanged(int index) {
    switch (index) {
    case 1: m_sortMode = ManajerKatalog::SortMode::HargaAsc;   break;
    case 2: m_sortMode = ManajerKatalog::SortMode::RamDesc;    break;
    case 3: m_sortMode = ManajerKatalog::SortMode::StorageDesc; break;
    case 4: m_sortMode = ManajerKatalog::SortMode::StokDesc;   break;
    default:m_sortMode = ManajerKatalog::SortMode::None;        break;
    }
    refreshTable();
}

void CatalogPage::onCompareModeToggled() {
    m_compareMode = m_btnCompareMode->isChecked();
    m_selectedIds.clear();
    if (m_compareMode) { m_lblCompare->show(); updateCompareBadge(); }
    else               { m_lblCompare->hide(); m_btnLihat->setEnabled(false); }
    refreshTable();
}

void CatalogPage::onCheckboxCompareChanged(const QString &id, bool checked) {
    if (checked) { if (!m_selectedIds.contains(id) && m_selectedIds.size()<3) m_selectedIds.append(id); }
    else           m_selectedIds.removeAll(id);
    updateCompareBadge(); refreshTable();
}

void CatalogPage::updateCompareBadge() {
    m_lblCompare->setText(
        QString("⚖️ Pilih maksimal 3 produk (%1/3) — klik ganda untuk detail").arg(m_selectedIds.size()));
    m_btnLihat->setEnabled(m_selectedIds.size() >= 2);
}

void CatalogPage::onLihatPerbandingan() {
    QVector<Laptop*> dipilih;
    for (const QString &id : m_selectedIds)
        if (Laptop *l = m_katalog->cariLaptopById(id)) dipilih.append(l);
    if (dipilih.size() < 2) return;
    CompareDialog dlg(dipilih, m_isDark, this);
    dlg.exec();
}

void CatalogPage::onItemDoubleClicked(int row, int) {
    if (row < 0 || row >= m_displayedLaptops.size()) return;
    DetailDialog dlg(m_displayedLaptops[row], m_isDark,
                     DetailDialog::Mode::Katalog, this);
    dlg.exec();
}
