#pragma once
#include <QString>

// =============================================================
//  SpekPenyimpanan — HDD/SSD (Encapsulation)
// =============================================================
class SpekPenyimpanan {
public:
    SpekPenyimpanan() = default;
    SpekPenyimpanan(const QString &tipe, int kapasitas, int slots, int slotTersedia);

    QString getTipe()         const { return m_tipe;         }
    int     getKapasitas()    const { return m_kapasitas;    }
    int     getSlots()        const { return m_slots;        }
    int     getSlotTersedia() const { return m_slotTersedia; }

    void setTipe        (const QString &v) { m_tipe         = v; }
    void setKapasitas   (int v)            { m_kapasitas    = v; }
    void setSlots       (int v)            { m_slots        = v; }
    void setSlotTersedia(int v)            { m_slotTersedia = v; }

    QString toDisplayString() const;
    QString kapasitasLabel()  const;

private:
    QString m_tipe;
    int     m_kapasitas    = 256;
    int     m_slots        = 1;
    int     m_slotTersedia = 0;
};
