#pragma SPARK_NO_PREPROCESSOR
#include "button.h"

Button::Button(const int pin, const int debounce_timeout,
       std::function<void(void)> on_press)
: m_pin(pin),
m_debounce_timeout(debounce_timeout),
m_last_press(0),
m_current_state(false),
m_on_press(on_press) {
  pinMode(m_pin, INPUT_PULLUP);
  attachInterrupt(m_pin, std::bind(&Button::HandleInterrupt, this), RISING);
}

Button::~Button() {
  detachInterrupt(m_pin);
}

void Button::HandleInterrupt() {
  unsigned long current_time = millis();
  if ((current_time - m_last_press) > m_debounce_timeout)
  {
    m_last_press = current_time;
    m_on_press();
  }
}
