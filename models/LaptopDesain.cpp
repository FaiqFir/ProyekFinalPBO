#include "LaptopDesain.h"

LaptopDesain::LaptopDesain(const QString &id, const QString &nama, const QString &brand,
                           const SpekCPU &cpu, const SpekRAM &ram,
                           const SpekPenyimpanan &storage, const SpekDisplay &display,
                           const SpekWireless &wireless,
                           const QString &gpuBrand, const QString &gpuModel,
                           int usbA, int usbC, bool usbCPD, int hdmi, int audioJack,
                           int harga, int stok, const QString &garansi, int batteryWh,
                           bool touchscreen)
    : Laptop(id, nama, brand, cpu, ram, storage, display, wireless,
             gpuBrand, gpuModel, usbA, usbC, usbCPD, hdmi, audioJack,
             harga, stok, garansi, batteryWh),
      m_touchscreen(touchscreen)
{}

QString LaptopDesain::tampilkanInfo() const {
    return QString(
        "Nama       : %1\n"
        "Brand      : %2\n"
        "Tipe       : %3\n"
        "CPU        : %4\n"
        "GPU        : %5 %6\n"
        "RAM        : %7\n"
        "Storage    : %8\n"
        "Display    : %9\n"
        "sRGB       : %10%\n"
        "Touchscreen: %11\n"
        "Wireless   : %12\n"
        "Port       : %13\n"
        "Battery    : %14 Wh\n"
        "Harga      : %15\n"
        "Stok       : %16 unit\n"
        "Garansi    : %17"
    )
    .arg(m_nama, m_brand, getTipePerangkat())
    .arg(m_cpu.toDisplayString())
    .arg(m_gpuBrand, m_gpuModel)
    .arg(m_ram.toDisplayString())
    .arg(m_storage.toDisplayString())
    .arg(m_display.toDisplayString())
    .arg(m_display.getSrgb())
    .arg(m_touchscreen ? "Ya" : "Tidak")
    .arg(m_wireless.toDisplayString())
    .arg(portsString())
    .arg(m_batteryWh)
    .arg(formatHarga())
    .arg(m_stok)
    .arg(m_garansi);
}

QString LaptopDesain::getRingkasanSpek() const {
    return QString("%1 | %2 | %3GB %4 | %5 | %6% sRGB")
        .arg(m_cpu.getSeri())
        .arg(m_display.getTipe())
        .arg(m_ram.getKapasitas())
        .arg(m_ram.getTipe())
        .arg(m_storage.kapasitasLabel() + " " + m_storage.getTipe())
        .arg(m_display.getSrgb());
}
