#pragma once
#include "PerangkatKeras.h"
#include "SpekCPU.h"
#include "SpekRAM.h"
#include "SpekPenyimpanan.h"
#include "SpekDisplay.h"
#include "SpekWireless.h"
#include <QString>
#include <QStringList>

// =============================================================
//  Laptop — Turunan konkret dari PerangkatKeras
//  Menerapkan Inheritance + Composition (memiliki objek Spek*)
// =============================================================
class Laptop : public PerangkatKeras {
public:
    Laptop() = default;
    Laptop(const QString &id, const QString &nama, const QString &brand,
           const SpekCPU &cpu, const SpekRAM &ram,
           const SpekPenyimpanan &storage, const SpekDisplay &display,
           const SpekWireless &wireless,
           const QString &gpuBrand, const QString &gpuModel,
           int usbA, int usbC, bool usbCPD, int hdmi, int audioJack,
           int harga, int stok, const QString &garansi, int batteryWh);

    // === Override pure virtual (Polymorphism) ===
    QString tampilkanInfo()    const override;
    QString getRingkasanSpek() const override;
    QString getTipePerangkat() const override { return "Laptop"; }

    // === Getters komponen spek ===
    const SpekCPU&         getCPU()      const { return m_cpu;      }
    const SpekRAM&         getRAM()      const { return m_ram;      }
    const SpekPenyimpanan& getStorage()  const { return m_storage;  }
    const SpekDisplay&     getDisplay()  const { return m_display;  }
    const SpekWireless&    getWireless() const { return m_wireless; }

    QString getGpuBrand()  const { return m_gpuBrand;  }
    QString getGpuModel()  const { return m_gpuModel;  }
    QString getGaransi()   const { return m_garansi;   }
    QString getId()        const { return m_id;        }
    int     getUsbA()      const { return m_usbA;      }
    int     getUsbC()      const { return m_usbC;      }
    bool    hasUsbC()      const { return m_usbC > 0;  }
    bool    hasUsbCPD()    const { return m_usbCPD;    }
    int     getHdmi()      const { return m_hdmi;      }
    bool    hasHdmi()      const { return m_hdmi > 0;  }
    int     getAudioJack() const { return m_audioJack; }
    bool    hasAudioJack() const { return m_audioJack > 0; }
    int     getBatteryWh() const { return m_batteryWh; }

    // === Setters ===
    void setCPU     (const SpekCPU &v)         { m_cpu      = v; }
    void setRAM     (const SpekRAM &v)         { m_ram      = v; }
    void setStorage (const SpekPenyimpanan &v) { m_storage  = v; }
    void setDisplay (const SpekDisplay &v)     { m_display  = v; }
    void setWireless(const SpekWireless &v)    { m_wireless = v; }
    void setGpuBrand (const QString &v)        { m_gpuBrand  = v; }
    void setGpuModel (const QString &v)        { m_gpuModel  = v; }
    void setGaransi  (const QString &v)        { m_garansi   = v; }
    void setId       (const QString &v)        { m_id        = v; }
    void setUsbA     (int v)                   { m_usbA      = v; }
    void setUsbC     (int v)                   { m_usbC      = v; }
    void setUsbCPD   (bool v)                  { m_usbCPD    = v; }
    void setHdmi     (int v)                   { m_hdmi      = v; }
    void setAudioJack(int v)                   { m_audioJack = v; }
    void setBatteryWh(int v)                   { m_batteryWh = v; }

    // === Gambar produk ===
    QStringList getImagePaths()       const { return m_imagePaths; }
    void setImagePaths(const QStringList &v)  { m_imagePaths = v; }
    void addImagePath (const QString   &v)   { m_imagePaths << v; }

    QString formatHarga() const;
    QString portsString() const;

protected:
    // Komponen spesifikasi (Composition)
    SpekCPU         m_cpu;
    SpekRAM         m_ram;
    SpekPenyimpanan m_storage;
    SpekDisplay     m_display;
    SpekWireless    m_wireless;

    QString m_id;
    QString m_gpuBrand;
    QString m_gpuModel;
    QString m_garansi;

    // Port (semua int = jumlah, kecuali USB-C PD = bool fitur)
    int  m_usbA      = 2;
    int  m_usbC      = 1;
    bool m_usbCPD    = false;
    int  m_hdmi      = 1;
    int  m_audioJack = 1;

    // Baterai (Wh)
    int  m_batteryWh = 50;

    QStringList m_imagePaths;  // path ke file gambar produk
};
