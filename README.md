# Kalkulator CRC-16 Modbus

Samodzielna aplikacja do obliczania sum kontrolnych Modbus CRC-16 oraz testowania wydajności algorytmu CRC.

## Funkcje

- Obliczanie sum kontrolnych Modbus CRC-16 dla danych w formacie szesnastkowym
- Testowanie wydajności obliczeń CRC z możliwością ustawienia liczby powtórzeń
- Wielowątkowe obliczenia dla dużych testów wydajności
- Obsługa danych szesnastkowych zarówno z odstępami, jak i bez
- Zoptymalizowany algorytm CRC z wykorzystaniem tablic
- Kopiowanie wyników do schowka jednym kliknięciem
- Samodzielny plik wykonywalny bez zewnętrznych zależności

## Kompilacja ze źródeł

### Wymagania wstępne

- CMake 3.16 lub nowszy
- Qt 6.4.3 lub nowszy
- Kompilator zgodny z C++20 (dla Windows zalecany MSVC 2019 lub nowszy)

### Kroki kompilacji

1. Klonowanie repozytorium:
   ```
   git clone https://github.com/twojnazwauzytkownika/ModbusCrc.git
   cd ModbusCrc
   ```

2. Utworzenie katalogu kompilacji:
   ```
   mkdir build
   cd build
   ```

3. Konfiguracja z CMake:
   ```
   cmake .. -DCMAKE_BUILD_TYPE=Release
   ```

4. Kompilacja projektu:
   ```
   cmake --build . --config Release
   ```

5. Instalacja lub utworzenie samodzielnego pakietu:
   ```
   cmake --install . --prefix ./install
   ```

## Automatyczna kompilacja

Projekt zawiera workflow GitHub Actions, który automatycznie kompiluje samodzielny plik wykonywalny dla Windows. Workflow:

1. Konfiguruje środowisko kompilacji z Qt 6.4.3
2. Kompiluje aplikację w trybie Release
3. Tworzy samodzielny pakiet ze wszystkimi potrzebnymi zależnościami
4. Przesyła pakiet jako artefakt kompilacji

## Użytkowanie

1. Wprowadź dane szesnastkowe w polu "Bajty ramki" (z odstępami lub bez)
2. Określ liczbę powtórzeń dla testu wydajności w polu "Liczba powtórzeń"
3. Kliknij "START" aby obliczyć CRC i zmierzyć wydajność
4. Zobacz wyniki w panelu informacyjnym
5. Skopiuj wartość CRC do schowka używając przycisku kopiowania lub menu kontekstowego

## Opis projektu

Projekt realizuje kalkulator sumy kontrolnej CRC-16 używanej w protokole Modbus. Aplikacja pozwala na obliczanie sum kontrolnych dla ciągów bajtów wprowadzonych w formacie szesnastkowym oraz na testowanie wydajności algorytmu poprzez wielokrotne wykonanie obliczeń.

Główne komponenty projektu:
- Interfejs użytkownika zbudowany z wykorzystaniem biblioteki Qt
- Wielowątkowy algorytm obliczeń CRC z optymalizacją wydajności
- Parser danych szesnastkowych obsługujący różne formaty wejściowe
- System budowania i pakowania oparty na CMake

## Licencja

Projekt udostępniony na licencji MIT - szczegóły w pliku LICENSE.