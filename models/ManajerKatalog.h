#pragma once
#include "Laptop.h"
#include "LaptopGaming.h"
#include <QVector>
#include <QStringList>
#include <memory>

// =============================================================
//  ManajerKatalog — Mengelola koleksi PerangkatKeras*
//  Polymorphism nyata: vector<PerangkatKeras*> menyimpan
//  Laptop dan LaptopGaming secara transparan.
// =============================================================
class ManajerKatalog {
public:
    ManajerKatalog();
    ~ManajerKatalog();

    // Akses data
    const QVector<PerangkatKeras*>& semuaProduk() const { return m_produk; }
    PerangkatKeras* cariById(const QString &id) const;
    Laptop*         cariLaptopById(const QString &id) const;

    // CRUD
    void tambahLaptop      (Laptop *laptop);
    void tambahLaptopGaming(LaptopGaming *laptop);
    bool hapusProduk       (const QString &id);
    bool updateStokMasuk   (const QString &id, int jumlah);
    bool updateStokTerjual (const QString &id, int jumlah);  // validasi: tidak boleh > stok

    // Filter & Sort — mengembalikan list pointer yang sudah difilter
    struct FilterParam {
        QStringList cpuSeri;         // single-select (max 1)
        QStringList gpuBrand;        // single-select
        QStringList ramTipe;         // single-select
        QStringList ramKapasitas;    // single-select (sebagai string "8", "16", dll)
        QStringList storageTipe;     // single-select
        QStringList storageKapasitas;// single-select
        QStringList displayTipe;     // single-select
        QStringList ports;           // multi-select
    };

    enum class SortMode { None, HargaAsc, RamDesc, StokDesc };

    QVector<Laptop*> filterDanSort(const FilterParam &f, SortMode sort) const;

    // Statistik untuk badge angka di filter
    int hitungDenganCpuSeri      (const QString &seri)  const;
    int hitungDenganGpuBrand     (const QString &brand) const;
    int hitungDenganRamKapasitas (int kapasitas)         const;
    int hitungDenganStorageTipe  (const QString &tipe)  const;
    int hitungDenganDisplayTipe  (const QString &tipe)  const;

    QString generateId() const;

private:
    QVector<PerangkatKeras*> m_produk;   // Polymorphism: simpan semua tipe
    void loadDataAwal();
    bool laptopLolosFilter(Laptop *l, const FilterParam &f) const;
};
