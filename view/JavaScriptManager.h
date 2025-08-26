#ifndef JAVASCRIPT_MANAGER_H
#define JAVASCRIPT_MANAGER_H

#include <Arduino.h>

class JavaScriptManager
{
public:
    // Script per aggiornamento dinamico contenuto
    static String getDynamicUpdateContentScript();
    static String getFetchContentScript();
    static String getDynamicUpdateContent(String divId, String api, int timeToUpdate);
};

#endif // JAVASCRIPT_MANAGER_H