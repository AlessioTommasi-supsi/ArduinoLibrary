#include "pinout.h"
#include "viewGraph.h"

String Pinout::html = "";

String Pinout::generateHTML()
{
    // Header ultra-minimalista
    html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1.0'>";
    html += "<title>Pinout</title>";
    html += "<style>body{margin:0;padding:60px 10px 120px;background:#f4f4f4}</style>";
    html += "</head><body>";
    
    // Tutto caricato dinamicamente per ridurre heap
    html += viewGeneric::dynamicUpdateContentScript();
    html += viewGeneric::dynamicUpdateContent("navbar_area", "/navbarStyle", -1);
    html += viewGeneric::dynamicUpdateContent("pinout_content", "/pinoutPageContent", -1);
    
    html += "<div id='navbar_area'></div>";
    html += viewGeneric::addNavbar();
    html += "<div id='pinout_content'><div style='text-align:center;padding:20px'>Loading pins...</div></div>";
    
    html += "</body></html>";
    return html;
}

String Pinout::pageContent()
{
    // CSS minimalista inline
    html = "<style>";
    html += ".pin-container{background:rgba(255,255,255,0.9);border-radius:8px;padding:15px;margin:10px auto;width:90%;max-width:500px}";
    html += ".pin-info{margin-bottom:8px;font-weight:bold;font-size:14px}";
    html += ".pin-actions{margin-top:10px}.pin-actions button{padding:6px 10px;border:none;border-radius:4px;margin-right:6px;font-size:12px;cursor:pointer}";
    html += ".start{background:#4CAF50;color:white}.stop{background:#f44336;color:white}.edit{background:#2196F3;color:white}";
    html += "</style>";

    html += "<h1 style='text-align:center;margin:20px 0'>🔌 Pinout Configuration</h1>";

    // Genera pin dinamicamente per evitare heap overflow
    std::vector<int> pinNumbers = SystemState::getInstance()->pinoutData->getPinNumbers();
    
    for (int pinNumber : pinNumbers) 
    {
        Pin currentPin = SystemState::getInstance()->pinoutData->getPin(pinNumber);
        
        html += "<div class='pin-container'>";
        html += "<div class='pin-info'>";
        char noteBuffer[200];
        currentPin.getNote(noteBuffer, sizeof(noteBuffer));
        html += "Pin number: " + String(pinNumber) + ", Type: " + currentPin.getType() + ", Voltage: " + String(currentPin.getVoltage() / 1000.0, 3) + " V, Input: " + (currentPin.getIsInput() ? "Yes" : "No") + ", Note: " + String(noteBuffer);
        html += "</div>";
        
        html += "<div class='pin-actions'>";
        
        // Form per start recording (solo per pin input)
        if (currentPin.getIsInput()) {
            html += "<form action='/startRecordingPin' method='get' style='display:inline-block;margin-right:5px'>";
            html += "<input type='hidden' name='pin' value='" + String(pinNumber) + "'>";
            html += "<label for='milliseconds'>ms:</label>";
            html += "<input type='text' name='milliseconds' value='1000' style='width:60px;margin-right:5px'>";
            html += "<button type='submit' class='start'>Start Recording</button>";
            html += "</form>";
            
            html += "<form action='/stopRecordingPin' method='get' style='display:inline-block;margin-right:5px'>";
            html += "<input type='hidden' name='pin' value='" + String(pinNumber) + "'>";
            html += "<button type='submit' class='stop'>Stop Recording</button>";
            html += "</form>";
        }
        
        // Form per edit (sempre presente)
        html += "<form action='/editPin' method='get' style='display:inline-block'>";
        html += "<input type='hidden' name='pin' value='" + String(pinNumber) + "'>";
        html += "<button type='submit' class='edit'>Edit</button>";
        html += "</form>";
        
        html += "</div></div>";
    }

    // JavaScript minimalista per refresh periodico
    html += "<script>setTimeout(()=>{if(typeof loadPageContent==='function')loadPageContent('/pinoutPageContent','pinout_content',-1)},30000)</script>";
    
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
    // Header completo con CSS e navbar
    html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1.0'>";
    html += "<title>Pinout</title>";
    html += "<style>";
    html += viewGeneric::addCss();
    html += viewGeneric::addFormCss();
    html += viewGeneric::addNavbarCss();
    html += "</style>";
    html += "</head><body>";
    
    // Navbar sempre presente
    html += viewGeneric::addNavbar();

    // Contenuto della pagina
    html += pageContent();

    // Popup
    html += "<div id='popup' style='display:none; position:fixed; top:50%; left:50%; transform:translate(-50%, -50%); padding:20px; background-color:white; border:1px solid black; z-index:1000;'>";
    html += "    <p id='popupMessage'></p>";
    html += "    <button onclick=\"document.getElementById('popup').style.display='none';\">Close</button>";
    html += "</div>";

    // JavaScript per popup
    html += "<script>";
    html += "function showPopup(message) {";
    html += "    document.getElementById('popupMessage').innerText = message;";
    html += "    document.getElementById('popup').style.display = 'block';";
    html += "}";
    html += "</script>";

    // Script personalizzato se fornito
    if (popupScript != "")
    {
        html += "<script>";
        html += popupScript;
        html += "</script>";
    }

    html += viewGeneric::defaultFooter();

    return html;
}