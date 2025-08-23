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
    // Aggiungi script per aggiornamento dinamico
    html += viewGeneric::dynamicUpdateContentScript();
    
    // Carica dinamicamente tutti i CSS necessari
    html += viewGeneric::dynamicUpdateContent("", "/navbarStyle", UI_AUTO_UPDATE_DISABLED);
    html += viewGeneric::dynamicUpdateContent("", "/emoji_buttonStyle", UI_AUTO_UPDATE_DISABLED);
    
    // Aggiungi navbar
    html += viewGeneric::addNavbar();
    
    // Struttura base della pagina
    html += R"(
    <div class="main_container_absolute">
        <div class="main_container_relative">
            <div id="particles-js"></div>
        </div>
        
        <!-- Contenitore per header dinamico -->
        <div id="homeHeaderContent">
            <!-- Caricato dinamicamente da /homeHeader -->
        </div>

        <!-- Contenitore per contenuto principale dinamico -->
        <div id="homeMainContentContainer">
            <!-- Caricato dinamicamente da /homeMainContent -->
        </div>
        
        <!-- Contenitore per script dinamici -->
        <div id="homeScriptsContainer">
            <!-- Caricato dinamicamente da /homeScripts -->
        </div>
    </div>
    )";
    
    // Script per caricare tutti i componenti dinamicamente
    html += "<script>";
    html += "document.addEventListener('DOMContentLoaded', function() {";
    html += "  loadPageContent('/homeHeader', 'homeHeaderContent', " + String(UI_AUTO_UPDATE_DISABLED) + ");";
    html += "  loadPageContent('/homeMainContent', 'homeMainContentContainer', " + String(UI_AUTO_UPDATE_DISABLED) + ");";
    html += "  loadPageContent('/servicesButtonsOnly', 'servicesContent', " + String(UI_AUTO_UPDATE_DISABLED) + ");";
    html += "  loadPageContent('/homeScripts', 'homeScriptsContainer', " + String(UI_AUTO_UPDATE_DISABLED) + ");";
    html += "});";
    html += "</script>";

    return html;
}
