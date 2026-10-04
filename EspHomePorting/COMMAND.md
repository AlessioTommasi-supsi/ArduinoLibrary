# Guida ai Comandi Utili (COMMAND.md)

Questo documento elenca tutti i comandi necessari per compilare, caricare il firmware ed effettuare il debug sul modulo **ESP32-S3**, con la spiegazione dettagliata del loro funzionamento e della motivazione per cui eseguirli.

---

### 1. Verifica e Compilazione del Progetto (Senza Flash)

```bash
cd /home/none/Arduino/libraries/EspHomePorting
esphome compile smarthome-esps3.yaml
```

* **A cosa serve:** Esegue la generazione del codice sorgente C++ a partire dalla configurazione YAML, compila tutti i file in `src/` e verifica la presenza di errori di sintassi o librerie mancanti.
* **Perché eseguirlo:** Permette di accertarsi che il codice C++, le rotte del Web Server, la logica di debounce e le definizioni hardware siano perfette al 100% prima di collegare la scheda ESP32-S3 via cavo USB.

---

### 2. Compilazione, Caricamento (Flash) e Monitor Seriale

```bash
cd /home/none/Arduino/libraries/EspHomePorting
esphome run smarthome-esps3.yaml
```

* **A cosa serve:** Compila il firmware, individua la porta seriale USB (es. `/dev/ttyACM0` o `/dev/ttyUSB0`) oppure rileva la scheda via Wi-Fi (OTA), carica il binario nella memoria Flash dell'ESP32-S3 e apre automaticamente il visualizzatore di log a 115200 baud.
* **Perché eseguirlo:** È il comando principale per rendere effettive le modifiche hardware e software sull'ESP32-S3 montato sulla moto/veicolo.

---

### 3. Visualizzazione dei Log in Tempo Reale

```bash
cd /home/none/Arduino/libraries/EspHomePorting
esphome logs smarthome-esps3.yaml
```

* **A cosa serve:** Si connette alla console seriale o alla porta di log wireless dell'ESP32-S3 e stampa in tempo reale tutti gli eventi di sistema.
* **Perché eseguirlo:** Utile per verificare il rilevamento dei fronti dei pulsanti fisici (GPIO 7 e 6), il cambio di stato dei relè (GPIO 4 e 5), la connessione dei client Wi-Fi e le richieste HTTP in arrivo.

---

### 4. Pulizia della Cache di Compilazione (Build Clean)

```bash
cd /home/none/Arduino/libraries/EspHomePorting
esphome clean smarthome-esps3.yaml
```

* **A cosa serve:** Elimina tutti i file oggetto compilati e la cache intermedia presente nella cartella `.esphome/build/smarthome-s3/`.
* **Perché eseguirlo:** Da usare solo in caso di problemi di linking, modifiche strutturali profonde ai file header C++ o corruzione della cache di build.

---

### 5. Accesso e Verifica `panigalemel.local` & Captive Portal

1. **Accesso da Browser (Rete di Casa o AP):**
   ```text
   http://panigalemel.local
   ```
   oppure direttamente:
   ```text
   http://192.168.4.1
   ```

2. **Comportamento Captive Portal in AP Mode (`Panigale-Mel-AP`):**
   * Connettendo lo smartphone alla rete Wi-Fi `Panigale-Mel-AP` (password: `mammamelap`), il sistema operativo riconosce automaticamente la rete come portale di accesso:
     * **Android:** Compare la notifica di sistema *"Accedi alla rete Wi-Fi"*; cliccandola si apre immediatamente la dashboard.
     * **iOS (Apple):** Si apre automaticamente la finestra a comparsa a schermo intero (CNA - Captive Network Assistant).
     * **Qualsiasi browser:** Qualsiasi URL digitato viene reindirizzato istantaneamente alla dashboard delle luci.

