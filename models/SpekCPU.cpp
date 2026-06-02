#include "SpekCPU.h"
#include <QStringList>

SpekCPU::SpekCPU(const QString &brand, const QString &seri,
                 int cores, const QString &modelNumber,
                 const QString &maxBoostGHz)
    : m_brand(brand), m_seri(seri), m_cores(cores),
      m_kecepatan(modelNumber), m_maxBoost(maxBoostGHz) {}

QString SpekCPU::toDisplayString() const {
    // Format penuh: "Intel Core i5 1335U (10 cores, 4.6 GHz)"
    QString base = QString("%1 %2 %3").arg(m_brand, m_seri, m_kecepatan);
    if (m_cores > 0 && !m_maxBoost.isEmpty())
        base += QString(" (%1 cores, %2 GHz)").arg(m_cores).arg(m_maxBoost);
    else if (m_cores > 0)
        base += QString(" (%1 cores)").arg(m_cores);
    return base;
}

QString SpekCPU::toShortString() const {
    // Format ringkas untuk tabel: "i5 1335U" atau "R5 5600H"
    QString seriShort;
    if (m_seri.contains("Core i", Qt::CaseInsensitive))
        seriShort = m_seri.mid(m_seri.indexOf("i"));       // "i5", "i7", dst
    else if (m_seri.contains("Ryzen", Qt::CaseInsensitive)) {
        QStringList parts = m_seri.split(" ");
        seriShort = "R" + (parts.size() > 1 ? parts.last() : ""); // "R5", "R7"
    } else
        seriShort = m_seri;

    return m_kecepatan.isEmpty() ? seriShort : seriShort + " " + m_kecepatan;
}
