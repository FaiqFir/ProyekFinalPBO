#include "MainWindow.h"
#include "CatalogPage.h"
#include "AdminPage.h"
#include "LoginDialog.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QApplication>

// ── Warna tema ────────────────────────────────────────────────────────────────
// Light
static const char* LIGHT_BG      = "#EFF6FF";
static const char* LIGHT_BG2     = "#F5F3FF";
static const char* LIGHT_TITLE   = "#1E293B";
static const char* LIGHT_SUB     = "#64748B";
static const char* LIGHT_CARD_BG = "white";
static const char* LIGHT_CARD_BORDER_KATALOG = "#BFDBFE";
static const char* LIGHT_CARD_BORDER_ADMIN   = "#DDD6FE";
static const char* LIGHT_CARD_TXT_KATALOG    = "#1D4ED8";
static const char* LIGHT_CARD_TXT_ADMIN      = "#7C3AED";
static const char* LIGHT_CARD_HOVER_KATALOG  = "#EFF6FF";
static const char* LIGHT_CARD_HOVER_ADMIN    = "#F5F3FF";
static const char* LIGHT_THEME_BG  = "white";
static const char* LIGHT_THEME_TXT = "#475569";
static const char* LIGHT_THEME_BORDER = "#CBD5E1";
static const char* LIGHT_THEME_HOVER  = "#F1F5F9";

// Dark
static const char* DARK_BG      = "#0F172A";
static const char* DARK_BG2     = "#1E1B4B";
static const char* DARK_TITLE   = "#F1F5F9";
static const char* DARK_SUB     = "#94A3B8";
static const char* DARK_CARD_BG = "#1E293B";
static const char* DARK_CARD_BORDER_KATALOG = "#1D4ED8";
static const char* DARK_CARD_BORDER_ADMIN   = "#6D28D9";
static const char* DARK_CARD_TXT_KATALOG    = "#93C5FD";
static const char* DARK_CARD_TXT_ADMIN      = "#C4B5FD";
static const char* DARK_CARD_HOVER_KATALOG  = "#1E3A5F";
static const char* DARK_CARD_HOVER_ADMIN    = "#2E1065";
static const char* DARK_THEME_BG  = "#1E293B";
static const char* DARK_THEME_TXT = "#94A3B8";
static const char* DARK_THEME_BORDER = "#334155";
static const char* DARK_THEME_HOVER  = "#334155";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Sistem Komparasi & Katalog Hardware");
    setMinimumSize(1100, 680);

    m_katalog = new ManajerKatalog();

    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    buildMenuAwal();

    auto *catalogPage = new CatalogPage(m_katalog, this);
    m_stack->addWidget(catalogPage);   // index 1

    auto *adminPage = new AdminPage(m_katalog, this);
    m_stack->addWidget(adminPage);     // index 2

    connect(adminPage,   &AdminPage::kembali,   this, &MainWindow::keMenuAwal);
    connect(catalogPage, &CatalogPage::kembali, this, &MainWindow::keMenuAwal);
    connect(this, &MainWindow::themeChanged, catalogPage, &CatalogPage::applyTheme);
    connect(this, &MainWindow::themeChanged, adminPage,   &AdminPage::applyTheme);

    m_stack->setCurrentIndex(0);
    applyTheme();
}

