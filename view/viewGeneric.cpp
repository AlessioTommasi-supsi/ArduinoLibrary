#include "viewGeneric.h"
#include "Config.h"

String viewGeneric::html = "";

// Header e Footer - delegati a HtmlHeaderManager
String viewGeneric::defaultHeader() {
    return HtmlHeaderManager::getDefaultHeader();
}

String viewGeneric::defaultHeader(String title) {
    return HtmlHeaderManager::getDefaultHeader(title);
}

String viewGeneric::basicHeader(String title) {
    return HtmlHeaderManager::getBasicHeader(title);
}

String viewGeneric::defaultCssHeader(String title) {
    return HtmlHeaderManager::getDefaultCssHeader(title);
}

String viewGeneric::defaultFooter() {
    return HtmlHeaderManager::getDefaultFooter();
}

String viewGeneric::defaultPorfolioCss() {
    return HtmlHeaderManager::getPortfolioCss();
}

// CSS - delegati a CssStyleManager
String viewGeneric::addCss() {
    return CssStyleManager::getBaseCss();
}

String viewGeneric::addFormCss() {
    return CssStyleManager::getFormCss();
}

String viewGeneric::addFormStyleCss() {
    return CssStyleManager::getFormStyleCss();
}

String viewGeneric::addFieldFormCss() {
    return CssStyleManager::getFieldFormCss();
}

String viewGeneric::addFieldFormStyleCss() {
    return CssStyleManager::getFieldFormStyleCss();
}

String viewGeneric::addPinCss() {
    return CssStyleManager::getPinCss();
}

String viewGeneric::addPinStyleCss() {
    return CssStyleManager::getPinStyleCss();
}

String viewGeneric::addNavbarCss() {
    return CssStyleManager::getNavbarCss();
}

String viewGeneric::addNavbarStyleCss() {
    return CssStyleManager::getNavbarStyleCss();
}

String viewGeneric::addEmoji_buttonCss() {
    return CssStyleManager::getEmojiButtonCss();
}

String viewGeneric::addEmoji_buttonStyleCss() {
    return CssStyleManager::getEmojiButtonStyleCss();
}

String viewGeneric::addMonitorGraphCss() {
    return CssStyleManager::getMonitorGraphCss();
}

String viewGeneric::addMonitorGraphStyleCss() {
    return CssStyleManager::getMonitorGraphStyleCss();
}

// Navigazione - delegata a NavigationManager
String viewGeneric::addNavbar() {
    return NavigationManager::getNavbar();
}

// JavaScript - delegato a JavaScriptManager
String viewGeneric::dynamicUpdateContent(String divId, String api, int timeToUpdate) {
    return JavaScriptManager::getDynamicUpdateContent(divId, api, timeToUpdate);
}

String viewGeneric::dynamicUpdateContentScript() {
    return JavaScriptManager::getDynamicUpdateContentScript();
}

String viewGeneric::fetchContentScript() {
    return JavaScriptManager::getFetchContentScript();
}



