#include "viewHistory.h"

String viewHistory::html = "";

String viewHistory::generateHTML()
{
    html = viewGeneric::defaultCssHeader("History Register");

    html += "<h1>History</h1>";
    html += "<div class='scrollable-container' style='overflow-y: auto; max-height: 80vh;'>"; // Add inline style for scrollbar
    html += "<table class='history-table'>";
    html += "<tr>";
    html += "<th>Address</th>";
    html += "<th>Value</th>";
    html += "<th>Actions</th>"; // Add Actions column header
    html += "</tr>";

    html += "<tbody id='history-content'>"; // Add tbody with id for updating content

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
    html += "  }, 3000);"; // Aggiorna ogni 3 secondi (puoi cambiare il valore a k*1000 per k secondi)
    html += "});";
    html += "</script>";

    html += viewGeneric::defaultFooter();

    return html;
}