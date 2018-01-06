#pragma SPARK_NO_PREPROCESSOR
#ifndef IRUNNER_H
#define IRUNNER_H

#include <Particle.h>

class IRunner : public std::enable_shared_from_this<IRunner> {
public:
  virtual void Defer(std::function<void()> callback) = 0;
};

#endif
