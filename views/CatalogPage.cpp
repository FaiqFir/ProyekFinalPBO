#include "CatalogPage.h"
#include "DetailDialog.h"
#include "CompareDialog.h"
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

// ── Helper: buat tree item checkbox ─────────────────────────────────────────
static QTreeWidgetItem* makeCheckItem(const QString &label) {
    auto *item = new QTreeWidgetItem;
    item->setText(0, label);
    item->setCheckState(0, Qt::Unchecked);
    return item;
}

static QTreeWidgetItem* makeFolderItem(QTreeWidget *tree, const QString &label,
                                       QTreeWidgetItem *parent = nullptr) {
    QTreeWidgetItem *item;
    if (parent) item = new QTreeWidgetItem(parent);
    else        item = new QTreeWidgetItem(tree);
    item->setText(0, "▶ " + label);
    item->setFlags(item->flags() & ~Qt::ItemIsUserCheckable);
    QFont f = item->font(0);
    f.setBold(true);
    item->setFont(0, f);
    return item;
}

// ── Constructor ──────────────────────────────────────────────────────────────
CatalogPage::CatalogPage(ManajerKatalog *katalog, QWidget *parent)
    : QWidget(parent), m_katalog(katalog)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    // ── Top bar: tombol kembali (kiri) ────────────────────────────────────
    auto *topBar = new QWidget(this);
    topBar->setObjectName("catalogTopBar");
    topBar->setFixedHeight(50);
    auto *topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(12, 0, 12, 0);
    topBarLayout->setSpacing(10);

    m_btnBack = new QPushButton("← Menu Utama", topBar);
    m_btnBack->setObjectName("btnBack");
    m_btnBack->setFixedHeight(34);
    m_btnBack->setCursor(Qt::PointingHandCursor);

    auto *pageTitle = new QLabel("📋  Katalog Produk", topBar);
    pageTitle->setObjectName("catalogPageTitle");
    pageTitle->setStyleSheet("font-size:15px; font-weight:bold;");

    topBarLayout->addWidget(m_btnBack);
    topBarLayout->addSpacing(12);
    topBarLayout->addWidget(pageTitle);
    topBarLayout->addStretch();
    mainLayout->addWidget(topBar);

    // ── Garis pemisah ─────────────────────────────────────────────────────
    auto *divider = new QFrame(this);
    divider->setObjectName("catalogDivider");
    divider->setFrameShape(QFrame::HLine);
    divider->setFixedHeight(1);
    mainLayout->addWidget(divider);

    // ── Konten utama (splitter) ───────────────────────────────────────────
    auto *contentArea = new QWidget(this);
    auto *contentLayout = new QHBoxLayout(contentArea);
    contentLayout->setContentsMargins(0,0,0,0);
    contentLayout->setSpacing(0);

    auto *splitter = new QSplitter(Qt::Horizontal, contentArea);

    // ── Panel Filter (kiri) ──────────────────────────────────────────────
    m_filterWidget = new QWidget;
    m_filterWidget->setObjectName("filterPanel");
    m_filterWidget->setMinimumWidth(240);
    m_filterWidget->setMaximumWidth(280);
    buildFilterPanel(m_filterWidget);
    splitter->addWidget(m_filterWidget);

    // ── Panel Tabel (kanan) ──────────────────────────────────────────────
    m_tableWidget = new QWidget;
    m_tableWidget->setObjectName("tablePanel");
    buildTablePanel(m_tableWidget);
    splitter->addWidget(m_tableWidget);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    contentLayout->addWidget(splitter);
    mainLayout->addWidget(contentArea, 1);

    connect(m_btnBack, &QPushButton::clicked, this, &CatalogPage::kembali);

    refreshTable();
    // terapkan tema default (light)
    applyTheme(false);
}

