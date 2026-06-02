#pragma once
#include <QString>

// =============================================================
//  SpekDisplay — Layar laptop (Encapsulation)
// =============================================================
class SpekDisplay {
public:
    SpekDisplay() = default;
    SpekDisplay(double ukuran, const QString &tipe,
                const QString &resolusi, int refreshRate, int srgb = 45);

    double  getUkuran()     const { return m_ukuran;     }
    QString getTipe()       const { return m_tipe;       }
    QString getResolusi()   const { return m_resolusi;   }
    int     getRefreshRate()const { return m_refreshRate;}
    int     getSrgb()       const { return m_srgb;       }

    void setUkuran     (double v)         { m_ukuran      = v; }
    void setTipe       (const QString &v) { m_tipe        = v; }
    void setResolusi   (const QString &v) { m_resolusi    = v; }
    void setRefreshRate(int v)            { m_refreshRate = v; }
    void setSrgb       (int v)            { m_srgb        = v; }

    QString toDisplayString() const;

private:
    double  m_ukuran      = 15.6;
    QString m_tipe;
    QString m_resolusi;
    int     m_refreshRate = 60;
    int     m_srgb        = 45;   // persentase sRGB, default IPS biasa ~45%
};
