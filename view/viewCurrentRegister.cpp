#include "viewCurrentRegister.h"

String viewCurrentRegister::html = "";

String viewCurrentRegister::generateEmptyFormRegister()
{
    int registerAddress = 0;
    float registerValue = 0.0;
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

    form += "<br><br><br>";

    form += "<div class=\"form-container\">";
    form += "    <form action=\"modbusSlave\" method=\"get\">";
    form += "        <label for=\"registerAddress\">Register Address:</label>";
    form += "        <input type=\"text\" id=\"registerAddress\" name=\"registerAddress\" value=\"" + String(registerAddress) + "\" required>";
    form += "        <label for=\"registerAddress\">Register Value:</label>";
    form += "        <input type=\"text\" id=\"registerAddress\" name=\"registerValue\" value=\"" + String(registerValue) + "\" required>";
    form += "        <label for=\"registerType\">Register Type:</label>";
    form += "        <select id=\"registerType\" name=\"registerType\" required>";
    form += "            <option value=\"int\">int</option>";
    form += "            <option value=\"float\">float</option>";
    form += "        </select>";\
    form += "        <button type=\"submit\">Write As Slave</button>";
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
    String css = viewGeneric::basicHeader("Current Register"); 
    css += viewGeneric::dynamicUpdateContentScript();
    css += viewGeneric::dynamicUpdateContent("", "/navbarStyle", -1);

    String page_content = "";
    page_content += viewGeneric::addNavbar();
    page_content += viewGeneric::dynamicUpdateContent(
        "", 
        "/modbusMasterPageContent?registerAddress=" + registerAddress 
          + "&registerValue=" + String(registerValue), 
        -1
    );

    // 1) Definizione della funzione showPopup
    page_content += "<script>"
                    "function showPopup(message) {"
                    "  document.getElementById('popupMessage').innerText = message;"
                    "  document.getElementById('popup').style.display = 'block';"
                    "}"
                    "</script>";

    // 2) Inserisco **qui** il <div> del popup, **fuori** dal contenuto dinamico
    page_content += "<div id=\"popup\" "
                    "style=\"display:none;position:fixed;top:50%;left:50%;"
                    "transform:translate(-50%,-50%);padding:20px;"
                    "background-color:white;border:1px solid black;z-index:1000;\">"
                    "<p id=\"popupMessage\"></p>"
                    "<button onclick=\"document.getElementById('popup').style.display='none';\">Close</button>"
                    "</div>";

    // 3) Se c'è uno script di popup, lo wrappo in DOMContentLoaded
    if (popupScript != "") {
        page_content += "<script>"
                        "window.addEventListener('DOMContentLoaded', function() {"
                        + popupScript +
                        "});"
                        "</script>";
    }

    // Footer e restituzione
    page_content += viewGeneric::defaultFooter();
    return css + page_content;
}


