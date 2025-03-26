#include "viewGeneric.h"

String viewGeneric::html = "";

String viewGeneric::addCss(){
    String css = R"(
    body {
        font-family: Arial, sans-serif;
        line-height: 1.6;
        background-color: #f4f4f4;
        margin: 0;
        padding: 20px;
        padding-top: 60px; /* Evita la sovrapposizione della navbar */
        display: flex;
        flex-direction: column;
        align-items: center;
    }
    )";
    return css+ viewGeneric::addNavbarCss();
}

String viewGeneric::addNavbarCss(){
    String addNavbarCss = R"(
        .navbar {
            display: flex;
            justify-content: space-evenly;
            align-items: center;
            background: rgba(48, 48, 48, 0.6); /* Grigio traslucido glass effect */
            backdrop-filter: blur(12px);
            -webkit-backdrop-filter: blur(12px);
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.4);
            border-radius: 15px;
            padding: 10px 20px;
            z-index: 2000;
            position: fixed;
        }
        .navbar a {
            text-decoration: none;
            color: white;
            text-align: center;
            display: inline-flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            padding: 10px;
            transition: transform 0.2s ease-in-out, color 0.3s ease;
        }
        .navbar .icon {
            width: 80px;
            height: 80px;
            background-color: rgba(255, 255, 255, 0.752); /* Fondo traslucido */
            border-radius: 8px; /* Angoli arrotondati */
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 40px; /* Emoji grande */
            color: #333;
            transition: transform 0.3s ease, background-color 0.3s ease, box-shadow 0.3s ease;
            box-shadow: inset 0 2px 5px rgba(255, 255, 255, 0.2), 0 4px 8px rgba(0, 0, 0, 0.4);
        }
        .navbar span {
            font-size: 16px;
            margin-top: 8px;
            color: white;
            transition: transform 0.3s ease, color 0.3s ease;
        }
        .navbar a:hover .icon {
            transform: scale(1.3); /* Ingrandisce l'icona */
            background-color: rgba(161, 255, 167, 0.7); /* Verde traslucido per hover */
            color: white;
            box-shadow: 0 8px 16px rgba(0, 0, 0, 0.6);
        }
        .navbar a:hover span {
            color: #4CAF50;
        }
        /* Per desktop e tablet: navbar verticale */
        @media (min-width: 1500px) {
            .navbar {
                flex-direction: column;
                top: 50%;
                left: 15px;
                transform: translateY(-50%);
                position: fixed;
                height: auto;
                padding: 20px 10px;
                width: auto;
            }
            .navbar a {
                margin-bottom: 15px;
            }
            .navbar span {
                font-size: 12px; /* Testo più piccolo per layout laterale */
            }
        }
        /* Per dispositivi mobili: navbar in basso con scorrimento */
        @media (max-width: 1499px) {
            .navbar {
                position: fixed;
                bottom: 15px;
                left: 0;
                right: 0;
                flex-direction: row;
                overflow-x: auto; /* Scorrimento orizzontale */
                scroll-behavior: smooth; /* Scorrimento fluido */
                padding: 10px 0;
                gap: 10px;
            }
            .navbar::-webkit-scrollbar {
                display: none; /* Rimuove la scrollbar visibile */
            }
        }
        @media (max-width: 480px) {
            .navbar .icon {
                width: 60px;
                height: 60px;
                font-size: 30px; /* Icone compatte */
            }
            .navbar span {
                font-size: 10px; /* Testo più piccolo */
            }
        }
    )" ;
    return addNavbarCss;
}

String viewGeneric::addPinCss(){
    String pinCss = R"(

        .pin-container {
            background: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            backdrop-filter: blur(10px);
            -webkit-backdrop-filter: blur(10px);
            padding: 20px;
            margin-bottom: 20px;
            width: 80%;
            max-width: 600px;
            border: 1px solid rgba(255, 255, 255, 0.2);
        }
        .pin-info {
            margin-bottom: 10px;
            font-weight: bold;
        }
        .pin-actions form {
            display: inline-block;
            margin-right: 10px;
        }
        .pin-actions button {
            padding: 10px 15px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-weight: bold;
        }
        .pin-actions button.start {
            background-color: rgba(255, 0, 0, 0.7);
            color: white;
        }
        .pin-actions button.stop {
            background-color: rgba(128, 128, 128, 0.7);
            color: white;
        }
        .pin-actions button.edit {
            background-color: rgba(0, 0, 255, 0.7);
            color: white;
        }
    )";
    return pinCss;
}

String viewGeneric::addPinStyleCss(){
    String pinStyleCss ="<style>";
    pinStyleCss += viewGeneric::addPinCss();
    pinStyleCss += "</style>";
    return pinStyleCss;
}

String viewGeneric::addFormStyleCss(){
    String formStyleCss = "<style>";
    formStyleCss += viewGeneric::addFormCss();
    formStyleCss += "</style>";
    return formStyleCss;
}

