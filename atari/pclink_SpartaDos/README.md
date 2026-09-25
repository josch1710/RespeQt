# SpartaDOS PCLink Tools & Test Suite

Dieses Verzeichnis enthält Atari 8-Bit Programme für **SpartaDOS / SpartaDOS X** zur Interaktion mit und Verifikation des **PCLink SIO-Protokolls** (Device ID `$6F`) in RespeQt.

---

## 1. PCLTEST (`pcltest.asm` / `PCLTEST.COM`) - Unit-Test Gegenstück

`PCLTEST` ist das direkte Atari-seitige Gegenstück zur C++/QtTest Test-Suite (`PclinkTest` / `tests/units/pclinktest.cpp`). Es führt eine vollständige, automatisierte End-to-End Protokollverifikation über die SIO-Schnittstelle aus und testet sämtliche PCLink-Funktionen:

1. **TEST 1: SIO Speed Poll (`?`)**: Aushandlung und Auslesen des Pokey-Divisors.
2. **TEST 2: Server Init (`Init`, $08)**: Zurücksetzen aller Server-Slots.
3. **TEST 3: DFree Volume Info (`DFree`, $13)**: Auslesen des Volume-Labels und der Sektorkapazität.
4. **TEST 4: Get Working Directory (`GetCwd`, $11)**: Abfrage des gemounteten Pfads.
5. **TEST 5: Create & Remove Directory (`Mkdir` $0E, `Rmdir` $0F)**: Verzeichnisanlage und -entfernung.
6. **TEST 6: Create, Write & Close File (`FOpen` $09 mode $08, `FWrite` $01, `FClose` $07)**: Dateierstellung und Datentransfer.
7. **TEST 7: Open, Read & Verify (`FOpen` $09 mode $04, `FRead` $00, `FClose` $07)**: Dateiauslesen und Inhaltsabgleich mit Test-Payload.
8. **TEST 8: FLen, FSeek & FTell (`FLen` $04, `FSeek` $02, `FTell` $03)**: Dateigröße, Positionierung und Positionsabfrage.
9. **TEST 9: Directory Enumeration (`FFirst` $0A, `FNEXT` $06, `FClose` $07)**: Durchsuchen von Verzeichniseinträgen per Wildcard.
10. **TEST 10: Rename & Remove File (`Rename` $0B, `Remove` $0C)**: Umbenennen und Löschen von Dateien.
11. **TEST 11: Error Handling (`ErrFileNotFound` 170, `ErrBadHandle` 134)**: Validierung von Fehlerzuständen und Statuscodes.

### Aufruf
```
PCLTEST [unit]
```
*(Standardmäßig wird Unit 1 getestet, z. B. `PCLTEST 1`)*

---

## 2. PCLINK (`pclink.asm` / `PCLINK.COM`) - CLI Utility

Ein interaktives Befehlszeilenwerkzeug zur manuellen Steuerung und Diagnose:

- `PCLINK PROBE` - Verbindungs- und Geschwindigkeitsdiagnose.
- `PCLINK INIT` - Server-Initialisierung.
- `PCLINK DFREE <unit>` - Datenträgerinformationen und freier Speicher.
- `PCLINK CWD <unit>` - Aktuellen Pfad anzeigen.
- `PCLINK CD <unit> <pfad>` - Arbeitsverzeichnis wechseln.
- `PCLINK MD <unit> <ordner>` - Verzeichnis erstellen.
- `PCLINK RD <unit> <ordner>` - Verzeichnis löschen.

---

## Bauen mit CMake

Die Programme können automatisiert über **CMake** und den **MADS Assembler** (Mad-Assembler) assembliert werden.

### 1. Als Teil des RespeQt-Gesamtprojekts
Im Gesamtprojekt ist das Bauen der Atari-Binärdateien standardmäßig deaktiviert (`BUILD_ATARI_EXECUTABLES=OFF`). Um sie zu aktivieren, muss die Option `-DBUILD_ATARI_EXECUTABLES=ON` gesetzt werden:

```bash
cmake -B build -S . -DBUILD_ATARI_EXECUTABLES=ON -DMADS_EXECUTABLE=/pfad/zu/mads
cmake --build build --target pclink_atari_all
```

### 2. Standalone-Build
Das Verzeichnis `atari/pclink_SpartaDos` kann auch als eigenständiges CMake-Projekt konfiguriert und gebaut werden:

```bash
cd atari/pclink_SpartaDos
cmake -B build -S . -DMADS_EXECUTABLE=/pfad/zu/mads
cmake --build build
```

Generierte Binärdateien:
- `build/pclink.com`
- `build/pcltest.com`

---

## Direktes Assemblieren (CLI)

Alternativ kann MADS direkt über die Kommandozeile aufgerufen werden:

```bash
mads pcltest.asm -o:pcltest.com
mads pclink.asm -o:pclink.com
```
