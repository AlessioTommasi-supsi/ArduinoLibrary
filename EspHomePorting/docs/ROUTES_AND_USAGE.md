# Guida Completa alle Rotte HTTP e all'Utilizzo (API & Usage)

Questa guida documenta tutte le rotte HTTP, gli endpoint API ed i casi d'uso del porting **Panigale ESP32-S3 Lighting Control**.

---

## 🌐 1. Panoramica delle Rotte HTTP

| Rotta / Endpoint | Metodo | Descrizione | Output / Azione |
| :--- | :--- | :--- | :--- |
| **`/home`** | `GET` | **Dashboard Principale Controllo Luci** | UI con sfondo `#020001`, logo `LogoMel.webp` in alto, 2 pulsanti con immagini al 100% (Posizione e Anabbagliante) corredati di spia LED centrale (Verde/Grigia), bottoni custom, link a `/addButton` e navbar fissa (`🏠 Home`, `📶 WiFi Config`) |
| **`/togglePin`** | `GET` | **Inverte (toggle) lo stato logico del relè** | Parametri: `?pin=4` (Posizione) o `?pin=5` (Anabbagliante). Risponde in JSON: `{"pin":4,"state":true}` |
| **`/status`** | `GET` | **Stato attuale di tutti i relè** | Risponde in JSON: `{"pos":true,"anab":false}` per consentire il polling live automatico da parte del frontend |
| **`/pulsePin`** | `GET` | Alias di retrocompatibilità | Inoltrato internamente a `/togglePin` |
| **`/`** | `GET` | Redirect automatico Captive Portal / Root | Reindirizza con HTTP 302 a `http://192.168.4.1/home` |
| **`/services`** | `GET` | Alias di retrocompatibilità | Reindirizza a `/home` |
| **`/config`** | `GET` | Pagina Gestione e Scansione Wi-Fi | Form HTML con menu a tendina delle reti Wi-Fi scansionate e campo password |
| **`/switch_wifi`** | `GET` | Cambia rete Wi-Fi client (STA) | Parametri: `?ssid=NOME&password=PASS`. Salva la rete in Flash NVS e avvia la connessione mantenendo l'AP sempre attivo |
| **`/logo.webp`** | `GET` | Immagine Logo locale | Restituisce il file binario `LogoMel.webp` salvato nella Flash dell'ESP32-S3 |
| **`/posizione.webp`** | `GET` | Immagine Luci di Posizione | Restituisce il file binario `Luci_di_posizione.webp` salvato in Flash |
| **`/anabbagliante.webp`**| `GET` | Immagine Luce Anabbagliante | Restituisce il file binario `Anabbagliante.webp` salvato in Flash |
| **`/customButtons`** | `GET` | Fragment HTML dei bottoni personalizzati | Restituisce il frammento HTML dei bottoni caricati dalla memoria Flash NVS |
| **`/addButton`** | `GET` | Form per aggiungere un nuovo bottone | Form HTML per inserire Etichetta, Emoji ed URL/Rotta |
| **`/saveButton`** | `GET` | Salva un nuovo bottone emoji | Parametri: `?label=NOME&emoji=💡&url=togglePin?pin=7`. Scrive in NVS e reindirizza a `/home` |
| **`/removeButton`** | `GET` | Rimuove un bottone emoji salvato | Parametri: `?label=NOME`. Elimina il bottone dalla NVS e reindirizza a `/home` |
| **`/generate_204`, `/hotspot-detect.html`, etc.** | `GET` | Endpoints Captive Portal (Android / iOS / Windows) | Reindirizzano immediatamente (HTTP 302) a `http://192.168.4.1/home` |

---

## 📲 2. Funzionamento Captive Portal

1. **Connessione al Wi-Fi:** All'aggancio alla rete `Panigale-Mel-AP`, il server DNS interno risolve automaticamente tutte le richieste su `192.168.4.1`.
2. **Apertura automatica della schermata comandi:** Il sistema operativo dello smartphone interroga i propri endpoint di captive detection (`generate_204`, `hotspot-detect.html`, ecc.) e riceve un reindirizzamento immediato a `http://192.168.4.1/home`.
3. **Nessun popup di configurazione Wi-Fi indesiderato:** La schermata che si apre è direttamente la dashboard luci (`/home`).

---

## 💡 3. Indicatori Visivi e Sincronizzazione Live

* **Spia LED centrale sotto a ciascun pulsante:**
  * **Verde fluorescente (`#00ff66`) con alone luminoso neon:** Indica che la luce è **ACCESA** (`is-on`). Il contorno del pulsante si illumina in tinta coordinata.
  * **Grigio antracite scuro (`#404040`):** Indica che la luce è **SPENTA** (`is-off`).
* **Sincronizzazione in tempo reale:** Il frontend effettua polling asincrono su `/status` ogni **1.2 secondi**. Se viene azionato il pulsante fisico sulla moto o inviato un comando esterno, la spia e il bordo cambiano stato automaticamente senza necessità di ricaricare la pagina web.
