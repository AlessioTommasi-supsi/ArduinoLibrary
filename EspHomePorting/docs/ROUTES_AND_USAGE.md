# Guida Completa alle Rotte HTTP e all'Utilizzo (API & Usage)

Questa guida documenta tutte le rotte HTTP, gli endpoint API ed i casi d'uso del porting **Panigale ESP32-S3 Lighting Control**.

---

## 🌐 1. Panoramica delle Rotte HTTP

| Rotta / Endpoint | Metodo | Descrizione | Output / Azione |
| :--- | :--- | :--- | :--- |
| **`/services`** | `GET` | **Dashboard Principale Controllo Luci** | UI con sfondo `#020001`, logo `LogoMel.webp` in alto al naturale, i 3 pulsanti con immagini al 100% (Posizione, Anabbagliante, Abbagliante), bottoni custom e link a `/addButton` |
| **`/config`** | `GET` | Pagina Gestione e Scansione Wi-Fi | Form HTML con menu a tendina delle reti Wi-Fi scansionate e campo password (AP `Panigale-Mel-AP` sempre attivo) |
| **`/switch_wifi`** | `GET` | Cambia rete Wi-Fi client (STA) | Parametri: `?ssid=NOME&password=PASS`. Salva la rete in Flash NVS e avvia la connessione mantenendo l'AP sempre attivo |
| **`/pulsePin`** | `GET` | Attiva un relè ad impulso per **500 ms** | Parametri: `?pin=4` (Posizione), `?pin=5` (Anabbagliante), `?pin=6` (Abbagliante) |
| **`/logo.webp`** | `GET` | Immagine Logo locale | Restituisce il file binario `LogoMel.webp` salvato nella Flash dell'ESP32-S3 |
| **`/posizione.webp`** | `GET` | Immagine Luci di Posizione | Restituisce il file binario `Luci_di_posizione.webp` salvato in Flash |
| **`/anabbagliante.webp`**| `GET` | Immagine Luce Anabbagliante | Restituisce il file binario `Anabbagliante.webp` salvato in Flash |
| **`/abbagliante.webp`** | `GET` | Immagine Luce Abbagliante | Restituisce il file binario `Abbagliante.webp` salvato in Flash |
| **`/customButtons`** | `GET` | Fragment HTML dei bottoni personalizzati | Restituisce il frammento HTML dei bottoni caricati dalla memoria Flash NVS |
| **`/addButton`** | `GET` | Form per aggiungere un nuovo bottone | Form HTML per inserire Etichetta, Emoji ed URL/Rotta |
| **`/saveButton`** | `GET` | Salva un nuovo bottone emoji | Parametri: `?label=NOME&emoji=💡&url=pulsePin?pin=7`. Scrive in memoria Flash (NVS) e reindirizza a `/services` |
| **`/removeButton`** | `GET` | Rimuove un bottone emoji salvato | Parametri: `?label=NOME`. Elimina il bottone dalla NVS e reindirizza a `/services` |
| **`/`** | `GET` | Dashboard nativa ESPHome v3 | Dashboard standard di ESPHome per la gestione diretta delle entità |

---

## 🔌 2. Comportamento dell'Impulso (Hardware & UI)

* **Durata e Livello Logico:** **500 ms** ad impulso **3.3V (HIGH)** sul GPIO target, tornando poi a **0V (LOW)** a riposo (Active-HIGH), sincronizzato con l'animazione visiva.
* **Feedback Visivo:** Al clic su un pulsante, compare l'icona clessidra ⏳ in alto a destra per **500 ms** esatti, dopodiché scompare automaticamente tornando allo stato di riposo.
* **Mappatura Pin:**
  * **Posizione:** GPIO 4 (`?pin=4`) -> Impulso 3.3V per 500ms
  * **Anabbagliante:** GPIO 5 (`?pin=5`) -> Impulso 3.3V per 500ms
  * **Abbagliante:** GPIO 6 (`?pin=6`) -> Impulso 3.3V per 500ms
