#include "ManajerKatalog.h"
#include <algorithm>

ManajerKatalog::ManajerKatalog() {
    loadDataAwal();
}

ManajerKatalog::~ManajerKatalog() {
    for (auto *p : m_produk) delete p;
}

// ── CRUD ────────────────────────────────────────────────────────────────────

void ManajerKatalog::tambahLaptop(Laptop *laptop) {
    m_produk.append(laptop);
}

void ManajerKatalog::tambahLaptopGaming(LaptopGaming *laptop) {
    m_produk.append(laptop);
}

bool ManajerKatalog::hapusProduk(const QString &id) {
    for (int i = 0; i < m_produk.size(); ++i) {
        Laptop *l = dynamic_cast<Laptop*>(m_produk[i]);
        if (l && l->getId() == id) {
            delete m_produk[i];
            m_produk.removeAt(i);
            return true;
        }
    }
    return false;
}

bool ManajerKatalog::updateStokMasuk(const QString &id, int jumlah) {
    Laptop *l = cariLaptopById(id);
    if (!l || jumlah <= 0) return false;
    l->setStok(l->getStok() + jumlah);
    return true;
}

bool ManajerKatalog::updateStokTerjual(const QString &id, int jumlah) {
    Laptop *l = cariLaptopById(id);
    if (!l || jumlah <= 0) return false;
    if (jumlah > l->getStok()) return false;  // Validasi!
    l->setStok(l->getStok() - jumlah);
    return true;
}

// ── CARI ────────────────────────────────────────────────────────────────────

PerangkatKeras* ManajerKatalog::cariById(const QString &id) const {
    for (auto *p : m_produk) {
        Laptop *l = dynamic_cast<Laptop*>(p);
        if (l && l->getId() == id) return p;
    }
    return nullptr;
}

Laptop* ManajerKatalog::cariLaptopById(const QString &id) const {
    return dynamic_cast<Laptop*>(cariById(id));
}

// ── FILTER & SORT ────────────────────────────────────────────────────────────

bool ManajerKatalog::laptopLolosFilter(Laptop *l, const FilterParam &f) const {
    if (!f.cpuSeri.isEmpty() && !f.cpuSeri.contains(l->getCPU().getSeri()))
        return false;
    if (!f.gpuBrand.isEmpty() && !f.gpuBrand.contains(l->getGpuBrand()))
        return false;
    if (!f.ramTipe.isEmpty() && !f.ramTipe.contains(l->getRAM().getTipe()))
        return false;
    if (!f.ramKapasitas.isEmpty() &&
        !f.ramKapasitas.contains(QString::number(l->getRAM().getKapasitas())))
        return false;
    if (!f.storageTipe.isEmpty() && !f.storageTipe.contains(l->getStorage().getTipe()))
        return false;
    if (!f.storageKapasitas.isEmpty() &&
        !f.storageKapasitas.contains(QString::number(l->getStorage().getKapasitas())))
        return false;
    if (!f.displayTipe.isEmpty() && !f.displayTipe.contains(l->getDisplay().getTipe()))
        return false;

    // Port: multi-select — semua port yang dipilih harus ada
    for (const QString &port : f.ports) {
        if (port == "USB-A"     && l->getUsbA() == 0)   return false;
        if (port == "USB-C"     && !l->hasUsbC())        return false;
        if (port == "USB-C PD"  && !l->hasUsbCPD())      return false;
        if (port == "HDMI"      && !l->hasHdmi())         return false;
        if (port == "Audio Jack"&& !l->hasAudioJack())    return false;
    }
    return true;
}

QVector<Laptop*> ManajerKatalog::filterDanSort(const FilterParam &f, SortMode sort) const {
    QVector<Laptop*> hasil;
    for (auto *p : m_produk) {
        Laptop *l = dynamic_cast<Laptop*>(p);
        if (l && laptopLolosFilter(l, f))
            hasil.append(l);
    }

    if (sort == SortMode::HargaAsc)
        std::sort(hasil.begin(), hasil.end(),
                  [](Laptop *a, Laptop *b){ return a->getHarga() < b->getHarga(); });
    else if (sort == SortMode::RamDesc)
        std::sort(hasil.begin(), hasil.end(),
                  [](Laptop *a, Laptop *b){ return a->getRAM().getKapasitas() > b->getRAM().getKapasitas(); });
    else if (sort == SortMode::StokDesc)
        std::sort(hasil.begin(), hasil.end(),
                  [](Laptop *a, Laptop *b){ return a->getStok() > b->getStok(); });

    return hasil;
}

