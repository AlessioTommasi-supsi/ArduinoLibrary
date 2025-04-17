#include "viewHistory.h"

String viewHistory::html = "";

String viewHistory::generateHTML()
{
    html = viewGeneric::defaultCssHeader("History Register");

    html += "<h1>History</h1>";
    html += "<div class='scrollable-container' style='overflow-y: auto; max-height: 80vh;'>"; // Add inline style for scrollbar
    // Creazione della tabella con header fisso
    html += "<table class='history-table' border='1'>";
    html += "<thead>";
    html += "<tr>";
    html += "<th>Pin</th>";
    html += "<th>Index</th>";
    html += "<th>Value</th>";
    html += "<th>Actions</th>";
    html += "</tr>";
    html += "</thead>";
    
    // Il tbody è identificato per l'aggiornamento automatico tramite fetch
    html += "<tbody id='history-pin-content'>";
    html += viewHistory::pinoutContent(); 
    html += "</tbody>";
    html += "</table>";
    html += "<br>"; // Spazio tra le tabelle

    html += "<table class='history-table'>";
    html += "<tr>";
    html += "<th>Address</th>";
    html += "<th>Value</th>";
    html += "<th>Actions</th>"; // Add Actions column header
    html += "</tr>";

    html += "<tbody id='history-content'>"; // Add tbody with id for updating content

    html += viewHistory::modbusContent(); 
    html += "</tbody>";
    html += "</table>";


    html += "</div>";

    // Aggiungi lo script per aggiornare il div ogni k secondi
    html += "<script>";
    html += "document.addEventListener('DOMContentLoaded', () => {";
    html += "  setInterval(() => {";
    html += "    fetch('/getHistoryContent')";
    html += "      .then(response => response.text())";
    html += "      .then(data => {";
    html += "        document.getElementById('history-content').innerHTML = data;";
    html += "      });";
    html += "  }, 3000);"; // Aggiorna ogni 10 secondi (puoi cambiare il valore a k*1000 per k secondi)
    html += "});";
    html += "</script>";

    html += "<script>";
    html += "document.addEventListener('DOMContentLoaded', () => {";
    html += "  setInterval(() => {";
    html += "    fetch('/getPinValuesHistory')";
    html += "      .then(response => response.text())";
    html += "      .then(data => {";
    html += "        document.getElementById('history-pin-content').innerHTML = data;";
    html += "      });";
    html += "  }, 3000);"; // Aggiorna ogni 10 secondi (puoi cambiare il valore a k*1000 per k secondi)
    html += "});";
    html += "</script>";

    //TODO vedi pinout! script che quando faccio focus su un input mi ferma l'aggiornamento e quando lo tolgo lo riavvia
    

    html += viewGeneric::defaultFooter();

    return html;
}

String viewHistory::modbusContent()
{
    String html = "";
    
    std::vector<float> valuesVector = SystemState::getInstance()->getAllRegisterValue();
    std::vector<int> addresses = SystemState::getInstance()->getAllRegisterAddress();

    // Loop through each value in the vector in reverse order
    for (size_t i = valuesVector.size(); i > 0; i--)
    {
        size_t index = i - 1;
        html += "<tr>";
        // Controlla se l'indirizzo è maggiore di 0 
        if (addresses[index] > 0) { 
            html += "<td>" + String(addresses[index]) + "</td>"; 
            // Display the address 
        } else { 
            html += "<td>GPIO:" + String(-addresses[index]) + "</td>"; 
            // Display the negative address as GPIO 
        }

        // Display the value with an input field for editing inside a form
        html += "<td>";
        html += "<form action='/editRegister' method='GET'>";
        html += "<input type='hidden' name='index' value='" + String(index) + "'>";
        html += "<input type='text' class='edit-input' name='value' value='" + String(valuesVector[index]) + "'>";
        html += "<input type='submit' value='Edit' class='action-link edit-link'>";
        html += "</form>";
        html += "</td>";

        // Add delete button with link
        html += "<td>";
        html += "<a href='/deleteRegister?index=" + String(index) + "' class='action-link delete-link'>Delete</a>"; // Delete link
        html += "</td>";

        html += "</tr>";
    }

    
    return html;
}

String viewHistory::pinoutContent() {
    String content = "";
    // Otteniamo tutti i numeri dei pin gestiti tramite l'interfaccia PinoutData
    std::vector<int> pinNumbers = SystemState::getInstance()->pinoutData->getPinNumbers();
    
    // Per ogni pin...
    for (size_t i = 0; i < pinNumbers.size(); i++) {
        int gpioPin = pinNumbers[i];
        // Recupera il riferimento al Pin corrispondente
        Pin &currentPin = SystemState::getInstance()->pinoutData->getPin(gpioPin);
        // Ottieni il vettore dei valori di tensione registrati per questo pin
        std::vector<float> valuesVector = currentPin.getValuesVoltage();

        // Se non sono presenti valori, stampa una riga informativa
        if (valuesVector.empty()) {
            content += "<tr>";
            content += "<td>" + String(gpioPin) + "</td>";
            content += "<td colspan='2'>Nessun valore registrato</td>";
            content += "</tr>";
        } else {
            // Cicla in ordine inverso (simile a modbusContent)
            for (size_t j = valuesVector.size(); j > 0; j--) {
                size_t index = j - 1;
                content += "<tr>";
                // Colonna 1: numero del pin (come indirizzo)
                content += "<td>" + String(gpioPin) + "</td>";
                
                // Colonna 2: form per modificare il valore
                content += "<td>";
                content += "<form action='/editPinValue' method='GET'>";
                content += "<input type='hidden' name='pin' value='" + String(gpioPin) + "'>";
                content += "<input type='hidden' name='index' value='" + String(index) + "'>";
                content += "<input type='text' class='edit-input' name='value' value='" + String(valuesVector[index]) + "'>";
                content += "<input type='submit' value='Edit' class='action-link edit-link'>";
                content += "</form>";
                content += "</td>";
                
                // Colonna 3: link per cancellare il valore
                content += "<td>";
                content += "<a href='/deletePinValue?pin=" + String(gpioPin) + "&index=" + String(index) + "' class='action-link delete-link'>Delete</a>";
                content += "</td>";
                
                content += "</tr>";
            }
        }
    }
    return content;
}
