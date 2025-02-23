#include "viewGeneric.h"

String viewGeneric::html = "";

String viewGeneric::addCss()
{
    String css = "body {";
    css += "font-family: Arial, sans-serif;";
    css += "line-height: 1.6;";
    css += "background-color: #f4f4f4;";
    css += "margin: 0;";
    css += "padding: 20px;";
    css += "padding-top: 60px;"; // Aggiungi padding-top per evitare la sovrapposizione della navbar
    css += "display: flex;";
    css += "flex-direction: column;";
    css += "align-items: center;";
    css += "}";
    css += ".pin-container {";
    css += "background: rgba(255, 255, 255, 0.1);";
    css += "border-radius: 15px;";
    css += "box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);";
    css += "backdrop-filter: blur(10px);";
    css += "-webkit-backdrop-filter: blur(10px);";
    css += "padding: 20px;";
    css += "margin-bottom: 20px;";
    css += "width: 80%;";
    css += "max-width: 600px;";
    css += "border: 1px solid rgba(255, 255, 255, 0.2);";
    css += "}";
    css += ".pin-info {";
    css += "margin-bottom: 10px;";
    css += "font-weight: bold;";
    css += "}";
    css += ".pin-actions form {";
    css += "display: inline-block;";
    css += "margin-right: 10px;";
    css += "}";
    css += ".pin-actions button {";
    css += "padding: 10px 15px;";
    css += "border: none;";
    css += "border-radius: 5px;";
    css += "cursor: pointer;";
    css += "font-weight: bold;";
    css += "}";
    css += ".pin-actions button.start {";
    css += "background-color: rgba(255, 0, 0, 0.7);";
    css += "color: white;";
    css += "}";
    css += ".pin-actions button.stop {";
    css += "background-color: rgba(128, 128, 128, 0.7);";
    css += "color: white;";
    css += "}";
    css += ".pin-actions button.edit {";
    css += "background-color: rgba(0, 0, 255, 0.7);";
    css += "color: white;";
    css += "}";
    css += ".navbar {";
    css += "display: flex;";
    css += "justify-content: center;";
    css += "background-color: #333;";
    css += "width: 100%;";
    css += "padding: 10px 0;";
    css += "position: fixed;";
    css += "top: 0;";
    css += "z-index: 1000;"; // Assicurati che la navbar sia sempre in cima
    css += "}";
    css += ".navbar a {";
    css += "color: white;";
    css += "padding: 14px 20px;";
    css += "text-align: center;";
    css += "text-decoration: none;";
    css += "font-size: 17px;";
    css += "}";
    css += ".navbar a:hover {";
    css += "background-color: #ddd;";
    css += "color: black;";
    css += "}";
    // Puoi mantenere gli altri stili se necessari...
    return css;
}

String viewGeneric::addNavbar()
{
    String navbar = "<div class='navbar'>";
    navbar += "<a href='/monitor'>Monitor</a>";
    navbar += "<a href='/pinout'>Pinout</a>";
    navbar += "<a href='/currentregister'>Current Register</a>";
    navbar += "<a href='/history'>History</a>";
    navbar += "<a href='/graph'>Graph</a>";
    navbar += "<a href='/config'>Config</a>";
    navbar += "</div>";
    navbar += "<br><br><br>";
    navbar += "<br><br><br>";
    return navbar;
}

String viewGeneric::defaultHeader()
{
    html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    // html += "<meta http-equiv='refresh' content='5'>";
    html += "<title>DefaultTitle</title>";
    html += "</head>";
    html += "<body>";
    html += addNavbar();
    return html;
}

String viewGeneric::defaultCssHeader(String title)
{
    html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    // html += "<meta http-equiv='refresh' content='5'>";
    html += "<title>";
    html += title;
    html += "</title>";
    html += "<style>";
    html += addCss();
    html += "</style>";
    html += "</head>";
    html += "<body>";
    html += addNavbar();
    return html;
}

String viewGeneric::defaultHeader(String title)
{
    html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    // html += "<meta http-equiv='refresh' content='5'>";//refresh automatico della pagina ogni 5 sec indesiderato!
    html += "<title>";
    html += title;
    html += "</title>";
    html += "</head>";
    html += "<body>";
    html += addNavbar();
    return html;
}

String viewGeneric::defaultFooter()
{
    html = "</body>";
    html += "</html>";
    return html;
}


String viewGeneric::dynamicUpdateContent(String divId/*una stringa univoca nella pagina*/, String api, int timeToUpdate)
{
    String var_html = "";
    var_html += "<div id='" + divId + "'></div>";
    var_html += "<script>";
    var_html += "function updateContent(api, divId) {";
    var_html += " fetch(api)";
    var_html += " .then(response => response.text())";
    var_html += " .then(data => {";
    var_html += " document.getElementById(divId).innerHTML = data;";
    var_html += " })";
    var_html += " .catch(error => {";
    var_html += " console.error('Errore durante il recupero dei dati:', error);";
    var_html += " document.getElementById(divId).innerHTML = '<p>Errore durante il recupero dei dati</p>';";
    var_html += " });";
    var_html += "}";
    var_html += "document.addEventListener('DOMContentLoaded', () => {";
    var_html += " updateContent('" + api + "', '" + divId + "');";
    var_html += " setInterval(() => {";
    var_html += " updateContent('" + api + "', '" + divId + "');";
    var_html += " }, " + String(timeToUpdate) + ");";
    var_html += "});";
    var_html += "</script>";
    return var_html;
}