// ── applyTheme ────────────────────────────────────────────────────────────────
void CatalogPage::applyTheme(bool isDark)
{
    m_isDark = isDark;

    // ── Warna mode ────────────────────────────────────────────────────────
    const QString bg        = isDark ? "#0F172A" : "#F8FAFC";
    const QString bgPanel   = isDark ? "#1E293B" : "#F8FAFC";
    const QString bgTable   = isDark ? "#0F172A" : "#FFFFFF";
    const QString border    = isDark ? "#334155" : "#E2E8F0";
    const QString textMain  = isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub   = isDark ? "#94A3B8" : "#475569";
    const QString headerBg  = isDark ? "#1E293B" : "#F8FAFC";
    const QString rowAlt    = isDark ? "#1A2744" : "#F0F9FF";
    const QString selBg     = isDark ? "#1D4ED8" : "#EFF6FF";
    const QString selTxt    = isDark ? "#FFFFFF" : "#1E293B";
    const QString treeHover = isDark ? "#1E3A5F" : "#EFF6FF";
    const QString treeBg    = isDark ? "#1E293B" : "#F8FAFC";
    const QString btnBackBg = isDark ? "#1E293B" : "white";
    const QString btnBackBorder = isDark ? "#334155" : "#CBD5E1";
    const QString btnBackTxt    = isDark ? "#94A3B8" : "#475569";
    const QString btnBackHover  = isDark ? "#334155" : "#F1F5F9";
    const QString topBarBg      = isDark ? "#1E293B" : "#FFFFFF";
    const QString dividerColor  = isDark ? "#334155" : "#E2E8F0";
    const QString compareBadgeBg  = isDark ? "#1E3A5F" : "#EFF6FF";
    const QString compareBadgeBrd = isDark ? "#1D4ED8" : "#BFDBFE";
    const QString compareBadgeTxt = isDark ? "#93C5FD" : "#1D4ED8";
    const QString cmbBg   = isDark ? "#1E293B" : "white";
    const QString cmbBrd  = isDark ? "#334155" : "#CBD5E1";
    const QString cmbTxt  = isDark ? "#F1F5F9" : "#1E293B";
    const QString hintTxt = isDark ? "#64748B"  : "#94A3B8";

    // Warna tombol Compare Mode
    const QString cmpNormalBg  = isDark ? "#1E293B" : "white";
    const QString cmpNormalTxt = isDark ? "#93C5FD" : "#3B82F6";
    const QString cmpNormalBrd = isDark ? "#1D4ED8" : "#3B82F6";
    const QString cmpHover     = isDark ? "#1E3A5F" : "#EFF6FF";

    // Tombol kembali
    m_btnBack->setStyleSheet(QString(
        "QPushButton { background:%1; border:1px solid %2; border-radius:7px;"
        " color:%3; font-size:13px; padding:0 14px; }"
        "QPushButton:hover { background:%4; }")
        .arg(btnBackBg, btnBackBorder, btnBackTxt, btnBackHover));

    // Top bar background
    if (auto *topBar = findChild<QWidget*>("catalogTopBar"))
        topBar->setStyleSheet(QString("QWidget#catalogTopBar { background:%1; }").arg(topBarBg));

    // Judul halaman
    if (auto *lbl = findChild<QLabel*>("catalogPageTitle"))
        lbl->setStyleSheet(QString("font-size:15px; font-weight:bold; color:%1; background:transparent;").arg(textMain));

    // Garis pemisah
    if (auto *div = findChild<QFrame*>("catalogDivider"))
        div->setStyleSheet(QString("background:%1;").arg(dividerColor));

    // Filter panel background & border
    m_filterWidget->setStyleSheet(QString(
        "QWidget#filterPanel { background:%1; border-right:1px solid %2; }")
        .arg(bgPanel, border));

    // Filter title
    if (m_lblFilterTitle)
        m_lblFilterTitle->setStyleSheet(QString(
            "font-weight:bold; font-size:14px; color:%1; padding:4px; background:transparent;")
            .arg(textMain));

    // Tombol Reset Filter
    if (m_btnReset)
        m_btnReset->setStyleSheet(QString(
            "QPushButton { border:1px solid %1; border-radius:6px; background:%2;"
            " color:%3; font-size:12px; padding:4px; }"
            "QPushButton:hover { background:%4; }")
            .arg(border, btnBackBg, textSub, btnBackHover));

    // Filter tree
    if (m_filterTree)
        m_filterTree->setStyleSheet(QString(
            "QTreeWidget { border:none; background:%1; font-size:12px; color:%2; }"
            "QTreeWidget::item { padding:3px; color:%2; }"
            "QTreeWidget::item:hover { background:%3; border-radius:4px; }")
            .arg(treeBg, textMain, treeHover));

    // Table panel background
    m_tableWidget->setStyleSheet(QString("QWidget#tablePanel { background:%1; }").arg(bgTable));

    // Label jumlah
    if (m_lblJumlah)
        m_lblJumlah->setStyleSheet(QString("font-size:13px; color:%1; background:transparent;").arg(textSub));

    // ComboBox sort
    if (m_cmbSort)
        m_cmbSort->setStyleSheet(QString(
            "QComboBox { border:1px solid %1; border-radius:6px; padding:0 10px;"
            " font-size:13px; background:%2; color:%3; }"
            "QComboBox::drop-down { border:none; }"
            "QComboBox QAbstractItemView { background:%2; color:%3; border:1px solid %1; }")
            .arg(cmbBrd, cmbBg, cmbTxt));

    // Tombol Compare Mode
    if (m_btnCompareMode)
        m_btnCompareMode->setStyleSheet(QString(
            "QPushButton { border:1px solid %1; border-radius:6px; background:%2;"
            " color:%3; font-size:13px; padding:0 12px; }"
            "QPushButton:checked { background:%4; color:white; border-color:%4; }"
            "QPushButton:hover:!checked { background:%5; }")
            .arg(cmpNormalBrd, cmpNormalBg, cmpNormalTxt, "#3B82F6", cmpHover));

    // Tombol Lihat (selalu biru-hijau, tidak berubah per tema)
    if (m_btnLihat)
        m_btnLihat->setStyleSheet(
            "QPushButton { background:#059669; color:white; border-radius:6px;"
            " font-size:13px; padding:0 12px; border:none; }"
            "QPushButton:disabled { background:#94A3B8; }"
            "QPushButton:hover:!disabled { background:#047857; }");

    // Badge compare
    if (m_lblCompare)
        m_lblCompare->setStyleSheet(QString(
            "background:%1; border:1px solid %2; border-radius:6px;"
            " color:%3; font-size:12px; padding:4px 10px;")
            .arg(compareBadgeBg, compareBadgeBrd, compareBadgeTxt));

    // Tabel
    if (m_table)
        m_table->setStyleSheet(QString(
            "QTableWidget { border:1px solid %1; border-radius:8px; font-size:13px;"
            " background:%2; color:%3; gridline-color:%1; }"
            "QTableWidget::item { padding:6px; color:%3;"
            " border-bottom:1px solid %1; background:%2; }"
            "QTableWidget::item:alternate { background:%4; }"
            "QTableWidget::item:selected { background:%5; color:%6; }"
            "QHeaderView::section { background:%7; font-weight:bold; color:%8;"
            " border:none; border-bottom:2px solid %1; padding:8px; }")
            .arg(border, bgTable, textMain, rowAlt, selBg, selTxt, headerBg, textSub));

    // Hint label
    auto hints = findChildren<QLabel*>();
    for (auto *lbl : hints) {
        if (lbl->text().contains("Klik ganda"))
            lbl->setStyleSheet(QString("color:%1; font-size:11px; background:transparent;").arg(hintTxt));
    }
}

