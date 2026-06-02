#pragma once
#include "Laptop.h"

// =============================================================
//  LaptopWorkstation — Laptop kelas workstation profesional
//  Ciri khas: CPU/GPU kelas workstation (HX series / Quadro /
//  RTX Ada), slot RAM > 1, slot SSD > 1, untuk engineering
//  dan komputasi berat.
// =============================================================
class LaptopWorkstation : public Laptop {
public:
    LaptopWorkstation() = default;
    LaptopWorkstation(const QString &id, const QString &nama, const QString &brand,
                      const SpekCPU &cpu, const SpekRAM &ram,
                      const SpekPenyimpanan &storage, const SpekDisplay &display,
                      const SpekWireless &wireless,
                      const QString &gpuBrand, const QString &gpuModel,
                      int usbA, int usbC, bool usbCPD, int hdmi, int audioJack,
                      int harga, int stok, const QString &garansi, int batteryWh,
                      bool isvCertified = false, bool eccRam = false);

    // === Override (Polymorphism) ===
    QString tampilkanInfo()    const override;
    QString getRingkasanSpek() const override;
    QString getTipePerangkat() const override { return "Laptop Workstation"; }

    bool isIsvCertified() const { return m_isvCertified; }
    bool isEccRam()       const { return m_eccRam; }
    void setIsvCertified(bool v) { m_isvCertified = v; }
    void setEccRam(bool v)       { m_eccRam = v; }

private:
    bool m_isvCertified = false; // Sertifikasi ISV (AutoCAD, SolidWorks, dll)
    bool m_eccRam       = false; // ECC RAM support
};
