#pragma once
#include <QString>

// =============================================================
//  SpekDisplay — Layar laptop (Encapsulation)
// =============================================================
class SpekDisplay {
public:
    SpekDisplay() = default;
    SpekDisplay(double ukuran, const QString &tipe,
                const QString &resolusi, int refreshRate);

    double  getUkuran()     const { return m_ukuran;     }
    QString getTipe()       const { return m_tipe;       }
    QString getResolusi()   const { return m_resolusi;   }
    int     getRefreshRate()const { return m_refreshRate;}

    void setUkuran     (double v)          { m_ukuran      = v; }
    void setTipe       (const QString &v)  { m_tipe        = v; }
    void setResolusi   (const QString &v)  { m_resolusi    = v; }
    void setRefreshRate(int v)             { m_refreshRate = v; }

    QString toDisplayString() const;

private:
    double  m_ukuran      = 15.6;
    QString m_tipe;
    QString m_resolusi;
    int     m_refreshRate = 60;
};
