#include "SpekWireless.h"

SpekWireless::SpekWireless(const QString &wifiVersi, const QString &btVersi)
    : m_wifiVersi(wifiVersi), m_btVersi(btVersi) {}

QString SpekWireless::toDisplayString() const {
    return QString("%1, Bluetooth %2").arg(m_wifiVersi, m_btVersi);
}
