#include "NvtxProfile.h"

void NvtxProfile::initializeAnalyzer(){

}

void NvtxProfile::executeEvent(){


  AnalyzerParameter param;

  executeEventFromParameter(param);

}

void NvtxProfile::executeEventFromParameter(AnalyzerParameter param){

  if(!PassMETFilter()) return;
  //cout << "[executeEventFromParameter] MCSample : " << MCSample << endl;

  
  Event ev = GetEvent();
  
  int nPV = ev.nPV();
  double pileup_reweight = mcCorr.GetPileUpWeightAsSampleName(0, nPV);
  
  //cout << "[executeEventFromParameter] pileup_reweight : " << pileup_reweight << endl;
  FillHist("Nvtx", nPV, 1., 100, 0., 100.);



}

NvtxProfile::NvtxProfile(){

}

NvtxProfile::~NvtxProfile(){

}


