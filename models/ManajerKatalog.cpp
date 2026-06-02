#include "ManajerKatalog.h"
#include "LaptopWorkstation.h"
#include <algorithm>

ManajerKatalog::ManajerKatalog() { loadDataAwal(); }
ManajerKatalog::~ManajerKatalog() { for (auto *p : m_produk) delete p; }

// ── CRUD ─────────────────────────────────────────────────────────────────────
void ManajerKatalog::tambahLaptop(Laptop *l)                    { m_produk.append(l); }
void ManajerKatalog::tambahLaptopGaming(LaptopGaming *l)         { m_produk.append(l); }
void ManajerKatalog::tambahLaptopDesain(LaptopDesain *l)         { m_produk.append(l); }
void ManajerKatalog::tambahLaptopWorkstation(LaptopWorkstation *l){ m_produk.append(l); }

bool ManajerKatalog::hapusProduk(const QString &id) {
    for (int i = 0; i < m_produk.size(); ++i) {
        if (auto *l = dynamic_cast<Laptop*>(m_produk[i]); l && l->getId() == id) {
            delete m_produk[i]; m_produk.removeAt(i); return true;
        }
    }
    return false;
}

bool ManajerKatalog::updateStokMasuk(const QString &id, int jml) {
    Laptop *l = cariLaptopById(id);
    if (!l || jml <= 0) return false;
    l->setStok(l->getStok() + jml); return true;
}

bool ManajerKatalog::updateStokTerjual(const QString &id, int jml) {
    Laptop *l = cariLaptopById(id);
    if (!l || jml <= 0 || jml > l->getStok()) return false;
    l->setStok(l->getStok() - jml); return true;
}

// ── CARI ─────────────────────────────────────────────────────────────────────
PerangkatKeras* ManajerKatalog::cariById(const QString &id) const {
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && l->getId() == id) return p;
    return nullptr;
}
Laptop* ManajerKatalog::cariLaptopById(const QString &id) const {
    return dynamic_cast<Laptop*>(cariById(id));
}

// ── FILTER ────────────────────────────────────────────────────────────────────
bool ManajerKatalog::laptopLolosFilter(Laptop *l, const FilterParam &f) const {
    // Filter jenis laptop
    if (!f.jenisLaptop.isEmpty()) {
        bool cocok = false;
        for (const QString &j : f.jenisLaptop) {
            if (j == "Laptop Office"      && l->getTipePerangkat() == "Laptop")               { cocok = true; break; }
            if (j == "Laptop Gaming"      && l->getTipePerangkat() == "Laptop Gaming")        { cocok = true; break; }
            if (j == "Laptop Desain"      && l->getTipePerangkat() == "Laptop Desain")        { cocok = true; break; }
            if (j == "Laptop Workstation" && l->getTipePerangkat() == "Laptop Workstation")   { cocok = true; break; }
        }
        if (!cocok) return false;
    }
    if (!f.cpuSeri.isEmpty()         && !f.cpuSeri.contains(l->getCPU().getSeri()))              return false;
    if (!f.gpuBrand.isEmpty()        && !f.gpuBrand.contains(l->getGpuBrand()))                  return false;
    if (!f.ramTipe.isEmpty()         && !f.ramTipe.contains(l->getRAM().getTipe()))               return false;
    if (!f.ramKapasitas.isEmpty()    && !f.ramKapasitas.contains(QString::number(l->getRAM().getKapasitas())))  return false;
    if (!f.storageTipe.isEmpty()     && !f.storageTipe.contains(l->getStorage().getTipe()))       return false;
    if (!f.storageKapasitas.isEmpty()&& !f.storageKapasitas.contains(QString::number(l->getStorage().getKapasitas()))) return false;
    if (!f.displayTipe.isEmpty()     && !f.displayTipe.contains(l->getDisplay().getTipe()))       return false;
    if (!f.wifiVersi.isEmpty()       && !f.wifiVersi.contains(l->getWireless().getWifiVersi()))   return false;
    if (!f.btVersi.isEmpty()         && !f.btVersi.contains(l->getWireless().getBluetoothVersi())) return false;

    for (const QString &port : f.ports) {
        if (port == "USB-A"     && l->getUsbA() == 0)  return false;
        if (port == "USB-C"     && !l->hasUsbC())       return false;
        if (port == "USB-C PD"  && !l->hasUsbCPD())     return false;
        if (port == "HDMI"      && !l->hasHdmi())        return false;
        if (port == "Audio Jack"&& !l->hasAudioJack())   return false;
    }
    return true;
}

