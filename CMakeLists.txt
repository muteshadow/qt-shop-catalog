cmake_minimum_required(VERSION 3.16)

project(ShopCatalog VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# Пошук .ui файлів
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

# Пошук БД
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

# Пошук assets
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# Файли вихідного коду
set(SOURCES
    src/main.cpp

    # Windows
    src/windows/mainwindow.cpp
    src/windows/catalogwindow.cpp
    src/windows/cartwindow.cpp
    src/windows/favoriteswindow.cpp

    # Widgets
    src/widgets/productcard.cpp
    src/widgets/productdetails.cpp
    src/widgets/producteditor.cpp

    # Database
    src/database/database_manager.cpp
)

set(HEADERS
    # Windows
    include/windows/mainwindow.h
    include/windows/catalogwindow.h
    include/windows/cartwindow.h
    include/windows/favoriteswindow.h

    # Widgets
    include/widgets/productcard.h
    include/widgets/productdetails.h
    include/widgets/producteditor.h
    include/widgets/savedpopup.h

    # Database
    include/database/database_manager.h
)

set(UI
    ${CMAKE_SOURCE_DIR}/ui/mainwindow.ui
)

set(RESOURCES
    resources/resources.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ShopCatalog
        MANUAL_FINALIZATION
        ${SOURCES}
        ${HEADERS}
        ${UI}
        ${RESOURCES}
    )
else()
    add_executable(ShopCatalog
        ${SOURCES}
        ${HEADERS}
        ${UI}
        ${RESOURCES}
    )
endif()

# Include dirs
target_include_directories(ShopCatalog PRIVATE
    include
)

# Підключення Qt бібліотек
target_link_libraries(ShopCatalog PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Властивості для Windows і Mac
set_target_properties(ShopCatalog PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Інсталяція
include(GNUInstallDirs)
install(TARGETS ShopCatalog
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Qt6 фіналізація
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ShopCatalog)
endif()
