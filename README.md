# MaggiClient

MaggiClient ist eine modifizierte Clientbasis für Minecraft 1.8, angelehnt an den Lunar Client. Das Projekt dient primär Lernzwecken und demonstriert, wie man mithilfe von C++ und Java verschiedene Funktionen innerhalb des Spiels erweitern kann.

## Funktionsübersicht

Die wichtigsten Module befinden sich unter `maggiclient/src/base/moduleManager/modules` und sind über das integrierte Menü aktivierbar. Aktuell verfügbare Funktionen sind u. a.:

- **Visual**
  - ESP mit Boxen, Healthbar und Tracern
- **Combat**
  - AimAssist mit anpassbarer FOV und Glättung
  - Reach-Modul zur Veränderung der Angriffsdistanz
- **Clicker**
  - Links- und Rechtsklick-Autoclicker
- **Settings**
  - Allgemeine Einstellungen des Clients

Das Menü wird über die *Insert*-Taste geöffnet und bietet verschiedene Tabs für die oben genannten Kategorien.

## Projektstruktur

```
.
├── asm/               # Java-Patcher auf Basis von ASM
├── maggiclient/       # Hauptcode in C++ (DLL-Projekt)
│   ├── src/           # Quellcode des Clients
│   ├── include/       # Externe Header (z.B. nlohmann/json)
│   └── ext/           # Eingebundene Bibliotheken (ImGui, MinHook, JNI)
├── LICENSE.txt        # MIT-Lizenz
└── README.md          # Diese Datei
```

## Voraussetzungen

- **Windows** mit Visual Studio 2022 (v143 Toolset)
- Eine lokale JDK-Installation für die JNI-Header
- [Maven](https://maven.apache.org/) zum Erstellen des Java-Patchers im Ordner `asm`

## Kompilieren

1. **Java-Patcher bauen**
   ```bash
   cd asm
   mvn package
   ```
   Dadurch entsteht eine JAR-Datei im `target`-Verzeichnis, die von der C++-Komponente geladen wird.

2. **C++-Projekt erstellen**
   
   Das Projekt `maggiclient.sln` kann mit Visual Studio geöffnet werden. Es wird eine DLL erzeugt, die in den Minecraft-Prozess injiziert werden muss. Abhängig von der Konfiguration können Debug- oder Release-Builds für x64 erstellt werden.

## Nutzung

Nach erfolgreichem Build lässt sich die erzeugte DLL in einen laufenden Minecraft 1.8 Client laden. Das Menü kann über *Insert* geöffnet werden. Alle Module lassen sich dort aktivieren und konfigurieren.

**Hinweis:** Der Code ist zu Lern- und Forschungszwecken veröffentlicht. Für etwaige Schäden oder Regelverstöße durch eine unrechtmäßige Nutzung wird keine Haftung übernommen.

## Lizenz

Dieses Projekt steht unter der MIT-Lizenz. Siehe [LICENSE.txt](LICENSE.txt) für weitere Informationen.

## Danksagungen

- [MinHook](https://github.com/TsudaKageyu/minhook) für das Hooking-Framework
- [ImGui](https://github.com/ocornut/imgui) für das Ingame-Menü
- [nlohmann/json](https://github.com/nlohmann/json) zur Verarbeitung von JSON