// ── Build Filter Panel ───────────────────────────────────────────────────────
void CatalogPage::buildFilterPanel(QWidget *panel) {
    auto *vl = new QVBoxLayout(panel);
    vl->setContentsMargins(8, 8, 8, 8);
    vl->setSpacing(6);

    m_lblFilterTitle = new QLabel("🔍 Filter Spesifikasi", panel);
    vl->addWidget(m_lblFilterTitle);

    m_btnReset = new QPushButton("Reset Filter", panel);
    vl->addWidget(m_btnReset);

    // Filter Tree
    m_filterTree = new QTreeWidget(panel);
    m_filterTree->setHeaderHidden(true);

    // ── CPU ──────────────────────────────────────────────────────────────
    auto *cpuFolder = makeFolderItem(m_filterTree, "CPU");
    auto *cpuIntel  = makeFolderItem(m_filterTree, "Intel", cpuFolder);
    for (const QString &s : {"Celeron","Core i3","Core i5","Core i7","Core i9"})
        cpuIntel->addChild(makeCheckItem(
            QString("%1 (%2)").arg(s).arg(m_katalog->hitungDenganCpuSeri(s))));
    auto *cpuAmd = makeFolderItem(m_filterTree, "AMD", cpuFolder);
    for (const QString &s : {"Athlon","Ryzen 3","Ryzen 5","Ryzen 7","Ryzen 9"})
        cpuAmd->addChild(makeCheckItem(
            QString("%1 (%2)").arg(s).arg(m_katalog->hitungDenganCpuSeri(s))));

    // ── GPU ──────────────────────────────────────────────────────────────
    auto *gpuFolder = makeFolderItem(m_filterTree, "GPU");
    for (const QString &b : {"Intel","AMD","Nvidia"})
        gpuFolder->addChild(makeCheckItem(
            QString("%1 (%2)").arg(b).arg(m_katalog->hitungDenganGpuBrand(b))));

    // ── RAM ──────────────────────────────────────────────────────────────
    auto *ramFolder = makeFolderItem(m_filterTree, "RAM");
    auto *ddr4 = makeFolderItem(m_filterTree, "DDR4", ramFolder);
    auto *ddr5 = makeFolderItem(m_filterTree, "DDR5", ramFolder);
    for (int kap : {4,8,16,32}) {
        ddr4->addChild(makeCheckItem(QString("%1GB (%2)").arg(kap).arg(m_katalog->hitungDenganRamKapasitas(kap))));
        ddr5->addChild(makeCheckItem(QString("%1GB (%2)").arg(kap).arg(m_katalog->hitungDenganRamKapasitas(kap))));
    }

    // ── Storage ──────────────────────────────────────────────────────────
    auto *storFolder = makeFolderItem(m_filterTree, "Storage");
    auto *ssd = makeFolderItem(m_filterTree, "SSD", storFolder);
    auto *hdd = makeFolderItem(m_filterTree, "HDD", storFolder);
    for (const QString &k : {"128GB","256GB","512GB","1TB"}) {
        ssd->addChild(makeCheckItem(k));
        hdd->addChild(makeCheckItem(k));
    }

    // ── Display ──────────────────────────────────────────────────────────
    auto *dispFolder = makeFolderItem(m_filterTree, "Display");
    for (const QString &t : {"IPS","OLED"})
        dispFolder->addChild(makeCheckItem(
            QString("%1 (%2)").arg(t).arg(m_katalog->hitungDenganDisplayTipe(t))));

    // ── Port (multi-select) ───────────────────────────────────────────────
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

        m_filterCpuSeri.clear(); m_filterGpuBrand.clear();
        m_filterRamTipe.clear(); m_filterRamKap = 0;
        m_filterStorTipe.clear(); m_filterStorKap = 0;
        m_filterDispTipe.clear(); m_filterPorts.clear();
        refreshTable();
    });
}

