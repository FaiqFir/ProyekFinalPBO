#include "AdminPage.h"
#include "StockDialog.h"
#include <QFrame>
#include "../models/LaptopGaming.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QIntValidator>
#include <QFrame>

// ══════════════════════════════════════════════════════════════════════════════
//  FormDialog — Dialog tambah/edit produk dengan validasi input
// ══════════════════════════════════════════════════════════════════════════════
class FormDialog : public QDialog {
    Q_OBJECT
public:
    explicit FormDialog(Laptop *existing, ManajerKatalog *katalog, QWidget *parent = nullptr)
        : QDialog(parent), m_existing(existing), m_katalog(katalog)
    {
        setWindowTitle(existing ? "Edit Produk" : "Tambah Produk Baru");
        setMinimumSize(520, 660);
        setModal(true);

        auto *root = new QVBoxLayout(this);
        root->setSpacing(0);

        // Scroll area agar muat semua field
        auto *scroll = new QScrollArea(this);
        scroll->setWidgetResizable(true);
        auto *content = new QWidget;
        auto *cl = new QVBoxLayout(content);
        cl->setSpacing(16);
        cl->setContentsMargins(20, 16, 20, 16);

        // ── Informasi Umum ────────────────────────────────────────────────
        auto *grpUmum = new QGroupBox("Informasi Umum", content);
        auto *flUmum  = new QFormLayout(grpUmum);
        flUmum->setSpacing(8);

        m_edtNama  = new QLineEdit(existing ? existing->getNama()  : "", content);
        m_edtBrand = new QLineEdit(existing ? existing->getBrand() : "", content);
        m_cmbTipe  = new QComboBox(content);
        m_cmbTipe->addItems({"Laptop","Laptop Gaming"});
        if (existing && dynamic_cast<LaptopGaming*>(existing))
            m_cmbTipe->setCurrentIndex(1);

        flUmum->addRow("Nama Produk *:", m_edtNama);
        flUmum->addRow("Brand *:",       m_edtBrand);
        flUmum->addRow("Tipe:",          m_cmbTipe);
        cl->addWidget(grpUmum);

        // ── CPU ───────────────────────────────────────────────────────────
        auto *grpCpu = new QGroupBox("CPU", content);
        auto *flCpu  = new QFormLayout(grpCpu);
        m_cmbCpuBrand = new QComboBox(content);
        m_cmbCpuBrand->addItems({"Intel","AMD"});
        m_edtCpuSeri  = new QLineEdit(content);
        m_edtCpuSeri->setPlaceholderText("Core i5 / Ryzen 5 ...");
        m_spinCores   = new QSpinBox(content);
        m_spinCores->setRange(1, 64);
        m_edtCpuSpeed = new QLineEdit(content);
        m_edtCpuSpeed->setPlaceholderText("2.4GHz");

        if (existing) {
            m_cmbCpuBrand->setCurrentText(existing->getCPU().getBrand());
            m_edtCpuSeri ->setText(existing->getCPU().getSeri());
            m_spinCores  ->setValue(existing->getCPU().getCores());
            m_edtCpuSpeed->setText(existing->getCPU().getKecepatan());
        }
        flCpu->addRow("Brand CPU:", m_cmbCpuBrand);
        flCpu->addRow("Seri *:",    m_edtCpuSeri);
        flCpu->addRow("Cores:",     m_spinCores);
        flCpu->addRow("Kecepatan:", m_edtCpuSpeed);
        cl->addWidget(grpCpu);

        // ── GPU ───────────────────────────────────────────────────────────
        auto *grpGpu = new QGroupBox("GPU", content);
        auto *flGpu  = new QFormLayout(grpGpu);
        m_cmbGpuBrand = new QComboBox(content);
        m_cmbGpuBrand->addItems({"Intel","AMD","Nvidia"});
        m_edtGpuModel = new QLineEdit(content);
        m_edtGpuModel->setPlaceholderText("UHD Graphics / RTX 3060 ...");
        if (existing) {
            m_cmbGpuBrand->setCurrentText(existing->getGpuBrand());
            m_edtGpuModel->setText(existing->getGpuModel());
        }
        flGpu->addRow("Brand GPU:", m_cmbGpuBrand);
        flGpu->addRow("Model *:",   m_edtGpuModel);
        cl->addWidget(grpGpu);

        // ── RAM ───────────────────────────────────────────────────────────
        auto *grpRam = new QGroupBox("RAM", content);
        auto *flRam  = new QFormLayout(grpRam);
        m_cmbRamTipe = new QComboBox(content);
        m_cmbRamTipe->addItems({"DDR3","DDR4","DDR5"});
        m_spinRamKap  = new QSpinBox(content);
        m_spinRamKap->setRange(1, 256);
        m_spinRamKap->setSuffix(" GB");
        m_spinRamSlot = new QSpinBox(content);
        m_spinRamSlot->setRange(1, 4);
        if (existing) {
            m_cmbRamTipe ->setCurrentText(existing->getRAM().getTipe());
            m_spinRamKap ->setValue(existing->getRAM().getKapasitas());
            m_spinRamSlot->setValue(existing->getRAM().getSlots());
        } else {
            m_spinRamKap->setValue(8);
        }
        flRam->addRow("Tipe RAM:", m_cmbRamTipe);
        flRam->addRow("Kapasitas *:", m_spinRamKap);
        flRam->addRow("Jumlah Slot:", m_spinRamSlot);
        cl->addWidget(grpRam);

        // ── Storage ───────────────────────────────────────────────────────
        auto *grpStor = new QGroupBox("Storage", content);
        auto *flStor  = new QFormLayout(grpStor);
        m_cmbStorTipe = new QComboBox(content);
        m_cmbStorTipe->addItems({"SSD","HDD"});
        m_spinStorKap = new QSpinBox(content);
        m_spinStorKap->setRange(1, 8192);
        m_spinStorKap->setSuffix(" GB");
        if (existing) {
            m_cmbStorTipe->setCurrentText(existing->getStorage().getTipe());
            m_spinStorKap->setValue(existing->getStorage().getKapasitas());
        } else {
            m_spinStorKap->setValue(256);
        }
        flStor->addRow("Tipe:", m_cmbStorTipe);
        flStor->addRow("Kapasitas *:", m_spinStorKap);
        cl->addWidget(grpStor);

        // ── Display ───────────────────────────────────────────────────────
        auto *grpDisp = new QGroupBox("Display", content);
        auto *flDisp  = new QFormLayout(grpDisp);
        m_spinDispUkuran = new QDoubleSpinBox(content);
        m_spinDispUkuran->setRange(10.0, 20.0);
        m_spinDispUkuran->setSingleStep(0.1);
        m_spinDispUkuran->setSuffix(" inci");
        m_cmbDispTipe  = new QComboBox(content);
        m_cmbDispTipe->addItems({"IPS","OLED","TN","VA"});
        m_edtResolusi  = new QLineEdit(content);
        m_edtResolusi->setPlaceholderText("1920x1080");
        m_spinRefresh  = new QSpinBox(content);
        m_spinRefresh->setRange(30, 360);
        m_spinRefresh->setSuffix(" Hz");
        if (existing) {
            m_spinDispUkuran->setValue(existing->getDisplay().getUkuran());
            m_cmbDispTipe   ->setCurrentText(existing->getDisplay().getTipe());
            m_edtResolusi   ->setText(existing->getDisplay().getResolusi());
            m_spinRefresh   ->setValue(existing->getDisplay().getRefreshRate());
        } else {
            m_spinDispUkuran->setValue(15.6);
            m_spinRefresh->setValue(60);
        }
        flDisp->addRow("Ukuran:", m_spinDispUkuran);
        flDisp->addRow("Tipe Panel:", m_cmbDispTipe);
        flDisp->addRow("Resolusi:", m_edtResolusi);
        flDisp->addRow("Refresh Rate:", m_spinRefresh);
        cl->addWidget(grpDisp);

        // ── Port ──────────────────────────────────────────────────────────
        auto *grpPort = new QGroupBox("Port", content);
        auto *flPort  = new QFormLayout(grpPort);
        m_spinUsbA = new QSpinBox(content);
        m_spinUsbA->setRange(0, 6);
        m_chkUsbC  = new QCheckBox("Ada USB-C", content);
        m_chkUsbCPD= new QCheckBox("USB-C mendukung Power Delivery", content);
        m_chkHdmi  = new QCheckBox("Ada HDMI", content);
        m_chkAudio = new QCheckBox("Ada Audio Jack", content);
        if (existing) {
            m_spinUsbA ->setValue(existing->getUsbA());
            m_chkUsbC  ->setChecked(existing->hasUsbC());
            m_chkUsbCPD->setChecked(existing->hasUsbCPD());
            m_chkHdmi  ->setChecked(existing->hasHdmi());
            m_chkAudio ->setChecked(existing->hasAudioJack());
        } else {
            m_spinUsbA->setValue(2);
            m_chkUsbC->setChecked(true);
            m_chkHdmi->setChecked(true);
            m_chkAudio->setChecked(true);
        }
        flPort->addRow("Jumlah USB-A:", m_spinUsbA);
        flPort->addRow("", m_chkUsbC);
        flPort->addRow("", m_chkUsbCPD);
        flPort->addRow("", m_chkHdmi);
        flPort->addRow("", m_chkAudio);
        cl->addWidget(grpPort);

        // ── Harga & Stok ─────────────────────────────────────────────────
        auto *grpHarga = new QGroupBox("Harga & Stok", content);
        auto *flHarga  = new QFormLayout(grpHarga);

        // Harga: QLineEdit dengan validator integer (Validasi Input)
        m_edtHarga = new QLineEdit(content);
        m_edtHarga->setValidator(new QIntValidator(0, 999999999, this));
        m_edtHarga->setPlaceholderText("Contoh: 15000000 (hanya angka)");
        if (existing) m_edtHarga->setText(QString::number(existing->getHarga()));

        m_spinStok = new QSpinBox(content);
        m_spinStok->setRange(0, 9999);
        if (existing) m_spinStok->setValue(existing->getStok());

        m_cmbGaransi = new QComboBox(content);
        m_cmbGaransi->addItems({"1 Tahun","2 Tahun","3 Tahun","Tidak ada garansi"});
        if (existing) m_cmbGaransi->setCurrentText(existing->getGaransi());

        flHarga->addRow("Harga (Rp) *:", m_edtHarga);
        flHarga->addRow("Stok awal:", m_spinStok);
        flHarga->addRow("Garansi:", m_cmbGaransi);
        cl->addWidget(grpHarga);

        // Label error validasi
        m_lblError = new QLabel(content);
        m_lblError->setStyleSheet("color:#DC2626; font-size:12px;");
        m_lblError->hide();
        cl->addWidget(m_lblError);

        scroll->setWidget(content);
        root->addWidget(scroll);

        // Tombol
        auto *btnBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
        btnBox->button(QDialogButtonBox::Ok)->setText("Simpan");
        btnBox->button(QDialogButtonBox::Cancel)->setText("Batal");
        btnBox->button(QDialogButtonBox::Ok)->setStyleSheet(
            "QPushButton { background:#3B82F6; color:white; border-radius:6px;"
            " padding:6px 20px; font-size:13px; border:none; }"
            "QPushButton:hover { background:#2563EB; }");
        root->addWidget(btnBox);

        connect(btnBox, &QDialogButtonBox::accepted, this, &FormDialog::onSimpan);
        connect(btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    }

    // Hasil setelah simpan — ambil pointer laptop baru/terupdate
    Laptop* getResult() const { return m_result; }

private slots:
    void onSimpan() {
        // ── Validasi ─────────────────────────────────────────────────────
        if (m_edtNama->text().trimmed().isEmpty()) {
            showError("Nama produk tidak boleh kosong.");  return;
        }
        if (m_edtBrand->text().trimmed().isEmpty()) {
            showError("Brand tidak boleh kosong.");        return;
        }
        if (m_edtCpuSeri->text().trimmed().isEmpty()) {
            showError("Seri CPU tidak boleh kosong.");     return;
        }
        if (m_edtGpuModel->text().trimmed().isEmpty()) {
            showError("Model GPU tidak boleh kosong.");    return;
        }
        if (m_edtHarga->text().trimmed().isEmpty()) {
            showError("Harga tidak boleh kosong.");        return;
        }
        // Validasi harga harus angka positif
        bool ok;
        int harga = m_edtHarga->text().toInt(&ok);
        if (!ok || harga < 0) {
            showError("Harga harus berupa angka positif (tanpa huruf/simbol).");
            return;
        }

        // Buat objek Spek
        SpekCPU cpu(m_cmbCpuBrand->currentText(), m_edtCpuSeri->text().trimmed(),
                    m_spinCores->value(), m_edtCpuSpeed->text().trimmed());
        SpekRAM ram(m_cmbRamTipe->currentText(), m_spinRamKap->value(),
                    m_spinRamSlot->value(), 0);
        SpekPenyimpanan stor(m_cmbStorTipe->currentText(), m_spinStorKap->value(), 1, 0);
        SpekDisplay disp(m_spinDispUkuran->value(), m_cmbDispTipe->currentText(),
                         m_edtResolusi->text().trimmed(), m_spinRefresh->value());

        QString id = m_existing ? m_existing->getId() : m_katalog->generateId();
        bool isGaming = m_cmbTipe->currentIndex() == 1;

        if (isGaming) {
            auto *lg = new LaptopGaming(
                id, m_edtNama->text().trimmed(), m_edtBrand->text().trimmed(),
                cpu, ram, stor, disp,
                m_cmbGpuBrand->currentText(), m_edtGpuModel->text().trimmed(),
                m_spinUsbA->value(), m_chkUsbC->isChecked(), m_chkUsbCPD->isChecked(),
                m_chkHdmi->isChecked(), m_chkAudio->isChecked(),
                harga, m_spinStok->value(), m_cmbGaransi->currentText(),
                45, false);
            m_result = lg;
        } else {
            m_result = new Laptop(
                id, m_edtNama->text().trimmed(), m_edtBrand->text().trimmed(),
                cpu, ram, stor, disp,
                m_cmbGpuBrand->currentText(), m_edtGpuModel->text().trimmed(),
                m_spinUsbA->value(), m_chkUsbC->isChecked(), m_chkUsbCPD->isChecked(),
                m_chkHdmi->isChecked(), m_chkAudio->isChecked(),
                harga, m_spinStok->value(), m_cmbGaransi->currentText());
        }
        accept();
    }

    void showError(const QString &msg) {
        m_lblError->setText("❌ " + msg);
        m_lblError->show();
    }

private:
    Laptop         *m_existing;
    Laptop         *m_result = nullptr;
    ManajerKatalog *m_katalog;

    QLineEdit     *m_edtNama, *m_edtBrand;
    QComboBox     *m_cmbTipe;
    QComboBox     *m_cmbCpuBrand;
    QLineEdit     *m_edtCpuSeri, *m_edtCpuSpeed;
    QSpinBox      *m_spinCores;
    QComboBox     *m_cmbGpuBrand;
    QLineEdit     *m_edtGpuModel;
    QComboBox     *m_cmbRamTipe;
    QSpinBox      *m_spinRamKap, *m_spinRamSlot;
    QComboBox     *m_cmbStorTipe;
    QSpinBox      *m_spinStorKap;
    QDoubleSpinBox*m_spinDispUkuran;
    QComboBox     *m_cmbDispTipe;
    QLineEdit     *m_edtResolusi;
    QSpinBox      *m_spinRefresh;
    QSpinBox      *m_spinUsbA;
    QCheckBox     *m_chkUsbC, *m_chkUsbCPD, *m_chkHdmi, *m_chkAudio;
    QLineEdit     *m_edtHarga;
    QSpinBox      *m_spinStok;
    QComboBox     *m_cmbGaransi;
    QLabel        *m_lblError;
};

// ══════════════════════════════════════════════════════════════════════════════
//  AdminPage
// ══════════════════════════════════════════════════════════════════════════════
AdminPage::AdminPage(ManajerKatalog *katalog, QWidget *parent)
    : QWidget(parent), m_katalog(katalog)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ── Top bar: tombol kembali kiri atas ─────────────────────────────────
    auto *topBar = new QWidget(this);
    topBar->setObjectName("adminTopBar");
    topBar->setFixedHeight(50);
    auto *topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(12, 0, 12, 0);
    topBarLayout->setSpacing(10);