// ── Build Menu Awal ──────────────────────────────────────────────────────────
void MainWindow::buildMenuAwal()
{
    auto *page = new QWidget;
    page->setObjectName("menuAwal");

    auto *rootVL = new QVBoxLayout(page);
    rootVL->setContentsMargins(0, 0, 0, 0);
    rootVL->setSpacing(0);

    // ── Top bar: tombol tema di pojok kanan ───────────────────────────────
    auto *topBar = new QHBoxLayout;
    topBar->setContentsMargins(16, 12, 16, 0);
    topBar->addStretch();

    m_btnTheme = new QPushButton(page);
    m_btnTheme->setFixedSize(110, 36);
    m_btnTheme->setCursor(Qt::PointingHandCursor);
    m_btnTheme->setObjectName("btnTheme");
    m_btnTheme->setToolTip("Ganti tema terang/gelap");
    topBar->addWidget(m_btnTheme);
    rootVL->addLayout(topBar);

    // ── Konten tengah ─────────────────────────────────────────────────────
    auto *centerWidget = new QWidget(page);
    centerWidget->setObjectName("centerWidget");
    auto *vl = new QVBoxLayout(centerWidget);
    vl->setAlignment(Qt::AlignCenter);
    vl->setSpacing(0);

    auto *lblIcon = new QLabel("\U0001F4BB", centerWidget);
    lblIcon->setAlignment(Qt::AlignCenter);
    lblIcon->setStyleSheet("font-size:64px; background:transparent;");
    vl->addWidget(lblIcon);
    vl->addSpacing(16);

    auto *lblTitle = new QLabel("Sistem Komparasi &\nKatalog Hardware", centerWidget);
    lblTitle->setObjectName("lblTitle");
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-size:32px; font-weight:bold; background:transparent;");
    vl->addWidget(lblTitle);
    vl->addSpacing(8);

    auto *lblSub = new QLabel("Temukan, bandingkan, dan kelola laptop pilihanmu", centerWidget);
    lblSub->setObjectName("lblSub");
    lblSub->setAlignment(Qt::AlignCenter);
    lblSub->setStyleSheet("font-size:15px; background:transparent;");
    vl->addWidget(lblSub);
    vl->addSpacing(48);

    // ── Dua tombol utama ──────────────────────────────────────────────────
    auto *btnRow = new QHBoxLayout;
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(24);

    auto *btnKatalog = new QPushButton(centerWidget);
    btnKatalog->setFixedSize(220, 140);
    btnKatalog->setCursor(Qt::PointingHandCursor);
    btnKatalog->setObjectName("btnKatalog");
    btnKatalog->setText("\U0001F50D\n\nKatalog Produk\n\nCari & Bandingkan Laptop");

    auto *btnAdmin = new QPushButton(centerWidget);
    btnAdmin->setFixedSize(220, 140);
    btnAdmin->setCursor(Qt::PointingHandCursor);
    btnAdmin->setObjectName("btnAdmin");
    btnAdmin->setText("\U0001F512\n\nStok Produk\n\nPanel Admin (Login)");

    btnRow->addWidget(btnKatalog);
    btnRow->addWidget(btnAdmin);
    vl->addLayout(btnRow);

    rootVL->addWidget(centerWidget, 1);

    connect(btnKatalog, &QPushButton::clicked, this, &MainWindow::keHalamanKatalog);
    connect(btnAdmin,   &QPushButton::clicked, this, &MainWindow::keHalamanAdmin);
    connect(m_btnTheme, &QPushButton::clicked, this, &MainWindow::toggleDarkMode);

    m_stack->insertWidget(0, page);
}

