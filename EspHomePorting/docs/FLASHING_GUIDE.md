# Guida alla Selezione e Flashing dell'ESP32-S3 con ESPHome

Questa guida spiega in dettaglio come identificare, selezionare e caricare il codice sul tuo dispositivo **ESP32-S3** utilizzando ESPHome.

---

## 🔍 1. Come Identificare la Porta del tuo ESP32-S3 su Linux (Arch)

Quando colleghi la scheda ESP32-S3 al computer tramite il cavo USB:

1. **Verifica il dispositivo collegato:**
   Apri il terminale ed esegui:
   ```bash
   ls /dev/ttyACM* /dev/ttyUSB* 2>/dev/null
   ```
   * Le schede **ESP32-S3** con USB CDC nativa compaiono solitamente come **`/dev/ttyACM0`** o `/dev/ttyACM1`.
   * Le schede con convertitore Seriale USB esterno (es. CP2102 o CH340) compaiono come **`/dev/ttyUSB0`**.

2. **Verifica via Kernel (`dmesg`):**
   Puoi anche controllare i log del kernel quando colleghi il cavo:
   ```bash
   sudo dmesg -w
   ```
   Vedrai righe simili a: `cdc_acm 1-2:1.0: ttyACM0: USB ACM device`.

---

## 🚀 2. Come Selezionare l'ESP32 ed Eseguire il Flash

### **Metodo A: Menu Interattivo di ESPHome (Automatico)**

Se esegui semplicemente:
```bash
cd /home/none/Arduino/libraries/EspHomePorting
esphome run smarthome-esps3.yaml
```

ESPHome scansionerà le porte seriali e la rete locale. Se rileva più porte o schede, ti mostrerà un menu a scelta multipla:

```text
Found multiple devices:
 [1] /dev/ttyACM0 (ESP32-S3 USB CDC)
 [2] Over The Air (smarthome-s3.local)
Choose device [1-2]: 
```
Digitando **`1`** e premendo Invio, ESPHome avvierà la compilazione e caricherà il firmware sulla porta selezionata.

---

### **Metodo B: Selezione Esplicita della Porta Seriale (`--device`)**

Se vuoi specificare direttamente la porta seriale senza menu interattivi:

```bash
esphome run smarthome-esps3.yaml --device /dev/ttyACM0
```
*(Sostituisci `/dev/ttyACM0` con la tua porta effettiva se diversa).*

---

### **Metodo C: Caricamento Wireless via Wi-Fi (OTA - Over The Air)**

Dopo aver caricato il codice per la prima volta via USB, le successive modifiche o aggiornamenti possono essere inviati **senza cavo USB**, direttamente via Wi-Fi:

1. **Tramite indirizzo IP o hostname mDNS:**
   ```bash
   esphome run smarthome-esps3.yaml --device 192.168.1.50
   # oppure
   esphome run smarthome-esps3.yaml --device panigalemel.local
   ```

---

## 🛠️ 3. Risoluzione dei Problemi di Permessi (Permission Denied)

Se ESPHome restituisce un errore del tipo `Permission denied: '/dev/ttyACM0'`:

1. Assicurati che il tuo utente sia nei gruppi `uucp` e `lock`:
   ```bash
   sudo usermod -aG uucp $USER
   sudo usermod -aG lock $USER
   ```
2. Effettua il logout e il re-login oppure riavvia la sessione di terminale.
3. Puoi anche verificare temporaneamente i permessi della porta con:
   ```bash
   ls -l /dev/ttyACM0
   ```
