#pragma once
#include <QString>

// =============================================================
//  SpekRAM — Menyimpan spesifikasi memori (Encapsulation)
// =============================================================
class SpekRAM {
public:
    SpekRAM() = default;
    SpekRAM(const QString &tipe, int kapasitas, int slots, int slotTersedia);

    QString getTipe()         const { return m_tipe;         }
    int     getKapasitas()    const { return m_kapasitas;    }
    int     getSlots()        const { return m_slots;        }
    int     getSlotTersedia() const { return m_slotTersedia; }

    void setTipe        (const QString &v) { m_tipe         = v; }
    void setKapasitas   (int v)            { m_kapasitas    = v; }
    void setSlots       (int v)            { m_slots        = v; }
    void setSlotTersedia(int v)            { m_slotTersedia = v; }

    QString toDisplayString() const;

private:
    QString m_tipe;
    int     m_kapasitas    = 8;
    int     m_slots        = 2;
    int     m_slotTersedia = 1;
};
