#include "HomeRoutes.h"
#include "Config.h"

void HomeRoutes::defineRoutes(AsyncWebServer &server)
{
    // Route per componenti home page
    server.on("/homeHeader", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            String content = generateHomeHeaderContent();
            request->send(200, "text/html", content);
        }
        catch (...)
        {
            request->send(500, "text/html", "Error loading home header");
        }
    });
    
    server.on("/homeMainContent", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            String content = generateHomeMainContent();
            request->send(200, "text/html", content);
        }
        catch (...)
        {
            request->send(500, "text/html", "Error loading home main content");
        }
    });
    
    server.on("/homeScripts", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        try 
        {
            String content = generateHomeScriptsContent();
            request->send(200, "text/html", content);
        }
        catch (...)
        {
            request->send(500, "text/html", "Error loading home scripts");
        }
    });
}

String HomeRoutes::generateHomeHeaderContent()
{
    String content = R"(
        <div id="textTypingContainer">
            <div id="textTypingAnimation" class="glassEffect">I am: Developer|</div>
        </div>
    )";
    return content;
}

String HomeRoutes::generateHomeMainContent()
{
    String content = R"(
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
                <!-- Il contenuto verrà caricato dinamicamente da questo script -->
            </div>
            
            <a href="mailto:alessio.tommasi.lavoro@gmail.com"><button id="GetInTouchBtn">📲</button></a>
            <a href="http://alessiotommasi.com/model/docs/curriculum.pdf"><button id="ResumeBtn">📖</button></a>
        </div>
    )";
    return content;
}

String HomeRoutes::generateHomeScriptsContent()
{
    String content = R"(
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
    )";
    return content;
}