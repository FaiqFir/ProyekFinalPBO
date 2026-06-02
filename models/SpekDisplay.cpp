#include "SpekDisplay.h"

SpekDisplay::SpekDisplay(double ukuran, const QString &tipe,
                         const QString &resolusi, int refreshRate, int srgb)
    : m_ukuran(ukuran), m_tipe(tipe),
      m_resolusi(resolusi), m_refreshRate(refreshRate), m_srgb(srgb) {}

QString SpekDisplay::toDisplayString() const {
    return QString("%1\" %2 %3 @%4Hz | %5% sRGB")
        .arg(m_ukuran, 0, 'f', 1)
        .arg(m_tipe, m_resolusi)
        .arg(m_refreshRate)
        .arg(m_srgb);
}