    m_btnBack = new QPushButton("← Menu Utama", topBar);
    m_btnBack->setObjectName("adminBtnBack");
    m_btnBack->setFixedHeight(34);
    m_btnBack->setCursor(Qt::PointingHandCursor);

    auto *pageTitle = new QLabel("📦  Manajemen Stok Produk", topBar);
    pageTitle->setObjectName("adminPageTitle");
    pageTitle->setStyleSheet("font-size:15px; font-weight:bold;");

    m_lblTotal = new QLabel(topBar);
    m_lblTotal->setObjectName("adminLblTotal");

    topBarLayout->addWidget(m_btnBack);
    topBarLayout->addSpacing(12);
    topBarLayout->addWidget(pageTitle);
    topBarLayout->addSpacing(12);
    topBarLayout->addWidget(m_lblTotal);
    topBarLayout->addStretch();
    root->addWidget(topBar);

    // ── Garis pemisah ─────────────────────────────────────────────────────
    auto *divider = new QFrame(this);
    divider->setObjectName("adminDivider");
    divider->setFrameShape(QFrame::HLine);
    divider->setFixedHeight(1);
    root->addWidget(divider);

    // ── Isi konten (padding) ──────────────────────────────────────────────
    auto *contentWidget = new QWidget(this);
    contentWidget->setObjectName("adminContent");
    auto *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(16, 10, 16, 12);
    contentLayout->setSpacing(10);

