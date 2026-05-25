#include "SpekPenyimpanan.h"

SpekPenyimpanan::SpekPenyimpanan(const QString &tipe, int kapasitas,
                                 int slots, int slotTersedia)
    : m_tipe(tipe), m_kapasitas(kapasitas),
      m_slots(slots), m_slotTersedia(slotTersedia) {}

QString SpekPenyimpanan::kapasitasLabel() const {
    return m_kapasitas >= 1024
        ? QString("%1TB").arg(m_kapasitas / 1024)
        : QString("%1GB").arg(m_kapasitas);
}

QString SpekPenyimpanan::toDisplayString() const {
    return QString("%1 %2 (%3 slot, %4 tersedia)")
        .arg(kapasitasLabel(), m_tipe)
        .arg(m_slots)
        .arg(m_slotTersedia);
}
