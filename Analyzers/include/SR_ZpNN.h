#ifndef SR_ZpNN_h
#define SR_ZpNN_h

#include "AnalyzerCore.h"

class SR_ZpNN : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEvent();
  void Select_syst_objects(AnalyzerParameter param);
  void executeEventFromParameter(AnalyzerParameter param, std::vector<Electron> electron_all, std::vector<Muon> muons_all, std::vector<Jet> jets_all, std::vector<FatJet> fatjets_all, TString syst_flag, double PDF_weight);
  TH1D *hist_DYPtReweight;

  bool draw_all_plots = false;
  std::vector<JetTagging::Parameters> jtps;
  vector<TString> Triggers_Electron, Triggers_Muon;
  TString TriggerNameForSF_Electron, TriggerNameForSF_Muon;
  double TriggerSafePt_Electron, TriggerSafePt_Muon;
  
  //==== pileup
  int N_VTX;
  TH1D *hist_PUReweight;
  TH1D *hist_PUReweight_Up;
  TH1D *hist_PUReweight_Down;
  double weight_PU, weight_PU_Up, weight_PU_Down;

  //==== Analysis Loops
  void CR_Z_mass(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  void CR_ll_150(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto,
		 int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  void CR_ttbar_dom(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  void CR_EMu90(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto,
		int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  void SR(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  void CR_inv_mll(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto, int N_electron, int N_veto_ele, int N_muon, int N_veto_muon);
  
  //==== Functions
  vector<Particle> RecoPairN(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets);
  TString Get_N_jet_bin(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets);
  vector<FatJet> Fatjet_Z_clean(vector<FatJet> fatjets, Particle ll);
  

  SR_ZpNN();
  ~SR_ZpNN();

  double zpt_reweight = 1.;
  double top_pt_reweight = 1.;

  bool RunFake, RunCF;
  
};



#endif