    // Toolbar CRUD
    auto *toolbar = new QHBoxLayout;
    auto *btnTambah = new QPushButton("+ Tambah Produk", contentWidget);
    m_btnEdit  = new QPushButton("✏️ Edit", contentWidget);
    m_btnHapus = new QPushButton("🗑️ Hapus", contentWidget);
    m_btnStok  = new QPushButton("📦 Kelola Stok", contentWidget);

    auto styleBtn = [](QPushButton *b, const QString &bg, const QString &hover) {
        b->setFixedHeight(36);
        b->setStyleSheet(QString(
            "QPushButton { background:%1; color:white; border-radius:6px;"
            " font-size:13px; padding:0 14px; border:none; }"
            "QPushButton:hover { background:%2; }"
            "QPushButton:disabled { background:#64748B; color:#94A3B8; }").arg(bg, hover));
    };
    styleBtn(btnTambah, "#3B82F6","#2563EB");
    styleBtn(m_btnEdit, "#F59E0B","#D97706");
    styleBtn(m_btnHapus,"#EF4444","#DC2626");
    styleBtn(m_btnStok, "#059669","#047857");

    m_btnEdit ->setEnabled(false);
    m_btnHapus->setEnabled(false);
    m_btnStok ->setEnabled(false);

    toolbar->addWidget(btnTambah);
    toolbar->addWidget(m_btnEdit);
    toolbar->addWidget(m_btnHapus);
    toolbar->addWidget(m_btnStok);
    toolbar->addStretch();
    contentLayout->addLayout(toolbar);

