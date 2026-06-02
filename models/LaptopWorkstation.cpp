#include "LaptopWorkstation.h"

LaptopWorkstation::LaptopWorkstation(const QString &id, const QString &nama, const QString &brand,
                                     const SpekCPU &cpu, const SpekRAM &ram,
                                     const SpekPenyimpanan &storage, const SpekDisplay &display,
                                     const SpekWireless &wireless,
                                     const QString &gpuBrand, const QString &gpuModel,
                                     int usbA, int usbC, bool usbCPD, int hdmi, int audioJack,
                                     int harga, int stok, const QString &garansi, int batteryWh,
                                     bool isvCertified, bool eccRam)
    : Laptop(id, nama, brand, cpu, ram, storage, display, wireless,
             gpuBrand, gpuModel, usbA, usbC, usbCPD, hdmi, audioJack,
             harga, stok, garansi, batteryWh),
      m_isvCertified(isvCertified), m_eccRam(eccRam)
{}

QString LaptopWorkstation::tampilkanInfo() const {
    return QString(
        "Nama         : %1\n"
        "Brand        : %2\n"
        "Tipe         : %3\n"
        "CPU          : %4\n"
        "GPU          : %5 %6\n"
        "RAM          : %7\n"
        "Storage      : %8\n"
        "Display      : %9\n"
        "ECC RAM      : %10\n"
        "ISV Certified: %11\n"
        "Wireless     : %12\n"
        "Port         : %13\n"
        "Battery      : %14 Wh\n"
        "Harga        : %15\n"
        "Stok         : %16 unit\n"
        "Garansi      : %17"
    )
    .arg(m_nama, m_brand, getTipePerangkat())
    .arg(m_cpu.toDisplayString())
    .arg(m_gpuBrand, m_gpuModel)
    .arg(m_ram.toDisplayString())
    .arg(m_storage.toDisplayString())
    .arg(m_display.toDisplayString())
    .arg(m_eccRam       ? "Ya" : "Tidak")
    .arg(m_isvCertified ? "Ya" : "Tidak")
    .arg(m_wireless.toDisplayString())
    .arg(portsString())
    .arg(m_batteryWh)
    .arg(formatHarga())
    .arg(m_stok)
    .arg(m_garansi);
}

QString LaptopWorkstation::getRingkasanSpek() const {
    return QString("%1 | %2 %3 | %4GB %5 | %6 | %7")
        .arg(m_cpu.getSeri())
        .arg(m_gpuBrand, m_gpuModel)
        .arg(m_ram.getKapasitas())
        .arg(m_ram.getTipe())
        .arg(m_storage.kapasitasLabel() + " " + m_storage.getTipe())
        .arg(m_isvCertified ? "ISV Certified" : "Workstation");
}
