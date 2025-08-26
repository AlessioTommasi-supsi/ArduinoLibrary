#ifndef CSS_STYLE_MANAGER_H
#define CSS_STYLE_MANAGER_H

#include <Arduino.h>

class CssStyleManager
{
public:
    // CSS base e layout
    static String getBaseCss();
    
    // CSS per form
    static String getFormCss();
    static String getFormStyleCss();
    static String getFieldFormCss();
    static String getFieldFormStyleCss();
    
    // CSS per pin e hardware
    static String getPinCss();
    static String getPinStyleCss();
    
    // CSS per navbar
    static String getNavbarCss();
    static String getNavbarStyleCss();
    
    // CSS per bottoni emoji
    static String getEmojiButtonCss();
    static String getEmojiButtonStyleCss();
    
    // CSS per grafici monitor
    static String getMonitorGraphCss();
    static String getMonitorGraphStyleCss();
    
    // CSS portfolio
    static String getPortfolioCss();
};

#endif // CSS_STYLE_MANAGER_H