    // Tabel
    m_table = new QTableWidget(contentWidget);
    m_table->setObjectName("adminTable");
    m_table->setColumnCount(8);
    m_table->setHorizontalHeaderLabels(
        {"ID","Nama","Tipe","CPU","RAM","Storage","Harga","Stok"});
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setShowGrid(false);

    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_table->setColumnWidth(0, 40);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    for (int c : {2,3,4,5,6,7})
        m_table->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);

    contentLayout->addWidget(m_table);
    root->addWidget(contentWidget, 1);

    // Koneksi
    connect(m_btnBack,  &QPushButton::clicked, this, &AdminPage::kembali);
    connect(btnTambah,  &QPushButton::clicked, this, &AdminPage::onTambah);
    connect(m_btnEdit,  &QPushButton::clicked, this, &AdminPage::onEdit);
    connect(m_btnHapus, &QPushButton::clicked, this, &AdminPage::onHapus);
    connect(m_btnStok,  &QPushButton::clicked, this, &AdminPage::onKelolStok);
    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AdminPage::onSelectionChanged);

    refreshTable();
    applyTheme(false);
}

// ── applyTheme ────────────────────────────────────────────────────────────────
void AdminPage::applyTheme(bool isDark)
{
    m_isDark = isDark;

    const QString bg        = isDark ? "#0F172A" : "#FFFFFF";
    const QString topBarBg  = isDark ? "#1E293B" : "#FFFFFF";
    const QString border    = isDark ? "#334155" : "#E2E8F0";
    const QString textMain  = isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub   = isDark ? "#94A3B8" : "#64748B";
    const QString headerBg  = isDark ? "#1E293B" : "#F8FAFC";
    const QString rowBg     = isDark ? "#0F172A" : "#FFFFFF";
    const QString rowAlt    = isDark ? "#1A2744" : "#F0F9FF";
    const QString selBg     = isDark ? "#1D4ED8" : "#EFF6FF";
    const QString selTxt    = isDark ? "#FFFFFF"  : "#1E293B";
    const QString btnBg     = isDark ? "#1E293B" : "white";
    const QString btnBorder = isDark ? "#334155" : "#CBD5E1";
    const QString btnTxt    = isDark ? "#94A3B8" : "#475569";
    const QString btnHover  = isDark ? "#334155" : "#F1F5F9";

    // Top bar
    if (auto *w = findChild<QWidget*>("adminTopBar"))
        w->setStyleSheet(QString("QWidget#adminTopBar { background:%1; }").arg(topBarBg));

    // Garis pemisah
    if (auto *w = findChild<QFrame*>("adminDivider"))
        w->setStyleSheet(QString("background:%1;").arg(border));

    // Area konten
    if (auto *w = findChild<QWidget*>("adminContent"))
        w->setStyleSheet(QString("QWidget#adminContent { background:%1; }").arg(bg));

    // Judul halaman
    if (auto *w = findChild<QLabel*>("adminPageTitle"))
        w->setStyleSheet(QString("font-size:15px; font-weight:bold; color:%1; background:transparent;").arg(textMain));

    // Label total
    if (m_lblTotal)
        m_lblTotal->setStyleSheet(QString("color:%1; font-size:13px; background:transparent;").arg(textSub));

    // Tombol kembali
    if (m_btnBack)
        m_btnBack->setStyleSheet(QString(
            "QPushButton { background:%1; border:1px solid %2; border-radius:7px;"
            " color:%3; font-size:13px; padding:0 14px; }"
            "QPushButton:hover { background:%4; }")
            .arg(btnBg, btnBorder, btnTxt, btnHover));

    // Tabel
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
            .arg(border, rowBg, textMain, rowAlt, selBg, selTxt, headerBg, textSub));
}

