#include "pinout.h"
#include "viewGraph.h"

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
    //html += pageContentCss(); //Gia fatto in defaultCssHeader
    html += "<h1>Pinout</h1>";

    // Inserisco un'immagine presa da internet:
    //html += "<img src='https://raw.githubusercontent.com/AlessioTommasi-supsi/iotProject/refs/heads/main/image/ESP-38Pin-pinout.jpg' alt='ESP32 Pinout' style='width: 100%; max-width: 800px;'>";
    html += "<br><br>";
    // Aggiungo una sezione per i dati dei pin
    html += "<div id='pinData'></div>";

    // Aggiungi lo script per gestire l'aggiornamento e gli eventi di input
    html += "<script>";
    html += "document.addEventListener('DOMContentLoaded', () => {";
    html += "  let intervalId;";
    html += "  const startInterval = () => {";
    html += "    intervalId = setInterval(() => {";
    html += "      fetch('/pinoutContent')";
    html += "        .then(response => response.text())";
    html += "        .then(data => {";
    html += "          document.getElementById('pinData').innerHTML = data;";
    html += "        });";
    html += "    }, 5000);"; // Aggiorna ogni 5 secondi (puoi cambiare il valore a k*1000 per k secondi)
    html += "  };";
    html += "  const stopInterval = () => {";
    html += "    clearInterval(intervalId);";
    html += "  };";
    html += "  startInterval();";
    html += "  document.addEventListener('focusin', (event) => {";
    html += "    if (event.target.tagName === 'INPUT' && event.target.type === 'text') {";
    html += "      stopInterval();";
    html += "    }";
    html += "  });";
    html += "  document.addEventListener('focusout', (event) => {";
    html += "    if (event.target.tagName === 'INPUT' && event.target.type === 'text') {";
    html += "      startInterval();";
    html += "    }";
    html += "  });";
    html += "});";
    html += "</script>";

    html += "<br><br><br><br><br><br><br><br><br><br>";


    //aggiungo grafico da ViewGraph
    /*in realta al prof non interessa grafico quindi forse e meglio cosi!!
     html += viewGraph::generateGraph(SystemState::getInstance()->pinoutData->getPinNumbers(), "getPinValues", "pin");
    html += "<br><br><br><br><br><br><br><br><br><br>";
    */
   
    return html;
}

String Pinout::pageContentCss(){
    String css ="";

    css += viewGeneric::dynamicUpdateContentScript(); //aggiungo script per aggiornamento dinamico

    /**
     * Aggiungo Css effettivo
     */

    css += viewGeneric::dynamicUpdateContent("", "/formStyle", -1); //aggiungo script per aggiornamento dinamico
    css += viewGeneric::dynamicUpdateContent("", "/pinStyle", -1); //aggiungo script per aggiornamento dinamico
    css += viewGeneric::dynamicUpdateContent("", "/navbarStyle", -1); //aggiungo script per aggiornamento dinamico

    return css;
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