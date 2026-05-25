#include "SpekDisplay.h"

SpekDisplay::SpekDisplay(double ukuran, const QString &tipe,
                         const QString &resolusi, int refreshRate)
    : m_ukuran(ukuran), m_tipe(tipe),
      m_resolusi(resolusi), m_refreshRate(refreshRate) {}

QString SpekDisplay::toDisplayString() const {
    return QString("%1\" %2 %3 @%4Hz")
        .arg(m_ukuran, 0, 'f', 1)
        .arg(m_tipe, m_resolusi)
        .arg(m_refreshRate);
}
