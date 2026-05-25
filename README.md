[README.md](https://github.com/user-attachments/files/28234946/README.md)
# 🎓 Student w Labiryncie

> Projekt zespołowy na przedmiot **Programowanie 2: Programowanie Obiektowe w C++**

Gra inspirowana klasykiem PacMan, osadzona w realiach studenckiego życia. Wcielasz się w studenta przemierzającego labirynt zbudowany z bloków Minecrafta — zbierasz kebaby i talerze z kurczakiem, uciekając przed kamiennymi potworami. Obsługuje multiplayer przez sieć lokalną.

---

## 🕹️ Opis gry

- **Gracz** — pikselowa postać studenta sterowana klawiaturą
- **Wrogowie** — kamienne potwory poruszające się po labiryncie (AI oparte na BFS lub losowym ruchu)
- **Labirynt** — mapa zbudowana z tekstur Minecraft (Vanilla resource pack)
- **Punkty** — zbieranie kebabów i talerzy z chrupiącym kurczakiem
- **Tryb gry** — multiplayer przez sieć lokalną (2+ graczy), z możliwością trybu hot-seat

---

## 🛠️ Technologie i biblioteki

| Kategoria | Technologia |
|---|---|
| Język | C++17 |
| Interfejs graficzny | Qt6 / SDL2 |
| System budowania | CMake |
| Komunikacja sieciowa | TCP/UDP (multiplayer LAN) |
| Dokumentacja | Doxygen |
| Kontrola wersji | Git (gałęzie + pull requesty) |
| Tekstury | Minecraft Vanilla resource pack (darmowy) |

---

## 🏗️ Architektura projektu

Projekt podzielony na trzy moduły, rozwijane przez poszczególnych członków zespołu:

### Silnik gry (`Map`, `Tile`, `GameSession`)
- Ładowanie poziomów z pliku (format `.txt` / `.json`)
- Obsługa kolizji
- Pętla gry i zarządzanie stanem
- Pełna obsługa wyjątków, zapis/odczyt stanu

### Encje i sieć (`Entity`, `Player`, `Enemy`)
- Ruch gracza, zbieranie punktów
- AI przeciwników (BFS / ruch losowy)
- Protokół sieciowy, multiplayer 2+ graczy przez LAN

### Interfejs graficzny (Qt6/SDL2)
- Renderer mapy i encji z teksturami Minecraft
- Menu główne, ekran gry, ekran końca rundy
- HUD: liczba żyć, aktualny wynik
- Animacje postaci

---

## 📁 Struktura repozytorium

```
student-w-labiryncie/
├── src/
│   ├── engine/        # Map, Tile, GameSession
│   ├── entities/      # Entity, Player, Enemy
│   └── gui/           # Qt6/SDL2 renderer, menu, HUD
├── levels/            # Pliki map (.txt / .json)
├── assets/            # Tekstury Minecraft
├── docs/              # Dokumentacja Doxygen
├── tests/             # Testy jednostkowe
└── CMakeLists.txt
```

---

## ▶️ Uruchomienie

```bash
git clone https://github.com/<repo>/student-w-labiryncie
cd student-w-labiryncie
cmake -B build
cmake --build build
./build/StudentWLabiryncie
```

> **Wymagania:** Qt6 lub SDL2, CMake 3.16+, kompilator C++17

---

## 👥 Zespół

| Osoba | Moduł |
|---|---|
| Piotr Kluk | Silnik gry, Encje i sieć |
| Mateusz Wróblewski | Silnik gry, Encje i sieć |
| Jakub Pietruszka | Interfejs graficzny |

---

## 📋 Zasady pracy

- Każda osoba pracuje na osobnej gałęzi Git; scalanie przez pull requesty
- Komentarze Doxygen (`/** */`) pisane na bieżąco
- Rdzeń gry działa niezależnie od Qt/SDL (tryb konsolowy do testów)
- Każdy członek zespołu zna cały kod (wymóg prowadzącego)
