#pragma once
#include "Laptop.h"

// =============================================================
//  LaptopGaming — Turunan dari Laptop (Polymorphism)
//  Override tampilkanInfo() & getRingkasanSpek() untuk
//  menampilkan info gaming tambahan (TDP, refresh rate tinggi).
// =============================================================
class LaptopGaming : public Laptop {
public:
    LaptopGaming() = default;
    LaptopGaming(const QString &id, const QString &nama, const QString &brand,
                 const SpekCPU &cpu, const SpekRAM &ram,
                 const SpekPenyimpanan &storage, const SpekDisplay &display,
                 const QString &gpuBrand, const QString &gpuModel,
                 int usbA, bool usbC, bool usbCPD, bool hdmi, bool audioJack,
                 int harga, int stok, const QString &garansi,
                 int tdpWatt, bool hasRgb);

    // === Override (Polymorphism) ===
    QString tampilkanInfo()    const override;
    QString getRingkasanSpek() const override;
    QString getTipePerangkat() const override { return "Laptop Gaming"; }

    // Getter/setter khusus gaming
    int  getTdpWatt() const { return m_tdpWatt; }
    bool getHasRgb()  const { return m_hasRgb;  }
    void setTdpWatt(int v)   { m_tdpWatt = v; }
    void setHasRgb (bool v)  { m_hasRgb  = v; }

private:
    int  m_tdpWatt = 45;
    bool m_hasRgb  = false;
};
