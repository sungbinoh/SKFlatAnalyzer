#include "Signal_study.h"

void Signal_study::initializeAnalyzer(){

  Triggers_Electron.clear();
  Triggers_Muon.clear();

  if(DataYear==2016){

    Triggers_Electron = {
      "HLT_DoublePhoton60_v"
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_TkMu50_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 65.;
    TriggerSafePt_Muon = 52.;

  }
  else if(DataYear==2017){

    Triggers_Electron = {
      "HLT_DoublePhoton70_v"
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 75.;
    TriggerSafePt_Muon = 52.;

  }
  else if(DataYear==2018){

    Triggers_Electron = {
      "HLT_DoublePhoton70_v"
    };
    Triggers_Muon = {
      "HLT_Mu50_v",
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
    };
    TriggerNameForSF_Electron = "WREGammaTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 75.;
    TriggerSafePt_Muon = 52.;

  }

  
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::mujets, JetTagging::mujets) );
  mcCorr->SetJetTaggingParameters(jtps);

  if(!IsDATA){
    TString datapath = getenv("DATA_DIR");

    TString PUfname = datapath+"/"+TString::Itoa(DataYear,10)+"/PileUp/PUReweight_"+TString::Itoa(DataYear,10)+".root";
    TString PUhname = "PUReweight_"+TString::Itoa(DataYear,10);

    cout << "[HNWRAnalyzer::initializeAnalyzer()] PUfname = " << PUfname << endl;
    cout << "[HNWRAnalyzer::initializeAnalyzer()] PUhname = " << PUhname << endl;

    TFile *file_PUReweight = new TFile(PUfname);
    hist_PUReweight = (TH1D *)file_PUReweight->Get(PUhname);
    hist_PUReweight_Up = (TH1D *)file_PUReweight->Get(PUhname+"_Up");
    hist_PUReweight_Down = (TH1D *)file_PUReweight->Get(PUhname+"_Down");

  }

}

void Signal_study::executeEvent(){
  
  Event ev_dem = GetEvent();
  
  N_VTX = nPV;
  if(!IsDATA){
    N_VTX = nPileUp+1;
    int bin_pu = hist_PUReweight->FindBin(nPileUp);
    weight_PU = hist_PUReweight->GetBinContent(bin_pu);
    weight_PU_Up = hist_PUReweight_Up->GetBinContent(bin_pu);
    weight_PU_Down = hist_PUReweight_Down->GetBinContent(bin_pu);
  }
  
  AnalyzerParameter param;
  param.Clear();

  param.Name = "HNPair";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HEEP_dZ";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "CutBasedLooseNoIso";
  param.Electron_ID_SF_Key = "HEEP";
  param.Electron_MinPt = TriggerSafePt_Electron;

  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGLoose";
  param.Muon_Veto_ID = "POGLoose";
  param.Muon_MinPt = TriggerSafePt_Muon;
  param.Muon_RECO_SF_Key = "HighPtMuonRecoSF";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
  param.Muon_Trigger_SF_Key = "POGHighPtLooseTrkIso";

  param.Jet_ID = "tight";
  param.FatJet_ID = "HN";


  cout << "==============================================================================" << endl;
  std::vector<Gen> gen_signal = GetGens();
  std::vector<Gen> gen_b_mesons;
  std::vector<Gen> gen_b_quarks;
  gen_b_mesons.clear();
  gen_b_quarks.clear();
  int N_Bmeson = 0;
  int N_Bquark = 0;
  for(unsigned int i_gen = 0; i_gen < gen_signal.size(); i_gen++){
    int current_pid = gen_signal.at(i_gen).PID();
    int current_status = gen_signal.at(i_gen).Status();
    if(abs(current_pid) > 499 && abs(current_pid) < 600){
      N_Bmeson ++;
      gen_b_mesons.push_back(gen_signal.at(i_gen));
    }

    int current_mother_PID = 0;
    if(i_gen > 2) current_mother_PID = gen_signal.at( gen_signal.at(i_gen).MotherIndex() ).PID();
    if(abs(current_pid) == 5 && (current_mother_PID > 9900000 || abs(current_mother_PID) == 6)){
      N_Bquark ++;
      gen_b_quarks.push_back(gen_signal.at(i_gen));
    }
    gen_signal.at(i_gen).Print();
  }
  cout << "==================" << endl;
  cout << "N_Bmeson : "  << N_Bmeson << endl;
  cout << "N_Bquark : "  << N_Bquark << endl;
  
  int NBJets=0;
  std::vector<Jet> jets_all = GetAllJets();
  std::vector<Jet>      alljets         = SelectJets(jets_all, param.Jet_ID, 40., 2.7);
  for(unsigned int i=0; i<alljets.size(); i++){
    Jet this_jet = alljets.at(i);
    double this_discr = this_jet.GetTaggerResult(JetTagging::DeepCSV);
    if( mcCorr->IsBTagged_2a(jtps.at(0), this_jet) ) NBJets++; 
  }
  cout << "NBJets : " << NBJets << endl;
  cout << "=====================" << endl;
  
  FillHist("NBjet", NBJets, 1., 10, 0., 10.);
  FillHist("N_Bquark", N_Bquark, 1., 10, 0., 10.);
  FillHist("N_Bmeson", N_Bmeson, 1., 10, 0., 10.);

}


Signal_study::Signal_study(){

}

Signal_study::~Signal_study(){

}



