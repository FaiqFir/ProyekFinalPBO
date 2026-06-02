#include "Laptop.h"
#include <QLocale>

Laptop::Laptop(const QString &id, const QString &nama, const QString &brand,
               const SpekCPU &cpu, const SpekRAM &ram,
               const SpekPenyimpanan &storage, const SpekDisplay &display,
               const SpekWireless &wireless,
               const QString &gpuBrand, const QString &gpuModel,
               int usbA, int usbC, bool usbCPD, int hdmi, int audioJack,
               int harga, int stok, const QString &garansi, int batteryWh)
    : m_cpu(cpu), m_ram(ram), m_storage(storage), m_display(display),
      m_wireless(wireless),
      m_id(id), m_gpuBrand(gpuBrand), m_gpuModel(gpuModel), m_garansi(garansi),
      m_usbA(usbA), m_usbC(usbC), m_usbCPD(usbCPD),
      m_hdmi(hdmi), m_audioJack(audioJack),
      m_batteryWh(batteryWh)
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
    if (m_usbC > 0) {
        QString s = QString("USB-C x%1").arg(m_usbC);
        if (m_usbCPD) s += " (PD)";
        ports << s;
    }
    if (m_hdmi > 0)      ports << QString("HDMI x%1").arg(m_hdmi);
    if (m_audioJack > 0) ports << QString("Audio Jack x%1").arg(m_audioJack);
    return ports.join(", ");
}

QString Laptop::getRingkasanSpek() const {
    return QString("%1 | %2GB %3 | %4 | %5\"")
        .arg(m_cpu.toShortString())
        .arg(m_ram.getKapasitas())
        .arg(m_ram.getTipe())
        .arg(m_storage.kapasitasLabel() + " " + m_storage.getTipe())
        .arg(m_display.getUkuran(), 0, 'f', 1);
}

QString Laptop::tampilkanInfo() const {
    return QString(
        "Nama     : %1\n"
        "Brand    : %2\n"
        "Tipe     : %3\n"
        "CPU      : %4\n"
        "GPU      : %5 %6\n"
        "RAM      : %7\n"
        "Storage  : %8\n"
        "Display  : %9\n"
        "Wireless : %10\n"
        "Port     : %11\n"
        "Battery  : %12 Wh\n"
        "Harga    : %13\n"
        "Stok     : %14 unit\n"
        "Garansi  : %15"
    )
    .arg(m_nama, m_brand, getTipePerangkat())
    .arg(m_cpu.toDisplayString())
    .arg(m_gpuBrand, m_gpuModel)
    .arg(m_ram.toDisplayString())
    .arg(m_storage.toDisplayString())
    .arg(m_display.toDisplayString())
    .arg(m_wireless.toDisplayString())
    .arg(portsString())
    .arg(m_batteryWh)
    .arg(formatHarga())
    .arg(m_stok)
    .arg(m_garansi);
}