// ── SORT ─────────────────────────────────────────────────────────────────────
QVector<Laptop*> ManajerKatalog::filterDanSort(const FilterParam &f, SortMode sort) const {
    QVector<Laptop*> hasil;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && laptopLolosFilter(l, f))
            hasil.append(l);

    // Default: ID ascending (urutan ditambah)
    if (sort == SortMode::None) {
        std::stable_sort(hasil.begin(), hasil.end(),
            [](Laptop *a, Laptop *b) { return a->getId() < b->getId(); });
    }
    // Harga termurah
    else if (sort == SortMode::HargaAsc) {
        std::stable_sort(hasil.begin(), hasil.end(),
            [](Laptop *a, Laptop *b) { return a->getHarga() < b->getHarga(); });
    }
    // RAM terbesar: kapasitas desc, jika sama DDR5 > DDR4 > DDR3
    else if (sort == SortMode::RamDesc) {
        std::stable_sort(hasil.begin(), hasil.end(), [](Laptop *a, Laptop *b) {
            int ka = a->getRAM().getKapasitas(), kb = b->getRAM().getKapasitas();
            if (ka != kb) return ka > kb;
            // jika kapasitas sama, urutkan berdasarkan generasi DDR (lebih baru lebih atas)
            auto ddrNum = [](const QString &t) {
                if (t == "DDR5") return 5;
                if (t == "DDR4") return 4;
                return 3;
            };
            return ddrNum(a->getRAM().getTipe()) > ddrNum(b->getRAM().getTipe());
        });
    }
    // Stok terbanyak
    else if (sort == SortMode::StokDesc) {
        std::stable_sort(hasil.begin(), hasil.end(),
            [](Laptop *a, Laptop *b) { return a->getStok() > b->getStok(); });
    }
    // Storage terbesar: kapasitas desc, jika sama SSD > HDD
    else if (sort == SortMode::StorageDesc) {
        std::stable_sort(hasil.begin(), hasil.end(), [](Laptop *a, Laptop *b) {
            int ka = a->getStorage().getKapasitas(), kb = b->getStorage().getKapasitas();
            if (ka != kb) return ka > kb;
            // SSD lebih unggul dari HDD
            bool aSSD = a->getStorage().getTipe() == "SSD";
            bool bSSD = b->getStorage().getTipe() == "SSD";
            return aSSD > bSSD;
        });
    }

    return hasil;
}

// ── STATISTIK ─────────────────────────────────────────────────────────────────
int ManajerKatalog::hitungDenganCpuSeri(const QString &seri) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && l->getCPU().getSeri() == seri) ++n;
    return n;
}
int ManajerKatalog::hitungDenganGpuBrand(const QString &brand) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && l->getGpuBrand() == brand) ++n;
    return n;
}
int ManajerKatalog::hitungDenganRamKapasitas(int kap) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && l->getRAM().getKapasitas() == kap) ++n;
    return n;
}
int ManajerKatalog::hitungDenganStorageTipe(const QString &tipe) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && l->getStorage().getTipe() == tipe) ++n;
    return n;
}
int ManajerKatalog::hitungDenganDisplayTipe(const QString &tipe) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && l->getDisplay().getTipe() == tipe) ++n;
    return n;
}
int ManajerKatalog::hitungDenganJenis(const QString &jenis) const {
    int n = 0;
    for (auto *p : m_produk) {
        auto *l = dynamic_cast<Laptop*>(p);
        if (!l) continue;
        if (jenis == "Laptop Office"      && l->getTipePerangkat() == "Laptop")               ++n;
        if (jenis == "Laptop Gaming"      && l->getTipePerangkat() == "Laptop Gaming")        ++n;
        if (jenis == "Laptop Desain"      && l->getTipePerangkat() == "Laptop Desain")        ++n;
        if (jenis == "Laptop Workstation" && l->getTipePerangkat() == "Laptop Workstation")   ++n;
    }
    return n;
}
int ManajerKatalog::hitungDenganWifiVersi(const QString &v) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && l->getWireless().getWifiVersi() == v) ++n;
    return n;
}
int ManajerKatalog::hitungDenganBtVersi(const QString &v) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p); l && l->getWireless().getBluetoothVersi() == v) ++n;
    return n;
}

// ── GENERATE ID (format 0001) ─────────────────────────────────────────────────
QString ManajerKatalog::generateId() const {
    int maxId = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p))
            maxId = qMax(maxId, l->getId().toInt());
    return QString("%1").arg(maxId + 1, 4, 10, QChar('0'));
}

