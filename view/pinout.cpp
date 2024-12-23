#include "pinout.h"

String Pinout::html = "";

String Pinout::generateHTML()
{
    html = "";
    html = viewGeneric::defaultCssHeader("Pinout");
    pageContent();
    html += viewGeneric::defaultFooter();

    return html;
}

String Pinout::pageContent()
{
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
    return html;
}



String Pinout::generateHTML(String popupScript)
{
    html = "";
    html = viewGeneric::defaultCssHeader("Pinout");

    pageContent();

    // Aggiunta del popup
    html += "<div id='popup' style='display:none; position:fixed; top:50%; left:50%; transform:translate(-50%, -50%); padding:20px; background-color:white; border:1px solid black; z-index:1000;'>";
    html += "    <p id='popupMessage'></p>";
    html += "    <button onclick=\"document.getElementById('popup').style.display='none';\">Close</button>";
    html += "</div>";

    // Aggiunta del JavaScript per gestire il popup
    html += "<script>";
    html += "function showPopup(message) {";
    html += "    document.getElementById('popupMessage').innerText = message;";
    html += "    document.getElementById('popup').style.display = 'block';";
    html += "}";
    html += "</script>";

    // Esecuzione dello script passato come parametro
    if (popupScript != "")
    {
        html += "<script>";
        html += popupScript;
        html += "</script>";
    }

    html += viewGeneric::defaultFooter();

    return html;
}