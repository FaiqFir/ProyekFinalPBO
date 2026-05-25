#pragma once
#include <QString>

// =============================================================
//  SpekCPU — Menyimpan spesifikasi prosesor (Encapsulation)
// =============================================================
class SpekCPU {
public:
    SpekCPU() = default;
    SpekCPU(const QString &brand, const QString &seri,
            int cores, const QString &kecepatan);

    // Getters
    QString getBrand()     const { return m_brand;     }
    QString getSeri()      const { return m_seri;      }
    int     getCores()     const { return m_cores;     }
    QString getKecepatan() const { return m_kecepatan; }

    // Setters
    void setBrand    (const QString &v) { m_brand     = v; }
    void setSeri     (const QString &v) { m_seri      = v; }
    void setCores    (int v)            { m_cores     = v; }
    void setKecepatan(const QString &v) { m_kecepatan = v; }

    QString toDisplayString() const;

private:
    QString m_brand;
    QString m_seri;
    int     m_cores     = 4;
    QString m_kecepatan;
};
