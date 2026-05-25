#include "Laptop.h"
#include <QLocale>

Laptop::Laptop(const QString &id, const QString &nama, const QString &brand,
               const SpekCPU &cpu, const SpekRAM &ram,
               const SpekPenyimpanan &storage, const SpekDisplay &display,
               const QString &gpuBrand, const QString &gpuModel,
               int usbA, bool usbC, bool usbCPD, bool hdmi, bool audioJack,
               int harga, int stok, const QString &garansi)
    : m_cpu(cpu), m_ram(ram), m_storage(storage), m_display(display),
      m_id(id), m_gpuBrand(gpuBrand), m_gpuModel(gpuModel), m_garansi(garansi),
      m_usbA(usbA), m_usbC(usbC), m_usbCPD(usbCPD),
      m_hdmi(hdmi), m_audioJack(audioJack)
{
    m_nama  = nama;
    m_brand = brand;
    m_harga = harga;
    m_stok  = stok;
}

QString Laptop::formatHarga() const {
    return "Rp " + QLocale(QLocale::Indonesian).toString(m_harga);
}

QString Laptop::portsString() const {
    QStringList ports;
    if (m_usbA > 0)    ports << QString("USB-A x%1").arg(m_usbA);
    if (m_usbCPD)      ports << "USB-C (PD)";
    else if (m_usbC)   ports << "USB-C";
    if (m_hdmi)        ports << "HDMI";
    if (m_audioJack)   ports << "Audio Jack";
    return ports.join(", ");
}

QString Laptop::getRingkasanSpek() const {
    return QString("%1 | %2GB %3 | %4 | %5\"")
        .arg(m_cpu.getSeri())
        .arg(m_ram.getKapasitas())
        .arg(m_ram.getTipe())
        .arg(m_storage.kapasitasLabel() + " " + m_storage.getTipe())
        .arg(m_display.getUkuran(), 0, 'f', 1);
}

QString Laptop::tampilkanInfo() const {
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
        "Harga   : %11\n"
        "Stok    : %12 unit\n"
        "Garansi : %13"
    )
    .arg(m_nama, m_brand, getTipePerangkat())
    .arg(m_cpu.toDisplayString())
    .arg(m_gpuBrand, m_gpuModel)
    .arg(m_ram.toDisplayString())
    .arg(m_storage.toDisplayString())
    .arg(m_display.toDisplayString())
    .arg(portsString())
    .arg(formatHarga())
    .arg(m_stok)
    .arg(m_garansi);
}
