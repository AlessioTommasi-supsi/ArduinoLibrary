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
    html += viewGeneric::addNavbar();
    html+= R"(
    <div class="main_container_absolute">
        <div class="main_container_relative">
            <div id="particles-js"></div>
    )";

    html+= viewGeneric::addNavbar();

    html += R"(
    <div id="textTypingContainer">
                <div id="textTypingAnimation" class="glassEffect"></div>
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
                        <div class="glass_content">
                            <div class="glass_container">
                                <div class="image-with-text responsive_div">
                                    <img class="project_maxwidth responsive_img"
                                        src="https://raw.githubusercontent.com/AlessioTommasi-supsi/porfolio/main/html/view/images/c.png" alt="Description of the image">
                                    <div class="project_maxwidth responsive_text">
                                        <h3>ALPHA </h3>
                                        <p>
                                            <button class="knowledge-button">C++</button>
                                            <button class="knowledge-button">ESP32</button>
                                            <button class="knowledge-button">MODBUS</button>
                                            <button class="knowledge-button">CSS</button>
                                        </p>
                                        <p>
                                            Acquisizione Locale di Parametri con Hardware Avanzato
                                        </p>
                                        <p>
                                            Il progetto ALPHA `e stato sviluppato nel corso di IoT del Master in Informatica presso SUPSI. Il focus principale `e sull’ESP32 e il protocollo Modbus
                                        </p>
                                        <a href="https://github.com/AlessioTommasi-supsi/iotProject/blob/main/docs/tesi.pdf"> <button
                                                class="download-button">official</button> </a>
                                        <a href="https://github.com/AlessioTommasi-supsi/iotProject/tree/main/docs"> <button
                                            class="download-button">ghithub docs</button> </a>
                                        <a href="https://github.com/AlessioTommasi-supsi/iotProject/tree/main"> <button
                                            class="download-button">official repo</button> </a>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <a href="mailto:alessio.tommasi.lavoro@gmail.com"><button id="GetInTouchBtn">📲</button></a>
                <a href="http://alessiotommasi.com/model/docs/curriculum.pdf"><button id="ResumeBtn">📖</button></a>

                <!--  JavaScript files -->
                <script src="http://alessiotommasi.com/view/js/rotation_on_scroll.js"></script>
                <script src="http://alessiotommasi.com/view/js/autoType.js"></script>
                <script src="http://alessiotommasi.com/view/js/scrollUp.js"></script>
                <script src="http://alessiotommasi.com/view/js/class_switcher.js"></script>

                <script src="http://alessiotommasi.com/view/js/particleLib/particles.js"></script>
                <script src="http://alessiotommasi.com/view/js/particleLib/app.js"></script>

            </div>
        </div>
    </div>
    )";

    return html;
}