// ── Terapkan tema ─────────────────────────────────────────────────────────────
void MainWindow::applyTheme()
{
    QWidget *page = m_stack->widget(0);
    if (!page) return;

    const bool dark = m_isDark;

    const char* bg      = dark ? DARK_BG      : LIGHT_BG;
    const char* bg2     = dark ? DARK_BG2     : LIGHT_BG2;
    const char* titleC  = dark ? DARK_TITLE   : LIGHT_TITLE;
    const char* subC    = dark ? DARK_SUB     : LIGHT_SUB;
    const char* cardBg  = dark ? DARK_CARD_BG : LIGHT_CARD_BG;
    const char* bK      = dark ? DARK_CARD_BORDER_KATALOG : LIGHT_CARD_BORDER_KATALOG;
    const char* bA      = dark ? DARK_CARD_BORDER_ADMIN   : LIGHT_CARD_BORDER_ADMIN;
    const char* tK      = dark ? DARK_CARD_TXT_KATALOG    : LIGHT_CARD_TXT_KATALOG;
    const char* tA      = dark ? DARK_CARD_TXT_ADMIN      : LIGHT_CARD_TXT_ADMIN;
    const char* hK      = dark ? DARK_CARD_HOVER_KATALOG  : LIGHT_CARD_HOVER_KATALOG;
    const char* hA      = dark ? DARK_CARD_HOVER_ADMIN    : LIGHT_CARD_HOVER_ADMIN;
    const char* themeBg     = dark ? DARK_THEME_BG     : LIGHT_THEME_BG;
    const char* themeTx     = dark ? DARK_THEME_TXT    : LIGHT_THEME_TXT;
    const char* themeBorder = dark ? DARK_THEME_BORDER : LIGHT_THEME_BORDER;
    const char* themeHover  = dark ? DARK_THEME_HOVER  : LIGHT_THEME_HOVER;

    // Label & ikon tombol tema
    QString themeLabel = dark ? "\u2600\uFE0F  Light" : "\U0001F319  Dark";

    // Background halaman
    page->setStyleSheet(
        QString("QWidget#menuAwal { background: qlineargradient("
                "x1:0,y1:0,x2:1,y2:1, stop:0 %1, stop:1 %2); }")
        .arg(bg).arg(bg2));

    // Judul
    if (auto *w = page->findChild<QLabel*>("lblTitle"))
        w->setStyleSheet(QString("font-size:32px; font-weight:bold; color:%1; background:transparent;").arg(titleC));

    // Sub-judul
    if (auto *w = page->findChild<QLabel*>("lblSub"))
        w->setStyleSheet(QString("font-size:15px; color:%1; background:transparent;").arg(subC));

    // Tombol Katalog
    if (auto *w = page->findChild<QPushButton*>("btnKatalog"))
        w->setStyleSheet(
            QString("QPushButton { background:%1; border:2px solid %2; border-radius:16px;"
                    " font-size:14px; color:%3; font-weight:bold; }"
                    "QPushButton:hover { background:%4; border-color:%5; }")
            .arg(cardBg).arg(bK).arg(tK).arg(hK).arg(tK));

    // Tombol Admin
    if (auto *w = page->findChild<QPushButton*>("btnAdmin"))
        w->setStyleSheet(
            QString("QPushButton { background:%1; border:2px solid %2; border-radius:16px;"
                    " font-size:14px; color:%3; font-weight:bold; }"
                    "QPushButton:hover { background:%4; border-color:%5; }")
            .arg(cardBg).arg(bA).arg(tA).arg(hA).arg(tA));

    // Tombol tema — lebih lebar, ada label teks
    if (m_btnTheme) {
        m_btnTheme->setText(themeLabel);
        m_btnTheme->setStyleSheet(
            QString("QPushButton { background:%1; border:1px solid %2;"
                    " border-radius:8px; font-size:13px; color:%3;"
                    " padding:0 10px; font-weight:500; }"
                    "QPushButton:hover { background:%4; }")
            .arg(themeBg).arg(themeBorder).arg(themeTx).arg(themeHover));
    }

    // Propagasi tema ke CatalogPage dan AdminPage
    emit themeChanged(dark);
}

// ── Toggle tema ───────────────────────────────────────────────────────────────
void MainWindow::toggleDarkMode()
{
    m_isDark = !m_isDark;
    applyTheme();
}

// ── Navigasi ──────────────────────────────────────────────────────────────────
void MainWindow::keHalamanKatalog() {
    m_stack->setCurrentIndex(1);
}

void MainWindow::keHalamanAdmin() {
    LoginDialog loginDlg(this);
    if (loginDlg.exec() == QDialog::Accepted && loginDlg.loginBerhasil())
        m_stack->setCurrentIndex(2);
}

void MainWindow::keMenuAwal() {
    m_stack->setCurrentIndex(0);
}