void AdminPage::refreshTable() {
    m_displayed.clear();
    for (auto *p : m_katalog->semuaProduk())
        if (Laptop *l = dynamic_cast<Laptop*>(p))
            m_displayed.append(l);

    m_table->setRowCount(m_displayed.size());
    m_lblTotal->setText(QString("Total: %1 produk").arg(m_displayed.size()));

    for (int r = 0; r < m_displayed.size(); ++r) {
        Laptop *l = m_displayed[r];
        m_table->setRowHeight(r, 40);
        m_table->setItem(r, 0, new QTableWidgetItem(l->getId()));
        m_table->setItem(r, 1, new QTableWidgetItem(l->getNama()));
        m_table->setItem(r, 2, new QTableWidgetItem(l->getTipePerangkat()));
        m_table->setItem(r, 3, new QTableWidgetItem(l->getCPU().getSeri()));
        m_table->setItem(r, 4, new QTableWidgetItem(
            QString("%1GB %2").arg(l->getRAM().getKapasitas()).arg(l->getRAM().getTipe())));
        m_table->setItem(r, 5, new QTableWidgetItem(
            l->getStorage().kapasitasLabel() + " " + l->getStorage().getTipe()));
        m_table->setItem(r, 6, new QTableWidgetItem(l->formatHarga()));
        auto *stokItem = new QTableWidgetItem(QString::number(l->getStok()));
        stokItem->setForeground(l->getStok() <= 3 ? QColor("#EF4444") : QColor("#059669"));
        m_table->setItem(r, 7, stokItem);
    }
}

