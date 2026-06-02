#pragma once
#include "Laptop.h"
#include "LaptopGaming.h"
#include "LaptopDesain.h"
#include "LaptopWorkstation.h"
#include <QVector>
#include <QStringList>
#include <memory>

// =============================================================
//  ManajerKatalog — Mengelola koleksi PerangkatKeras*
// =============================================================
class ManajerKatalog {
public:
    ManajerKatalog();
    ~ManajerKatalog();

    const QVector<PerangkatKeras*>& semuaProduk() const { return m_produk; }
    PerangkatKeras* cariById(const QString &id) const;
    Laptop*         cariLaptopById(const QString &id) const;

    // CRUD
    void tambahLaptop            (Laptop           *laptop);
    void tambahLaptopGaming      (LaptopGaming      *laptop);
    void tambahLaptopDesain      (LaptopDesain      *laptop);
    void tambahLaptopWorkstation (LaptopWorkstation *laptop);
    bool hapusProduk       (const QString &id);
    bool updateStokMasuk   (const QString &id, int jumlah);
    bool updateStokTerjual (const QString &id, int jumlah);

    // Filter & Sort
    struct FilterParam {
        QStringList jenisLaptop;     // "Laptop Office", "Laptop Gaming", "Laptop Desain"
        QStringList cpuSeri;
        QStringList gpuBrand;
        QStringList ramTipe;
        QStringList ramKapasitas;
        QStringList storageTipe;
        QStringList storageKapasitas;
        QStringList displayTipe;
        QStringList wifiVersi;       // "Wi-Fi 5", "Wi-Fi 6", "Wi-Fi 6E", "Wi-Fi 7"
        QStringList btVersi;         // "4.2", "5.0", "5.1", "5.2", "5.3", "5.4"
        QStringList ports;
    };

    enum class SortMode { None, HargaAsc, RamDesc, StokDesc, StorageDesc };

    QVector<Laptop*> filterDanSort(const FilterParam &f, SortMode sort) const;

    // Statistik badge filter
    int hitungDenganCpuSeri      (const QString &seri)  const;
    int hitungDenganGpuBrand     (const QString &brand) const;
    int hitungDenganRamKapasitas (int kapasitas)         const;
    int hitungDenganStorageTipe  (const QString &tipe)  const;
    int hitungDenganDisplayTipe  (const QString &tipe)  const;
    int hitungDenganJenis        (const QString &jenis)  const;
    int hitungDenganWifiVersi    (const QString &v)     const;
    int hitungDenganBtVersi      (const QString &v)     const;

    QString generateId() const;  // format "0001", "0002", dst

private:
    QVector<PerangkatKeras*> m_produk;
    void loadDataAwal();
    bool laptopLolosFilter(Laptop *l, const FilterParam &f) const;
};
