#ifndef VIEWGENERIC_H
#define VIEWGENERIC_H

#include <Arduino.h>
#include "CssStyleManager.h"
#include "NavigationManager.h"
#include "JavaScriptManager.h"
#include "HtmlHeaderManager.h"

class viewGeneric
{
public:
    // Manteniamo la compatibilità con l'interfaccia esistente
    static String html;
    
    // Header e Footer - delegati a HtmlHeaderManager
    static String defaultHeader();
    static String defaultHeader(String title);
    static String basicHeader(String title);
    static String defaultCssHeader(String title);
    static String defaultFooter();
    static String defaultPorfolioCss();
    
    // CSS - delegati a CssStyleManager
    static String addCss();
    static String addFormCss();
    static String addFormStyleCss();
    static String addFieldFormCss();
    static String addFieldFormStyleCss();
    static String addPinCss();
    static String addPinStyleCss();
    static String addNavbarCss();
    static String addNavbarStyleCss();
    static String addEmoji_buttonCss();
    static String addEmoji_buttonStyleCss();
    static String addMonitorGraphCss();
    static String addMonitorGraphStyleCss();
    
    // Navigazione - delegata a NavigationManager
    static String addNavbar();
    
    // JavaScript - delegato a JavaScriptManager
    static String dynamicUpdateContent(String divId, String api, int timeToUpdate);
    static String dynamicUpdateContentScript();
    static String fetchContentScript();
};

#endif // VIEWGENERIC_H
