#pragma once
#include <QString>

// =============================================================
//  SpekCPU — Menyimpan spesifikasi prosesor (Encapsulation)
// =============================================================
class SpekCPU {
public:
    SpekCPU() = default;
    SpekCPU(const QString &brand, const QString &seri,
            int cores, const QString &modelNumber,
            const QString &maxBoostGHz = "");

    // Getters
    QString getBrand()       const { return m_brand;       }
    QString getSeri()        const { return m_seri;        }
    int     getCores()       const { return m_cores;       }
    QString getKecepatan()   const { return m_kecepatan;   } // model number
    QString getMaxBoost()    const { return m_maxBoost;    }

    // Setters
    void setBrand      (const QString &v) { m_brand     = v; }
    void setSeri       (const QString &v) { m_seri      = v; }
    void setCores      (int v)            { m_cores     = v; }
    void setKecepatan  (const QString &v) { m_kecepatan = v; }
    void setMaxBoost   (const QString &v) { m_maxBoost  = v; }

    // Format penuh: "Intel Core i5 1335U (10 cores, 4.6 GHz)"
    QString toDisplayString() const;
    // Format ringkas: "i5 1335U" atau "R5 5600H"
    QString toShortString()   const;

private:
    QString m_brand;
    QString m_seri;
    int     m_cores    = 4;
    QString m_kecepatan;  // model number, e.g. "1335U"
    QString m_maxBoost;   // max boost GHz, e.g. "4.6"
};
