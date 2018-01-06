#pragma SPARK_NO_PREPROCESSOR
#ifndef NIGHTLIGHT_H
#define NIGHTLIGHT_H

#include <Particle.h>
#include <map>
#include <list>
#include "button.h"
#include "light.h"
#include "pins.h"
#include "IRunner.h"

#define DEBOUNCE_TIMEOUT 100 // number of milliseconds to debounce buttons
#define BRIGHTNESS_STEP 1 // level to change at each step
#define RAMP_PERIOD 20 // number of milliseconds between brightness steps
#define LIGHTS_OUT 16 // GST hour when all lights go off

class NightLightDevice : public IRunner {
public:
  NightLightDevice();

  ~NightLightDevice() = default;

  void Initialize();

  void Run();

  virtual void Defer(std::function<void()> callback) override;

  int CloudHandler(String command);

  void CheckLightsOutTime();

private:
  std::shared_ptr<Light> GetLight (String command);
  std::map<int, std::shared_ptr<Button>> m_buttons;
  std::map<int, std::shared_ptr<Light>> m_lights;
  std::list<std::function<void()>> m_events;
  bool m_lights_out_ran;
  unsigned long m_last_time_check;
};

#endif
