#pragma SPARK_NO_PREPROCESSOR
#include "light.h"

Light::Light(const int pin, const unsigned int step, const unsigned int period, const std::shared_ptr<IRunner>& runner)
: m_pin(pin), m_enabled(true), m_current_level(0), m_brightness(100),
  m_step(step), m_period(period), m_ramp_direction(0),
  m_runner(runner), m_timer(NULL) {
  pinMode(m_pin, OUTPUT);
  analogWrite(m_pin, m_current_level);
  m_timer = new Timer(m_period, &Light::Step, *this, true);
}

bool Light::IsOn() const { return (m_current_level > 0); }

void Light::Toggle(bool ramp) {
  if (!m_enabled) return;

  // if not a ramping light just do a simple on/off toggle.
  if (!ramp) {
    if (IsOn()) {
      m_current_level = 0;
    } else {
      m_current_level = m_brightness;
    }
    analogWrite(m_pin, m_current_level);
    // we are done so just exit now
    return;
  }

  // if we get here this is a ramping light
  switch (m_ramp_direction) {
    case 0: // light is currently idle
      if (IsOn()) {
        m_ramp_direction = 1;
      } else {
        m_ramp_direction = 2;
      }
      break;
    case 1: // light is currently ramping down
      m_ramp_direction = 2;
      break;
    case 2: // light is currently ramping up
      m_ramp_direction = 1;
      break;
    }
    // start or reset the timer
    m_timer->start();
}

void Light::TurnOff() {
  if (m_timer->isActive()) m_timer->stop();
  m_ramp_direction = 0;
  m_current_level = 0;
  analogWrite(m_pin, m_current_level);
}

void Light::TurnOn() {
  // note: won't work if light is disabled
  if (!IsOn()) Toggle(false);
}

void Light::SetMaximumBrightness(int brightness) {
  if (brightness > 255) {
    brightness = 255;
  } else if (brightness < 0) {
    brightness = 0;
  }
  m_brightness = brightness;
  if (IsOn() && m_ramp_direction == 0) {
    m_current_level = m_brightness;
    analogWrite(m_pin, m_current_level);
  }
}

void Light::Disable() {
  m_enabled = false;
  TurnOff();
}

void Light::Enable() {
  m_enabled = true;
}

void Light::Ramp () {
  if (!m_enabled) return;
  int next_level = m_current_level;
  switch (m_ramp_direction) {
    case 0: //idle
      return;
    case 1: //ramp down
      next_level = next_level - m_step;
      break;
    case 2: //ramp up
      next_level = next_level + m_step;
      break;
  }
  if (next_level > m_brightness) {
    m_current_level = m_brightness;
    m_ramp_direction = 0;
  } else if (next_level < 0) {
    m_current_level = 0;
    m_ramp_direction = 0;
  } else {
    m_current_level = next_level;
    m_timer->start();
  }
  analogWrite(m_pin, m_current_level);
}

void Light::Step () {
  m_runner->Defer([&](void) {
    Ramp();
  });
}
