# Panigale ESP32-S3 Lighting Control - ESPHome Porting

Porting completo in **ESPHome** con architettura C++ MVC per **ESP32-S3**, con interfaccia web ad alto contrasto per il controllo di 2 lampade a 12V (Posizione e Anabbagliante), Access Point fisso `Panigale-Mel-AP`, gestione pulsanti fisici bistabili con anti-rimbalzo non bloccante e sincronizzazione bidirezionale in tempo reale.

---

## ⚡ Schema Elettrico e Collegamenti Hardware

### 1. Architettura dell'Alimentazione e Isolamento
* **Bus Primario:** 12V DC (es. impianto batteria veicolo/moto).
* **Step-Down DC-DC XL4015:** Converte i 12V in ingresso a **5.0V DC stabilizzati** in uscita.
  * Alimenta il pin **5V (VIN / VBUS)** dell'ESP32-S3.
  * Alimenta il morsetto **VCC** del modulo relè optoisolato.
* **Massa Comune (GND):** Il negativo a 12V del bus, il polo negativo dello step-down (IN- e OUT-), il pin **GND** dell'ESP32-S3, il pin **GND** del modulo relè, il ritorno dei pulsanti SPST e il ritorno negativo delle 2 lampadine a 12V sono tutti collegati a **massa comune**.

---

### 2. Tabella dei Collegamenti GPIO e Morsetti

| Componente | Pin ESP32-S3 | Pin Modulo / Carico | Tipo Segnale | Livello / Logica | Descrizione Funzionale |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Step-Down XL4015** | **5V (VIN)** | **OUT+ (5V)** | Alimentazione IN | +5.0V DC | Alimentazione logica ESP32-S3 |
| **Massa Comune** | **GND** | **OUT- / Bus GND**| Riferimento 0V | 0V (GND) | Riferimento massa condiviso del sistema |
| **Relè 1 (Posizione)** | **GPIO 4** | **IN 1** | Digitale OUT | Active-LOW (0V = ON) | Pilotaggio fotoaccoppiatore canale 1 |
| **Relè 2 (Anabbagliante)**| **GPIO 5** | **IN 2** | Digitale OUT | Active-LOW (0V = ON) | Pilotaggio fotoaccoppiatore canale 2 |
| **VCC Scheda Relè** | **—** | **VCC (Relè)** | Alimentazione 5V | +5.0V DC (da XL4015) | Corrente bobine e fotoaccoppiatori relè |
| **GND Scheda Relè** | **GND** | **GND (Relè)** | Riferimento 0V | 0V (GND) | Massa modulo relè |
| **Pulsante Bistabile 1** | **GPIO 7** | **Terminale A (Sw1)**| Digitale IN | `INPUT_PULLUP` (3.3V a riposo) | Comando manuale SPST Posizione (verso GND) |
| **Pulsante Bistabile 2** | **GPIO 6** | **Terminale A (Sw2)**| Digitale IN | `INPUT_PULLUP` (3.3V a riposo) | Comando manuale SPST Anabbagliante (verso GND) |
| **Ritorno Pulsanti 1 e 2**| **GND** | **Terminale B (Sw1/2)**| Riferimento 0V | 0V (GND) | Chiusura interruttore verso massa |
| **Linea Potenza Relè 1/2**| **—** | **COM 1 & COM 2** | Linea Potenza 12V | +12V Bus Primario | Ingresso alimentazione lampadine |
| **Lampadina 1 (+12V)** | **—** | **NO 1 (Relè 1)** | Contatto Potenza | 12V commutati | Uscita normalmente aperta per Posizione |
| **Lampadina 2 (+12V)** | **—** | **NO 2 (Relè 2)** | Contatto Potenza | 12V commutati | Uscita normalmente aperta per Anabbagliante|
| **Ritorno Lampade (-)** | **—** | **GND 12V Bus** | Riferimento 0V | 0V (GND) | Polo negativo delle lampade a 12V |

> [!IMPORTANT]
> **Nessun pulsante in serie ai relè:** I pulsanti bistabili/autobloccanti sono interfacciati esclusivamente come ingressi logici digitali tra i pin GPIO e GND, proteggendo i contatti da archi elettrici e garantendo la totale indipendenza dal carico a 12V.

---

### 3. Schema Elettrico Completo di Principio (ASCII Diagram)

