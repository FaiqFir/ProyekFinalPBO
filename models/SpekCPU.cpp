#include "SpekCPU.h"

SpekCPU::SpekCPU(const QString &brand, const QString &seri,
                 int cores, const QString &kecepatan)
    : m_brand(brand), m_seri(seri), m_cores(cores), m_kecepatan(kecepatan) {}

QString SpekCPU::toDisplayString() const {
    return QString("%1 %2 (%3 cores, %4)")
        .arg(m_brand, m_seri)
        .arg(m_cores)
        .arg(m_kecepatan);
}
