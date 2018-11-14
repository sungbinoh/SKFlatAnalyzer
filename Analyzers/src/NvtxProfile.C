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

  TString trig_mu50 = "HLT_Mu50_v";
  bool Pass_mu50 = ev.PassTrigger(trig_mu50);
  int n_true_vertex = nPileUp;
  int nPV = ev.nPV();
  //double pileup_reweight = mcCorr.GetPileUpWeightAsSampleName(0, nPV);
  
  //cout << "[executeEventFromParameter] pileup_reweight : " << pileup_reweight << endl;
  FillHist("Nvtx", nPV, 1., 100, 0., 100.);
  FillHist("n_true_vertex", n_true_vertex, 1., 100, 0., 100.);
  if(Pass_mu50) FillHist("Nvtx_HLT_Mu50", nPV, 1., 100, 0., 100.);


}

NvtxProfile::NvtxProfile(){

}

NvtxProfile::~NvtxProfile(){

}


