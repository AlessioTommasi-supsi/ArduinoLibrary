#include "pinout.h"

String Pinout::html = "";

String Pinout::generateHTML()
{
    html = viewGeneric::defaultCssHeader("Pinout");

    html += "<h1>Pinout</h1>";
    
    // Inserisco un'immagine presa da internet:
    html += "<img src='https://raw.githubusercontent.com/AlessioTommasi-supsi/iotProject/refs/heads/main/image/ESP-38Pin-pinout.jpg' alt='ESP32 Pinout' style='width: 100%; max-width: 800px;'>";
    html += "<br><br>";
    // Aggiungo una sezione per i dati dei pin
    html += "<div id='pinData'></div>";

    // Aggiungi lo script per aggiornare il div ogni k secondi
    html += "<script>";
    html += "document.addEventListener('DOMContentLoaded', () => {";
    html += "  setInterval(() => {";
    html += "    fetch('/pinoutContent')";
    html += "      .then(response => response.text())";
    html += "      .then(data => {";
    html += "        document.getElementById('pinData').innerHTML = data;";
    html += "      });";
    html += "  }, 3000);"; // Aggiorna ogni 3 secondi (puoi cambiare il valore a k*1000 per k secondi)
    html += "});";
    html += "</script>";

    html += viewGeneric::defaultFooter();

    return html;
}
