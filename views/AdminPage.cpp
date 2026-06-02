#include "AdminPage.h"
#include "StockDialog.h"
#include "DetailDialog.h"
#include "../models/LaptopGaming.h"
#include "../models/LaptopDesain.h"
#include "../models/LaptopWorkstation.h"
#include <QFrame>
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
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>

// ══════════════════════════════════════════════════════════════════════════════
//  FormDialog — Dialog tambah/edit produk (dengan tema)
// ══════════════════════════════════════════════════════════════════════════════
class FormDialog : public QDialog {
    Q_OBJECT

    // helper — harus dideklarasikan sebelum dipakai di constructor
    void updateCpuSeriOptions(const QString &brand) {
        m_cmbCpuSeri->clear();
        if (brand == "Intel")
            m_cmbCpuSeri->addItems({"Celeron","Core i3","Core i5","Core i7","Core i9"});
        else
            m_cmbCpuSeri->addItems({"Athlon","Ryzen 3","Ryzen 5","Ryzen 7","Ryzen 9"});
    }

public:
    explicit FormDialog(Laptop *existing, ManajerKatalog *katalog, bool isDark,
                        QWidget *parent = nullptr)
        : QDialog(parent), m_existing(existing), m_katalog(katalog)
    {
        setWindowTitle(existing ? "Edit Produk" : "Tambah Produk Baru");
        setMinimumSize(520, 660);
        setModal(true);

        // ── Warna tema ────────────────────────────────────────────────────
        const QString bgDialog  = isDark ? "#1E293B" : "#FFFFFF";
        const QString textMain  = isDark ? "#F1F5F9" : "#1E293B";
        const QString textSub   = isDark ? "#94A3B8" : "#64748B";
        const QString inputBg   = isDark ? "#0F172A"  : "#FFFFFF";
        const QString inputBrd  = isDark ? "#334155"  : "#CBD5E1";
        const QString inputTxt  = isDark ? "#F1F5F9"  : "#1E293B";
        const QString inputFoc  = isDark ? "#3B82F6"  : "#3B82F6";
        const QString grpBrd    = isDark ? "#334155"  : "#E2E8F0";
        const QString grpTitle  = isDark ? "#93C5FD"  : "#3B82F6";
        const QString btnCnlBg  = isDark ? "#1E293B"  : "#FFFFFF";
        const QString btnCnlBrd = isDark ? "#334155"  : "#CBD5E1";
        const QString btnCnlTxt = isDark ? "#94A3B8"  : "#475569";
        const QString btnCnlHov = isDark ? "#334155"  : "#F1F5F9";
        const QString scrollBg  = isDark ? "#1E293B"  : "#FFFFFF";
        const QString grpBg     = isDark ? "#1E293B"  : "#FFFFFF";
        // Custom checkbox style (sama seperti di CatalogPage filter)
        const QString cbBg      = isDark ? "#1E293B"  : "#FFFFFF";
        const QString cbBorder  = isDark ? "#334155"  : "#94A3B8";
        const QString cbCheck   = "#3B82F6";

        setStyleSheet(QString(
            "QDialog { background:%1; }"
            "QScrollArea { border:none; background:%8; }"
            "QWidget { background:%8; }"
            "QGroupBox { border:1px solid %2; border-radius:8px; margin-top:10px;"
            " padding:8px; background:%9; color:%3; font-weight:bold; font-size:13px; }"
            "QGroupBox::title { subcontrol-origin:margin; subcontrol-position:top left;"
            " left:10px; color:%4; background:%9; }"
            "QLabel { color:%3; background:transparent; font-size:13px; }"
            "QLineEdit, QSpinBox, QDoubleSpinBox { border:1px solid %2; border-radius:6px;"
            " padding:0 8px; font-size:13px; background:%5; color:%6; }"
            "QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus { border-color:%7; }"
            // Hapus tombol up/down spinner pada QSpinBox / QDoubleSpinBox
            "QSpinBox::up-button, QSpinBox::down-button,"
            " QDoubleSpinBox::up-button, QDoubleSpinBox::down-button { width:0; height:0; border:none; }"
            "QComboBox { border:1px solid %2; border-radius:6px; padding:0 8px;"
            " font-size:13px; background:%5; color:%6; }"
            "QComboBox QAbstractItemView { background:%5; color:%6; border:1px solid %2; }"
            // Custom checkbox indicator (sama seperti filter di katalog)
            "QCheckBox { color:%3; font-size:13px; background:transparent; spacing:8px; }"
            "QCheckBox::indicator { width:18px; height:18px; border-radius:4px; }"
            "QCheckBox::indicator:unchecked { border:2px solid %10; background:%11; }"
            "QCheckBox::indicator:checked { border:2px solid %12; background:%12;"
            " image:url(:/images/check_white.png); }")
            .arg(bgDialog, grpBrd, textMain, grpTitle, inputBg, inputTxt, inputFoc,
                 scrollBg, grpBg, cbBorder, cbBg, cbCheck));

        auto *root = new QVBoxLayout(this);
        root->setSpacing(0);

        auto *scroll  = new QScrollArea(this);
        scroll->setWidgetResizable(true);
        auto *content = new QWidget;
        auto *cl      = new QVBoxLayout(content);
        cl->setSpacing(16);
        cl->setContentsMargins(20,16,20,16);

        // ── Gambar Produk ──────────────────────────────────────────────────
        auto *grpGambar = new QGroupBox("Gambar Produk", content);
        auto *flGambar  = new QVBoxLayout(grpGambar);
        flGambar->setSpacing(8);

        // Preview thumbnail
        m_lblPreview = new QLabel(content);
        m_lblPreview->setFixedHeight(120);
        m_lblPreview->setAlignment(Qt::AlignCenter);
        m_lblPreview->setStyleSheet(
            QString("QLabel { border:1px dashed %1; border-radius:8px;"
                    " background:%2; color:%3; font-size:12px; }")
            .arg(grpBrd, inputBg, textSub));
        m_lblPreview->setText("Belum ada gambar");
        flGambar->addWidget(m_lblPreview);

        // Row tombol pilih & hapus
        auto *gambarBtnRow = new QHBoxLayout;
        m_btnPilihGambar = new QPushButton("📁  Pilih Gambar...", content);
        m_btnPilihGambar->setFixedHeight(34);
        m_btnPilihGambar->setStyleSheet(
            "QPushButton { background:#3B82F6; color:white; border-radius:6px;"
            " font-size:13px; border:none; }"
            "QPushButton:hover { background:#2563EB; }");

        m_btnHapusGambar = new QPushButton("✕  Hapus", content);
        m_btnHapusGambar->setFixedHeight(34);
        m_btnHapusGambar->setEnabled(false);
        m_btnHapusGambar->setStyleSheet(
            "QPushButton { background:#EF4444; color:white; border-radius:6px;"
            " font-size:13px; border:none; }"
            "QPushButton:hover { background:#DC2626; }"
            "QPushButton:disabled { background:#94A3B8; }");

        gambarBtnRow->addWidget(m_btnPilihGambar);
        gambarBtnRow->addWidget(m_btnHapusGambar);
        flGambar->addLayout(gambarBtnRow);

        m_lblGambarPath = new QLabel("", content);
        m_lblGambarPath->setStyleSheet(
            QString("color:%1; font-size:11px; background:transparent;").arg(textSub));
        m_lblGambarPath->setWordWrap(true);
        flGambar->addWidget(m_lblGambarPath);

        cl->addWidget(grpGambar);

        // Isi gambar jika edit existing
        if (existing && !existing->getImagePaths().isEmpty()) {
            m_imagePaths = existing->getImagePaths();
            m_lblGambarPath->setText(m_imagePaths.first());
            QPixmap px(m_imagePaths.first());
            if (!px.isNull()) {
                m_lblPreview->setPixmap(
                    px.scaled(m_lblPreview->width() > 0 ? m_lblPreview->width() : 460,
                              120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            m_btnHapusGambar->setEnabled(true);
        }

        connect(m_btnPilihGambar, &QPushButton::clicked, this, [this]() {
            QString path = QFileDialog::getOpenFileName(
                this, "Pilih Gambar Produk", "",
                "Gambar (*.png *.jpg *.jpeg *.webp *.bmp)");
            if (path.isEmpty()) return;
            m_imagePaths = {path};
            m_lblGambarPath->setText(path);
            QPixmap px(path);
            if (!px.isNull()) {
                m_lblPreview->setPixmap(
                    px.scaled(m_lblPreview->width() > 0 ? m_lblPreview->width() : 460,
                              120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
            m_btnHapusGambar->setEnabled(true);
        });

        connect(m_btnHapusGambar, &QPushButton::clicked, this, [this]() {
            m_imagePaths.clear();
            m_lblGambarPath->setText("");
            m_lblPreview->setPixmap(QPixmap());
            m_lblPreview->setText("Belum ada gambar");
            m_btnHapusGambar->setEnabled(false);
        });

        // ── Informasi Umum ─────────────────────────────────────────────────
        auto *grpUmum = new QGroupBox("Informasi Umum", content);
        auto *flUmum  = new QFormLayout(grpUmum);
        flUmum->setSpacing(8);

        m_edtNama  = new QLineEdit(existing ? existing->getNama()  : "", content);
        m_edtBrand = new QLineEdit(existing ? existing->getBrand() : "", content);
        m_edtNama->setFixedHeight(36);
        m_edtBrand->setFixedHeight(36);

        m_cmbJenis = new QComboBox(content);
        m_cmbJenis->addItems({"Laptop","Laptop Gaming","Laptop Desain","Laptop Workstation"});
        if (existing) {
            if      (dynamic_cast<LaptopGaming*>(existing))      m_cmbJenis->setCurrentIndex(1);
            else if (dynamic_cast<LaptopDesain*>(existing))      m_cmbJenis->setCurrentIndex(2);
            else if (dynamic_cast<LaptopWorkstation*>(existing)) m_cmbJenis->setCurrentIndex(3);
        }
        m_cmbJenis->setFixedHeight(36);

        flUmum->addRow("Nama Produk:", m_edtNama);
        flUmum->addRow("Brand:",       m_edtBrand);
        flUmum->addRow("Jenis:",       m_cmbJenis);
        cl->addWidget(grpUmum);

        // ── CPU ────────────────────────────────────────────────────────────
        auto *grpCpu = new QGroupBox("CPU", content);
        auto *flCpu  = new QFormLayout(grpCpu);
        flCpu->setSpacing(8);

        m_cmbCpuBrand = new QComboBox(content);
        m_cmbCpuBrand->addItems({"Intel","AMD"});
        m_cmbCpuBrand->setFixedHeight(36);

        // Seri jadi ComboBox — isi berubah sesuai brand
        m_cmbCpuSeri = new QComboBox(content);
        m_cmbCpuSeri->setFixedHeight(36);
        updateCpuSeriOptions(m_cmbCpuBrand->currentText());

        m_spinCores  = new QSpinBox(content);
        m_spinCores->setRange(1,64);
        m_spinCores->setFixedHeight(36);

        // Model number CPU, e.g. "1335U", "12700H", "5600H"
        m_edtCpuSpeed = new QLineEdit(content);
        m_edtCpuSpeed->setFixedHeight(36);
        m_edtCpuSpeed->setPlaceholderText("Contoh: 1335U, 12700H, 5600H");

        // Max Boost GHz
        m_edtCpuMaxBoost = new QLineEdit(content);
        m_edtCpuMaxBoost->setFixedHeight(36);
        m_edtCpuMaxBoost->setPlaceholderText("Contoh: 4.6");
        m_edtCpuMaxBoost->setValidator(new QRegularExpressionValidator(
            QRegularExpression(R"(\d+(\.\d{1,2})?)"), this));

        if (existing) {
            m_cmbCpuBrand->setCurrentText(existing->getCPU().getBrand());
            updateCpuSeriOptions(existing->getCPU().getBrand());
            m_cmbCpuSeri ->setCurrentText(existing->getCPU().getSeri());
            m_spinCores  ->setValue(existing->getCPU().getCores());
            m_edtCpuSpeed->setText(existing->getCPU().getKecepatan());
            m_edtCpuMaxBoost->setText(existing->getCPU().getMaxBoost());
        }

        connect(m_cmbCpuBrand, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this]() { updateCpuSeriOptions(m_cmbCpuBrand->currentText()); });

        flCpu->addRow("Brand CPU:",    m_cmbCpuBrand);
        flCpu->addRow("Seri:",         m_cmbCpuSeri);
        flCpu->addRow("Cores:",        m_spinCores);
        flCpu->addRow("Model:",        m_edtCpuSpeed);

        auto *boostRow = new QHBoxLayout;
        boostRow->addWidget(m_edtCpuMaxBoost);
        boostRow->addWidget(new QLabel("GHz", content));
        flCpu->addRow("Max Boost:", boostRow);
        cl->addWidget(grpCpu);

        // ── GPU ────────────────────────────────────────────────────────────
        auto *grpGpu = new QGroupBox("GPU", content);
        auto *flGpu  = new QFormLayout(grpGpu);
        flGpu->setSpacing(8);

        m_cmbGpuBrand = new QComboBox(content);
        m_cmbGpuBrand->addItems({"Intel","AMD","Nvidia"});
        m_cmbGpuBrand->setFixedHeight(36);

        m_edtGpuModel = new QLineEdit(content);
        m_edtGpuModel->setPlaceholderText("Contoh: RTX 4060");
        m_edtGpuModel->setFixedHeight(36);

        if (existing) {
            m_cmbGpuBrand->setCurrentText(existing->getGpuBrand());
            m_edtGpuModel->setText(existing->getGpuModel());
        }
        flGpu->addRow("Brand GPU:", m_cmbGpuBrand);
        flGpu->addRow("Model:",     m_edtGpuModel);
        cl->addWidget(grpGpu);

        // ── RAM ────────────────────────────────────────────────────────────
        auto *grpRam = new QGroupBox("RAM", content);
        auto *flRam  = new QFormLayout(grpRam);
        flRam->setSpacing(8);

        m_cmbRamTipe = new QComboBox(content);
        m_cmbRamTipe->addItems({"DDR3","DDR4","DDR5"});
        m_cmbRamTipe->setFixedHeight(36);

        m_spinRamKap = new QSpinBox(content);
        m_spinRamKap->setRange(1,256);
        m_spinRamKap->setSuffix(" GB");
        m_spinRamKap->setFixedHeight(36);

        m_spinRamSlot = new QSpinBox(content);
        m_spinRamSlot->setRange(1,4);
        m_spinRamSlot->setFixedHeight(36);

        if (existing) {
            m_cmbRamTipe ->setCurrentText(existing->getRAM().getTipe());
            m_spinRamKap ->setValue(existing->getRAM().getKapasitas());
            m_spinRamSlot->setValue(existing->getRAM().getSlots());
        } else { m_spinRamKap->setValue(8); }

        flRam->addRow("Tipe RAM:",    m_cmbRamTipe);
        flRam->addRow("Kapasitas:",   m_spinRamKap);
        flRam->addRow("Jumlah Slot:", m_spinRamSlot);
        cl->addWidget(grpRam);

        // ── Storage ────────────────────────────────────────────────────────
        auto *grpStor = new QGroupBox("Storage", content);
        auto *flStor  = new QFormLayout(grpStor);
        flStor->setSpacing(8);

        m_cmbStorTipe = new QComboBox(content);
        m_cmbStorTipe->addItems({"SSD","HDD"});
        m_cmbStorTipe->setFixedHeight(36);

        m_spinStorKap = new QSpinBox(content);
        m_spinStorKap->setRange(1,8192);
        m_spinStorKap->setSuffix(" GB");
        m_spinStorKap->setFixedHeight(36);

        if (existing) {
            m_cmbStorTipe->setCurrentText(existing->getStorage().getTipe());
            m_spinStorKap->setValue(existing->getStorage().getKapasitas());
        } else { m_spinStorKap->setValue(256); }

        flStor->addRow("Tipe:",     m_cmbStorTipe);
        flStor->addRow("Kapasitas:",m_spinStorKap);
        cl->addWidget(grpStor);

        // ── Display ────────────────────────────────────────────────────────
        auto *grpDisp = new QGroupBox("Display", content);
        auto *flDisp  = new QFormLayout(grpDisp);
        flDisp->setSpacing(8);

        m_spinDispUkuran = new QDoubleSpinBox(content);
        m_spinDispUkuran->setRange(10.0,20.0);
        m_spinDispUkuran->setSingleStep(0.1);
        m_spinDispUkuran->setSuffix(" inci");
        m_spinDispUkuran->setFixedHeight(36);

        m_cmbDispTipe = new QComboBox(content);
        m_cmbDispTipe->addItems({"IPS","OLED","TN","VA"});
        m_cmbDispTipe->setFixedHeight(36);

        m_edtResolusi = new QLineEdit(content);
        m_edtResolusi->setPlaceholderText("Contoh: 1920x1080");
        m_edtResolusi->setFixedHeight(36);

        m_spinRefresh = new QSpinBox(content);
        m_spinRefresh->setRange(30,360);
        m_spinRefresh->setSuffix(" Hz");
        m_spinRefresh->setFixedHeight(36);

        m_spinSrgb = new QSpinBox(content);
        m_spinSrgb->setRange(1,100);
        m_spinSrgb->setSuffix(" %");
        m_spinSrgb->setFixedHeight(36);

        if (existing) {
            m_spinDispUkuran->setValue(existing->getDisplay().getUkuran());
            m_cmbDispTipe   ->setCurrentText(existing->getDisplay().getTipe());
            m_edtResolusi   ->setText(existing->getDisplay().getResolusi());
            m_spinRefresh   ->setValue(existing->getDisplay().getRefreshRate());
            m_spinSrgb      ->setValue(existing->getDisplay().getSrgb());
        } else {
            m_spinDispUkuran->setValue(15.6);
            m_spinRefresh->setValue(60);
            m_spinSrgb->setValue(72);
        }

        flDisp->addRow("Ukuran:",       m_spinDispUkuran);
        flDisp->addRow("Tipe Panel:",   m_cmbDispTipe);
        flDisp->addRow("Resolusi:",     m_edtResolusi);
        flDisp->addRow("Refresh Rate:", m_spinRefresh);
        flDisp->addRow("sRGB:",         m_spinSrgb);
        cl->addWidget(grpDisp);

        // ── Wireless ──────────────────────────────────────────────────────
        auto *grpWireless = new QGroupBox("Wireless", content);
        auto *flWireless  = new QFormLayout(grpWireless);
        flWireless->setSpacing(8);

        m_cmbWifi = new QComboBox(content);
        m_cmbWifi->addItems({"Wi-Fi 5","Wi-Fi 6","Wi-Fi 6E","Wi-Fi 7"});
        m_cmbWifi->setFixedHeight(36);

        m_cmbBt = new QComboBox(content);
        m_cmbBt->addItems({"4.2","5.0","5.1","5.2","5.3","5.4"});
        m_cmbBt->setFixedHeight(36);

        if (existing) {
            m_cmbWifi->setCurrentText(existing->getWireless().getWifiVersi());
            m_cmbBt  ->setCurrentText(existing->getWireless().getBluetoothVersi());
        } else {
            m_cmbWifi->setCurrentText("Wi-Fi 6");
            m_cmbBt  ->setCurrentText("5.2");
        }

        flWireless->addRow("Wi-Fi:",     m_cmbWifi);
        flWireless->addRow("Bluetooth:", m_cmbBt);
        cl->addWidget(grpWireless);

        // ── Battery ───────────────────────────────────────────────────────
        auto *grpBattery = new QGroupBox("Battery", content);
        auto *flBattery  = new QFormLayout(grpBattery);
        flBattery->setSpacing(8);

        m_spinBattery = new QSpinBox(content);
        m_spinBattery->setRange(10, 200);
        m_spinBattery->setSuffix(" Wh");
        m_spinBattery->setFixedHeight(36);
        if (existing) m_spinBattery->setValue(existing->getBatteryWh());
        else          m_spinBattery->setValue(60);

        flBattery->addRow("Kapasitas:", m_spinBattery);
        cl->addWidget(grpBattery);

        // ── Port ──────────────────────────────────────────────────────────
        auto *grpPort = new QGroupBox("Port", content);
        auto *flPort  = new QFormLayout(grpPort);
        flPort->setSpacing(8);

        m_spinUsbA  = new QSpinBox(content);
        m_spinUsbA->setRange(0, 10);
        m_spinUsbA->setFixedHeight(36);

        m_spinUsbC  = new QSpinBox(content);
        m_spinUsbC->setRange(0, 10);
        m_spinUsbC->setFixedHeight(36);

        m_chkUsbCPD = new QCheckBox("USB-C mendukung Power Delivery", content);

        m_spinHdmi  = new QSpinBox(content);
        m_spinHdmi->setRange(0, 10);
        m_spinHdmi->setFixedHeight(36);

        m_spinAudio = new QSpinBox(content);
        m_spinAudio->setRange(0, 10);
        m_spinAudio->setFixedHeight(36);

        if (existing) {
            m_spinUsbA ->setValue(existing->getUsbA());
            m_spinUsbC ->setValue(existing->getUsbC());
            m_chkUsbCPD->setChecked(existing->hasUsbCPD());
            m_spinHdmi ->setValue(existing->getHdmi());
            m_spinAudio->setValue(existing->getAudioJack());
        } else {
            m_spinUsbA ->setValue(2);
            m_spinUsbC ->setValue(1);
            m_spinHdmi ->setValue(1);
            m_spinAudio->setValue(1);
        }

        flPort->addRow("USB-A:",      m_spinUsbA);
        flPort->addRow("USB-C:",      m_spinUsbC);
        flPort->addRow("",            m_chkUsbCPD);
        flPort->addRow("HDMI:",       m_spinHdmi);
        flPort->addRow("Audio Jack:", m_spinAudio);
        cl->addWidget(grpPort);

        // ── Harga dan Stok ────────────────────────────────────────────────
        auto *grpHarga = new QGroupBox("Harga dan Stok", content);
        auto *flHarga  = new QFormLayout(grpHarga);
        flHarga->setSpacing(8);

        m_edtHarga = new QLineEdit(content);
        m_edtHarga->setValidator(new QIntValidator(0, 999999999, this));
        m_edtHarga->setPlaceholderText("Contoh: 15000000");
        m_edtHarga->setFixedHeight(36);
        if (existing) m_edtHarga->setText(QString::number(existing->getHarga()));

        m_spinStok = new QSpinBox(content);
        m_spinStok->setRange(0,9999);
        m_spinStok->setFixedHeight(36);
        if (existing) m_spinStok->setValue(existing->getStok());

        m_cmbGaransi = new QComboBox(content);
        m_cmbGaransi->addItems({"1 Tahun","2 Tahun","3 Tahun","Tidak ada garansi"});
        m_cmbGaransi->setFixedHeight(36);
        if (existing) m_cmbGaransi->setCurrentText(existing->getGaransi());

        flHarga->addRow("Harga (Rp):", m_edtHarga);
        flHarga->addRow("Stok awal:", m_spinStok);
        flHarga->addRow("Garansi:",   m_cmbGaransi);
        cl->addWidget(grpHarga);

        m_lblError = new QLabel(content);
        m_lblError->setStyleSheet("color:#EF4444; font-size:12px;");
        m_lblError->hide();
        cl->addWidget(m_lblError);

        scroll->setWidget(content);
        root->addWidget(scroll);

        // ── Tombol simpan/batal ───────────────────────────────────────────
        auto *btnRow = new QHBoxLayout;
        btnRow->setContentsMargins(20,8,20,12);

        auto *btnBatal = new QPushButton("Batal", this);
        btnBatal->setFixedHeight(38);
        btnBatal->setStyleSheet(QString(
            "QPushButton { border:1px solid %1; border-radius:8px; background:%2;"
            " color:%3; font-size:13px; }"
            "QPushButton:hover { background:%4; }")
            .arg(btnCnlBrd, btnCnlBg, btnCnlTxt, btnCnlHov));

        auto *btnSimpan = new QPushButton("Simpan", this);
        btnSimpan->setFixedHeight(38);
        btnSimpan->setStyleSheet(
            "QPushButton { background:#3B82F6; color:white; border-radius:8px;"
            " font-size:13px; font-weight:bold; border:none; }"
            "QPushButton:hover { background:#2563EB; }");

        btnRow->addWidget(btnBatal);
        btnRow->addWidget(btnSimpan);
        root->addLayout(btnRow);

        connect(btnBatal,  &QPushButton::clicked, this, &QDialog::reject);
        connect(btnSimpan, &QPushButton::clicked, this, &FormDialog::onSimpan);
    }

    Laptop* getResult() const { return m_result; }

private slots:
    void onSimpan() {
        if (m_edtNama->text().trimmed().isEmpty())   { showError("Nama produk tidak boleh kosong."); return; }
        if (m_edtBrand->text().trimmed().isEmpty())  { showError("Brand tidak boleh kosong.");        return; }
        if (m_edtGpuModel->text().trimmed().isEmpty()){ showError("Model GPU tidak boleh kosong.");   return; }
        if (m_edtHarga->text().trimmed().isEmpty())  { showError("Harga tidak boleh kosong.");        return; }

        bool ok;
        int harga = m_edtHarga->text().toInt(&ok);
        if (!ok || harga < 0) { showError("Harga harus berupa angka positif."); return; }

        QString modelVal    = m_edtCpuSpeed->text().trimmed();
        QString maxBoostVal = m_edtCpuMaxBoost->text().trimmed();

        SpekCPU cpu(m_cmbCpuBrand->currentText(), m_cmbCpuSeri->currentText(),
                    m_spinCores->value(), modelVal, maxBoostVal);
        SpekRAM ram(m_cmbRamTipe->currentText(), m_spinRamKap->value(),
                    m_spinRamSlot->value(), 0);
        SpekPenyimpanan stor(m_cmbStorTipe->currentText(), m_spinStorKap->value(), 1, 0);
        SpekDisplay disp(m_spinDispUkuran->value(), m_cmbDispTipe->currentText(),
                         m_edtResolusi->text().trimmed(), m_spinRefresh->value(),
                         m_spinSrgb->value());
        SpekWireless wireless(m_cmbWifi->currentText(), m_cmbBt->currentText());
        int batteryWh = m_spinBattery->value();

        int  usbA      = m_spinUsbA->value();
        int  usbC      = m_spinUsbC->value();
        bool usbCPD    = m_chkUsbCPD->isChecked();
        int  hdmi      = m_spinHdmi->value();
        int  audio     = m_spinAudio->value();

        QString id = m_existing ? m_existing->getId() : m_katalog->generateId();
        int jenis  = m_cmbJenis->currentIndex();

        if (jenis == 1) {
            m_result = new LaptopGaming(
                id, m_edtNama->text().trimmed(), m_edtBrand->text().trimmed(),
                cpu, ram, stor, disp, wireless,
                m_cmbGpuBrand->currentText(), m_edtGpuModel->text().trimmed(),
                usbA, usbC, usbCPD, hdmi, audio,
                harga, m_spinStok->value(), m_cmbGaransi->currentText(), batteryWh,
                45, true);
        } else if (jenis == 2) {
            m_result = new LaptopDesain(
                id, m_edtNama->text().trimmed(), m_edtBrand->text().trimmed(),
                cpu, ram, stor, disp, wireless,
                m_cmbGpuBrand->currentText(), m_edtGpuModel->text().trimmed(),
                usbA, usbC, usbCPD, hdmi, audio,
                harga, m_spinStok->value(), m_cmbGaransi->currentText(), batteryWh, false);
        } else if (jenis == 3) {
            m_result = new LaptopWorkstation(
                id, m_edtNama->text().trimmed(), m_edtBrand->text().trimmed(),
                cpu, ram, stor, disp, wireless,
                m_cmbGpuBrand->currentText(), m_edtGpuModel->text().trimmed(),
                usbA, usbC, usbCPD, hdmi, audio,
                harga, m_spinStok->value(), m_cmbGaransi->currentText(), batteryWh,
                false, false);
        } else {
            m_result = new Laptop(
                id, m_edtNama->text().trimmed(), m_edtBrand->text().trimmed(),
                cpu, ram, stor, disp, wireless,
                m_cmbGpuBrand->currentText(), m_edtGpuModel->text().trimmed(),
                usbA, usbC, usbCPD, hdmi, audio,
                harga, m_spinStok->value(), m_cmbGaransi->currentText(), batteryWh);
        }
        if (!m_imagePaths.isEmpty())
            m_result->setImagePaths(m_imagePaths);
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
    QStringList     m_imagePaths;
    QLabel         *m_lblPreview;
    QLabel         *m_lblGambarPath;
    QPushButton    *m_btnPilihGambar;
    QPushButton    *m_btnHapusGambar;

    QLineEdit      *m_edtNama, *m_edtBrand;
    QComboBox      *m_cmbJenis;
    QComboBox      *m_cmbCpuBrand, *m_cmbCpuSeri;
    QLineEdit      *m_edtCpuSpeed;     // model number
    QLineEdit      *m_edtCpuMaxBoost;  // max boost GHz
    QSpinBox       *m_spinCores;
    QComboBox      *m_cmbGpuBrand;
    QLineEdit      *m_edtGpuModel;
    QComboBox      *m_cmbRamTipe;
    QSpinBox       *m_spinRamKap, *m_spinRamSlot;
    QComboBox      *m_cmbStorTipe;
    QSpinBox       *m_spinStorKap;
    QDoubleSpinBox *m_spinDispUkuran;
    QComboBox      *m_cmbDispTipe;
    QLineEdit      *m_edtResolusi;
    QSpinBox       *m_spinRefresh, *m_spinSrgb;
    QSpinBox       *m_spinUsbA, *m_spinUsbC, *m_spinHdmi, *m_spinAudio;
    QCheckBox      *m_chkUsbCPD;
    QComboBox      *m_cmbWifi, *m_cmbBt;
    QSpinBox       *m_spinBattery;
    QLineEdit      *m_edtHarga;
    QSpinBox       *m_spinStok;
    QComboBox      *m_cmbGaransi;
    QLabel         *m_lblError;
};

// ══════════════════════════════════════════════════════════════════════════════
//  AdminPage
// ══════════════════════════════════════════════════════════════════════════════
AdminPage::AdminPage(ManajerKatalog *katalog, QWidget *parent)
    : QWidget(parent), m_katalog(katalog)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    // ── Top bar ───────────────────────────────────────────────────────────
    auto *topBar = new QWidget(this);
    topBar->setObjectName("adminTopBar");
    topBar->setFixedHeight(50);
    auto *topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(12,0,12,0);
    topBarLayout->setSpacing(10);

    m_btnBack = new QPushButton("← Menu Utama", topBar);
    m_btnBack->setObjectName("adminBtnBack");
    m_btnBack->setFixedHeight(34);
    m_btnBack->setCursor(Qt::PointingHandCursor);

    auto *pageTitle = new QLabel("📦  Manajemen Stok Produk", topBar);
    pageTitle->setObjectName("adminPageTitle");

    m_lblTotal = new QLabel(topBar);
    m_lblTotal->setObjectName("adminLblTotal");

    // Tombol tema di kanan atas
    m_btnTheme = new QPushButton(topBar);
    m_btnTheme->setObjectName("adminBtnTheme");
    m_btnTheme->setFixedSize(110,34);
    m_btnTheme->setCursor(Qt::PointingHandCursor);

    topBarLayout->addWidget(m_btnBack);
    topBarLayout->addSpacing(12);
    topBarLayout->addWidget(pageTitle);
    topBarLayout->addSpacing(12);
    topBarLayout->addWidget(m_lblTotal);
    topBarLayout->addStretch();
    topBarLayout->addWidget(m_btnTheme);
    root->addWidget(topBar);

    auto *divider = new QFrame(this);
    divider->setObjectName("adminDivider");
    divider->setFrameShape(QFrame::HLine);
    divider->setFixedHeight(1);
    root->addWidget(divider);

    // ── Konten ────────────────────────────────────────────────────────────
    auto *contentWidget = new QWidget(this);
    contentWidget->setObjectName("adminContent");
    auto *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(16,10,16,12);
    contentLayout->setSpacing(10);

    auto *toolbar   = new QHBoxLayout;
    auto *btnTambah = new QPushButton("+ Tambah Produk", contentWidget);
    m_btnEdit  = new QPushButton("✏️ Edit",        contentWidget);
    m_btnHapus = new QPushButton("🗑️ Hapus",       contentWidget);
    m_btnStok  = new QPushButton("📦 Kelola Stok", contentWidget);

    auto styleBtn = [](QPushButton *b, const QString &bg, const QString &hover) {
        b->setFixedHeight(36);
        b->setStyleSheet(QString(
            "QPushButton { background:%1; color:white; border-radius:6px;"
            " font-size:13px; padding:0 14px; border:none; }"
            "QPushButton:hover { background:%2; }"
            "QPushButton:disabled { background:#64748B; color:#94A3B8; }").arg(bg, hover));
    };
    styleBtn(btnTambah,  "#3B82F6","#2563EB");
    styleBtn(m_btnEdit,  "#F59E0B","#D97706");
    styleBtn(m_btnHapus, "#EF4444","#DC2626");
    styleBtn(m_btnStok,  "#059669","#047857");

    m_btnEdit  ->setEnabled(false);
    m_btnHapus ->setEnabled(false);
    m_btnStok  ->setEnabled(false);

    toolbar->addWidget(btnTambah);
    toolbar->addWidget(m_btnEdit);
    toolbar->addWidget(m_btnHapus);
    toolbar->addWidget(m_btnStok);
    toolbar->addStretch();
    contentLayout->addLayout(toolbar);

    m_table = new QTableWidget(contentWidget);
    m_table->setObjectName("adminTable");
    // Kolom: ID, Nama, Jenis, CPU, RAM, Storage, Harga, Stok
    m_table->setColumnCount(10);
    m_table->setHorizontalHeaderLabels(
        {"ID","Nama","Jenis","CPU","GPU","RAM","Storage","Display","Harga","Stok"});
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setShowGrid(false);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_table->setColumnWidth(0, 60);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    for (int c : {2,3,4,5,6,7,8,9})
        m_table->horizontalHeader()->setSectionResizeMode(c, QHeaderView::ResizeToContents);

    contentLayout->addWidget(m_table);

    // Catatan klik ganda (style mengikuti tema dilakukan di applyTheme)
    auto *hint = new QLabel(
        "Klik ganda pada laptop untuk melihat detail spesifikasi lengkap", contentWidget);
    hint->setObjectName("adminHint");
    contentLayout->addWidget(hint);

    root->addWidget(contentWidget, 1);

    connect(m_btnBack,  &QPushButton::clicked, this, &AdminPage::kembali);
    connect(m_btnTheme, &QPushButton::clicked, this, [this]() {
        QWidget *p = qobject_cast<QWidget*>(this->parent());
        if (p) QMetaObject::invokeMethod(p->window(), "toggleDarkMode", Qt::DirectConnection);
    });
    connect(btnTambah,   &QPushButton::clicked, this, &AdminPage::onTambah);
    connect(m_btnEdit,   &QPushButton::clicked, this, &AdminPage::onEdit);
    connect(m_btnHapus,  &QPushButton::clicked, this, &AdminPage::onHapus);
    connect(m_btnStok,   &QPushButton::clicked, this, &AdminPage::onKelolStok);
    connect(m_table,     &QTableWidget::cellDoubleClicked,
            this, [this](int, int) { onDetail(); });
    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &AdminPage::onSelectionChanged);

    refreshTable();
    applyTheme(false);
}

// ── applyTheme ────────────────────────────────────────────────────────────────
void AdminPage::applyTheme(bool isDark)
{
    m_isDark = isDark;

    const QString bg       = isDark ? "#0F172A" : "#FFFFFF";
    const QString topBarBg = isDark ? "#1E293B" : "#FFFFFF";
    const QString border   = isDark ? "#334155" : "#E2E8F0";
    const QString textMain = isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub  = isDark ? "#94A3B8" : "#64748B";
    const QString headerBg = isDark ? "#1E293B" : "#F8FAFC";
    const QString rowBg    = isDark ? "#0F172A" : "#FFFFFF";
    const QString rowAlt   = isDark ? "#1A2744" : "#F0F9FF";
    const QString selBg    = isDark ? "#1D4ED8" : "#EFF6FF";
    const QString selTxt   = isDark ? "#FFFFFF"  : "#1E293B";
    const QString btnBg    = isDark ? "#1E293B" : "white";
    const QString btnBrd   = isDark ? "#334155" : "#CBD5E1";
    const QString btnTxt   = isDark ? "#94A3B8" : "#475569";
    const QString btnHov   = isDark ? "#334155" : "#F1F5F9";

    if (auto *w = findChild<QWidget*>("adminTopBar"))
        w->setStyleSheet(QString("QWidget#adminTopBar { background:%1; }").arg(topBarBg));
    if (auto *w = findChild<QFrame*>("adminDivider"))
        w->setStyleSheet(QString("background:%1;").arg(border));
    if (auto *w = findChild<QWidget*>("adminContent"))
        w->setStyleSheet(QString("QWidget#adminContent { background:%1; }").arg(bg));
    if (auto *w = findChild<QLabel*>("adminPageTitle"))
        w->setStyleSheet(QString("font-size:15px; font-weight:bold; color:%1; background:transparent;").arg(textMain));
    if (m_lblTotal)
        m_lblTotal->setStyleSheet(QString("color:%1; font-size:13px; background:transparent;").arg(textSub));

    m_btnBack->setStyleSheet(QString(
        "QPushButton { background:%1; border:1px solid %2; border-radius:7px;"
        " color:%3; font-size:13px; padding:0 14px; }"
        "QPushButton:hover { background:%4; }")
        .arg(btnBg, btnBrd, btnTxt, btnHov));

    if (m_btnTheme) {
        m_btnTheme->setText(isDark ? "☀ Light" : "☾ Dark");
        m_btnTheme->setStyleSheet(QString(
            "QPushButton { background:%1; border:1px solid %2; border-radius:8px;"
            " font-size:13px; color:%3; padding:0 10px; font-weight:500; }"
            "QPushButton:hover { background:%4; }")
            .arg(btnBg, btnBrd, btnTxt, btnHov));
    }

    // Styling hint catatan klik ganda
    if (auto *h = findChild<QLabel*>("adminHint"))
        h->setStyleSheet(QString(
            "color:%1; font-size:11px; background:transparent;").arg(textSub));

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

// ── refreshTable ──────────────────────────────────────────────────────────────
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
        m_table->setItem(r, 3, new QTableWidgetItem(l->getCPU().toShortString()));
        m_table->setItem(r, 4, new QTableWidgetItem(
            l->getGpuBrand() + " " + l->getGpuModel()));
        m_table->setItem(r, 5, new QTableWidgetItem(
            QString("%1GB %2").arg(l->getRAM().getKapasitas()).arg(l->getRAM().getTipe())));
        m_table->setItem(r, 6, new QTableWidgetItem(
            l->getStorage().kapasitasLabel()+" "+l->getStorage().getTipe()));
        m_table->setItem(r, 7, new QTableWidgetItem(
            QString("%1\" %2 %3Hz")
            .arg(l->getDisplay().getUkuran(), 0, 'f', 1)
            .arg(l->getDisplay().getTipe())
            .arg(l->getDisplay().getRefreshRate())));
        m_table->setItem(r, 8, new QTableWidgetItem(l->formatHarga()));
        auto *stokItem = new QTableWidgetItem(QString::number(l->getStok()));
        stokItem->setForeground(l->getStok()<=3 ? QColor("#EF4444") : QColor(m_isDark?"#34D399":"#059669"));
        m_table->setItem(r, 9, stokItem);
    }
}

void AdminPage::onSelectionChanged() {
    bool ada = !m_table->selectedItems().isEmpty();
    m_btnEdit ->setEnabled(ada);
    m_btnHapus->setEnabled(ada);
    m_btnStok ->setEnabled(ada);
}

void AdminPage::onTambah() {
    FormDialog dlg(nullptr, m_katalog, m_isDark, this);
    if (dlg.exec() == QDialog::Accepted) {
        Laptop *baru = dlg.getResult();
        if      (auto *lg = dynamic_cast<LaptopGaming*>(baru))      m_katalog->tambahLaptopGaming(lg);
        else if (auto *ld = dynamic_cast<LaptopDesain*>(baru))      m_katalog->tambahLaptopDesain(ld);
        else if (auto *lw = dynamic_cast<LaptopWorkstation*>(baru)) m_katalog->tambahLaptopWorkstation(lw);
        else                                                          m_katalog->tambahLaptop(baru);
        refreshTable();
        QMessageBox::information(this, "Berhasil",
            "Produk \"" + baru->getNama() + "\" berhasil ditambahkan.");
    }
}

void AdminPage::onEdit() {
    int row = m_table->currentRow();
    if (row < 0 || row >= m_displayed.size()) return;
    Laptop *l = m_displayed[row];

    FormDialog dlg(l, m_katalog, m_isDark, this);
    if (dlg.exec() == QDialog::Accepted) {
        Laptop *upd = dlg.getResult();
        l->setNama   (upd->getNama());
        l->setBrand  (upd->getBrand());
        l->setCPU    (upd->getCPU());
        l->setRAM    (upd->getRAM());
        l->setStorage(upd->getStorage());
        l->setDisplay(upd->getDisplay());
        l->setWireless(upd->getWireless());
        l->setGpuBrand(upd->getGpuBrand());
        l->setGpuModel(upd->getGpuModel());
        l->setUsbA   (upd->getUsbA());
        l->setUsbC   (upd->getUsbC());
        l->setUsbCPD (upd->hasUsbCPD());
        l->setHdmi   (upd->getHdmi());
        l->setAudioJack(upd->getAudioJack());
        l->setBatteryWh(upd->getBatteryWh());
        l->setHarga  (upd->getHarga());
        l->setStok   (upd->getStok());
        l->setGaransi(upd->getGaransi());
        // Salin juga gambar yang dipilih di form
        l->setImagePaths(upd->getImagePaths());
        delete upd;
        refreshTable();
        QMessageBox::information(this, "Berhasil", "Data produk berhasil diperbarui.");
    }
}

void AdminPage::onHapus() {
    int row = m_table->currentRow();
    if (row < 0 || row >= m_displayed.size()) return;
    Laptop *l = m_displayed[row];

    // Custom themed confirm dialog (light/dark)
    QDialog dlg(this);
    dlg.setWindowTitle("Konfirmasi Hapus");
    dlg.setFixedSize(420, 240);
    dlg.setModal(true);

    const QString bgDialog  = m_isDark ? "#1E293B" : "#FFFFFF";
    const QString textMain  = m_isDark ? "#F1F5F9" : "#1E293B";
    const QString textSub   = m_isDark ? "#94A3B8" : "#475569";
    const QString btnCnlBg  = m_isDark ? "#1E293B" : "#FFFFFF";
    const QString btnCnlBrd = m_isDark ? "#334155" : "#CBD5E1";
    const QString btnCnlTxt = m_isDark ? "#94A3B8" : "#475569";
    const QString btnCnlHov = m_isDark ? "#334155" : "#F1F5F9";

    dlg.setStyleSheet(QString("QDialog { background:%1; }").arg(bgDialog));

    auto *root = new QVBoxLayout(&dlg);
    root->setContentsMargins(24, 20, 24, 20);
    root->setSpacing(12);

    auto *title = new QLabel("🗑️ Hapus Produk", &dlg);
    title->setStyleSheet(QString("font-size:16px; font-weight:bold; color:%1;").arg(textMain));
    root->addWidget(title);

    auto *msg = new QLabel(
        QString("Anda yakin ingin menghapus produk:\n\n\"%1\"?\n\n"
                "Tindakan ini tidak dapat dibatalkan.").arg(l->getNama()), &dlg);
    msg->setStyleSheet(QString("color:%1; font-size:13px;").arg(textSub));
    msg->setWordWrap(true);
    root->addWidget(msg);
    root->addStretch();

    auto *btnRow = new QHBoxLayout;
    auto *btnBatal = new QPushButton("Batal", &dlg);
    btnBatal->setFixedHeight(38);
    btnBatal->setStyleSheet(QString(
        "QPushButton { border:1px solid %1; border-radius:8px; background:%2;"
        " color:%3; font-size:13px; }"
        "QPushButton:hover { background:%4; }")
        .arg(btnCnlBrd, btnCnlBg, btnCnlTxt, btnCnlHov));

    auto *btnHapus = new QPushButton("Hapus", &dlg);
    btnHapus->setFixedHeight(38);
    btnHapus->setStyleSheet(
        "QPushButton { background:#EF4444; color:white; border-radius:8px;"
        " font-size:13px; font-weight:bold; border:none; }"
        "QPushButton:hover { background:#DC2626; }");

    btnRow->addWidget(btnBatal);
    btnRow->addWidget(btnHapus);
    root->addLayout(btnRow);

    connect(btnBatal, &QPushButton::clicked, &dlg, &QDialog::reject);
    connect(btnHapus, &QPushButton::clicked, &dlg, &QDialog::accept);

    if (dlg.exec() == QDialog::Accepted) {
        QString nama = l->getNama();
        m_katalog->hapusProduk(l->getId());
        refreshTable();
        QMessageBox::information(this, "Berhasil",
            "Produk \"" + nama + "\" berhasil dihapus.");
    }
}

void AdminPage::onDetail() {
    int row = m_table->currentRow();
    if (row < 0 || row >= m_displayed.size()) return;
    Laptop *l = m_displayed[row];

    DetailDialog dlg(l, m_isDark, DetailDialog::Mode::Stok, this);
    bool editClicked = false;
    connect(&dlg, &DetailDialog::editRequested, this, [&]{ editClicked = true; });
    dlg.exec();
    if (editClicked) onEdit();
}

void AdminPage::onKelolStok() {
    int row = m_table->currentRow();
    if (row < 0 || row >= m_displayed.size()) return;
    Laptop *l = m_displayed[row];

    StockDialog dlg(l, m_isDark, this);
    if (dlg.exec() == QDialog::Accepted) {
        bool ok = dlg.getTipe() == "in"
            ? m_katalog->updateStokMasuk(l->getId(), dlg.getJumlah())
            : m_katalog->updateStokTerjual(l->getId(), dlg.getJumlah());

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
