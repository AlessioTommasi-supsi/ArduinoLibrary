#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <Arduino.h>
#include <Preferences.h>
#include "Config.h"

// Struttura per bottoni personalizzati
struct CustomButton {
    String label;
    String emoji;
    String url;
    
    CustomButton() {}
    CustomButton(const String& l, const String& e, const String& u) : label(l), emoji(e), url(u) {}
};

class CustomButtonManager {
private:
    CustomButton buttons[MAX_CUSTOM_BUTTONS];
    int buttonCount;
    Preferences preferences;
    
public:
    CustomButtonManager();
    
    // Gestione bottoni
    bool addButton(const String& label, const String& emoji, const String& url);
    bool removeButton(int index);
    bool removeButton(const String& label);
    
    // Accesso dati
    int getButtonCount() const { return buttonCount; }
    const CustomButton& getButton(int index) const { return buttons[index]; }
    bool isFull() const { return buttonCount >= MAX_CUSTOM_BUTTONS; }
    
    // Persistenza
    void loadButtons();
    void saveButtons();
    
    // Utility
    void clear();
    String generateButtonsHTML();
};

#endif // CUSTOMBUTTON_H