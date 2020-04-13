#ifndef Signal_study_h
#define Signal_study_h

#include "AnalyzerCore.h"

class Signal_study : public AnalyzerCore {

public:

  void initializeAnalyzer();

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  bool RunSyst;
  bool RunNewPDF;
  bool RunXSecSyst;

  std::vector<JetTagging::Parameters> jtps;
  vector<TString> Triggers_Electron, Triggers_Muon;
  TString TriggerNameForSF_Electron, TriggerNameForSF_Muon;
  double TriggerSafePt_Electron, TriggerSafePt_Muon;
  
  int N_VTX;
  TH1D *hist_PUReweight;
  TH1D *hist_PUReweight_Up;
  TH1D *hist_PUReweight_Down;
  double weight_PU, weight_PU_Up, weight_PU_Down;

  TString IsoMuTriggerName;
  double TriggerSafePtCut;

  vector<TString> MuonIDs, MuonIDSFKeys;
  vector<Muon> AllMuons;
  vector<Jet> AllJets;

  double weight_Prefire;

  Signal_study();
  ~Signal_study();

};



#endif

