#pragma once
#include <QString>

// =============================================================
//  PerangkatKeras — Abstract Base Class (Abstraction + Inheritance)
//  Semua perangkat keras diturunkan dari class ini.
//  Memiliki pure virtual method yang wajib di-override.
// =============================================================
class PerangkatKeras {
public:
    virtual ~PerangkatKeras() = default;

    // Pure virtual — wajib diimplementasikan oleh setiap turunan
    virtual QString tampilkanInfo()      const = 0;
    virtual QString getRingkasanSpek()   const = 0;
    virtual QString getTipePerangkat()   const = 0;

    // Getter/setter umum (encapsulation)
    QString getNama()  const { return m_nama; }
    QString getBrand() const { return m_brand; }
    int     getHarga() const { return m_harga; }
    int     getStok()  const { return m_stok;  }

    void setNama (const QString &v) { m_nama  = v; }
    void setBrand(const QString &v) { m_brand = v; }
    void setHarga(int v)            { m_harga = v; }
    void setStok (int v)            { m_stok  = v; }

protected:
    // Semua atribut sensitif → private via protected agar turunan bisa akses
    QString m_nama;
    QString m_brand;
    int     m_harga = 0;
    int     m_stok  = 0;
};
