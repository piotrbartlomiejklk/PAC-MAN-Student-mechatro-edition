cmake_minimum_required(VERSION 3.16)
project(PAC_MAN_MECHATRO VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Svg)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Svg)

set(PROJECT_SOURCES
    main.cpp
    Okno_Gry.cpp  Okno_gry.h
    Gra.cpp       Gra.h
    Gracz.cpp     Gracz.h
    elementy_gry.cpp  elementy_gry.h
    wrog.cpp      wrog.h
    Przedmiot.cpp Przedmiot.h
    mapa.cpp      Mapa.h
    Aktualizowanie.h
    Rysowanie.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PAC_MAN_MECHATRO MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    add_executable(PAC_MAN_MECHATRO ${PROJECT_SOURCES})
endif()

target_link_libraries(PAC_MAN_MECHATRO PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Svg
)

set_target_properties(PAC_MAN_MECHATRO PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Kopiuj wszystkie zasoby do katalogu build automatycznie po kompilacji
set(ASSETS
    poziom_1.txt
    poziom_2.txt
    kebsik.svg
    studentdebil.svg
    skalniak.svg
    filarnaroznikscianamchtr.svg
    klimatyzacjamchtr.svg
    oknomchtr.svg
)

foreach(ASSET ${ASSETS})
    add_custom_command(TARGET PAC_MAN_MECHATRO POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}"
            "$<TARGET_FILE_DIR:PAC_MAN_MECHATRO>/${ASSET}"
        COMMENT "Kopiowanie ${ASSET}..."
    )
endforeach()

include(GNUInstallDirs)
install(TARGETS PAC_MAN_MECHATRO
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PAC_MAN_MECHATRO)
endif()
