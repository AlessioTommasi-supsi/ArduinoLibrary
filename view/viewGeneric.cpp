#include "viewGeneric.h"
#include "Config.h"

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
        padding-bottom: 200px; /* Spazio extra per evitare la navbar mobile */
    }
    )";
    //return css+ viewGeneric::addNavbarCss();
    return css;
}

String viewGeneric::addNavbarStyleCss(){
    String navbarStyleCss ="<style>";
    navbarStyleCss += viewGeneric::addNavbarCss();
    navbarStyleCss += "</style>";
    return navbarStyleCss;
};

String viewGeneric::addNavbarCss(){
    String addNavbarCss = R"(
        /* Versione Mobile (default): navbar in basso, più piccola */
    .navbar {
      display: flex;
      justify-content: space-evenly;
      align-items: center;
      background: rgba(48, 48, 48, 0.6); /* Grigio traslucido */
      backdrop-filter: blur(12px);
      -webkit-backdrop-filter: blur(12px);
      box-shadow: 0 4px 10px rgba(0, 0, 0, 0.4);
      border-radius: 15px;
      padding: 8px 16px; /* padding ridotto */
      z-index: )" + String(CSS_NAVBAR_Z_INDEX) + R"(;
      
      /* Posizionata in basso e centrata */
      position: fixed;
      bottom: 15px;
      left: 50%;
      transform: translateX(-50%);
      width: 80vw; /* ridotta dal 90vw all'80vw */
      
      flex-direction: row;
      overflow-x: auto; /* scroll orizzontale */
      scroll-behavior: smooth;
      gap: 8px; /* gap ridotto */
    }

    /* Rimuove la scrollbar per estetica */
    .navbar::-webkit-scrollbar {
      display: none;
    }

    /* Link della navbar */
    .navbar a {
      text-decoration: none;
      color: white;
      text-align: center;
      display: inline-flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      padding: 8px; /* padding interno leggermente ridotto */
      transition: transform 0.2s ease-in-out, color 0.3s ease;
    }

    /* Icone della navbar, dimensioni ridotte */
    .navbar .icon {
      width: 50px;   /* diminuito rispetto ai 60px */
      height: 50px;  /* diminuito rispetto ai 60px */
      background-color: rgba(255, 255, 255, 0.752);
      border-radius: 8px;
      display: flex;
      align-items: center;
      justify-content: center;
      font-size: 24px;  /* ridotto rispetto ai 30px */
      color: #333;
      transition: transform 0.3s ease, background-color 0.3s ease, box-shadow 0.3s ease;
      box-shadow: inset 0 2px 5px rgba(255, 255, 255, 0.2), 0 4px 8px rgba(0, 0, 0, 0.4);
    }

    /* Testo sotto le icone, dimensione ridotta */
    .navbar span {
      font-size: 12px;  /* ridotto rispetto ai 14px */
      margin-top: 8px;
      color: white;
      transition: transform 0.3s ease, color 0.3s ease;
    }

    /* Effetto hover */
    .navbar a:hover .icon {
      transform: scale(1.3);
      background-color: rgba(161, 255, 167, 0.7);
      color: white;
      box-shadow: 0 8px 16px rgba(0, 0, 0, 0.6);
    }
    .navbar a:hover span {
      color: #4CAF50;
    }

    /* Responsive per smartphone */
    @media (max-width: 480px) {
      .navbar .icon {
        width: 45px;
        height: 45px;
        font-size: 20px;
      }
      .navbar span {
        font-size: 10px;
      }
    }

    /* Versione desktop/tablet: layout verticale (min-width >= 1500px) */
    @media (min-width: 1500px) {
      .navbar {
        flex-direction: column;
        top: 50%;
        left: 15px;
        transform: translateY(-50%);
        bottom: auto;
        right: auto;
        
        /* Limita l'altezza e la larghezza per non occupare troppo spazio */
        max-height: 80vh;  /* ridotto dal 90vh all'80vh */
        width: auto;
        
        overflow-y: auto;  /* scroll verticale */
        overflow-x: hidden;
        padding: 10px;     /* padding leggermente ridotto */
      }
    }
    )"; 
    return addNavbarCss;
}


String viewGeneric::addEmoji_buttonStyleCss(){
    String Emoji_buttonStyleCss ="<style>";
    Emoji_buttonStyleCss += viewGeneric::addEmoji_buttonCss();
    Emoji_buttonStyleCss += "</style>";
    return Emoji_buttonStyleCss;
};

