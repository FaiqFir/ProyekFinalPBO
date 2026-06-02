#include "SpekRAM.h"

SpekRAM::SpekRAM(const QString &tipe, int kapasitas, int slots, int slotTersedia)
    : m_tipe(tipe), m_kapasitas(kapasitas),
      m_slots(slots), m_slotTersedia(slotTersedia) {}

QString SpekRAM::toDisplayString() const {
    return QString("%1GB %2 (%3 slot, %4 tersedia)")
        .arg(m_kapasitas)
        .arg(m_tipe)
        .arg(m_slots)
        .arg(m_slotTersedia);
}