String viewCurrentRegister::pageContent(String registerAddress, float registerValue)
{
    String page_content = "";
    // Aggiunta del contenitore principale per form e valore del registro
   
    page_content += "<div class=\"container\">";

    // Creazione del primo form per monitorare il registro
    
    page_content += "<div class=\"form-container\">";
    
    page_content += "    <form action=\"modbusMaster\" method=\"get\">";
    page_content += "        <label for=\"registerAddress\">Register Address:</label>";
    page_content += "        <input type=\"text\" id=\"registerAddress\" name=\"registerAddress\" value=\"" + registerAddress + "\" required>";
    page_content += "        <label for=\"registerType\">Register Type:</label>";
    page_content += "        <select id=\"registerType\" name=\"registerType\" required>";
    page_content += "            <option value=\"int\">int</option>";
    page_content += "            <option value=\"float\">float</option>";
    page_content += "        </select>";\
    page_content += "        <button type=\"submit\">Monitor</button>";
    page_content += "    </form>";

    page_content += "    <form action=\"modbusSlave\" method=\"get\">";
    page_content += "        <label for=\"registerAddress\">Register Address:</label>";
    page_content += "        <input type=\"text\" id=\"registerAddress\" name=\"registerAddress\" value=\"" + registerAddress + "\" required>";
    page_content += "        <label for=\"registerAddress\">Register Value:</label>";
    page_content += "        <input type=\"text\" id=\"registerAddress\" name=\"registerValue\" value=\"" + String(registerValue) + "\" required>";
    page_content += "        <label for=\"registerType\">Register Type:</label>";
    page_content += "        <select id=\"registerType\" name=\"registerType\" required>";
    page_content += "            <option value=\"int\">int</option>";
    page_content += "            <option value=\"float\">float</option>";
    page_content += "        </select>";\
    page_content += "        <button type=\"submit\">Write As Slave</button>";
    page_content += "    </form>";
    
    page_content += "</div>";
    

    page_content += "</div>";
    
    // Aggiunta del valore del registro sotto il primo form
    page_content += "<div style=\"text-align: center; margin-top: 20px;\">";
    page_content += "    <h2>Register Value: " + String(registerValue) + "</h2>";
    page_content += "</div>";

    // Creazione del secondo form per memorizzare il valore del registro
    page_content += "<div class=\"form-container\" style=\"margin-top: 20px; text-align: center;\">";
    page_content += "    <form action=\"/storevalue\" method=\"get\">";
    page_content += "        <input type=\"hidden\" name=\"registerValue\" value=\"" + String(registerValue) + "\">"; // Campo nascosto per il valore del registro
    page_content += "        <input type=\"hidden\" name=\"registerAddress\" value=\"" + registerAddress + "\">";     // Campo nascosto per l'indirizzo del registro
    page_content += "        <button type=\"submit\" style=\"padding: 10px; background-color: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer;\">Store Register Value</button>";
    page_content += "    </form>";
    page_content += "</div>";

    // Aggiunta della casella di testo per i millisecondi e dei pulsanti Start/Stop Recording
    page_content += "<div class=\"form-container\" style=\"margin-top: 20px; text-align: center;\">";
    page_content += "    <label for=\"milliseconds\">Milliseconds:</label>";
    page_content += "    <input type=\"text\" id=\"milliseconds\" name=\"milliseconds\" required>";
    page_content += "    <form action=\"/startRecording\" method=\"get\" style=\"display: inline;\" onsubmit=\"document.getElementById('startMilliseconds').value = document.getElementById('milliseconds').value;\">";
    page_content += "        <input type=\"hidden\" name=\"milliseconds\" id=\"startMilliseconds\">";
    page_content += "        <input type=\"hidden\" name=\"registerAddress\" value=\"" + registerAddress + "\">";     // Campo nascosto per l'indirizzo del registro
    page_content += "        <button type=\"submit\" style=\"padding: 10px; background-color: red; color: white; border: none; border-radius: 4px; cursor: pointer; margin-left: 10px;\">Start Recording</button>";
    page_content += "    </form>";
    page_content += "    <form action=\"/stopRecording\" method=\"get\" style=\"display: inline;\">";
    page_content += "        <input type=\"hidden\" name=\"registerAddress\" value=\"" + registerAddress + "\">";     // Campo nascosto per l'indirizzo del registro
    page_content += "        <button type=\"submit\" style=\"padding: 10px; background-color: grey; color: white; border: none; border-radius: 4px; cursor: pointer; margin-left: 10px;\">Stop Recording</button>";
    page_content += "    </form>";
    page_content += "</div>";

    // Aggiunta del popup
    page_content += "<div id=\"popup\" style=\"display:none; position:fixed; top:50%; left:50%; transform:translate(-50%, -50%); padding:20px; background-color:white; border:1px solid black; z-index:1000;\">";
    page_content += "    <p id=\"popupMessage\"></p>";
    page_content += "    <button onclick=\"document.getElementById('popup').style.display='none';\">Close</button>";
    page_content += "</div>";

    // Chiusura del contenitore principale
    page_content += "</div>";
    
    return page_content;
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
