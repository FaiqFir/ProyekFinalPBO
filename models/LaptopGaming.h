#pragma once
#include "Laptop.h"

// =============================================================
//  LaptopGaming — Turunan dari Laptop (Polymorphism)
//  Override tampilkanInfo() & getRingkasanSpek() untuk
//  menampilkan info gaming tambahan (TDP, refresh rate tinggi).
//  Spesifikasi khas gaming: TDP dan sistem pendingin dual fan.
// =============================================================
class LaptopGaming : public Laptop {
public:
    LaptopGaming() = default;
    LaptopGaming(const QString &id, const QString &nama, const QString &brand,
                 const SpekCPU &cpu, const SpekRAM &ram,
                 const SpekPenyimpanan &storage, const SpekDisplay &display,
                 const SpekWireless &wireless,
                 const QString &gpuBrand, const QString &gpuModel,
                 int usbA, int usbC, bool usbCPD, int hdmi, int audioJack,
                 int harga, int stok, const QString &garansi, int batteryWh,
                 int tdpWatt, bool dualFan);

    // === Override (Polymorphism) ===
    QString tampilkanInfo()    const override;
    QString getRingkasanSpek() const override;
    QString getTipePerangkat() const override { return "Laptop Gaming"; }

    // Getter/setter khusus gaming
    int  getTdpWatt()  const { return m_tdpWatt; }
    bool getDualFan()  const { return m_dualFan; }
    void setTdpWatt(int v)   { m_tdpWatt = v; }
    void setDualFan(bool v)  { m_dualFan = v; }

private:
    int  m_tdpWatt = 45;
    bool m_dualFan = false;  // sistem pendingin dual fan
};