void AdminPage::onSelectionChanged() {
    bool ada = !m_table->selectedItems().isEmpty();
    m_btnEdit ->setEnabled(ada);
    m_btnHapus->setEnabled(ada);
    m_btnStok ->setEnabled(ada);
}

void AdminPage::onTambah() {
    FormDialog dlg(nullptr, m_katalog, this);
    if (dlg.exec() == QDialog::Accepted) {
        Laptop *baru = dlg.getResult();
        if (auto *lg = dynamic_cast<LaptopGaming*>(baru))
            m_katalog->tambahLaptopGaming(lg);
        else
            m_katalog->tambahLaptop(baru);
        refreshTable();
        QMessageBox::information(this, "Berhasil",
            "Produk \"" + baru->getNama() + "\" berhasil ditambahkan.");
    }
}

void AdminPage::onEdit() {
    int row = m_table->currentRow();
    if (row < 0 || row >= m_displayed.size()) return;
    Laptop *l = m_displayed[row];

    FormDialog dlg(l, m_katalog, this);
    if (dlg.exec() == QDialog::Accepted) {
        Laptop *updated = dlg.getResult();
        // Salin data baru ke objek yang sudah ada
        l->setNama   (updated->getNama());
        l->setBrand  (updated->getBrand());
        l->setCPU    (updated->getCPU());
        l->setRAM    (updated->getRAM());
        l->setStorage(updated->getStorage());
        l->setDisplay(updated->getDisplay());
        l->setGpuBrand(updated->getGpuBrand());
        l->setGpuModel(updated->getGpuModel());
        l->setUsbA   (updated->getUsbA());
        l->setUsbC   (updated->hasUsbC());
        l->setUsbCPD (updated->hasUsbCPD());
        l->setHdmi   (updated->hasHdmi());
        l->setAudioJack(updated->hasAudioJack());
        l->setHarga  (updated->getHarga());
        l->setStok   (updated->getStok());
        l->setGaransi(updated->getGaransi());
        delete updated;
        refreshTable();
        QMessageBox::information(this, "Berhasil", "Data produk berhasil diperbarui.");
    }
}

