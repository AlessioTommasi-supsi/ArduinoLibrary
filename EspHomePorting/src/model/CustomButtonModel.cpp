#include "CustomButtonModel.h"

namespace smarthome {

CustomButtonModel::CustomButtonModel() {
    nvs_flash_init();
    loadCustomButtons();
}

CustomButtonModel::~CustomButtonModel() {}

void CustomButtonModel::loadCustomButtons() {
    customButtons_.clear();
    nvs_handle_t handle;
    if (nvs_open("custom_btn", NVS_READONLY, &handle) == ESP_OK) {
        int32_t count = 0;
        nvs_get_i32(handle, "btn_count", &count);
        for (int i = 0; i < count; i++) {
            char l_buf[64] = {0};
            char e_buf[16] = {0};
            char u_buf[128] = {0};
            size_t l_len = sizeof(l_buf);
            size_t e_len = sizeof(e_buf);
            size_t u_len = sizeof(u_buf);

            nvs_get_str(handle, ("l_" + String(i)).c_str(), l_buf, &l_len);
            nvs_get_str(handle, ("e_" + String(i)).c_str(), e_buf, &e_len);
            nvs_get_str(handle, ("u_" + String(i)).c_str(), u_buf, &u_len);

            if (strlen(l_buf) > 0) {
                customButtons_.push_back({String(l_buf), String(e_buf), String(u_buf)});
            }
        }
        nvs_close(handle);
    }
}

void CustomButtonModel::saveCustomButtons() {
    nvs_handle_t handle;
    if (nvs_open("custom_btn", NVS_READWRITE, &handle) == ESP_OK) {
        nvs_set_i32(handle, "btn_count", (int32_t)customButtons_.size());
        for (size_t i = 0; i < customButtons_.size(); i++) {
            nvs_set_str(handle, ("l_" + String(i)).c_str(), customButtons_[i].label.c_str());
            nvs_set_str(handle, ("e_" + String(i)).c_str(), customButtons_[i].emoji.c_str());
            nvs_set_str(handle, ("u_" + String(i)).c_str(), customButtons_[i].url.c_str());
        }
        nvs_commit(handle);
        nvs_close(handle);
    }
}

void CustomButtonModel::addButton(const String &label, const String &emoji, const String &url) {
    customButtons_.push_back({label, emoji, url});
    saveCustomButtons();
}

void CustomButtonModel::removeButton(const String &label) {
    for (auto it = customButtons_.begin(); it != customButtons_.end(); ++it) {
        if (it->label == label) {
            customButtons_.erase(it);
            break;
        }
    }
    saveCustomButtons();
}

const std::vector<CustomButtonItem>& CustomButtonModel::getButtons() const {
    return customButtons_;
}

int CustomButtonModel::getValidGpio(int pinNum) {
    if (pinNum == 4 || pinNum == 5 || pinNum == 6) {
        return pinNum;
    }
    return pinNum; // Direct ESP32-S3 GPIO
}

} // namespace smarthome
