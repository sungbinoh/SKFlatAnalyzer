#include "NvtxProfile.h"

void NvtxProfile::initializeAnalyzer(){

}

void NvtxProfile::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void NvtxProfile::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;
  Event ev = GetEvent();

  int nPV = ev.nPV();
  FillHist("Nvtx", nPV, 1., 100, 0., 100.);

}

NvtxProfile::NvtxProfile(){

}

NvtxProfile::~NvtxProfile(){

}


