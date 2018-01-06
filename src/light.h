#pragma SPARK_NO_PREPROCESSOR
#ifndef LIGHT_H
#define LIGHT_H

#include <Particle.h>
#include "IRunner.h"

class Light {
public:
  Light(const int pin, const unsigned int step, const unsigned int period, const std::shared_ptr<IRunner>& runner);

  ~Light() = default;

  bool IsOn() const;

  void TurnOff();

  void TurnOn();

  void SetMaximumBrightness(int brightness);

  void Disable();

  void Enable();

  void Toggle(bool ramp);

  void Ramp();

  void Step();

private:
  int m_pin;
  bool m_enabled;
  int m_current_level;
  int m_brightness;
  unsigned int m_step;
  unsigned int m_period;
  int m_ramp_direction;
  std::shared_ptr<IRunner> m_runner;
  Timer* m_timer;
};

#endif
