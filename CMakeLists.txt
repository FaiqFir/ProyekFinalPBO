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
    models/Laptop.cpp
    models/LaptopGaming.cpp
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
    models/Laptop.h
    models/LaptopGaming.h
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
        images/acer-aspire-5.jpg
        images/dell-inspiron-15.jpeg
        images/lenovo-thinkpad-x1.jpg
)
