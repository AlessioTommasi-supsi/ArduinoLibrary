#ifndef CUSTOM_BUTTON_MODEL_H
#define CUSTOM_BUTTON_MODEL_H

#include <nvs_flash.h>
#include <nvs.h>
#include <vector>
#include <string>

namespace smarthome {

struct CustomButtonItem {
    std::string label;
    std::string emoji;
    std::string url;
};

class CustomButtonModel {
private:
    std::vector<CustomButtonItem> customButtons_;

public:
    CustomButtonModel();
    ~CustomButtonModel();

    void loadCustomButtons();
    void saveCustomButtons();
    void addButton(const std::string &label, const std::string &emoji, const std::string &url);
    void removeButton(const std::string &label);
    const std::vector<CustomButtonItem>& getButtons() const;
    int getValidGpio(int pinNum);
};

} // namespace smarthome

#endif // CUSTOM_BUTTON_MODEL_H