```text
                    +-----------------------------+
                    |      BUS 12V PRIMARIO       |
                    |   (+12V)            (GND)   |
                    +-----+-----------------+-----+
                          |                 |
                          |                 +-----------------------------------------+
                          |                 | (GND 12V)                               |
                          v [IN+]           v [IN-]                                   |
                  +-------------------------------+                                   |
                  |       STEP-DOWN XL4015        |                                   |
                  |     (Uscita fissa a 5.0V)     |                                   |
                  +-------+---------------+-------+                                   |
                          | [OUT+]        | [OUT-]                                    |
                          | (+5V)         | (GND)                                     |
                          |               +--------------------+                      |
                          |                                    |                      |
                          +------------------+                 |                      |
                                             |                 |                      |
                                             v [5V / VIN]      v [GND]                |
                                     +---------------------------------+              |
                                     |            ESP32-S3             |              |
                                     |                                 |              |
   [SW 1 - Posizione]                |                                 |              |
   GND <---[ / ]---------------------+ [GPIO  7] (INPUT_PULLUP)        |              |
                                     |                                 |              |
   [SW 2 - Anabbagliante]            |                                 |              |
   GND <---[ / ]---------------------+ [GPIO 6] (INPUT_PULLUP)        |              |
                                     |                                 |              |
                                     |  [GPIO 4] (Comando OUT) --------+----+         |
                                     |  [GPIO 5] (Comando OUT) --------+--+ |         |
                                     +---------------------------------+  | |         |
                                                                          | |         |
                      +-------------------+                               | |         |
                      | +5V (da OUT+ XL)  |                               | |         |
                      +---------+---------+                               | |         |
                                |                                         | |         |
                                v [VCC]                                   | |         |
                  +-------------------------------+                       | |         |
                  |                               |                       | |         |
                  |  MODULO RELÈ 2 CANALI (5V)    |                       | |         |
                  |         OPTOISOLATO           |                       | |         |
                  |                               |                       | |         |
                  |  [IN1] (Active-LOW) <---------+-----------------------+ | (GPIO 4)|
                  |  [IN2] (Active-LOW) <---------+-------------------------+ (GPIO 5)|
                  |                               |                                   |
                  |  [GND] -----------------------+--------------------+              |
                  |                               |                    |              |
                  |  RELÈ 1 (Posizione):          |                    |              |
(+12V Bus) ------>|    COM 1                      |                    |              |
                  |    NO 1 ----------------------+---> (+) LAMPADINA 1 (Posizione)   |
                  |                               |     (-) ------------+             |
                  |  RELÈ 2 (Anabbagliante):      |                     |             |
(+12V Bus) ------>|    COM 2                      |                     |             |
                  |    NO 2 ----------------------+---> (+) LAMPADINA 2 (Anabbagl.)   |
                  |                               |     (-) ------------+             |
                  +-------------------------------+                     |             |
                                                                        v             v
                                                                 ========================
                                                                    MASSA COMUNE (GND)
                                                                 ========================
```

---

### 4. Logica di Pilotaggio Active-LOW dei Relè
I moduli a 2 canali con fotoaccoppiatore conducono quando l'anodo interno è alimentato a 5V e il catodo viene abbassato a massa (0V) dall'ESP32-S3:
* **Relè Diseccitato / Lampada Spenta:** Pin GPIO a **3.3V (HIGH)** -> Fotoaccoppiatore spento, contatto NO aperto.
* **Relè Eccitato / Lampada Accesa:** Pin GPIO a **0V (LOW)** -> Fotoaccoppiatore attivo, contatto NO chiuso su COM (+12V).
* **Configurazione ESPHome:** Impostata con `inverted: true` su `GPIO4` e `GPIO5` per mantenere le entità switch coerenti (`state: true` = luce accesa = relè scattato a livello basso).

---

## 🧠 Firmware & Logica di Controllo

### 1. Toggle Logico su Transizione di Fronte (Edge Transition)
I pulsanti SPST utilizzati sono autobloccanti/bistabili (mantengono la posizione premuto o rilasciato). 
* **Problema del controllo statico (HIGH/LOW):** Se si leggesse il livello statico, un'accensione remota via Web verrebbe subito sovrascritta dalla posizione fisica del pulsante non appena valutato.
* **Soluzione adottata (Edge Toggle):** Il firmware rileva esclusivamente la **variazione di fronte** (sia fronte di discesa da rilascio a pressione, sia fronte di salita da pressione a rilascio). Ogni click fisico inverte (toggle) lo stato logico della luce corrispondente, consentendo l'accensione manuale e lo spegnimento da remoto (o viceversa) senza alcun disallineamento.

### 2. Meccanismo Software di Anti-Rimbalzo Non Bloccante (Debounce Nativo ESPHome)
La gestione dei pulsanti bistabili e del debounce hardware (50 ms) è affidata nativamente a ESPHome in `smarthome-esps3.yaml`:
```yaml
binary_sensor:
  - platform: gpio
    pin:
      number: GPIO7
      mode: INPUT_PULLUP
      inverted: true
    filters:
      - delayed_on_off: 50ms   # Anti-rimbalzo hardware non bloccante
    on_press:
      - switch.toggle: relay_posizione

  - platform: gpio
    pin:
      number: GPIO6
      mode: INPUT_PULLUP
      inverted: true
    filters:
      - delayed_on_off: 50ms   # Anti-rimbalzo hardware non bloccante
    on_press:
      - switch.toggle: relay_anabbagliante
```
* **Nessun ritardo bloccante (`delay()`):** La gestione hardware nativa tramite `binary_sensor` di ESPHome garantisce un rilevamento del fronte istantaneo e affidabile, preservando la CPU e la stabilità dello stack Wi-Fi.