// ── Build Table Panel ────────────────────────────────────────────────────────
void CatalogPage::buildTablePanel(QWidget *panel) {
    auto *vl = new QVBoxLayout(panel);
    vl->setContentsMargins(12, 10, 12, 10);
    vl->setSpacing(8);

    // ── Toolbar atas ────────────────────────────────────────────────────
    auto *toolbar = new QHBoxLayout;

    m_lblJumlah = new QLabel(panel);

    m_cmbSort = new QComboBox(panel);
    m_cmbSort->addItems({"Urutkan: Default","Harga Termurah","RAM Terbesar","Stok Terbanyak"});
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

    // Badge compare
    m_lblCompare = new QLabel(panel);
    m_lblCompare->hide();
    vl->addWidget(m_lblCompare);

    // ── Tabel ───────────────────────────────────────────────────────────
    m_table = new QTableWidget(panel);
    m_table->setColumnCount(7);
    m_table->setHorizontalHeaderLabels(
        {"","Nama Produk","CPU","RAM","Storage","Harga","Stok"});
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setShowGrid(false);

    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_table->setColumnWidth(0, 32);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    for (int c = 2; c <= 6; ++c)
        m_table->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);
    m_table->setRowHeight(0, 44);

    vl->addWidget(m_table);

    // Hint double-click
    auto *hint = new QLabel("💡 Klik ganda pada laptop untuk melihat detail spesifikasi lengkap", panel);
    vl->addWidget(hint);

    // Koneksi
    connect(m_cmbSort,        QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CatalogPage::onSortChanged);
    connect(m_btnCompareMode, &QPushButton::toggled,
            this, &CatalogPage::onCompareModeToggled);
    connect(m_btnLihat,       &QPushButton::clicked,
            this, &CatalogPage::onLihatPerbandingan);
    connect(m_table,          &QTableWidget::cellDoubleClicked,
            this, &CatalogPage::onItemDoubleClicked);
}