String viewGeneric::addFormCss(){
    String formCss = R"(
        .form-container {
            width: 80%; /* Adatta larghezza */
            background: rgba(255, 255, 255, 0.85); /* Sfondo semi-trasparente */
            padding: 20px;
            border-radius: 15px; /* Angoli arrotondati */
            box-shadow: 0 4px 10px rgba(0, 0, 0, 0.1); /* Ombra leggera */
            margin-top: 20px;
            backdrop-filter: blur(10px); /* Effetto glass */
            -webkit-backdrop-filter: blur(10px); /* Compatibilità Safari */
            border: 1px solid rgba(255, 255, 255, 0.2); /* Bordo traslucido */
            max-width: 600px; /* Imposta una larghezza massima */
        }

        form {
            display: flex;
            flex-direction: column; /* Allineamento verticale */
        }

        label {
            font-weight: bold;
            margin-bottom: 8px; /* Spaziatura inferiore */
            color: #333; /* Colore del testo */
        }

        input, select {
            margin-bottom: 15px; 
            padding: 10px; 
            border: 1px solid #ccc; 
            border-radius: 8px; /* Angoli arrotondati */
            font-size: 16px; /* Dimensione del testo */
            width: 100%; /* Larghezza piena */
            box-sizing: border-box; /* Include padding nei calcoli di larghezza */
        }

        button {
            padding: 12px;
            font-size: 16px; /* Testo leggibile */
            background-color: #4CAF50; /* Colore verde */
            color: white; /* Testo bianco */
            border: none;
            border-radius: 5px; /* Angoli arrotondati */
            cursor: pointer;
            transition: background-color 0.3s ease; /* Transizione per hover */
        }

        button:hover {
            background-color: #45a049; /* Verde più scuro per hover */
        }

        /* Tabelle per visualizzare dati */
        .history-table {
            width: 80%;
            border-collapse: collapse; /* Rimuove spazi tra celle */
            margin: 20px 0;
            font-size: 18px;
            text-align: left;
        }

        .history-table th, .history-table td {
            padding: 12px 15px; /* Spaziatura interna */
            border: 1px solid #ddd; /* Bordo sottile */
        }

        .history-table th {
            background-color: #f2f2f2; /* Sfondo per intestazioni */
        }

        .history-table tr:nth-child(even) {
            background-color: #f9f9f9; /* Riga pari */
        }

        .history-table tr:nth-child(odd) {
            background-color: #fff; /* Riga dispari */
        }

        canvas {
            max-width: 100%; /* Adattabile */
            height: auto;
        }

        /* Responsive Form Container */
        @media (max-width: 768px) {
            .form-container {
                width: 90%; /* Riduce larghezza per schermi medi */
                padding: 15px; /* Riduce padding */
            }

            input, select, button {
                font-size: 14px; /* Riduce font */
                padding: 8px; /* Adatta padding */
            }

            button {
                font-size: 14px; /* Testo più piccolo */
            }
        }
        @media (max-width: 480px) {
            .form-container {
                width: 100%; /* Adatta completamente */
                padding: 10px;
            }

            label {
                font-size: 14px; /* Riduce dimensione dei label */
            }

            input, select, button {
                font-size: 12px; /* Testo più piccolo */
            }

            button {
                padding: 8px; /* Spaziatura interna ridotta */
            }
        }
    )";
    return formCss;
}

String viewGeneric::defaultPorfolioCss(){
    String porfolioCss = R"(
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Porfolio - Alessio Tommasi</title>
            <link rel="icon" href="https://raw.githubusercontent.com/AlessioTommasi-supsi/porfolio/main/html/view/images/logo.png" type="image/png">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/style.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/gear.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/logo.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/glass_effect.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/autoType.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/buttons.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/dashboard.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/particle_style.css">
            
    )";
    return porfolioCss;
}

String viewGeneric::addNavbar()
{
    String navbar = R"(
        <!-- Navbar Responsive con Scorrimento Orizzontale -->
        <div class="navbar">
            <a href="/monitor">
            <div class="icon">🖥️</div>
            <span>Monitor</span>
            </a>
            <a href="/pinout">
            <div class="icon">🔌</div>
            <span>Pinout</span>
            </a>
            <a href="/currentregister">
            <div class="icon">📋</div>
            <span>Register</span>
            </a>
            <a href="/history">
            <div class="icon">⏳</div>
            <span>History</span>
            </a>
            <a href="/graph">
            <div class="icon">📈</div>
            <span>Graph</span>
            </a>
            <a href="/config">
            <div class="icon">📶</div>
            <span>WIfi Config</span>
            </a>
            <a href="/paramConfig">
            <div class="icon">⚙️</div>
            <span>Extra</span>
            </a>
            <a href="/extra">
            <div class="icon">📱</div>
            <span>Mobile</span>
            </a>
        </div>
        )";
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
    //per  far si che riconosca emoji!
    html += R"(
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
    )";
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
    var_html += " updateContent('" + api + "', '" + divId + "');"; // Caricamento iniziale
    var_html += " if (" + String(timeToUpdate) + " > 0) {"; // Controllo del valore di timeToUpdate
    var_html += "   setInterval(() => {";
    var_html += "     updateContent('" + api + "', '" + divId + "');";
    var_html += "   }, " + String(timeToUpdate) + ");";
    var_html += " }";
    var_html += "});";
    var_html += "</script>";
    return var_html;
}