---

## 🎨 Interfaccia Web Grafica e Feedback di Stato

1. **Dashboard a 2 Riquadri Centrali:** Ottimizzata per smartphone con i pulsanti grafici ad alta risoluzione per **Posizione** e **Anabbagliante**.
2. **Spia LED Centrale di Stato:** Posizionata direttamente al di sotto di ciascun pulsante:
   * **Stato ACCESO:** Spia circolare verde fluorescente brillante (`#00ff66`) con bagliore neon ad alone (`box-shadow`), bordo card illuminato in tinta verde e dicitura **ACCESO**.
   * **Stato SPENTO:** Spia grigio scuro (`#404040`), bordo card neutro trasparente e dicitura **SPENTO**.
3. **Sincronizzazione Live Continua:** 
   * La pagina esegue polling in background su `/status` ogni **1.2 secondi**. Se l'utente agisce sul pulsante fisico sul manubrio/cruscotto, la spia LED e il bordo del tasto nella pagina web cambiano istantaneamente senza dover ricaricare la schermata.
   * Toccando il riquadro grafico sul touchscreen dello smartphone, viene invocata la rotta `/togglePin?pin=X` che inverte il relè e aggiorna la grafica in tempo reale.

---

## 🌐 Connettività di Rete: `panigalemel.local` e Captive Portal Automatico

### 1. Risoluzione Hostname mDNS (`http://panigalemel.local`)
* L'ESP32-S3 pubblica il nome host mDNS `panigalemel.local` su multicast DNS (porta 5353 UDP).
* **Rete di Casa (STA Mode):** Da qualsiasi PC o smartphone connesso alla rete Wi-Fi domestica, digitando semplicemente nel browser `http://panigalemel.local` si accede direttamente alla dashboard di controllo.
* **Access Point Moto (AP Mode):** Anche connessi all'hotspot `Panigale-Mel-AP`, `http://panigalemel.local` è risolto nativamente sia via mDNS sia dal server DNS spoofing integrato.

### 2. Captive Portal Automatico in Modalità AP
Quando l'ESP32-S3 è in modalità Access Point (SSID: `Panigale-Mel-AP`, password: `mammamelap`, IP: `192.168.4.1`):
1. **Server DNS e Captive Portal Nativo:** Il componente nativo `captive_portal:` di ESPHome gestisce le query DNS e i rilevamenti automatici dei sistemi operativi (Android, iOS CNA, Windows NCSI).
2. **Priorità Rotte Dirette:** Gli handler personalizzati del controller hanno precedenza assoluta, servendo istantaneamente la dashboard Panigale su `/` e `/home`.

---

## 📁 Struttura del Progetto

```text
EspHomePorting/
├── smarthome-esps3.yaml         # Configurazione ESPHome (S3, 2 Relays, 2 Binary Sensors, Captive Portal, WebServer)
├── glassmorphism.css            # Stile personalizzato con sfondo #020001 e bottoni immagine
├── secrets.yaml                 # Credenziali Wi-Fi e chiave crittografica
├── README.md                    # Questa documentazione con schema elettrico e dettagli tecnici
├── COMMAND.md                   # Comandi utili per la compilazione, flash e monitor seriale
├── docs/
│   ├── FLASHING_GUIDE.md        # Istruzioni passo-passo per il flashing USB/OTA
│   └── ROUTES_AND_USAGE.md      # Elenco completo delle rotte API (/home, /status, /togglePin)
├── img/compresse/               # Immagini WebP salvate in Flash (LogoMel, Posizione, Anabbagliante)
└── src/                         # Architettura MVC in C++
    ├── model/
    │   ├── CustomButtonModel.h
    │   └── CustomButtonModel.cpp # Gestione bottoni personalizzati NVS Flash
    ├── view/
    │   ├── ImagesData.h         # Dati binari PROGMEM delle immagini WebP
    │   ├── ImagesData.cpp
    │   ├── NavigationManager.h
    │   ├── NavigationManager.cpp # Navbar fissa inferiore
    │   ├── ViewConfig.h
    │   ├── ViewConfig.cpp       # Form e scansione reti Wi-Fi con IP assegnato
    │   ├── ViewServices.h
    │   └── ViewServices.cpp     # Dashboard 2 bottoni con LED verde/grigio e polling live
    └── controller/
        ├── CustomWebController.h # Router Web asincrono ed esposizione relè
        └── CustomWebController.cpp # Gestione rotte HTTP (/home, /togglePin, /status, /customButtons)
```
