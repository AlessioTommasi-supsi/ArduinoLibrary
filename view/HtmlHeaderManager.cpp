#include "HtmlHeaderManager.h"
#include "CssStyleManager.h"
#include "NavigationManager.h"
#include "JavaScriptManager.h"
#include "Config.h"

String HtmlHeaderManager::html = "";

String HtmlHeaderManager::getDefaultHeader() {
    html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<title>DefaultTitle</title>";
    html += "</head>";
    html += "<body>";
    html += NavigationManager::getNavbar();
    return html;
}

String HtmlHeaderManager::getBasicHeader(String title) {
    html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += R"(
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
    )";
    html += "<title>";
    html += title;
    html += "</title>";
    html += "<style>";
    html += CssStyleManager::getBaseCss();
    html += "</style>";
    html += "</head>";
    html += "<body>";
    
    return html;
}

String HtmlHeaderManager::getDefaultCssHeader(String title) {
    html = getBasicHeader(title);
    html += NavigationManager::getNavbar();
    
    // Aggiungo gli script comuni a tutti i documenti
    String css = "";
    css += JavaScriptManager::getDynamicUpdateContentScript();
    css += JavaScriptManager::getDynamicUpdateContent("", "/formStyle", UI_AUTO_UPDATE_DISABLED);
    css += JavaScriptManager::getDynamicUpdateContent("", "/pinStyle", UI_AUTO_UPDATE_DISABLED);
    css += JavaScriptManager::getDynamicUpdateContent("", "/navbarStyle", UI_AUTO_UPDATE_DISABLED);

    html += css;
    return html;
}

String HtmlHeaderManager::getDefaultHeader(String title) {
    html = "<!DOCTYPE html>";
    html += "<html>";
    html += "<head>";
    html += "<title>";
    html += title;
    html += "</title>";
    html += "</head>";
    html += "<body>";
    html += NavigationManager::getNavbar();
    return html;
}

String HtmlHeaderManager::getDefaultFooter() {
    html = "</body>";
    html += "</html>";
    return html;
}

String HtmlHeaderManager::getPortfolioCss() {
    return CssStyleManager::getPortfolioCss();
}