#ifndef CalcCFRate_h
#define CalcCFRate_h

#include "AnalyzerCore.C"

class CalcCFRate : public AnalyzerCore {

public:

  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  void FillCFRatePlots(TString name, Lepton *lep, bool IsCF, double weight);

  vector<double> Electron_FakeRatePtBinnings, Muon_FakeRatePtBinnings;

};



#endif

