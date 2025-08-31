#include "outView.h"

String outView::generateHTML(String signalType, String popupScript)
{
    String html = viewGeneric::defaultCssHeader("Uscite - " + signalType);

    // Include CSS from cssControlGroup
    html += cssControlGroup::getCSS();

    // Add popup elements if popupScript is provided
    if (popupScript.length() > 0) {
        html += "<div id='popupOverlay' style='display:none;position:fixed;top:0;left:0;width:100%;height:100%;background:rgba(0,0,0,0.5);z-index:999' onclick=\"document.getElementById('popup').style.display='none';document.getElementById('popupOverlay').style.display='none'\"></div>";
        html += "<div id='popup' style='display:none;position:fixed;top:50%;left:50%;transform:translate(-50%,-50%);padding:15px;background:white;border:1px solid #ccc;border-radius:8px;z-index:1000;box-shadow:0 4px 8px rgba(0,0,0,0.2)'>";
        html += "<p id='popupMessage'></p>";
        html += "<button onclick=\"document.getElementById('popup').style.display='none';document.getElementById('popupOverlay').style.display='none'\" style='background:#4CAF50;color:white;border:none;padding:8px 12px;border-radius:4px;cursor:pointer'>Close</button>";
        html += "</div>";
        html += "<script>function showPopup(m){document.getElementById('popupMessage').innerText=m;document.getElementById('popup').style.display='block';document.getElementById('popupOverlay').style.display='block'}</script>";
        html += "<script>" + popupScript + "</script>";
    }

    html += "<div class=\"form-container\" style=\"text-align: center;\">";
    html += "<h1>Uscite per " + signalType + "</h1>";
    html += "<form action='/setAlarm' method='get'>";
    html += "<input type='hidden' name='signalType' value='" + signalType + "'>";
    html += "<div class='control-group'>";
    html += "<label for='pin'>Seleziona Pin:</label>";
    html += "<select id='pin' name='pin'>";
    html += "<option value='" + String(PIN_25) + "'>" + String(PIN_25) + "</option>";
    html += "<option value='" + String(PIN_26) + "'>" + String(PIN_26) + "</option>";
    html += "</select>";
    html += "</div>";
    html += "<div class='control-group'>";
    html += "<label for='type'>Tipologia:</label>";
    html += "<select id='type' name='type'>";
    html += "<option value='monitor'>Monitor</option>";
    html += "<option value='alarm'>Allarme</option>";
    html += "</select>";
    html += "</div>";
    html += "<div id='alarmLevelDiv' style='display:none;' class='control-group'>";
    html += "<label for='alarmLevel'>Livello Soglia Allarme:</label>";
    html += "<input type='number' id='alarmLevel' name='alarmLevel' step='0.01' placeholder='Es. 25.5'>";
    html += "</div>";
    html += "<button type='submit'>Imposta Allarme</button>";
    html += "</form>";
    html += "<script>";
    html += "document.getElementById('type').addEventListener('change', function() {";
    html += "    if (this.value === 'alarm') {";
    html += "        document.getElementById('alarmLevelDiv').style.display = 'block';";
    html += "    } else {";
    html += "        document.getElementById('alarmLevelDiv').style.display = 'none';";
    html += "    }";
    html += "});";
    html += "</script>";
    //devo mettere bottone di stop e ampiezza segnale: mappa cosa voglio a 3.3V e a 0 per quando sono in monitor!!
    html += viewGeneric::defaultFooter();

    return html;
}

String outView::generatePopupPage(String signalType, String message, String redirectPage)
{
    
    String html = viewGeneric::defaultCssHeader("Notifica");

    html += "<div style='display:flex;align-items:center;justify-content:center;height:60vh;'>";
    html += "<div style='background:rgba(255,255,255,0.98);padding:24px;border-radius:10px;max-width:520px;text-align:center;box-shadow:0 6px 20px rgba(0,0,0,0.08)'>";
    html += "<h2 style='color:#333;margin-bottom:12px;'>" + message + "</h2>";
    html += "<a href='" + redirectPage + "' style='display:inline-block;margin-top:12px;padding:10px 18px;background:#4CAF50;color:#fff;text-decoration:none;border-radius:6px;'>OK</a>";
    html += "</div>";
    html += "</div>";

    html += viewGeneric::defaultFooter();
    return html;
}