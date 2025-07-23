#include "viewHome.h"
#include "viewGraph.h"

String viewHome::html = "";

String viewHome::generateHTML()
{
    // Header minimalista senza CSS esterni
    html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1.0'>";
    html += "<title>ALPHA</title>";
    html += "<style>body{margin:0;padding:0;background:#f4f4f4}</style>";
    html += "</head><body>";
    
    // Carica tutto dinamicamente per ridurre heap iniziale
    html += viewGeneric::dynamicUpdateContentScript();
    html += viewGeneric::dynamicUpdateContent("main_content", "/homePageContent", -1);
    html += "<div id='main_content'><div style='text-align:center;padding:50px'>Loading...</div></div>";
    
    html += "</body></html>";
    return html;
}

String viewHome::pageContent()
{
    html = "";
    
    // CSS inline minimalista solo per questa pagina
    html += "<style>";
    html += ".main_container_absolute{position:relative;width:100%;min-height:100vh}";
    html += ".glass_container{background:rgba(255,255,255,0.9);border-radius:15px;padding:20px;margin:20px;max-width:600px}";
    html += "@media(min-width:1500px){.glass_container{margin-left:120px!important}}";
    html += "</style>";
    
    // Navbar dinamica
    html += viewGeneric::dynamicUpdateContent("navbar_area", "/navbarStyle", -1);
    html += "<div id='navbar_area'></div>";
    html += viewGeneric::addNavbar();
    
    html += "<div class='main_container_absolute'>";
    html += "<div class='main_container_relative'>";
    
    // Contenuto principale frammentato
    html += "<div class='glass_container'>";
    html += "<h1>🏠 Sistema IoT</h1>";
    html += "<p>Benvenuto nel sistema di monitoraggio IoT</p>";
    
    // Links di navigazione semplificati
    html += "<div style='display:flex;flex-wrap:wrap;gap:10px;margin:20px 0'>";
    html += "<a href='/monitor' style='background:#4CAF50;color:white;padding:10px 15px;text-decoration:none;border-radius:5px;font-size:14px'>🖥️ Monitor</a>";
    html += "<a href='/pinout' style='background:#2196F3;color:white;padding:10px 15px;text-decoration:none;border-radius:5px;font-size:14px'>🔌 Pinout</a>";
    html += "<a href='/currentregister' style='background:#FF9800;color:white;padding:10px 15px;text-decoration:none;border-radius:5px;font-size:14px'>📋 Register</a>";
    html += "<a href='/graph' style='background:#9C27B0;color:white;padding:10px 15px;text-decoration:none;border-radius:5px;font-size:14px'>📈 Graph</a>";
    html += "</div>";
    html += "</div>";
    
    html += "</div></div>";
    return html;
}
