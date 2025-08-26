#ifndef HTML_HEADER_MANAGER_H
#define HTML_HEADER_MANAGER_H

#include <Arduino.h>

class HtmlHeaderManager
{
public:
    // Header HTML
    static String getDefaultHeader();
    static String getDefaultHeader(String title);
    static String getBasicHeader(String title);
    static String getDefaultCssHeader(String title);
    
    // Footer HTML
    static String getDefaultFooter();
    
    // Portfolio CSS specifico
    static String getPortfolioCss();

private:
    static String html;
};

#endif // HTML_HEADER_MANAGER_H