#include "viewCurrentRegister.h"

String viewCurrentRegister::html = "";

String viewCurrentRegister::generateEmptyFormRegister()
{
    String form ;

    form += "<div class=\"form-container\">";
    form += "    <form action=\"modbusMaster\" method=\"get\">";
    form += "        <label for=\"registerAddress\">Register Address:</label>";
    form += "        <input type=\"text\" id=\"registerAddress\" name=\"registerAddress\" required>";
    form += "        <label for=\"registerType\">Register Type:</label>";
    form += "        <select id=\"registerType\" name=\"registerType\" required>";
    form += "            <option value=\"int\">int</option>";
    form += "            <option value=\"float\">float</option>";
    form += "        </select>";
    form += "        <button type=\"submit\">Monitor</button>";
    form += "    </form>";
    form += "</div>";

    return form;
}


String viewCurrentRegister::generateHTML() //codice generato solo la prima volta quando nessun registro e stato ancora selezionato!
{
    html = viewGeneric::defaultCssHeader("Current Register");

    html += viewCurrentRegister::generateEmptyFormRegister();

    html += viewGeneric::defaultFooter();

    return html;
}

String viewCurrentRegister::generateHTML(String registerAddress, float registerValue)
{
    return viewCurrentRegister::generateHTML(registerAddress, registerValue, "");
}


String viewCurrentRegister::generateHTML(String registerAddress, float registerValue, String popupScript = "")
{
    // Creazione dell'header HTML con il foglio di stile CSS
    String html = viewGeneric::defaultCssHeader("Current Register");

    // Aggiunta del contenitore principale per form e valore del registro
    html += "<div class=\"container\">";

    // Creazione del primo form per monitorare il registro
    html += "<div class=\"form-container\">";
    html += "    <form action=\"modbusMaster\" method=\"get\">";
    html += "        <label for=\"registerAddress\">Register Address:</label>";
    html += "        <input type=\"text\" id=\"registerAddress\" name=\"registerAddress\" value=\"" + registerAddress + "\" required>";
    html += "        <label for=\"registerType\">Register Type:</label>";
    html += "        <select id=\"registerType\" name=\"registerType\" required>";
    html += "            <option value=\"int\">int</option>";
    html += "            <option value=\"float\">float</option>";
    html += "        </select>";
    html += "        <button type=\"submit\">Monitor</button>";
    html += "    </form>";
    html += "</div>";

    // Aggiunta del valore del registro sotto il primo form
    html += "<div style=\"text-align: center; margin-top: 20px;\">";
    html += "    <h2>Register Value: " + String(registerValue) + "</h2>";
    html += "</div>";

    // Creazione del secondo form per memorizzare il valore del registro
    html += "<div class=\"form-container\" style=\"margin-top: 20px; text-align: center;\">";
    html += "    <form action=\"/storevalue\" method=\"get\">";
    html += "        <input type=\"hidden\" name=\"registerValue\" value=\"" + String(registerValue) + "\">"; // Campo nascosto per il valore del registro
    html += "        <input type=\"hidden\" name=\"registerAddress\" value=\"" + registerAddress + "\">";     // Campo nascosto per l'indirizzo del registro
    html += "        <button type=\"submit\" style=\"padding: 10px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer;\">Store Register Value</button>";
    html += "    </form>";
    html += "</div>";

    // Aggiunta della casella di testo per i millisecondi e dei pulsanti Start/Stop Recording
    html += "<div class=\"form-container\" style=\"margin-top: 20px; text-align: center;\">";
    html += "    <label for=\"milliseconds\">Milliseconds:</label>";
    html += "    <input type=\"text\" id=\"milliseconds\" name=\"milliseconds\" required>";
    html += "    <form action=\"/startRecording\" method=\"get\" style=\"display: inline;\" onsubmit=\"document.getElementById('startMilliseconds').value = document.getElementById('milliseconds').value;\">";
    html += "        <input type=\"hidden\" name=\"milliseconds\" id=\"startMilliseconds\">";
    html += "        <input type=\"hidden\" name=\"registerAddress\" value=\"" + registerAddress + "\">";     // Campo nascosto per l'indirizzo del registro
    html += "        <button type=\"submit\" style=\"padding: 10px; background-color: red; color: white; border: none; border-radius: 4px; cursor: pointer; margin-left: 10px;\">Start Recording</button>";
    html += "    </form>";
    html += "    <form action=\"/stopRecording\" method=\"get\" style=\"display: inline;\">";
    html += "        <input type=\"hidden\" name=\"registerAddress\" value=\"" + registerAddress + "\">";     // Campo nascosto per l'indirizzo del registro
    html += "        <button type=\"submit\" style=\"padding: 10px; background-color: grey; color: white; border: none; border-radius: 4px; cursor: pointer; margin-left: 10px;\">Stop Recording</button>";
    html += "    </form>";
    html += "</div>";

    // Aggiunta del popup
    html += "<div id=\"popup\" style=\"display:none; position:fixed; top:50%; left:50%; transform:translate(-50%, -50%); padding:20px; background-color:white; border:1px solid black; z-index:1000;\">";
    html += "    <p id=\"popupMessage\"></p>";
    html += "    <button onclick=\"document.getElementById('popup').style.display='none';\">Close</button>";
    html += "</div>";

    // Chiusura del contenitore principale
    html += "</div>";

    // Aggiunta del footer
    html += viewGeneric::defaultFooter();

    // Aggiunta del JavaScript per gestire il popup
    html += "<script>";
    html += "function showPopup(message) {";
    html += "    document.getElementById('popupMessage').innerText = message;";
    html += "    document.getElementById('popup').style.display = 'block';";
    html += "}";
    html += "</script>";

    // Aggiunta del popupScript se presente
    if (popupScript != "") {
        html += "<script>";
        html += popupScript;
        html += "</script>";
    }

    return html;
}
String viewCurrentRegister::generateHTMLConfirm(String registerAddress, float registerValue)
{
    // Creazione dell'header HTML con il foglio di stile CSS
    String html = viewGeneric::defaultCssHeader(" Register Saved!");

    // Aggiunta del contenitore principale per il messaggio di conferma
    html += "<div class=\"container\">";

    // Creazione del messaggio di conferma
    html += "<div style=\"text-align: center; margin-top: 20px;\">";
    html += "    <h2>Register Value " + String(registerValue) + " stored at address " + registerAddress + "</h2>";
    html += "</div>";

    // Chiusura del contenitore principale
    html += "</div>";

    html += viewCurrentRegister::generateEmptyFormRegister();

    // Aggiunta del footer
    html += viewGeneric::defaultFooter();

    return html;
}