// ── STATISTIK FILTER ─────────────────────────────────────────────────────────

int ManajerKatalog::hitungDenganCpuSeri(const QString &seri) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p))
            if (l->getCPU().getSeri() == seri) ++n;
    return n;
}

int ManajerKatalog::hitungDenganGpuBrand(const QString &brand) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p))
            if (l->getGpuBrand() == brand) ++n;
    return n;
}

int ManajerKatalog::hitungDenganRamKapasitas(int kapasitas) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p))
            if (l->getRAM().getKapasitas() == kapasitas) ++n;
    return n;
}

int ManajerKatalog::hitungDenganStorageTipe(const QString &tipe) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p))
            if (l->getStorage().getTipe() == tipe) ++n;
    return n;
}

int ManajerKatalog::hitungDenganDisplayTipe(const QString &tipe) const {
    int n = 0;
    for (auto *p : m_produk)
        if (auto *l = dynamic_cast<Laptop*>(p))
            if (l->getDisplay().getTipe() == tipe) ++n;
    return n;
}

// ── GENERATE ID ─────────────────────────────────────────────────────────────

QString ManajerKatalog::generateId() const {
    int maxId = 0;
    for (auto *p : m_produk) {
        if (auto *l = dynamic_cast<Laptop*>(p))
            maxId = qMax(maxId, l->getId().toInt());
    }
    return QString::number(maxId + 1);
}

// ── DATA AWAL (12 laptop) ───────────────────────────────────────────────────

