#pragma once

#include <cmath>
#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "light_output.h"
#include "presets.h"

namespace esphome {
namespace xiaomi {
namespace bslamp2 {
    
class BrightnessTrigger : public Trigger<float> {
public:
    explicit BrightnessTrigger(XiaomiBslamp2LightOutput *parent) {
        parent->add_on_state_callback([this](light::LightColorValues values) {
            auto new_brightness = values.get_brightness();
            if (values.get_state() == 0) {
                new_brightness = 0.0f;
            }
            new_brightness = roundf(new_brightness * 100.0f) / 100.0f;
            if (last_brightness_ != new_brightness) {
                trigger(new_brightness);
                last_brightness_ = new_brightness;
            }
        });
    }
protected:
    float last_brightness_ = -1.0f;
};

template<typename... Ts> class ActivatePresetAction : public Action<Ts...> {
public:
    explicit ActivatePresetAction(PresetsContainer *presets) : presets_(presets) {}

    TEMPLATABLE_VALUE(std::string, operation);
    TEMPLATABLE_VALUE(std::string, group);
    TEMPLATABLE_VALUE(std::string, preset);

    void play(Ts... x) override { 
        auto operation = this->operation_.value(x...);

        if (operation == "next_group") {
            presets_->activate_next_group();
        } else if (operation == "next_preset") {
            presets_->activate_next_preset();
        } else if (operation == "activate_group") {
            auto group = this->group_.value(x...);
            presets_->activate_group(group);
        } else if (operation == "activate_preset") {
            auto group = this->group_.value(x...);
            auto preset = this->preset_.value(x...);
            presets_->activate_preset(group, preset);
        }
    }

protected:
    PresetsContainer *presets_;
};

} // namespace bslamp2
} // namespace xiaomi
} // namespace esphome
