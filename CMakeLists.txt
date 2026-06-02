cmake_minimum_required(VERSION 3.16)
project(LaptopCatalog VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    main.cpp
    models/SpekCPU.cpp
    models/SpekRAM.cpp
    models/SpekPenyimpanan.cpp
    models/SpekDisplay.cpp
    models/SpekWireless.cpp
    models/Laptop.cpp
    models/LaptopGaming.cpp
    models/LaptopDesain.cpp
    models/LaptopWorkstation.cpp
    models/ManajerKatalog.cpp
    views/MainWindow.cpp
    views/CatalogPage.cpp
    views/AdminPage.cpp
    views/DetailDialog.cpp
    views/CompareDialog.cpp
    views/StockDialog.cpp
    views/LoginDialog.cpp
)

set(HEADERS
    models/PerangkatKeras.h
    models/SpekCPU.h
    models/SpekRAM.h
    models/SpekPenyimpanan.h
    models/SpekDisplay.h
    models/SpekWireless.h
    models/Laptop.h
    models/LaptopGaming.h
    models/LaptopDesain.h
    models/LaptopWorkstation.h
    models/ManajerKatalog.h
    views/MainWindow.h
    views/CatalogPage.h
    views/AdminPage.h
    views/DetailDialog.h
    views/CompareDialog.h
    views/StockDialog.h
    views/LoginDialog.h
)

qt_add_executable(LaptopCatalog
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(LaptopCatalog PRIVATE Qt6::Widgets)

# ── Resource gambar produk ────────────────────────────────────────────────────
qt_add_resources(LaptopCatalog "produk_images"
    PREFIX "/"
    FILES
        images/check_white.png
        images/acer-aspire-5.png
        images/asus-proart-studiobook-16.png
        images/asus-rog-strix-g17.png
        images/dell-inspiron-15.png
        images/dell-precision-5680.png
        images/dell-xps-13.png
        images/dell-xps-15-9530.png
        images/gigabyte-aorus-17.png
        images/hp-envy-x360-13.png
        images/hp-pavilion-gaming-15.png
        images/hp-zbook-studio-g10.png
        images/lenovo-legion-5i.png
        images/lenovo-thinkpad-x1.png
        images/lenovo-thinkpad-x1-extreme.png
        images/lg-gram-style.png
        images/microsoft-surface-laptop-studio-2.png
        images/msi-creatorpro-Z17.png
        images/msi-katana-gf66.png
        images/razer-blade-15.png
        images/samsung-galaxy-book3.png
        images/logo-silap.png
)
