#pragma once
#include "PerangkatKeras.h"
#include "SpekCPU.h"
#include "SpekRAM.h"
#include "SpekPenyimpanan.h"
#include "SpekDisplay.h"
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
           const QString &gpuBrand, const QString &gpuModel,
           int usbA, bool usbC, bool usbCPD, bool hdmi, bool audioJack,
           int harga, int stok, const QString &garansi);

    // === Override pure virtual (Polymorphism) ===
    QString tampilkanInfo()    const override;
    QString getRingkasanSpek() const override;
    QString getTipePerangkat() const override { return "Laptop"; }

    // === Getters komponen spek ===
    const SpekCPU&         getCPU()     const { return m_cpu;     }
    const SpekRAM&         getRAM()     const { return m_ram;     }
    const SpekPenyimpanan& getStorage() const { return m_storage; }
    const SpekDisplay&     getDisplay() const { return m_display; }

    QString getGpuBrand()  const { return m_gpuBrand;  }
    QString getGpuModel()  const { return m_gpuModel;  }
    QString getGaransi()   const { return m_garansi;   }
    QString getId()        const { return m_id;        }
    int     getUsbA()      const { return m_usbA;      }
    bool    hasUsbC()      const { return m_usbC;      }
    bool    hasUsbCPD()    const { return m_usbCPD;    }
    bool    hasHdmi()      const { return m_hdmi;      }
    bool    hasAudioJack() const { return m_audioJack; }

    // === Setters ===
    void setCPU    (const SpekCPU &v)         { m_cpu     = v; }
    void setRAM    (const SpekRAM &v)         { m_ram     = v; }
    void setStorage(const SpekPenyimpanan &v) { m_storage = v; }
    void setDisplay(const SpekDisplay &v)     { m_display = v; }
    void setGpuBrand (const QString &v)       { m_gpuBrand  = v; }
    void setGpuModel (const QString &v)       { m_gpuModel  = v; }
    void setGaransi  (const QString &v)       { m_garansi   = v; }
    void setId       (const QString &v)       { m_id        = v; }
    void setUsbA     (int v)                  { m_usbA      = v; }
    void setUsbC     (bool v)                 { m_usbC      = v; }
    void setUsbCPD   (bool v)                 { m_usbCPD    = v; }
    void setHdmi     (bool v)                 { m_hdmi      = v; }
    void setAudioJack(bool v)                 { m_audioJack = v; }

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

    QString m_id;
    QString m_gpuBrand;
    QString m_gpuModel;
    QString m_garansi;

    // Port
    int  m_usbA      = 2;
    bool m_usbC      = true;
    bool m_usbCPD    = false;
    bool m_hdmi      = true;
    bool m_audioJack = true;

    QStringList m_imagePaths;  // path ke file gambar produk
};