void ManajerKatalog::loadDataAwal() {
    // ── Laptop biasa ──────────────────────────────────────────────────────
    { auto *l = new Laptop(
        "1", "Dell Inspiron 15", "Dell",
        SpekCPU("Intel","Core i5",4,"2.4GHz"),
        SpekRAM("DDR4",8,2,1),
        SpekPenyimpanan("SSD",256,1,0),
        SpekDisplay(15.6,"IPS","1920x1080",60),
        "Intel","UHD Graphics",
        2,true,true,true,true,
        12500000,15,"1 Tahun");
    l->setImagePaths({":/images/dell-inspiron-15.jpeg"});
    tambahLaptop(l); }

    { auto *l = new Laptop(
        "2", "Lenovo ThinkPad X1", "Lenovo",
        SpekCPU("Intel","Core i7",8,"3.0GHz"),
        SpekRAM("DDR5",16,2,0),
        SpekPenyimpanan("SSD",512,1,0),
        SpekDisplay(14.0,"OLED","2560x1600",60),
        "Intel","Iris Xe",
        1,true,true,true,true,
        25000000,8,"2 Tahun");
    l->setImagePaths({":/images/lenovo-thinkpad-x1.jpg"});
    tambahLaptop(l); }

    { auto *l = new Laptop(
        "3", "Acer Aspire 5", "Acer",
        SpekCPU("Intel","Core i3",2,"2.1GHz"),
        SpekRAM("DDR4",4,2,1),
        SpekPenyimpanan("HDD",1024,1,0),
        SpekDisplay(15.6,"IPS","1366x768",60),
        "Intel","UHD Graphics 620",
        2,false,false,true,true,
        7500000,20,"1 Tahun");
    l->setImagePaths({":/images/acer-aspire-5.jpg"});
    tambahLaptop(l); }

    tambahLaptop(new Laptop(
        "4", "Samsung Galaxy Book3", "Samsung",
        SpekCPU("Intel","Core i5",4,"2.6GHz"),
        SpekRAM("DDR4",8,1,0),
        SpekPenyimpanan("SSD",256,1,0),
        SpekDisplay(15.6,"OLED","1920x1080",60),
        "Intel","Iris Xe",
        1,true,true,true,true,
        14000000,18,"1 Tahun"));

    tambahLaptop(new Laptop(
        "5", "HP Envy 13", "HP",
        SpekCPU("AMD","Ryzen 3",4,"2.6GHz"),
        SpekRAM("DDR4",8,1,0),
        SpekPenyimpanan("SSD",256,1,0),
        SpekDisplay(13.3,"IPS","1920x1080",60),
        "AMD","Radeon Graphics",
        1,true,true,false,true,
        11000000,14,"1 Tahun"));

    tambahLaptop(new Laptop(
        "6", "Dell XPS 13", "Dell",
        SpekCPU("Intel","Core i7",8,"3.2GHz"),
        SpekRAM("DDR5",16,1,0),
        SpekPenyimpanan("SSD",512,1,0),
        SpekDisplay(13.4,"OLED","3456x2160",60),
        "Intel","Iris Xe",
        0,true,true,false,true,
        28000000,9,"2 Tahun"));

    tambahLaptop(new Laptop(
        "7", "Apple MacBook Air M2", "Apple",
        SpekCPU("AMD","Ryzen 9",8,"3.0GHz"),
        SpekRAM("DDR5",8,1,0),
        SpekPenyimpanan("SSD",256,1,0),
        SpekDisplay(13.6,"IPS","2560x1664",60),
        "AMD","Integrated",
        0,true,true,false,true,
        18500000,7,"1 Tahun"));

    // ── Laptop Gaming (LaptopGaming) ─────────────────────────────────────
    tambahLaptopGaming(new LaptopGaming(
        "8", "HP Pavilion Gaming", "HP",
        SpekCPU("AMD","Ryzen 5",6,"3.2GHz"),
        SpekRAM("DDR4",16,2,0),
        SpekPenyimpanan("SSD",512,2,1),
        SpekDisplay(15.6,"IPS","1920x1080",144),
        "Nvidia","GTX 1650",
        3,true,false,true,true,
        18000000,12,"1 Tahun",
        45,false));

    tambahLaptopGaming(new LaptopGaming(
        "9", "ASUS ROG Strix", "ASUS",
        SpekCPU("AMD","Ryzen 7",8,"3.8GHz"),
        SpekRAM("DDR5",32,2,0),
        SpekPenyimpanan("SSD",1024,2,1),
        SpekDisplay(17.3,"IPS","2560x1440",165),
        "Nvidia","RTX 3060",
        4,true,true,true,true,
        35000000,5,"2 Tahun",
        80,true));

    tambahLaptopGaming(new LaptopGaming(
        "10", "MSI Katana GF66", "MSI",
        SpekCPU("Intel","Core i7",8,"3.5GHz"),
        SpekRAM("DDR4",16,2,0),
        SpekPenyimpanan("SSD",512,1,0),
        SpekDisplay(15.6,"IPS","1920x1080",144),
        "Nvidia","RTX 3050",
        3,true,false,true,true,
        22000000,10,"2 Tahun",
        60,false));

    tambahLaptopGaming(new LaptopGaming(
        "11", "Gigabyte Aorus 17", "Gigabyte",
        SpekCPU("Intel","Core i7",8,"3.6GHz"),
        SpekRAM("DDR4",16,2,0),
        SpekPenyimpanan("SSD",512,2,1),
        SpekDisplay(17.3,"IPS","1920x1080",240),
        "Nvidia","RTX 3070",
        4,true,true,true,true,
        32000000,6,"2 Tahun",
        80,true));

    tambahLaptopGaming(new LaptopGaming(
        "12", "Razer Blade 15", "Razer",
        SpekCPU("Intel","Core i9",10,"4.2GHz"),
        SpekRAM("DDR5",32,2,0),
        SpekPenyimpanan("SSD",1024,2,1),
        SpekDisplay(15.6,"OLED","3840x2160",120),
        "Nvidia","RTX 4070",
        3,true,true,true,true,
        48000000,3,"2 Tahun",
        95,true));
}
