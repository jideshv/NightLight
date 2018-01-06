#pragma SPARK_NO_PREPROCESSOR

#include <Particle.h>
#include <list>
#include "nightlight.h"

std::shared_ptr<NightLightDevice> night_light;

void setup()
{
  night_light = std::make_shared<NightLightDevice>();
  night_light->Initialize();
}

void loop()
{
  night_light->Run();
}
