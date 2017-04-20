#ifndef __IPLUGEFFECTCAIROGRAPHICS__
#define __IPLUGEFFECTCAIROGRAPHICS__

#include "IPlug_include_in_plug_hdr.h"

class IPlugEffectCairoGraphics : public IPlug
{
public:
  IPlugEffectCairoGraphics(IPlugInstanceInfo instanceInfo);
  ~IPlugEffectCairoGraphics();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  double mGain = 1.0;
};

#endif
