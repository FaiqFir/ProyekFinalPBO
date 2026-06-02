#pragma once
#include "Laptop.h"

// =============================================================
//  LaptopDesain — Laptop untuk keperluan desain grafis
//  Ciri khas: layar IPS/OLED dengan sRGB >= 100% atau OLED,
//  digunakan untuk desain, editing foto/video.
// =============================================================
class LaptopDesain : public Laptop {
public:
    LaptopDesain() = default;
    LaptopDesain(const QString &id, const QString &nama, const QString &brand,
                 const SpekCPU &cpu, const SpekRAM &ram,
                 const SpekPenyimpanan &storage, const SpekDisplay &display,
                 const SpekWireless &wireless,
                 const QString &gpuBrand, const QString &gpuModel,
                 int usbA, int usbC, bool usbCPD, int hdmi, int audioJack,
                 int harga, int stok, const QString &garansi, int batteryWh,
                 bool touchscreen = false);

    // === Override (Polymorphism) ===
    QString tampilkanInfo()    const override;
    QString getRingkasanSpek() const override;
    QString getTipePerangkat() const override { return "Laptop Desain"; }

    bool isTouchscreen() const { return m_touchscreen; }
    void setTouchscreen(bool v) { m_touchscreen = v; }

private:
    bool m_touchscreen = false;
};
