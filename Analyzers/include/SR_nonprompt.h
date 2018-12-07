#ifndef SR_nonprompt_h
#define SR_nonprompt_h

#include "AnalyzerCore.h"

class SR_nonprompt : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEvent();
  void Select_syst_objects(AnalyzerParameter param);
  void executeEventFromParameter(AnalyzerParameter param, std::vector<Electron> electron_all, std::vector<Muon> muons_all, std::vector<Jet> jets_all, std::vector<FatJet> fatjets_all, TString syst_flag, double PDF_weight);

  
  void CR_Z_mass(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  void CR_ttbar_dom(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  void SR(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  void CR_inv_mll(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  
  vector<Particle> RecoPairN(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets);
  TString Get_N_jet_bin(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets);
  
  

  SR_nonprompt();
  ~SR_nonprompt();

  bool RunFake, RunCF;
  

};



#endif