// ── DATA AWAL ─────────────────────────────────────────────────────────────────
void ManajerKatalog::loadDataAwal() {
    // SpekDisplay(ukuran, tipe, resolusi, refreshRate, sRGB%)
    // IPS biasa ~45-72%, IPS 100% sRGB, OLED ~130% (gamut DCI-P3 lebih luas)
    // SpekCPU(brand, seri, cores, modelNumber, maxBoostGHz)
    // SpekWireless(wifiVersi, bluetoothVersi)

    // ── Laptop Biasa (6) ──────────────────────────────────────────────────

    { auto *l = new Laptop(
        "0001","Dell Inspiron 15","Dell",
        SpekCPU("Intel","Core i5",10,"1135G7","4.2"),
        SpekRAM("DDR4",8,2,1),
        SpekPenyimpanan("SSD",256,1,0),
        SpekDisplay(15.6,"IPS","1920x1080",60,72),
        SpekWireless("Wi-Fi 6","5.1"),
        "Intel","UHD Graphics",2,1,true,1,1,
        12500000,15,"1 Tahun",54);
      l->setImagePaths({":/images/dell-inspiron-15.png"});
      tambahLaptop(l); }

    { auto *l = new Laptop(
        "0002","Lenovo ThinkPad X1","Lenovo",
        SpekCPU("Intel","Core i7",10,"1265U","4.8"),
        SpekRAM("DDR5",16,2,0),
        SpekPenyimpanan("SSD",512,1,0),
        SpekDisplay(14.0,"OLED","2560x1600",60,130),
        SpekWireless("Wi-Fi 6E","5.2"),
        "Intel","Iris Xe",1,1,true,1,1,
        25000000,8,"2 Tahun",57);
      l->setImagePaths({":/images/lenovo-thinkpad-x1.png"});
      tambahLaptop(l); }

    { auto *l = new Laptop(
        "0003","Acer Aspire 5","Acer",
        SpekCPU("Intel","Core i3",2,"1115G4","4.1"),
        SpekRAM("DDR4",4,2,1),
        SpekPenyimpanan("HDD",1024,1,0),
        SpekDisplay(15.6,"IPS","1366x768",60,45),
        SpekWireless("Wi-Fi 5","5.0"),
        "Intel","UHD Graphics 620",2,0,false,1,1,
        7500000,20,"1 Tahun",48);
      l->setImagePaths({":/images/acer-aspire-5.png"});
      tambahLaptop(l); }

    { auto *l = new Laptop(
        "0004","Samsung Galaxy Book3","Samsung",
        SpekCPU("Intel","Core i5",12,"1235U","4.4"),
        SpekRAM("DDR4",8,1,0),
        SpekPenyimpanan("SSD",256,1,0),
        SpekDisplay(15.6,"OLED","1920x1080",60,130),
        SpekWireless("Wi-Fi 6E","5.1"),
        "Intel","Iris Xe",1,1,true,1,1,14000000,18,"1 Tahun",54);
      l->setImagePaths({":/images/samsung-galaxy-book3.png"});
      tambahLaptop(l); }

    { auto *l = new Laptop(
        "0005","HP Envy x360 13","HP",
        SpekCPU("AMD","Ryzen 3",4,"5300U","3.8"),
        SpekRAM("DDR4",8,1,0),
        SpekPenyimpanan("SSD",256,1,0),
        SpekDisplay(13.3,"IPS","1920x1080",60,72),
        SpekWireless("Wi-Fi 6","5.0"),
        "AMD","Radeon Graphics",1,1,true,0,1,11000000,14,"1 Tahun",51);
      l->setImagePaths({":/images/hp-envy-x360-13.png"});
      tambahLaptop(l); }

    { auto *l = new Laptop(
        "0006","Dell XPS 13 Plus","Dell",
        SpekCPU("Intel","Core i7",10,"1360P","5.0"),
        SpekRAM("DDR5",16,1,0),
        SpekPenyimpanan("SSD",512,1,0),
        SpekDisplay(13.4,"OLED","3456x2160",60,130),
        SpekWireless("Wi-Fi 6E","5.2"),
        "Intel","Iris Xe",0,1,true,0,1,28000000,9,"2 Tahun",55);
      l->setImagePaths({":/images/dell-xps-13.png"});
      tambahLaptop(l); }

    // ── Laptop Gaming (6) ─────────────────────────────────────────────────

    { auto *l = new LaptopGaming(
        "0007","HP Pavilion Gaming 15","HP",
        SpekCPU("AMD","Ryzen 5",6,"5600H","4.2"),
        SpekRAM("DDR4",16,2,0),
        SpekPenyimpanan("SSD",512,2,1),
        SpekDisplay(15.6,"IPS","1920x1080",144,72),
        SpekWireless("Wi-Fi 6","5.0"),
        "Nvidia","GTX 1650",3,1,false,1,1,18000000,12,"1 Tahun",70,45,true);
      l->setImagePaths({":/images/hp-pavilion-gaming-15.png"});
      tambahLaptopGaming(l); }

    { auto *l = new LaptopGaming(
        "0008","ASUS ROG Strix G17","ASUS",
        SpekCPU("AMD","Ryzen 7",8,"7745HX","5.1"),
        SpekRAM("DDR5",32,2,0),
        SpekPenyimpanan("SSD",1024,2,1),
        SpekDisplay(17.3,"IPS","2560x1440",165,72),
        SpekWireless("Wi-Fi 6E","5.2"),
        "Nvidia","RTX 3060",4,1,true,1,1,35000000,5,"2 Tahun",90,80,true);
      l->setImagePaths({":/images/asus-rog-strix-g17.png"});
      tambahLaptopGaming(l); }

    { auto *l = new LaptopGaming(
        "0009","MSI Katana GF66","MSI",
        SpekCPU("Intel","Core i7",14,"12700H","4.7"),
        SpekRAM("DDR4",16,2,0),
        SpekPenyimpanan("SSD",512,1,0),
        SpekDisplay(15.6,"IPS","1920x1080",144,72),
        SpekWireless("Wi-Fi 6","5.2"),
        "Nvidia","RTX 3050",3,1,false,1,1,22000000,10,"2 Tahun",53,60,true);
      l->setImagePaths({":/images/msi-katana-gf66.png"});
      tambahLaptopGaming(l); }

    { auto *l = new LaptopGaming(
        "0010","Gigabyte Aorus 17","Gigabyte",
        SpekCPU("Intel","Core i7",16,"13700H","5.0"),
        SpekRAM("DDR4",16,2,0),
        SpekPenyimpanan("SSD",512,2,1),
        SpekDisplay(17.3,"IPS","1920x1080",240,72),
        SpekWireless("Wi-Fi 6E","5.2"),
        "Nvidia","RTX 3070",4,1,true,1,1,32000000,6,"2 Tahun",99,80,true);
      l->setImagePaths({":/images/gigabyte-aorus-17.png"});
      tambahLaptopGaming(l); }

    { auto *l = new LaptopGaming(
        "0011","Razer Blade 15","Razer",
        SpekCPU("Intel","Core i9",24,"13950HX","5.5"),
        SpekRAM("DDR5",32,2,0),
        SpekPenyimpanan("SSD",1024,2,1),
        SpekDisplay(15.6,"OLED","3840x2160",120,130),
        SpekWireless("Wi-Fi 6E","5.3"),
        "Nvidia","RTX 4070",3,1,true,1,1,48000000,3,"2 Tahun",80,95,true);
      l->setImagePaths({":/images/razer-blade-15.png"});
      tambahLaptopGaming(l); }

    { auto *l = new LaptopGaming(
        "0012","Lenovo Legion 5i","Lenovo",
        SpekCPU("Intel","Core i7",14,"12700H","4.7"),
        SpekRAM("DDR5",16,2,0),
        SpekPenyimpanan("SSD",512,2,1),
        SpekDisplay(15.6,"IPS","2560x1440",165,72),
        SpekWireless("Wi-Fi 6","5.2"),
        "Nvidia","RTX 4060",3,1,true,1,1,22000000,14,"1 Tahun",80,65,true);
      l->setImagePaths({":/images/lenovo-legion-5i.png"});
      tambahLaptopGaming(l); }

    // ── Laptop Desain (4) ─────────────────────────────────────────────────

    { auto *l = new LaptopDesain(
        "0013","ASUS ProArt Studiobook 16","ASUS",
        SpekCPU("Intel","Core i9",24,"13980HX","5.6"),
        SpekRAM("DDR5",32,2,0),
        SpekPenyimpanan("SSD",1024,1,0),
        SpekDisplay(16.0,"OLED","3840x2400",60,130),
        SpekWireless("Wi-Fi 6E","5.3"),
        "Nvidia","RTX 4070",2,1,true,1,1,45000000,4,"2 Tahun",90,true);
      l->setImagePaths({":/images/asus-proart-studiobook-16.png"});
      tambahLaptopDesain(l); }

    { auto *l = new LaptopDesain(
        "0014","Microsoft Surface Laptop Studio 2","Microsoft",
        SpekCPU("Intel","Core i7",14,"13700H","5.0"),
        SpekRAM("DDR5",16,1,0),
        SpekPenyimpanan("SSD",512,1,0),
        SpekDisplay(14.4,"IPS","2400x1600",120,100),
        SpekWireless("Wi-Fi 6E","5.3"),
        "Nvidia","RTX 4050",2,1,true,0,1,26000000,7,"2 Tahun",58,true);
      l->setImagePaths({":/images/microsoft-surface-laptop-studio-2.png"});
      tambahLaptopDesain(l); }

    { auto *l = new LaptopDesain(
        "0015","Dell XPS 15 9530","Dell",
        SpekCPU("Intel","Core i7",14,"13700H","5.0"),
        SpekRAM("DDR5",16,2,0),
        SpekPenyimpanan("SSD",512,2,0),
        SpekDisplay(15.6,"OLED","3456x2160",60,130),
        SpekWireless("Wi-Fi 6E","5.2"),
        "Nvidia","RTX 4060",2,1,true,1,1,24000000,9,"2 Tahun",86,false);
      l->setImagePaths({":/images/dell-xps-15-9530.png"});
      tambahLaptopDesain(l); }

    { auto *l = new LaptopDesain(
        "0016","LG Gram Style","LG",
        SpekCPU("Intel","Core i7",10,"1365U","5.2"),
        SpekRAM("DDR5",16,1,0),
        SpekPenyimpanan("SSD",512,1,0),
        SpekDisplay(16.0,"OLED","2560x1600",60,130),
        SpekWireless("Wi-Fi 6E","5.1"),
        "Intel","Iris Xe",2,1,true,1,1,20000000,11,"1 Tahun",80,false);
      l->setImagePaths({":/images/lg-gram-style.png"});
      tambahLaptopDesain(l); }

    // ── Laptop Workstation (4) ────────────────────────────────────────────

    { auto *l = new LaptopWorkstation(
        "0017","Lenovo ThinkPad X1 Extreme","Lenovo",
        SpekCPU("Intel","Core i7",14,"13700H","5.0"),
        SpekRAM("DDR5",16,2,0),
        SpekPenyimpanan("SSD",512,2,0),
        SpekDisplay(16.0,"IPS","3840x2400",60,100),
        SpekWireless("Wi-Fi 6E","5.2"),
        "Nvidia","RTX 3050 Ti",2,1,true,1,1,30000000,6,"2 Tahun",90,false,false);
      l->setImagePaths({":/images/lenovo-thinkpad-x1-extreme.png"});
      tambahLaptopWorkstation(l); }

    { auto *l = new LaptopWorkstation(
        "0018","HP ZBook Studio G10","HP",
        SpekCPU("Intel","Core i9",24,"13950HX","5.5"),
        SpekRAM("DDR5",32,2,0),
        SpekPenyimpanan("SSD",1024,2,0),
        SpekDisplay(16.0,"OLED","3840x2400",120,130),
        SpekWireless("Wi-Fi 6E","5.3"),
        "Nvidia","RTX Ada 4000",2,1,true,1,1,52000000,3,"3 Tahun",86,true,true);
      l->setImagePaths({":/images/hp-zbook-studio-g10.png"});
      tambahLaptopWorkstation(l); }

    { auto *l = new LaptopWorkstation(
        "0019","Dell Precision 5680","Dell",
        SpekCPU("Intel","Core i9",24,"13900H","5.4"),
        SpekRAM("DDR5",32,2,0),
        SpekPenyimpanan("SSD",1024,2,0),
        SpekDisplay(15.6,"OLED","3456x2160",60,130),
        SpekWireless("Wi-Fi 6E","5.2"),
        "Nvidia","RTX Ada 3000",2,1,true,1,1,48000000,4,"3 Tahun",100,true,true);
      l->setImagePaths({":/images/dell-precision-5680.png"});
      tambahLaptopWorkstation(l); }

    { auto *l = new LaptopWorkstation(
        "0020","MSI CreatorPro Z17","MSI",
        SpekCPU("Intel","Core i9",24,"13980HX","5.6"),
        SpekRAM("DDR5",32,2,0),
        SpekPenyimpanan("SSD",1024,2,0),
        SpekDisplay(17.0,"IPS","3840x2160",120,100),
        SpekWireless("Wi-Fi 6E","5.3"),
        "Nvidia","RTX A2000",2,1,true,1,1,40000000,5,"2 Tahun",90,true,false);
      l->setImagePaths({":/images/msi-creatorpro-Z17.png"});
      tambahLaptopWorkstation(l); }
}