String viewGeneric::addEmoji_buttonCss(){
    String addEmoji_buttonCss = R"(
     .emoji-button-container {
      display: flex;
      flex-wrap: wrap;
      justify-content: center;
      gap: 20px;
      margin-top: 30px;
    }
    
    .emoji-button-wrapper {
      position: relative;
      flex: 0 1 calc(33.333% - 20px);
      max-width: calc(33.333% - 20px);
      display: flex;
      justify-content: center;
    }
    
    .emoji-button {
      background-color: #fff;
      border: none;
      border-radius: 15px;
      box-shadow: 0 5px 10px rgba(0, 0, 0, 0.2);
      width: 100%;
      height: 250px;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      font-size: 100px;
      cursor: pointer;
      position: relative;
      transition: transform 0.3s ease, box-shadow 0.3s ease, opacity 0.3s ease;
      box-sizing: border-box;
    }
    
    /* Bottoni senza wrapper (legacy support) */
    .emoji-button-container > .emoji-button:not(.emoji-button-wrapper .emoji-button) {
      flex: 0 1 calc(33.333% - 20px);
      max-width: calc(33.333% - 20px);
    }
    
    .emoji-button:hover {
      transform: scale(1.05);
      box-shadow: 0 10px 15px rgba(0, 0, 0, 0.3);
    }
    
    .emoji-button label {
      display: block;
      margin-top: 10px;
      font-size: 24px;
      color: #333;
    }
    
    /* Stato di loading: bottone traslucido */
    .emoji-button.loading-state {
      opacity: 0.5;
      pointer-events: none;
    }

    /* Spinner all'interno del bottone */
    .emoji-button .loading-icon {
      display: none;
      position: absolute;
      top: 10px;
      right: 10px;
      font-size: 32px;
    }
    
    /* Mostra lo spinner in stato loading */
    .emoji-button.loading-state .loading-icon {
      display: block;
    }
    
    /* Bottone di cancellazione */
    .delete-btn {
      position: absolute;
      top: 5px;
      right: 5px;
      background: rgba(255, 0, 0, 0.8);
      color: white;
      border: none;
      border-radius: 50%;
      width: 30px;
      height: 30px;
      font-size: 14px;
      cursor: pointer;
      z-index: 10;
      transition: background-color 0.3s ease, transform 0.2s ease;
      display: flex;
      align-items: center;
      justify-content: center;
    }
    
    .delete-btn:hover {
      background: rgba(255, 0, 0, 1);
      transform: scale(1.1);
    }
    
    /* Nascondi il bottone di cancellazione per default */
    .emoji-button-wrapper:not(:hover) .delete-btn {
      opacity: 0;
      pointer-events: none;
    }
    
    .emoji-button-wrapper:hover .delete-btn {
      opacity: 1;
      pointer-events: auto;
    }
    
    @media (max-width: 768px) {
      .emoji-button-wrapper {
        flex: 0 1 calc(50% - 20px);
        max-width: calc(50% - 20px);
      }
      
      .emoji-button-container > .emoji-button:not(.emoji-button-wrapper .emoji-button) {
        flex: 0 1 calc(50% - 20px);
        max-width: calc(50% - 20px);
      }
      
      .emoji-button {
        height: 200px;
        font-size: 80px;
      }
      .emoji-button label {
        font-size: 20px;
      }
      /* Mostra sempre il bottone cancella su mobile */
      .delete-btn {
        opacity: 1;
        pointer-events: auto;
      }
    }
    
    @media (max-width: 480px) {
      .emoji-button-wrapper {
        flex: 0 1 90%;
        max-width: 90%;
      }
      
      .emoji-button-container > .emoji-button:not(.emoji-button-wrapper .emoji-button) {
        flex: 0 1 90%;
        max-width: 90%;
      }
      
      .emoji-button {
        height: auto;
        font-size: 80px;
        padding: 20px 0;
      }
      .emoji-button label {
        font-size: 18px;
      }
    }
    )"; 
    return addEmoji_buttonCss;
}


