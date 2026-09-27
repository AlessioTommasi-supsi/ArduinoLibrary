#ifndef CUSTOM_BUTTON_MODEL_H
#define CUSTOM_BUTTON_MODEL_H

#include <Arduino.h>
#include <nvs_flash.h>
#include <nvs.h>
#include <vector>

namespace smarthome {

struct CustomButtonItem {
    String label;
    String emoji;
    String url;
};

class CustomButtonModel {
private:
    std::vector<CustomButtonItem> customButtons_;

public:
    CustomButtonModel();
    ~CustomButtonModel();

    void loadCustomButtons();
    void saveCustomButtons();
    void addButton(const String &label, const String &emoji, const String &url);
    void removeButton(const String &label);
    const std::vector<CustomButtonItem>& getButtons() const;
    int getValidGpio(int pinNum);
};

} // namespace smarthome

#endif // CUSTOM_BUTTON_MODEL_H