// ── Refresh Tabel ────────────────────────────────────────────────────────────
void CatalogPage::refreshTable() {
    ManajerKatalog::FilterParam fp;
    if (!m_filterCpuSeri.isEmpty())   fp.cpuSeri         << m_filterCpuSeri;
    if (!m_filterGpuBrand.isEmpty())  fp.gpuBrand        << m_filterGpuBrand;
    if (!m_filterRamTipe.isEmpty())   fp.ramTipe         << m_filterRamTipe;
    if (m_filterRamKap > 0)           fp.ramKapasitas    << QString::number(m_filterRamKap);
    if (!m_filterStorTipe.isEmpty())  fp.storageTipe     << m_filterStorTipe;
    if (m_filterStorKap > 0)          fp.storageKapasitas<< QString::number(m_filterStorKap);
    if (!m_filterDispTipe.isEmpty())  fp.displayTipe     << m_filterDispTipe;
    fp.ports = m_filterPorts;

    m_displayedLaptops = m_katalog->filterDanSort(fp, m_sortMode);

    m_lblJumlah->setText(
        QString("📋 <b>%1</b> produk sesuai dengan hasil filtermu")
        .arg(m_displayedLaptops.size()));

    m_table->setRowCount(m_displayedLaptops.size());
    m_compareChecks.clear();

    QLocale locale(QLocale::Indonesian);

    for (int r = 0; r < m_displayedLaptops.size(); ++r) {
        Laptop *l = m_displayedLaptops[r];
        m_table->setRowHeight(r, 44);

        if (m_compareMode) {
            auto *cb = new QCheckBox;
            cb->setStyleSheet("margin-left:8px;");
            bool sudahDipilih = m_selectedIds.contains(l->getId());
            cb->setChecked(sudahDipilih);
            if (m_selectedIds.size() >= 3 && !sudahDipilih)
                cb->setEnabled(false);
            m_compareChecks[l->getId()] = cb;
            m_table->setCellWidget(r, 0, cb);
            connect(cb, &QCheckBox::toggled, this, [this, id = l->getId()](bool chk) {
                onCheckboxCompareChanged(id, chk);
            });
        } else {
            m_table->setCellWidget(r, 0, nullptr);
            m_table->setItem(r, 0, new QTableWidgetItem(""));
        }

        auto *itemNama = new QTableWidgetItem(
            l->getNama() + "\n" + l->getRingkasanSpek());
        itemNama->setData(Qt::UserRole, l->getId());
        itemNama->setToolTip(l->getRingkasanSpek());
        m_table->setItem(r, 1, itemNama);

        m_table->setItem(r, 2, new QTableWidgetItem(l->getCPU().getSeri()));
        m_table->setItem(r, 3, new QTableWidgetItem(
            QString("%1GB %2").arg(l->getRAM().getKapasitas()).arg(l->getRAM().getTipe())));
        m_table->setItem(r, 4, new QTableWidgetItem(
            l->getStorage().kapasitasLabel() + " " + l->getStorage().getTipe()));
        m_table->setItem(r, 5, new QTableWidgetItem(l->formatHarga()));
        auto *stokItem = new QTableWidgetItem(QString("%1").arg(l->getStok()));
        stokItem->setForeground(l->getStok() <= 5 ? QColor("#EF4444") : QColor(m_isDark ? "#34D399" : "#059669"));
        m_table->setItem(r, 6, stokItem);
    }
}

