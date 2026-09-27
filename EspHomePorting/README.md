# Panigale ESP32-S3 Lighting Control - ESPHome Porting

Porting completo in **ESPHome** del firmware C++ MVC per **ESP32-S3**, con interfaccia web personalizzata ad alto contrasto per il controllo luci (Posizione, Anabbagliante, Abbagliante), Access Point fisso `Panigale-Mel-AP` e risorse grafiche WebP salvate direttamente in Flash locale.

---

## ⚡ Schema Elettrico e Collegamenti Hardware

### 1. Tabella dei Collegamenti GPIO

| Funzione / Carico | Pin ESP32-S3 | Pin Modulo Relè | Tipo Segnale | Comportamento |
| :--- | :--- | :--- | :--- | :--- |
| **Luci di Posizione** | **GPIO 4** | **IN 1** | Digitale OUT (3.3V) | Impulso 500 ms (HIGH -> 500ms -> LOW) |
| **Anabbagliante** | **GPIO 5** | **IN 2** | Digitale OUT (3.3V) | Impulso 500 ms (HIGH -> 500ms -> LOW) |
| **Abbagliante** | **GPIO 6** | **IN 3** | Digitale OUT (3.3V) | Impulso 500 ms (HIGH -> 500ms -> LOW) |
| **Alimentazione Modulo Relè** | **5V (VIN / VBUS)** | **VCC** | Alimentazione 5V DC | Corrente per pilotaggio bobine relè |
| **Massa Comune** | **GND** | **GND** | Riferimento massa 0V | GND comune tra ESP32-S3 e scheda relè |

---

### 2. Schema di Principio dei Collegamenti (ASCII Diagram)

```text
       +---------------------------------------------+
       |                  ESP32-S3                   |
       |                                             |
       |  [GPIO 4] ----------------------------+     |
       |  [GPIO 5] ---------------------+      |     |
       |  [GPIO 6] --------------+      |      |     |
       |                         |      |      |     |
       |  [  5V  ] --------+     |      |      |     |
       |  [ GND  ] ----+   |     |      |      |     |
       +---------------+---+-----+------+------+-----+
                       |   |     |      |      |
                       |   |     |      |      |
        +--------------+   |     |      |      |
        |                  |     |      |      |
        |   +--------------+     |      |      |
        |   |                    |      |      |
       +v---+v-------------------v------v------v-----+
       |   GND    VCC           IN3    IN2    IN1    |
       |                                             |
       |         MODULO RELE' A 3 o 4 CANALI         |
       |                                             |
       |   RELE' 3 (Abbagliante)    NO3 ---+         |
       |                            COM ---|---> +12V / Circuito Abbagliante
       |                                             |
       |   RELE' 2 (Anabbagliante)  NO2 ---+         |
       |                            COM ---|---> +12V / Circuito Anabbagliante
       |                                             |
       |   RELE' 1 (Posizione)      NO1 ---+         |
       |                            COM ---|---> +12V / Circuito Posizione
       +---------------------------------------------+
```

### 3. Significato delle Sigle dei Relè (NO, COM, NC)
Ogni canale del modulo relè dispone di una morsettiera a 3 vie:
* **COM (Common / Comune):** Morsetto centrale comune. Si collega al polo positivo dell'alimentazione della lampada (es. +12V della moto o linea di comando).
* **NO (Normally Open / Normalmente Aperto):** 
  * **NO1, NO2, NO3** indicano rispettivamente il contatto Normalmente Aperto del **Relè 1 (Posizione)**, **Relè 2 (Anabbagliante)** e **Relè 3 (Abbagliante)**.
  * **Funzionamento:** A riposo (relè diseccitato/spento) il contatto tra COM e NO è aperto (circuito interrotto, luce spenta). Quando il relè viene eccitato (scatta), il contatto interno si chiude tra COM e NO, permettendo il passaggio della corrente e accendendo la lampada.
* **NC (Normally Closed / Normalmente Chiuso):** Contatto chiuso a riposo e aperto quando il relè si eccita (non utilizzato per il comando luci standard).

---

### 4. Livelli di Tensione GPIO: Scatto a 0V o 3.3V?

Nel firmware e nel controller attuale:
* **Logica Predefinita nel Codice: Active-HIGH (Scatto a 3.3V)**
  * **Stato di riposo:** Il pin GPIO si trova a **0V (LOW / GND)**.
  * **All'attivazione (impulso 500ms):** Il pin GPIO viene portato a **3.3V (HIGH)** per 500 millisecondi, per poi ritornare a **0V**.
* **Compatibilità Moduli Relè sul Mercato:**
  * **Moduli con jumper H / L:** Impostare il jumper su **H (High Level Trigger)** per scattare a 3.3V.
  * **Moduli solo Active-LOW (scatto a 0V):** Se il modulo relè necessita di **0V (GND)** per scattare (molto comuni i moduli con fotoaccoppiatore optoisolato che conducono a livello basso), è possibile invertire il segnale sia in `smarthome-esps3.yaml` (`inverted: true`) sia nel controller modificando lo stato da LOW ad HIGH.

