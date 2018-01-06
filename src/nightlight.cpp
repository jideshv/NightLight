#pragma SPARK_NO_PREPROCESSOR
#include "nightlight.h"

NightLightDevice::NightLightDevice() : m_lights_out_ran(false) {
  RGB.control(true);
  RGB.color(0, 0, 0);
}

void NightLightDevice::Initialize() {
  m_lights[CHILD_ONE_LIGHT] = std::make_shared<Light>(CHILD_ONE_LIGHT, BRIGHTNESS_STEP, RAMP_PERIOD, shared_from_this());
  m_lights[CHILD_TWO_LIGHT] = std::make_shared<Light>(CHILD_TWO_LIGHT, BRIGHTNESS_STEP, RAMP_PERIOD, shared_from_this());
  m_lights[NIGHT_LIGHT] = std::make_shared<Light>(NIGHT_LIGHT, BRIGHTNESS_STEP, RAMP_PERIOD, shared_from_this());

  m_buttons[CHILD_ONE_BUTTON] = std::make_shared<Button>(CHILD_ONE_BUTTON, DEBOUNCE_TIMEOUT, [&](void){
    Defer([=](){m_lights[CHILD_ONE_LIGHT]->Toggle(true);});
  });

  m_buttons[CHILD_TWO_BUTTON] = std::make_shared<Button>(CHILD_TWO_BUTTON, DEBOUNCE_TIMEOUT, [&](void){
    Defer([=](){m_lights[CHILD_TWO_LIGHT]->Toggle(true);});
  });

  Particle.function("nightlight", &NightLightDevice::CloudHandler, this);
}

void NightLightDevice::Run() {
  if (!m_events.empty()) {
    std::function<void()> event = m_events.front();
    event();
    m_events.pop_front();
  }
  CheckLightsOutTime();
}

void NightLightDevice::Defer(std::function<void()> callback) {
  m_events.push_back(callback);
}

std::shared_ptr<Light> NightLightDevice::GetLight (String command) {
  command.remove(0,1);
  switch (command.toInt()) {
    case 0:
      return m_lights[NIGHT_LIGHT];
    case 1:
      return m_lights[CHILD_ONE_LIGHT];
    case 2:
      return m_lights[CHILD_TWO_LIGHT];
    }
  return nullptr;
}

int NightLightDevice::CloudHandler(String command) {
  if (command.startsWith("b")) {
    command.remove(0,1);
    int brightness = command.toInt();
    Defer([=](){
      m_lights[CHILD_ONE_LIGHT]->SetMaximumBrightness(brightness);
      m_lights[CHILD_TWO_LIGHT]->SetMaximumBrightness(brightness);
      m_lights[NIGHT_LIGHT]->SetMaximumBrightness(brightness);
    });
    return 1;
  } else if (command.startsWith("+")) {
    auto light = GetLight(command);
    if (light) Defer([=](){light->TurnOn();}); return 1;
  } else if (command.startsWith("-")) {
    auto light = GetLight(command);
    if (light) Defer([=](){light->TurnOff();}); return 1;
  } else if (command.startsWith("d")) {
    auto light = GetLight(command);
    if (light) Defer([=](){light->Disable();}); return 1;
  } else if (command.startsWith("e")) {
    auto light = GetLight(command);
    if (light) Defer([=](){light->Enable();}); return 1;
  }
  return -1;
}

void NightLightDevice::CheckLightsOutTime() {
  if (Time.hour() == LIGHTS_OUT) {
    if (m_lights_out_ran) return;
    for (auto& light_it : m_lights) {
      Defer([=](){light_it.second->TurnOff();});
    }
    m_lights_out_ran = true;
  } else {
    m_lights_out_ran = false;
  }
}
