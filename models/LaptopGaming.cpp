#include "LaptopGaming.h"

LaptopGaming::LaptopGaming(const QString &id, const QString &nama, const QString &brand,
                           const SpekCPU &cpu, const SpekRAM &ram,
                           const SpekPenyimpanan &storage, const SpekDisplay &display,
                           const QString &gpuBrand, const QString &gpuModel,
                           int usbA, bool usbC, bool usbCPD, bool hdmi, bool audioJack,
                           int harga, int stok, const QString &garansi,
                           int tdpWatt, bool hasRgb)
    : Laptop(id, nama, brand, cpu, ram, storage, display,
             gpuBrand, gpuModel, usbA, usbC, usbCPD, hdmi, audioJack,
             harga, stok, garansi),
      m_tdpWatt(tdpWatt), m_hasRgb(hasRgb)
{}

// Override: tampilkan info dengan tambahan gaming specs
QString LaptopGaming::tampilkanInfo() const {
    return QString(
        "Nama    : %1\n"
        "Brand   : %2\n"
        "Tipe    : %3\n"
        "CPU     : %4\n"
        "GPU     : %5 %6\n"
        "RAM     : %7\n"
        "Storage : %8\n"
        "Display : %9\n"
        "Port    : %10\n"
        "TDP     : %11W\n"
        "RGB     : %12\n"
        "Harga   : %13\n"
        "Stok    : %14 unit\n"
        "Garansi : %15"
    )
    .arg(m_nama, m_brand, getTipePerangkat())
    .arg(m_cpu.toDisplayString())
    .arg(m_gpuBrand, m_gpuModel)
    .arg(m_ram.toDisplayString())
    .arg(m_storage.toDisplayString())
    .arg(m_display.toDisplayString())
    .arg(portsString())
    .arg(m_tdpWatt)
    .arg(m_hasRgb ? "Ya" : "Tidak")
    .arg(formatHarga())
    .arg(m_stok)
    .arg(m_garansi);
}

// Override: ringkasan spek menyertakan info GPU & refresh rate
QString LaptopGaming::getRingkasanSpek() const {
    return QString("%1 | %2 %3 | %4GB %5 | %6 | %7Hz")
        .arg(m_cpu.getSeri())
        .arg(m_gpuBrand, m_gpuModel)
        .arg(m_ram.getKapasitas())
        .arg(m_ram.getTipe())
        .arg(m_storage.kapasitasLabel() + " " + m_storage.getTipe())
        .arg(m_display.getRefreshRate());
}
