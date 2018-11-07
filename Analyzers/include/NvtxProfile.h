#ifndef NvtxProfile_h
#define NvtxProfile_h

#include "AnalyzerCore.h"

class NvtxProfile : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  NvtxProfile();
  ~NvtxProfile();

};



#endif