String viewGeneric::addPinCss(){
    String pinCss = R"(

        .pin-container {
            background: rgba(255, 255, 255, 0.1);
            border-radius: 15px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            backdrop-filter: blur(10px);
            -webkit-bacfieldsetkdrop-filter: blur(10px);
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
        .scrollable-container {
            justify-content: center; /* centra il contenuto */
            width: 90%;
            overflow-y: auto;
            max-height: 80vh;
        }
        .history-table {
            width: 100%;
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


String viewGeneric::addFieldFormStyleCss(){
    String formStyleCss = "<style>";
    formStyleCss += viewGeneric::addFieldFormCss();
    formStyleCss += "</style>";
    return formStyleCss;
}

String viewGeneric::addFieldFormCss(){
    String formCss = R"(
        /* Stili di base e layout */
        body {
        font-family: Arial, sans-serif;
        background: linear-gradient(135deg, #ece9e6, #ffffff);
        margin: 0;
        display: flex;
        flex-direction: column;
        align-items: center;
        padding: 20px;
        }
        
        /* Contenitore con effetto glass */
        .form-container {
        background: rgba(255, 255, 255, 0.75);
        backdrop-filter: blur(5px);
        -webkit-backdrop-filter: blur(5px);
        border-radius: 16px;
        box-shadow: 0 8px 32px rgba(31, 38, 135, 0.37);
        padding: 30px;
        margin-top: 100px;
        width: 90%;
        max-width: 700px;
        }
        
        h2 {
        text-align: center;
        margin-bottom: 30px;
        }
        
        fieldset {
        border: 1px solid #ccc;
        border-radius: 8px;
        margin-bottom: 25px;
        padding: 15px;
        }
        
        legend {
        font-size: 1.2em;
        font-weight: bold;
        padding: 0 10px;
        }
        
        h3 {
        margin: 15px 0 5px;
        font-size: 1.1em;
        }
        
        label {
        display: block;
        margin-bottom: 5px;
        font-weight: bold;
        }
        
        input[type="number"],
        input[type="text"],
        select {
        width: 100%;
        padding: 10px;
        margin-bottom: 15px;
        border: 1px solid #ccc;
        border-radius: 4px;
        font-size: 16px;
        }
        
        /* Bottone di invio */
        button {
        width: 100%;
        padding: 15px;
        font-size: 18px;
        background-color: #4CAF50;
        color: white;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        transition: background-color 0.3s ease;
        }
        
        button:hover {
        background-color: #45a049;
        }
        )";
    return formCss;
}


String viewGeneric::defaultPorfolioCss() {
    String porfolioCss = R"(
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>SmartHome</title>
        <link rel="icon" href="https://raw.githubusercontent.com/AlessioTommasi-supsi/porfolio/main/html/view/images/logo.png" type="image/png">

        <script>
        // Funzione che carica un CSS in modo asincrono
        function loadCSS(href) {
            var link = document.createElement('link');
            link.rel = 'stylesheet';
            link.href = href;
            link.onload = function() {
                console.log("CSS caricato: " + href);
            };
            link.onerror = function() {
                console.error("Errore nel caricamento del CSS: " + href);
            };
            document.head.appendChild(link);
        }

        // Carica i CSS una volta che il DOM è stato interamente caricato
        document.addEventListener("DOMContentLoaded", function() {
            loadCSS("http://alessiotommasi.com/view/css/style.css");
            loadCSS("http://alessiotommasi.com/view/css/gear.css");
            loadCSS("http://alessiotommasi.com/view/css/logo.css");
            loadCSS("http://alessiotommasi.com/view/css/glass_effect.css");
            loadCSS("http://alessiotommasi.com/view/css/autoType.css");
            loadCSS("http://alessiotommasi.com/view/css/buttons.css");
            loadCSS("http://alessiotommasi.com/view/css/dashboard.css");
            loadCSS("http://alessiotommasi.com/view/css/particle_style.css");
        });
        </script>

        <noscript>
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/style.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/gear.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/logo.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/glass_effect.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/autoType.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/buttons.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/dashboard.css">
            <link rel="stylesheet" href="http://alessiotommasi.com/view/css/particle_style.css">
        </noscript>
            )";
    return porfolioCss;
}

String viewGeneric::addNavbar()
{
    String navbar = R"(
        <!-- Navbar Responsive con Scorrimento Orizzontale -->
        <div class="navbar">
            <a href="/">
            <div class="icon">🏠</div>
            <span>Home</span>
            </a>
            
            <a href="/monitor">
            <div class="icon">🖥️</div>
            <span>Monitor</span>
            </a>
            <a href="/pinout">
            <div class="icon">🔌</div>
            <span>Pinout</span>
            </a>
            
            <a href="/history">
            <div class="icon">⏳</div>
            <span>History</span>
            </a>
            
            <a href="/services">
            <div class="icon">⚡</div>
            <span>Services</span>
            </a>
            <a href="/config">
            <div class="icon">📶</div>
            <span>WiFi Config</span>
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

String viewGeneric::basicHeader(String title)
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
    
    return html;
}

String viewGeneric::defaultCssHeader(String title)
{
    html = basicHeader(title);
    html += addNavbar();
    /**
     * Aggiungo il css comune a tutti i documenti anche se potrei farlo nelle singole viste per ottimizzare
     */
    String css ="";

    css += viewGeneric::dynamicUpdateContentScript(); //aggiungo script per aggiornamento dinamico
    css += viewGeneric::dynamicUpdateContent("", "/formStyle", UI_AUTO_UPDATE_DISABLED); //aggiungo script per aggiornamento dinamico
    css += viewGeneric::dynamicUpdateContent("", "/pinStyle", UI_AUTO_UPDATE_DISABLED); //aggiungo script per aggiornamento dinamico
    css += viewGeneric::dynamicUpdateContent("", "/navbarStyle", UI_AUTO_UPDATE_DISABLED); //aggiungo script per aggiornamento dinamico

    html += css;

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

/*
    MetteContenutoNuovo Sotto al Contenuto vecchio
    timeToUpdate > 0 il contenuto nuovo deve essere messo AL posto di quello vecchio alrimenti sotto 

    String divId: stringa dell elemento univoco da sostituira con il nuovo contenuto se timeToUpdate > 0
                  se timeToUpdate < 0 non serve e non verra considerata 
*/
String viewGeneric::dynamicUpdateContent(String divId/*una stringa univoca nella pagina*/, String api, int timeToUpdate /* - 1 per dire refresh automatico disattivato altrimenti millisecondi dopo i quali fare refresh*/)
{
    String var_html = "";
    var_html += R"(
    <script> 
        // Esegue il caricamento iniziale
        try {
            loadPageContent(')" + api + R"(', ')" + divId + R"(', )" + String(timeToUpdate) + R"();
        } catch (error) {
            console.error('Errore durante il caricamento:', error);
        }
    </script>
    )";
    return var_html;
}


String viewGeneric::fetchContentScript()
{
    String script = R"(
        <script>
             // Funzione per caricare dinamicamente i dati dal backend
            async function fetchContent(api) {
                try {
                    // Mostra il messaggio di caricamento (se esiste)
                    const loadingDiv = document.getElementById('loading');
                    if (loadingDiv) loadingDiv.style.display = 'block';

                    // Effettua la richiesta GET
                    const response = await fetch(api);
                    if (!response.ok) {
                        throw new Error('Errore durante il caricamento dei dati: ' + response.status);
                    }
                    return await response.text(); // Restituisce il contenuto della risposta
                } catch (error) {
                    console.error(error);
                    const loadingDiv = document.getElementById('loading');
                    if (loadingDiv) {
                        loadingDiv.innerText = 'Errore durante il caricamento dei dati';
                    }
                    return null; // In caso di errore, restituisce null
                }
            }
        </script>
    )";

    return script;
}

/*
    MetteContenutoNuovo Sotto al Contenuto vecchio
    timeToUpdate > 0 il contenuto nuovo deve essere messo AL posto di quello vecchio alrimenti sotto 
*/
String viewGeneric::dynamicUpdateContentScript()
{
    String script = "";
    
    script += fetchContentScript();

    script += R"(
        <script>
            // Variabile globale per tenere traccia dell'indice dei contenuti aggiunti
            let contentIndex = 0;
            // Funzione per gestire l'aggiunta o la sostituzione del contenuto
            function updateContent(divId, content, replace) {
                if (replace) {
                    // Sostituisci il contenuto del div esistente
                    const existingDiv = document.getElementById(divId);
                    if (existingDiv) {
                        existingDiv.innerHTML = content;
                    } else {
                        console.error('Div con ID ' + divId + ' non trovato per la sostituzione.');
                    }
                } else {
                    // Aggiungi un nuovo div con contenuto sotto
                    const newContentDiv = document.createElement('div');
                    newContentDiv.id = `contentAdd_${contentIndex}`;
                    newContentDiv.className = 'content';
                    newContentDiv.innerHTML = content;
                    contentIndex++;

                    const loadButton = document.getElementById('loadButton');
                    if (loadButton) {
                        document.body.insertBefore(newContentDiv, loadButton);
                    } else {
                        document.body.appendChild(newContentDiv);
                    }
                }
            }

            // Funzione per inizializzare il caricamento
            async function loadPageContent(api, divId, timeToUpdate) {
                const content = await fetchContent(api);
                if (content) {
                    updateContent(divId, content, timeToUpdate > 0);
                }

                // Se il refresh è attivo, imposta un intervallo
                if (timeToUpdate > 0) {
                    setInterval(async () => {
                        const refreshedContent = await fetchContent(api);
                        if (refreshedContent) {
                            updateContent(divId, refreshedContent, true); // Sostituisce il contenuto
                        }
                    }, timeToUpdate);
                }
            }

            // Funzione globale per gestire i click sui bottoni dei servizi (DEVE essere definita prima del caricamento dei bottoni)
            async function fetchData(button, url) {
                console.log('🔗 Chiamando URL:', url); // LOG dell'URL
                
                // Aggiungi classe loading al bottone
                button.classList.add('loading-state');
                
                try {
                    // Determina se l'URL è completo (http/https) o relativo
                    let fetchUrl;
                    let isCrossOrigin = false;
                    
                    if (url.startsWith('http://') || url.startsWith('https://')) {
                        // URL completo - chiamata diretta
                        fetchUrl = url;
                        isCrossOrigin = !url.includes(window.location.hostname);
                        console.log('🌐 URL esterno:', fetchUrl, isCrossOrigin ? '(Cross-Origin)' : '(Same-Origin)');
                    } else {
                        // URL relativo - chiamata locale all'ESP
                        fetchUrl = url;
                        console.log('🏠 URL locale ESP:', fetchUrl);
                    }
                    
                    const response = await fetch(fetchUrl, {
                        mode: isCrossOrigin ? 'no-cors' : 'cors', // Usa no-cors per evitare errori CORS
                        method: 'GET'
                    });
                    
                    if (isCrossOrigin) {
                        // Per richieste cross-origin con no-cors, non possiamo leggere la risposta
                        // ma possiamo assumere che sia andata a buon fine se non ci sono errori
                        console.log('✅ Comando cross-origin inviato (no-cors mode):', fetchUrl);
                        setTimeout(() => {
                            button.classList.remove('loading-state');
                        }, 2000);
                    } else if (response.ok) {
                        console.log('✅ Comando eseguito con successo per:', fetchUrl);
                        setTimeout(() => {
                            button.classList.remove('loading-state');
                        }, 2000);
                    } else {
                        throw new Error('Risposta del server non valida: ' + response.status);
                    }
                } catch (error) {
                    console.error('❌ Errore durante la richiesta:', error);
                    
                    // Verifica se è un errore CORS specifico
                    if (error.name === 'TypeError' && 
                        (error.message.includes('CORS') || error.message.includes('NetworkError'))) {
                        console.log('🔄 Tentativo alternativo per CORS...');
                        
                        // Prova con una richiesta diretta tramite iframe nascosto o popup
                        try {
                            // Usa window.open per bypassare CORS (apre e chiude velocemente)
                            const popup = window.open(url, '_blank', 'width=1,height=1,top=0,left=0');
                            if (popup) {
                                setTimeout(() => {
                                    popup.close();
                                }, 1000);
                                console.log('✅ Comando inviato tramite popup:', url);
                            } else {
                                // Fallback: apri in nuova tab (l'utente dovrà chiuderla)
                                window.open(url, '_blank');
                                console.log('✅ Comando aperto in nuova tab:', url);
                            }
                        } catch (popupError) {
                            console.log('ℹ️ CORS bypassato - comando probabilmente eseguito:', url);
                        }
                    } else {
                        // Solo per errori non-CORS mostra l'alert
                        alert('Errore durante l\'esecuzione del comando: ' + error.message);
                    }
                    
                    // Rimuovi classe loading dopo un breve delay
                    setTimeout(() => {
                        button.classList.remove('loading-state');
                    }, 1500);
                }
            }
        </script>
    )";

    return script;
}