// ── Filter Changed ────────────────────────────────────────────────────────────
void CatalogPage::onFilterChanged() {
    m_filterCpuSeri.clear(); m_filterGpuBrand.clear();
    m_filterRamTipe.clear(); m_filterRamKap = 0;
    m_filterStorTipe.clear(); m_filterStorKap = 0;
    m_filterDispTipe.clear(); m_filterPorts.clear();

    std::function<void(QTreeWidgetItem*, int)> scan;
    scan = [&](QTreeWidgetItem *node, int level) {
        QString txt = node->text(0).split(" (").first();
        txt = txt.trimmed();

        if ((node->flags() & Qt::ItemIsUserCheckable) &&
            node->checkState(0) == Qt::Checked)
        {
            QTreeWidgetItem *p1 = node->parent();
            QTreeWidgetItem *p2 = p1 ? p1->parent() : nullptr;
            QTreeWidgetItem *p3 = p2 ? p2->parent() : nullptr;

            QString cat1 = p1 ? p1->text(0).remove("▶ ").trimmed() : "";
            QString cat2 = p2 ? p2->text(0).remove("▶ ").trimmed() : "";
            QString cat3 = p3 ? p3->text(0).remove("▶ ").trimmed() : "";

            if (cat2 == "CPU" || cat3 == "CPU")      m_filterCpuSeri = txt;
            else if (cat1 == "GPU")                  m_filterGpuBrand = txt;
            else if (cat2 == "RAM" || cat3 == "RAM") {
                QString kapStr = txt; kapStr.remove("GB");
                m_filterRamKap = kapStr.toInt();
                if (cat1 == "DDR4" || cat2 == "DDR4") m_filterRamTipe = "DDR4";
                if (cat1 == "DDR5" || cat2 == "DDR5") m_filterRamTipe = "DDR5";
            }
            else if (cat2 == "Storage" || cat3 == "Storage") {
                QString kapStr = txt;
                if (kapStr.endsWith("TB")) m_filterStorKap = kapStr.remove("TB").toInt() * 1024;
                else { kapStr.remove("GB"); m_filterStorKap = kapStr.toInt(); }
                if (cat1 == "SSD" || cat2 == "SSD") m_filterStorTipe = "SSD";
                if (cat1 == "HDD" || cat2 == "HDD") m_filterStorTipe = "HDD";
            }
            else if (cat1 == "Display")              m_filterDispTipe = txt;
            else if (cat1.contains("Port")) {
                if (!m_filterPorts.contains(txt)) m_filterPorts << txt;
            }
        }
        for (int i = 0; i < node->childCount(); ++i)
            scan(node->child(i), level + 1);
    };

    for (int i = 0; i < m_filterTree->topLevelItemCount(); ++i)
        scan(m_filterTree->topLevelItem(i), 0);

    refreshTable();
}

void CatalogPage::onSortChanged(int index) {
    switch (index) {
    case 1: m_sortMode = ManajerKatalog::SortMode::HargaAsc;  break;
    case 2: m_sortMode = ManajerKatalog::SortMode::RamDesc;   break;
    case 3: m_sortMode = ManajerKatalog::SortMode::StokDesc;  break;
    default:m_sortMode = ManajerKatalog::SortMode::None;       break;
    }
    refreshTable();
}

void CatalogPage::onCompareModeToggled() {
    m_compareMode = m_btnCompareMode->isChecked();
    m_selectedIds.clear();
    if (m_compareMode) {
        m_lblCompare->show();
        updateCompareBadge();
    } else {
        m_lblCompare->hide();
        m_btnLihat->setEnabled(false);
    }
    refreshTable();
}

void CatalogPage::onCheckboxCompareChanged(const QString &id, bool checked) {
    if (checked) {
        if (!m_selectedIds.contains(id) && m_selectedIds.size() < 3)
            m_selectedIds.append(id);
    } else {
        m_selectedIds.removeAll(id);
    }
    updateCompareBadge();
    refreshTable();
}

void CatalogPage::updateCompareBadge() {
    m_lblCompare->setText(
        QString("⚖️ Pilih maksimal 3 produk (%1/3) — klik ganda untuk detail")
        .arg(m_selectedIds.size()));
    m_btnLihat->setEnabled(m_selectedIds.size() >= 2);
}

void CatalogPage::onLihatPerbandingan() {
    QVector<Laptop*> dipilih;
    for (const QString &id : m_selectedIds)
        if (Laptop *l = m_katalog->cariLaptopById(id))
            dipilih.append(l);
    if (dipilih.size() < 2) return;
    CompareDialog dlg(dipilih, this);
    dlg.exec();
}

void CatalogPage::onItemDoubleClicked(int row, int /*col*/) {
    if (row < 0 || row >= m_displayedLaptops.size()) return;
    Laptop *l = m_displayedLaptops[row];
    DetailDialog dlg(l, this);
    dlg.exec();
}

void CatalogPage::refreshFilterCounts() {}
