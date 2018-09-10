#ifndef HNPairAnalyzer_h
#define HNPairAnalyzer_h

#include "AnalyzerCore.C"

class HNPairAnalyzer : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  vector<Particle> RecoPairN(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets);

  HNPairAnalyzer();
  ~HNPairAnalyzer();

  bool RunFake, RunCF;

};



#endif

