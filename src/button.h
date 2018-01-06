#pragma SPARK_NO_PREPROCESSOR
#ifndef BUTTON_H
#define BUTTON_H

#include <Particle.h>

class Button {
public:
  Button(const int pin, const int debounce_timeout,
         std::function<void()> on_press);

  ~Button();

  void HandleInterrupt();

private:
  int m_pin;
  int m_debounce_timeout;
  unsigned long m_last_press;
  bool m_current_state;
  std::function<void()> m_on_press;
};

#endif
