#ifndef VIEWGENERIC_H
#define VIEWGENERIC_H

#include <Arduino.h>


class viewGeneric
{
    private:

    public:
        static String html;
        
        static String defaultHeader();
        static String defaultHeader(String title);
        static String defaultCssHeader(String title);
        static String defaultPorfolioCss();
        static String defaultFooter();
        static String addCss();
        static String addNavbar();
        static String dynamicUpdateContent(String divId, String api, int timeToUpdate);

};

#endif // !VIEWGENERIC_H#define VIEWGENERIC_H
