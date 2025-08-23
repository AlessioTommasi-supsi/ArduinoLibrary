#include "viewHome.h"
#include "viewGraph.h"
#include "Config.h"

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
    html += viewGeneric::dynamicUpdateContent("id_navbarStyle", "/navbarStyle", UI_AUTO_UPDATE_DISABLED); //aggiungo script per aggiornamento dinamico
    html += viewGeneric::dynamicUpdateContent("", "/emoji_buttonStyle", UI_AUTO_UPDATE_DISABLED); //aggiungo script per aggiornamento dinamico
    html += viewGeneric::dynamicUpdateContent("servicesContent", "/services", UI_AUTO_UPDATE_DISABLED); //carica contenuto servizi UNA SOLA VOLTA
    html += viewGeneric::addNavbar();
    html+= R"(
    <div class="main_container_absolute">
        <div class="main_container_relative">
            <div id="particles-js"></div>
        </div>
        
        <div id="textTypingContainer">
            <div id="textTypingAnimation" class="glassEffect">I am: Developer|</div>
        </div>

        <div>
            <div class="no_overflow">
                <div class="logo_container">
                    <div class="gear_logo_container">
                        <img class="gear" src="https://alessiotommasi.com/view/images/gearNobg.png" alt="Immagine senza sfondo">
                    </div>
                    <div class="logo">
                        <img class="logo_image" src="https://alessiotommasi.com/view/images/logo.png" alt="Immagine senza sfondo">
                    </div>
                </div>

                <div class="gear_container_left">
                    <img class="gear" src="https://alessiotommasi.com/view/images/gearNobg.png" alt="Immagine senza sfondo">
                </div>

                <div class="gear_container_bottom">
                    <img class="gear" src="https://alessiotommasi.com/view/images/gearNobg.png" alt="Immagine senza sfondo">
                </div>
            </div>

            <!-- Contenitore per il contenuto dinamico dei servizi -->
            <div id="servicesContent">
                <!-- Il contenuto verrà caricato dinamicamente da /services -->
            </div>
            
            <a href="mailto:alessio.tommasi.lavoro@gmail.com"><button id="GetInTouchBtn">📲</button></a>
            <a href="http://alessiotommasi.com/model/docs/curriculum.pdf"><button id="ResumeBtn">📖</button></a>

            <!-- JavaScript per gestire i click sui bottoni dei servizi -->
            <script>
                function fetchData(button, apiUrl) {
                    button.classList.add('loading-state');
                    
                    fetch(apiUrl)
                        .then(response => {
                            if (!response.ok) {
                                throw new Error('Network response was not ok');
                            }
                            return response.text();
                        })
                        .then(data => {
                            console.log('Dati ricevuti:', data);
                        })
                        .catch(err => {
                            console.error('Fetch error:', err);
                        })
                        .finally(() => {
                            button.classList.remove('loading-state');
                        });
                }
            </script>

            <!--  JavaScript files -->
            <script src="http://alessiotommasi.com/view/js/rotation_on_scroll.js"></script>
            <script src="http://alessiotommasi.com/view/js/autoType.js"></script>
            <script src="http://alessiotommasi.com/view/js/class_switcher.js"></script>

            <script src="http://alessiotommasi.com/view/js/particleLib/particles.js"></script>
            <script src="http://alessiotommasi.com/view/js/particleLib/app.js"></script>
        </div>
    </div>
    )";

    return html;
}
