#pragma once
#include <QString>

// =============================================================
//  SpekWireless — Spesifikasi konektivitas nirkabel (Encapsulation)
//  Menyimpan versi Wi-Fi dan Bluetooth.
// =============================================================
class SpekWireless {
public:
    SpekWireless() = default;
    SpekWireless(const QString &wifiVersi, const QString &btVersi);

    QString getWifiVersi()      const { return m_wifiVersi; }
    QString getBluetoothVersi() const { return m_btVersi;   }

    void setWifiVersi     (const QString &v) { m_wifiVersi = v; }
    void setBluetoothVersi(const QString &v) { m_btVersi   = v; }

    // Format: "Wi-Fi 6E, Bluetooth 5.3"
    QString toDisplayString() const;

private:
    QString m_wifiVersi;  // "Wi-Fi 5", "Wi-Fi 6", "Wi-Fi 6E", "Wi-Fi 7"
    QString m_btVersi;    // "4.2", "5.0", "5.1", "5.2", "5.3", "5.4"
};
