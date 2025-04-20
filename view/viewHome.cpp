#include "viewHome.h"
#include "viewGraph.h"

String viewHome::html = "";

String viewHome::generateHTML()
{
    html = "";
    html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += viewGeneric::defaultPorfolioCss();
    html += "<style>";
    html += viewGeneric::addCss();
    html += "</style>";
    html += "</head>";
    html += "<body>";
    pageContent();
    html += viewGeneric::defaultFooter();

    return html;
}

String viewHome::pageContent()
{
    html += viewGeneric::dynamicUpdateContentScript(); //aggiungo script per aggiornamento dinamico
    html += viewGeneric::dynamicUpdateContent("id_navbarStyle", "/navbarStyle", -1); //aggiungo script per aggiornamento dinamico
    html += viewGeneric::dynamicUpdateContent("", "/emoji_buttonStyle", -1); //aggiungo script per aggiornamento dinamico
    html += viewGeneric::addNavbar();
    html+= R"(
    <div class="main_container_absolute">
        <div class="main_container_relative">
            <div id="particles-js"></div>
    )";

    html+= viewGeneric::addNavbar();

    html += R"rawliteral(
    
    <div id="textTypingContainer">
        <div id="textTypingAnimation" class="glassEffect">I am: Developer|</div>
    </div>

    <div>
        <div class="no_overflow">

            <div class="logo_container">
                <div class="gear_logo_container">
                    <img class="gear" src="https://raw.githubusercontent.com/AlessioTommasi-supsi/porfolio/main/html/view/images/gearNobg.png" alt="Immagine senza sfondo">
                </div>
                <div class="logo">
                    <img class="logo_image" src="https://raw.githubusercontent.com/AlessioTommasi-supsi/porfolio/main/html/view/images/logo.png" alt="Immagine senza sfondo">
                </div>
            </div>

            <div class="gear_container_left">
                <img class="gear" src="https://raw.githubusercontent.com/AlessioTommasi-supsi/porfolio/main/html/view/images/gearNobg.png" alt="Immagine senza sfondo">
            </div>

            <div class="gear_container_bottom">
                <img class="gear" src="https://raw.githubusercontent.com/AlessioTommasi-supsi/porfolio/main/html/view/images/gearNobg.png" alt="Immagine senza sfondo">
            </div>
        </div>

        <div class="next_page" id="aboutMeTitle">
            <div class="glass_container">
                
  
                <div class="emoji-button-container">
                    <!-- Esempio: Bottone "Porta" con emoji 🚪 -->
                    <button class="emoji-button" onclick="fetchData(this, 'pulsePin?pin=32')">
                        🚪 
                      <label>Porta studio</label>
                      <!-- Contenitore per l'icona di loading -->
                      <div class="loading-icon">⏳</div>
                    </button>
                    
                    <!-- Puoi aggiungere altri bottoni analoghi con emoji differenti -->
                    <button class="emoji-button" onclick="fetchData(this, 'pulsePin?pin=33')">
                      💡
                      <label>Luce interna studio</label>
                      <div class="loading-icon">⏳</div>
                    </button>
                    
                    <button class="emoji-button" onclick="fetchData(this, 'pulsePin?pin=25')">
                        💡
                        <label>Luce esterna studio</label>
                        <div class="loading-icon">⏳</div>
                    </button> 
                    <button class="emoji-button" onclick="fetchData(this, 'pulsePin?pin=26')">
                        💡
                        <label>Luce presepe</label>
                        <div class="loading-icon">⏳</div>
                    </button> 

                    <button class="emoji-button" onclick="fetchData(this, 'pulsePin?pin=27')">
                        💡
                        <label>Luce cantina</label>
                        <div class="loading-icon">⏳</div>
                    </button>

                    <button class="emoji-button" onclick="fetchData(this, 'pulsePin?pin=14')">
                        💡
                        <label>Luce camino</label>
                        <div class="loading-icon">⏳</div>
                    </button>
                </div>
                
            </div>
            <script>
                // Funzione che viene chiamata al click del bottone.
                // 'button' è l'elemento HTML che è stato cliccato, 'apiUrl' è l'endpoint da chiamare.
                function fetchData(button, apiUrl) {
                  // Aggiunge la classe loading-state, riducendo l'opacità
                  button.classList.add("loading-state");
                  
                  // Esegue la chiamata fetch all'API
                  fetch(apiUrl)
                    .then(response => {
                      if (!response.ok) {
                        throw new Error("Network response was not ok");
                      }
                      return response.json(); // oppure response.text() se la risposta è testuale
                    })
                    .then(data => {
                      console.log("Dati ricevuti:", data);
                      // Qui puoi aggiornare il bottone o altre parti della pagina in base alla risposta.
                    })
                    .catch(err => {
                      console.error("Fetch error:", err);
                      // Puoi mostrare un messaggio di errore nell'interfaccia se desiderato.
                    })
                    .finally(() => {
                      // Rimuove la classe loading-state così il bottone torna alla normalità
                      button.classList.remove("loading-state");
                    });
                }
              </script>
        </div>
        <a href="mailto:alessio.tommasi.lavoro@gmail.com"><button id="GetInTouchBtn">📲</button></a>
        <a href="http://alessiotommasi.com/model/docs/curriculum.pdf"><button id="ResumeBtn">📖</button></a>

        <!--  JavaScript files -->
        <script src="http://alessiotommasi.com/view/js/rotation_on_scroll.js"></script>
        <script src="http://alessiotommasi.com/view/js/autoType.js"></script>
        <script src="http://alessiotommasi.com/view/js/class_switcher.js"></script>

        <script src="http://alessiotommasi.com/view/js/particleLib/particles.js"></script>
        <script src="http://alessiotommasi.com/view/js/particleLib/app.js"></script>

            </div>
        </div>
    </div>
    )rawliteral";

    return html;
}
