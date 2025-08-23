#include "CustomButton.h"

CustomButtonManager::CustomButtonManager() : buttonCount(0) {
    preferences.begin(CUSTOM_BUTTONS_PREF_NAMESPACE, false);
    loadButtons();
}

bool CustomButtonManager::addButton(const String& label, const String& emoji, const String& url) {
    if (isFull()) return false;
    
    // Controlla se esiste già un bottone con la stessa etichetta
    for (int i = 0; i < buttonCount; i++) {
        if (buttons[i].label.equals(label)) return false;
    }
    
    buttons[buttonCount] = CustomButton(label, emoji, url);
    buttonCount++;
    saveButtons();
    return true;
}

bool CustomButtonManager::removeButton(int index) {
    if (index < 0 || index >= buttonCount) return false;
    
    // Sposta tutti i bottoni dopo l'indice verso sinistra
    for (int i = index; i < buttonCount - 1; i++) {
        buttons[i] = buttons[i + 1];
    }
    buttonCount--;
    saveButtons();
    return true;
}

bool CustomButtonManager::removeButton(const String& label) {
    for (int i = 0; i < buttonCount; i++) {
        if (buttons[i].label.equals(label)) {
            return removeButton(i);
        }
    }
    return false;
}

void CustomButtonManager::loadButtons() {
    buttonCount = preferences.getInt("button_count", 0);
    
    for (int i = 0; i < buttonCount && i < MAX_CUSTOM_BUTTONS; i++) {
        String prefix = "btn" + String(i) + "_";
        
        buttons[i].label = preferences.getString((prefix + "label").c_str(), "");
        buttons[i].emoji = preferences.getString((prefix + "emoji").c_str(), "");
        buttons[i].url = preferences.getString((prefix + "url").c_str(), "");
    }
    
    Serial.printf("📱 Caricati %d bottoni personalizzati\n", buttonCount);
}

void CustomButtonManager::saveButtons() {
    preferences.putInt("button_count", buttonCount);
    
    for (int i = 0; i < buttonCount; i++) {
        String prefix = "btn" + String(i) + "_";
        
        preferences.putString((prefix + "label").c_str(), buttons[i].label);
        preferences.putString((prefix + "emoji").c_str(), buttons[i].emoji);
        preferences.putString((prefix + "url").c_str(), buttons[i].url);
    }
    
    Serial.printf("💾 Salvati %d bottoni personalizzati\n", buttonCount);
}

void CustomButtonManager::clear() {
    buttonCount = 0;
    preferences.clear();
}

String CustomButtonManager::generateButtonsHTML() {
    String content = "";
    
    for (int i = 0; i < buttonCount; i++) {
        const CustomButton& button = buttons[i];
        
        content += "        <div class=\"emoji-button-wrapper\" style=\"position: relative;\">";
        content += "          <button class=\"emoji-button\" onclick=\"fetchData(this, '" + button.url + "')\">";
        content += "            " + button.emoji;
        content += "            <label>" + button.label + "</label>";
        content += "            <div class=\"loading-icon\">&#x23F3;</div>";
        content += "          </button>";
        // Aggiungi bottone di cancellazione
        content += "          <button class=\"delete-btn\" onclick=\"if(confirm('Rimuovere " + button.label + "?')) window.location.href='/removeButton?label=" + button.label + "'\" ";
        content += "            style=\"position: absolute; top: 5px; right: 5px; background: rgba(255,0,0,0.7); color: white; border: none; border-radius: 50%; width: 25px; height: 25px; font-size: 12px; cursor: pointer; z-index: 10;\">";
        content += "            &#x2715;"; // ✕
        content += "          </button>";
        content += "        </div>";
    }
    
    return content;
}