void AdminPage::onHapus() {
    int row = m_table->currentRow();
    if (row < 0 || row >= m_displayed.size()) return;
    Laptop *l = m_displayed[row];

    // Konfirmasi sebelum hapus (pop-up peringatan)
    auto ret = QMessageBox::warning(this, "Konfirmasi Hapus",
        QString("Anda yakin ingin menghapus produk:\n\n\"%1\"?\n\n"
                "Tindakan ini tidak dapat dibatalkan.").arg(l->getNama()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        QString nama = l->getNama();
        m_katalog->hapusProduk(l->getId());
        refreshTable();
        QMessageBox::information(this, "Berhasil",
            "Produk \"" + nama + "\" berhasil dihapus.");
    }
}

void AdminPage::onKelolStok() {
    int row = m_table->currentRow();
    if (row < 0 || row >= m_displayed.size()) return;
    Laptop *l = m_displayed[row];

    StockDialog dlg(l, this);
    if (dlg.exec() == QDialog::Accepted) {
        bool ok;
        if (dlg.getTipe() == "in")
            ok = m_katalog->updateStokMasuk(l->getId(), dlg.getJumlah());
        else
            ok = m_katalog->updateStokTerjual(l->getId(), dlg.getJumlah());

        if (ok) {
            refreshTable();
            QString aksi = dlg.getTipe() == "in" ? "ditambahkan ke" : "dikurangi dari";
            QMessageBox::information(this, "Berhasil",
                QString("Stok %1 unit berhasil %2 produk \"%3\".\nStok sekarang: %4 unit.")
                .arg(dlg.getJumlah()).arg(aksi).arg(l->getNama()).arg(l->getStok()));
        } else {
            QMessageBox::warning(this, "Gagal",
                "Gagal memperbarui stok. Pastikan jumlah valid.");
        }
    }
}
#include "AdminPage.moc"