String viewGeneric::addMonitorGraphCss(){
    String monitorGraphCss = R"(
        /* CSS per grafici Monitor */
        .monitor-container {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            padding: 20px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
        }
        
        .monitor-section {
            background: rgba(255, 255, 255, 0.95);
            border-radius: 15px;
            padding: 25px;
            margin-bottom: 30px;
            box-shadow: 0 8px 32px rgba(31, 38, 135, 0.37);
            backdrop-filter: blur(4px);
            border: 1px solid rgba(255, 255, 255, 0.18);
        }
        
        .monitor-title {
            font-size: 2rem;
            font-weight: 700;
            color: #2c3e50;
            text-align: center;
            margin-bottom: 30px;
            text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1);
        }
        
        .monitor-subtitle {
            font-size: 1.5rem;
            font-weight: 600;
            color: #34495e;
            margin-bottom: 20px;
            text-align: center;
        }
        
        /* Container per grafici circolari */
        .charts-container {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 25px;
            padding: 20px 0;
        }
        
        /* Singolo grafico circolare */
        .chart-wrapper {
            display: flex;
            flex-direction: column;
            align-items: center;
            background: linear-gradient(135deg, #fff 0%, #f8f9fa 100%);
            border-radius: 15px;
            padding: 20px;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
            transition: transform 0.3s ease, box-shadow 0.3s ease;
            position: relative;
            border: 2px solid transparent;
        }
        
        .chart-wrapper:hover {
            transform: translateY(-5px);
            box-shadow: 0 8px 25px rgba(0, 0, 0, 0.15);
            border-color: #667eea;
        }
        
        .chart-label {
            font-size: 1.2rem;
            font-weight: 600;
            color: #2c3e50;
            margin-bottom: 15px;
            text-align: center;
        }
        
        .chart-canvas-container {
            position: relative;
            width: 200px;
            height: 200px;
            margin-bottom: 15px;
        }
        
        .chart-canvas {
            width: 100% !important;
            height: 100% !important;
        }
        
        .chart-center-label {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            font-size: 1.8rem;
            font-weight: 700;
            color: #2c3e50;
            text-shadow: 1px 1px 2px rgba(255, 255, 255, 0.8);
            pointer-events: none;
        }
        
        .chart-details {
            font-size: 0.95rem;
            color: #7f8c8d;
            text-align: center;
            line-height: 1.4;
        }
        
        .chart-status {
            margin-top: 10px;
            padding: 5px 15px;
            border-radius: 20px;
            font-size: 0.85rem;
            font-weight: 600;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        
        .status-good {
            background: linear-gradient(135deg, #2ecc71, #27ae60);
            color: white;
        }
        
        .status-warning {
            background: linear-gradient(135deg, #f39c12, #e67e22);
            color: white;
        }
        
        .status-critical {
            background: linear-gradient(135deg, #e74c3c, #c0392b);
            color: white;
        }
        
        /* Loading state */
        .chart-loading {
            opacity: 0.7;
            pointer-events: none;
        }
        
        .chart-loading::after {
            content: '⏳';
            position: absolute;
            top: 10px;
            right: 10px;
            font-size: 1.2rem;
            animation: spin 1s linear infinite;
        }
        
        @keyframes spin {
            from { transform: rotate(0deg); }
            to { transform: rotate(360deg); }
        }
        
        /* Responsive */
        @media (max-width: 768px) {
            .charts-container {
                grid-template-columns: 1fr;
                gap: 15px;
            }
            
            .chart-wrapper {
                padding: 15px;
            }
            
            .chart-canvas-container {
                width: 150px;
                height: 150px;
            }
            
            .chart-center-label {
                font-size: 1.4rem;
            }
            
            .monitor-title {
                font-size: 1.6rem;
            }
        }
        
        @media (max-width: 480px) {
            .monitor-container {
                padding: 10px;
            }
            
            .monitor-section {
                padding: 15px;
                margin-bottom: 20px;
            }
            
            .chart-canvas-container {
                width: 120px;
                height: 120px;
            }
        }
    )";
    return monitorGraphCss;
}

String viewGeneric::addMonitorGraphStyleCss(){
    String monitorGraphStyleCss = "<style>";
    monitorGraphStyleCss += viewGeneric::addMonitorGraphCss();
    monitorGraphStyleCss += "</style>";
    return monitorGraphStyleCss;
}