---

## 📁 Struttura della Cartella `/EspHomePorting`

```text
EspHomePorting/
├── smarthome-esps3.yaml         # Configurazione principale ESPHome (S3, AP Panigale, 3 Switches)
├── glassmorphism.css            # Stile personalizzato con sfondo #020001 e bottoni immagine
├── secrets.yaml                 # Credenziali Wi-Fi e chiave API
├── README.md                    # Questa documentazione con schema elettrico
├── docs/
│   ├── FLASHING_GUIDE.md        # Istruzioni dettagliate per compilazione e flash USB/OTA
│   └── ROUTES_AND_USAGE.md      # Elenco completo delle rotte e utilizzo API
├── img/compresse/               # Immagini sorgente WebP
│   ├── LogoMel.webp
│   ├── Luci_di_posizione.webp
│   ├── Anabbagliante.webp
│   └── Abbagliante.webp
└── src/                         # Architettura MVC in C++
    ├── model/
    │   ├── CustomButtonModel.h
    │   └── CustomButtonModel.cpp # Gestione NVS e mapping GPIO 4, 5, 6
    ├── view/
    │   ├── ImagesData.h         # Array binari immagini WebP salvati in Flash (PROGMEM)
    │   ├── ImagesData.cpp
    │   ├── NavigationManager.h
    │   ├── NavigationManager.cpp # Navbar fissa sul fondo dello schermo (Home e WiFi Config)
    │   ├── ViewConfig.h
    │   ├── ViewConfig.cpp       # Form gestione Wi-Fi con scansione reti
    │   ├── ViewServices.h
    │   └── ViewServices.cpp     # UI 3 Bottoni Immagine 100% puliti senza scritte sovrapposte
    └── controller/
        ├── DnsServerEspIdf.h    # Server DNS nativo ESP-IDF (lwip sockets) per Captive Portal
        ├── DnsServerEspIdf.cpp
        ├── CustomWebController.h
        └── CustomWebController.cpp # Gestione rotte HTTP (/home), invio binario WebP e pulso 500ms
```

---

## 🎨 Caratteristiche dell'Interfaccia Grafica

1. **Sfondo Tema:** Colore esatto `#020001` (nero profondo).
2. **Logo in alto al centro:** `LogoMel.webp` servito direttamente in locale alla rotta `/logo.webp` al naturale, senza effetti di sfumatura o drop-shadow.
3. **I 3 Pulsanti Immagine (100% contenitore):**
   * **Posizione** (`/posizione.webp`): Immagine pulita a tutto spazio, impulso GPIO 4 per 500 ms.
   * **Anabbagliante** (`/anabbagliante.webp`): Immagine pulita a tutto spazio, impulso GPIO 5 per 500 ms.
   * **Abbagliante** (`/abbagliante.webp`): Immagine pulita a tutto spazio, impulso GPIO 6 per 500 ms.
4. **Animazione Visiva a Impulso (500 ms):**
   * Al clic del pulsante, compare l'icona clessidra ⏳ sovrapposta per esattamente **500 ms** in sincronia con l'impulso hardware inviato al relè, per poi scomparire automaticamente.
5. **Navbar Fissa a Bordo Schermo:**
   * Barra di navigazione ancorata al bordo inferiore dello schermo con icona casa **🏠 Home** (`/home`) e icona segnale **📶 WiFi Config** (`/config`).
6. **Captive Portal Automatico alla Home:**
   * Appena ci si connette all'AP `Panigale-Mel-AP`, il sistema reindirizza il popup captive portal direttamente su `http://192.168.4.1/home` (aprendo i comandi luci).
   * La configurazione Wi-Fi non compare più in automatico: è accessibile manualmente premendo su `📶 WiFi Config` nella navbar.
7. **Funzionalità Offline al 100%:**
   * Tutte e 4 le immagini WebP sono memorizzate nella Flash dell'ESP32-S3 e servite localmente con invio binario nativo (`image/webp` con `Content-Length`), senza richiedere connessione ad internet.
8. **Wi-Fi Dual Mode:**
   * L'Access Point **`Panigale-Mel-AP`** rimane sempre attivo e accessibile per connessioni dirette da smartphone.

---

## 🚀 Compilazione e Flash (Da eseguire quando vuoi tu)

Come da tue istruzioni, la compilazione non viene avviata automaticamente. Quando desideri compilare e caricare il firmware sull'ESP32-S3:

```bash
cd /home/none/Arduino/libraries/EspHomePorting
esphome run smarthome-esps3.yaml
```



non va bene, ESP deve stare in pooling sullo stato dei bottoni se no ce un disallineamenteo