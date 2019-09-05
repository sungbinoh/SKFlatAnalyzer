#include "SR_signal.h"

void SR_signal::initializeAnalyzer(){
  
  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");
  
  //cout << "[HNPairAnalyzer::initializeAnalyzer] RunFake = " << RunFake << endl;
  //cout << "[HNPairAnalyzer::initializeAnalyzer] RunCF = " << RunCF << endl;
  
}

void SR_signal::executeEvent(){
  
  
  cout << "---------------" << endl;
  /*
  unsigned int N_triggers = HLT_TriggerName.size();
  for(int i_trig = 0; i_trig < N_triggers; i_trig++){
    cout << HLT_TriggerName.at(i_trig) << endl;
  }
  */
  //cout << "[executeEvent] start" << endl;
  
  Event ev_dem = GetEvent();

  FillHist("sumW", 0.5, ev_dem.MCweight(), 10, 0., 10.);
  //FillHist("sumW", 1.5, ev_dem.MCweight() * PDFWeights_Scale->at(0), 10, 0., 10.);
  
  AnalyzerParameter param;
  
  param.Name = "HNPair";
  
  param.MCCorrrectionIgnoreNoHist = true;
  
  param.Electron_Tight_ID = "HEEP_dZ";
  param.Electron_Loose_ID = "HEEP_OR_LooseNoIso";
  param.Electron_Veto_ID = "HEEP_OR_LooseNoIso";
  param.Electron_MinPt = 75.; // HLT_DoublePhoton70_v
  
  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  param.Muon_Loose_ID = "POGHighPt";
  param.Muon_Veto_ID = "POGHighPt";
  param.Muon_MinPt = 55.; // HLT_Mu50_v
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
  
  param.Jet_ID = "tight";
  param.FatJet_ID = "HN";  
  /*
  cout << "---------------------" << endl;
  cout << "genWeight_Q : " << genWeight_Q << endl;
  cout << "genWeight_id1 : " << genWeight_id1 << ", genWeight_X1 : " << genWeight_X1 << endl;
  cout << "genWeight_id2 : " << genWeight_id2 << ", genWeight_X2 : " << genWeight_X2 << endl;
  
  double nnpdf_nnlo_pdf = GetPDFWeight("NNPDF31_nnlo_hessian_pdfas", 0);
  double ct14_nlo_pdf = GetPDFWeight("CT14nlo", 0);
  cout << "nnpdf_nnlo_pdf : " << nnpdf_nnlo_pdf << endl;
  cout << "ct14_nlo_pdf : " << ct14_nlo_pdf << endl;
  */
  Select_syst_objects(param);
  //executeEventFromParameter(param);
  
}

void SR_signal::Select_syst_objects(AnalyzerParameter param){

  //cout << "[Select_syst_objects] call object vectors" << endl;
  
  //cout << "[Select_syst_objects] call electron vectors" << endl;

  // ==== Get Electron selection with syst flags & Save in to a vector of vectors
  std::vector<Electron> electron_all = GetAllElectrons();
  std::vector<Electron> electron_all_Scale_Up   = ScaleElectrons(electron_all,  1);
  std::vector<Electron> electron_all_Scale_Down = ScaleElectrons(electron_all, -1);
  std::vector<Electron> electron_all_Smear_Up   = SmearElectrons(electron_all,  1);
  std::vector<Electron> electron_all_Smear_Down = SmearElectrons(electron_all, -1);
  
  std::sort(electron_all.begin(), electron_all.end(), PtComparing);
  std::sort(electron_all_Scale_Up.begin(), electron_all_Scale_Up.end(), PtComparing);
  std::sort(electron_all_Scale_Down.begin(), electron_all_Scale_Down.end(), PtComparing);
  std::sort(electron_all_Smear_Up.begin(), electron_all_Smear_Up.end(), PtComparing);
  std::sort(electron_all_Smear_Down.begin(), electron_all_Smear_Down.end(), PtComparing);
  
  std::vector<std::vector<Electron>> electrons_all_syst;
  electrons_all_syst.clear();
  electrons_all_syst.push_back(electron_all);
  electrons_all_syst.push_back(electron_all_Scale_Up);
  electrons_all_syst.push_back(electron_all_Scale_Down);
  electrons_all_syst.push_back(electron_all_Smear_Up);
  electrons_all_syst.push_back(electron_all_Smear_Down);
  
  //cout << "[Select_syst_objects] call muon vectors" << endl;
  
  // ==== Save Muons with TuneP
  std::vector<Muon> muons_all;
  if(!IsData) muons_all = UseTunePMuon( GetAllMuons() );   else muons_all = UseTunePMuon( GetAllMuons() );
  std::vector<Muon> muons_all_Scale_Up;
  if(!IsData) muons_all_Scale_Up = ScaleMuons(muons_all, 1);              else muons_all_Scale_Up = UseTunePMuon( GetAllMuons() );
  std::vector<Muon> muons_all_Scale_Down;
  if(!IsData) muons_all_Scale_Down = ScaleMuons(muons_all, -1);           else muons_all_Scale_Down = UseTunePMuon( GetAllMuons() );
  std::vector<Muon> muons_all_Smear_Up;   
  if(!IsData) muons_all_Smear_Up = SmearTunePMuons(muons_all, 1);              else muons_all_Smear_Up = UseTunePMuon( GetAllMuons() );
  std::vector<Muon> muons_all_Smear_Down; 
  if(!IsData) muons_all_Smear_Down = SmearTunePMuons(muons_all, -1);           else muons_all_Smear_Down = UseTunePMuon( GetAllMuons() );
  
  std::sort(muons_all.begin(), muons_all.end(), PtComparing);
  std::sort(muons_all_Scale_Up.begin(), muons_all_Scale_Up.end(), PtComparing);
  std::sort(muons_all_Scale_Down.begin(), muons_all_Scale_Down.end(), PtComparing);
  std::sort(muons_all_Smear_Up.begin(), muons_all_Smear_Up.end(), PtComparing);
  std::sort(muons_all_Smear_Down.begin(), muons_all_Smear_Down.end(), PtComparing);
  /*
  cout << "-------------------------------------------------------------" << endl;
  for(unsigned int i_mu = 0; i_mu < muons_all.size(); i_mu++){
    cout << i_mu << "'th mu central pt = " << muons_all.at(i_mu).Pt() << ", Scale Up : " << muons_all_Scale_Up.at(i_mu).Pt() << ", Scale Down : " << muons_all_Scale_Down.at(i_mu).Pt() << 
      ", Smear Up : " << muons_all_Smear_Up.at(i_mu).Pt() << ", Smear Down : " << muons_all_Smear_Down.at(i_mu).Pt() << endl;
  }
  */
  std::vector<std::vector<Muon>> muons_all_syst;
  muons_all_syst.clear();
  muons_all_syst.push_back(muons_all);
  muons_all_syst.push_back(muons_all_Scale_Up);
  muons_all_syst.push_back(muons_all_Scale_Down);
  muons_all_syst.push_back(muons_all_Smear_Up);
  muons_all_syst.push_back(muons_all_Smear_Down);
  
  //cout << "[Select_syst_objects] call jet vectors" << endl;

  // ==== Get Jets selection with syst flags & Save in to a vector of vectors
  std::vector<Jet> Jets_all = GetAllJets();
  std::vector<Jet> Jets_all_Scale_Up = ScaleJets(GetAllJets(),  1); ;
  std::vector<Jet> Jets_all_Scale_Down = ScaleJets(GetAllJets(), -1);;
  std::vector<Jet> Jets_all_Res_Up;
  if(!IsData) Jets_all_Res_Up = SmearJets(  GetAllJets(),  1);
  else Jets_all_Res_Up = GetAllJets();
  std::vector<Jet> Jets_all_Res_Down;
  if(!IsData) Jets_all_Res_Down = SmearJets(  GetAllJets(), -1);
  else Jets_all_Res_Down = GetAllJets();
  
  std::sort(Jets_all.begin(), Jets_all.end(), PtComparing);
  std::sort(Jets_all_Scale_Up.begin(), Jets_all_Scale_Up.end(), PtComparing);
  std::sort(Jets_all_Scale_Down.begin(), Jets_all_Scale_Down.end(), PtComparing);
  std::sort(Jets_all_Res_Up.begin(), Jets_all_Res_Up.end(), PtComparing);
  std::sort(Jets_all_Res_Down.begin(), Jets_all_Res_Down.end(), PtComparing);
  
  std::vector<std::vector<Jet>> Jets_all_syst;
  Jets_all_syst.clear();
  Jets_all_syst.push_back(Jets_all);
  Jets_all_syst.push_back(Jets_all_Scale_Up);
  Jets_all_syst.push_back(Jets_all_Scale_Down);
  Jets_all_syst.push_back(Jets_all_Res_Up);
  Jets_all_syst.push_back(Jets_all_Res_Down);
  //cout << "[Select_syst_objects] call fatjet vectors" << endl;

  // ==== Get FatJets selection with syst flags & Save in to a vector of vectors
  std::vector<FatJet> FatJets_all = GetAllFatJets();
  std::vector<FatJet> FatJets_all_Scale_Up   = ScaleFatJets(GetAllFatJets(),  1);
  std::vector<FatJet> FatJets_all_Scale_Down = ScaleFatJets(GetAllFatJets(), -1);
  std::vector<FatJet> FatJets_all_Res_Up;
  if(!IsData) FatJets_all_Res_Up = SmearFatJets(  GetAllFatJets(),  1);
  else FatJets_all_Res_Up = GetAllFatJets();
  std::vector<FatJet> FatJets_all_Res_Down;
  if(!IsData) FatJets_all_Res_Down = SmearFatJets(  GetAllFatJets(), -1);
  else FatJets_all_Res_Down = GetAllFatJets();
  
  std::vector<FatJet> FatJets_all_SDMass_Scale_Up;
  if(!IsData) FatJets_all_SDMass_Scale_Up = ScaleSDMassFatJets(  GetAllFatJets(),  1);
  else FatJets_all_SDMass_Scale_Up = GetAllFatJets();
  
  std::vector<FatJet> FatJets_all_SDMass_Scale_Down;
  if(!IsData) FatJets_all_SDMass_Scale_Down = ScaleSDMassFatJets(  GetAllFatJets(),  -1);
  else FatJets_all_SDMass_Scale_Down = GetAllFatJets();
  
  std::vector<FatJet> FatJets_all_SDMass_Res_Up;
  if(!IsData) FatJets_all_SDMass_Res_Up = SmearSDMassFatJets(  GetAllFatJets(),  1);
  else FatJets_all_SDMass_Res_Up = GetAllFatJets();
  
  std::vector<FatJet> FatJets_all_SDMass_Res_Down;
  if(!IsData) FatJets_all_SDMass_Res_Down = SmearSDMassFatJets(  GetAllFatJets(),  -1);
  else FatJets_all_SDMass_Res_Down = GetAllFatJets();
  
  
  std::sort(FatJets_all.begin(), FatJets_all.end(), PtComparing);
  std::sort(FatJets_all_Scale_Up.begin(), FatJets_all_Scale_Up.end(), PtComparing);
  std::sort(FatJets_all_Scale_Down.begin(), FatJets_all_Scale_Down.end(), PtComparing);
  std::sort(FatJets_all_Res_Up.begin(), FatJets_all_Res_Up.end(), PtComparing);
  std::sort(FatJets_all_Res_Down.begin(), FatJets_all_Res_Down.end(), PtComparing);
  
  std::sort(FatJets_all_SDMass_Scale_Up.begin(), FatJets_all_SDMass_Scale_Up.end(), PtComparing);
  std::sort(FatJets_all_SDMass_Scale_Down.begin(), FatJets_all_SDMass_Scale_Down.end(), PtComparing);
  std::sort(FatJets_all_SDMass_Res_Up.begin(), FatJets_all_SDMass_Res_Up.end(), PtComparing);
  std::sort(FatJets_all_SDMass_Res_Down.begin(), FatJets_all_SDMass_Res_Down.end(), PtComparing);
  
  std::vector<std::vector<FatJet>> FatJets_all_syst;
  FatJets_all_syst.clear();
  FatJets_all_syst.push_back(FatJets_all);
  FatJets_all_syst.push_back(FatJets_all_Scale_Up);
  FatJets_all_syst.push_back(FatJets_all_Scale_Down);
  FatJets_all_syst.push_back(FatJets_all_Res_Up);
  FatJets_all_syst.push_back(FatJets_all_Res_Down);
  FatJets_all_syst.push_back(FatJets_all_SDMass_Scale_Up);
  FatJets_all_syst.push_back(FatJets_all_SDMass_Scale_Down);
  FatJets_all_syst.push_back(FatJets_all_SDMass_Res_Up);
  FatJets_all_syst.push_back(FatJets_all_SDMass_Res_Down);
  
  
  //cout << "[Select_syst_objects] call pdf errors" << endl;
  
  // ==== Get PDF weights with errors & Save in to a vector
  double PDF_weight_central = 1.;
  //cout << "[Select_syst_objects] PDF_Norm_Up" << endl;
  double PDF_Norm_Up    = 1.; //GetWeightPDFNormError(   1);
  //cout << "[Select_syst_objects] PDF_Norm_Up" << endl;
  double PDF_Norm_Down  = 1.; //GetWeightPDFNormError(  -1);
  //cout << "[Select_syst_objects] PDF_Norm_Up" << endl;
  double PDF_Scale_Up   = 1.; //GetWeightPDFScaleError(  1);
  //cout << "[Select_syst_objects] PDF_Norm_Up" << endl;
  double PDF_Scale_Down = 1.; //GetWeightPDFScaleError( -1);
  
  std::vector<double> PDF_weight_syst;
  PDF_weight_syst.clear();
  PDF_weight_syst.push_back(PDF_weight_central);
  PDF_weight_syst.push_back(PDF_Norm_Up);
  PDF_weight_syst.push_back(PDF_Norm_Down);
  PDF_weight_syst.push_back(PDF_Scale_Up);
  PDF_weight_syst.push_back(PDF_Scale_Down);
    
  //cout << "[Select_syst_objects] Let's run" << endl;

  // ==== Define Systematic flag strings and order
  const int N_systs = 23;
  TString syst_flags[N_systs] = {"central", 
				 "ElectronScaleUp", "ElectronScaleDown", "ElectronSmearUp", "ElectronSmearDown", 
				 "MuonScaleUp", "MuonScaleDown", "MuonSmearUp", "MuonSmearDown",
				 "JetsScaleUp", "JetsScaleDown", "JetsResUp", "JetsResDown", 
				 "SD_JMS_Up", "SD_JMS_Down", "SD_JMR_Up", "SD_JMR_Down", 
				 "PDFNormUp", "PDFNormDown", "PDFScaleUp", "PDFScaleDown", 
				 "PUReweight_Up", "PUReweight_Down",
  };
  
  int electron_index[N_systs] = {0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int muon_index[N_systs]     = {0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int jet_index[N_systs]      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int fatjet_index[N_systs]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0};
  int PDF_index[N_systs]      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0};
  
  double nnpdf_nnlo_pdf = 1.;
  double ct14_nlo_pdf = 1.;
  nnpdf_nnlo_pdf = GetPDFWeight("NNPDF31_nnlo_hessian_pdfas", 0);
  ct14_nlo_pdf = GetPDFWeight("CT14nlo", 0);
  
  double pdf_ratio = ct14_nlo_pdf / nnpdf_nnlo_pdf;
 
  for(int i_syst = 0; i_syst < N_systs; i_syst++){
    //executeEventFromParameter(  param, electrons_all_syst.at(electron_index[i_syst]), muons_all_syst.at(muon_index[i_syst]), Jets_all_syst.at(jet_index[i_syst]), FatJets_all_syst.at(fatjet_index[i_syst]), syst_flags[i_syst], PDF_weight_syst.at(PDF_index[i_syst])  ); 
    executeEventFromParameter(  param, electrons_all_syst.at(electron_index[i_syst]), muons_all_syst.at(muon_index[i_syst]), Jets_all_syst.at(jet_index[i_syst]), FatJets_all_syst.at(fatjet_index[i_syst]), syst_flags[i_syst], pdf_ratio  );
  }
  
}


void SR_signal::executeEventFromParameter(AnalyzerParameter param, std::vector<Electron> electron_all, std::vector<Muon> muons_all, std::vector<Jet> jets_all, std::vector<FatJet> fatjets_all, TString syst_flag, double PDF_weight){
  
  //cout << "----------------------" << endl;

  //if(!PassMETFilter()) return;
  
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();
    
  // -- Get Gen level info
  vector<Gen> gens = GetGens();
    
  
  // -- Save HLT String and Boolean. Return if the event does not fire any on these triggers
  // -- EGamma triggers to compare : HLT_DoubleEle33_CaloIdL_MW_v, HLT_Ele115_CaloIdVT_GsfTrkIdT_v (|| HLT_Photon200_v), HLT_Ele35_WPTight_Gsf_v 
  //TString trig_diele = "HLT_Photon200_v";
  TString trig_diele = "HLT_DoublePhoton70_v";
  TString trig_diele_bool = "HLT_Ele35_WPTight_Gsf_v";
  TString trig_mu50 = "HLT_Mu50_v";
  //TString trig_oldmu100 = "HLT_oldMu100_v";
  //TString trig_tkmu100 = "HLT_TkMu100_v";
  
  bool Pass_diele = ev.PassTrigger(trig_diele_bool);
  //Pass_diele = ev.PassTrigger("HLT_Photon200_v") || ev.PassTrigger("HLT_Ele115_CaloIdVT_GsfTrkIdT_v");
  Pass_diele = ev.PassTrigger("HLT_DoublePhoton70_v") || ev.PassTrigger("HLT_DoubleEle33_CaloIdL_MW_v");

  bool Pass_mu50 = ev.PassTrigger(trig_mu50);
  
    
  // -- Set weight for Normalization 
  double weight_1pb = 1.;
  double weight_trig_diele = 1.;
  double weight_trig_mu50 = 1.;
  double pileup_reweight = 1.;
  
  if(!IsData){
    weight_1pb = weight_norm_1invpb * ev.MCweight();
    weight_trig_diele = ev.GetTriggerLumi(trig_diele);
    weight_trig_mu50 = ev.GetTriggerLumi(trig_mu50);
    
    pileup_reweight = GetPileUpWeight(nPileUp, 0);
    if(syst_flag.Contains("PUReweight_Up")) pileup_reweight = GetPileUpWeight(nPileUp, 1);
    if(syst_flag.Contains("PUReweight_Down")) pileup_reweight = GetPileUpWeight(nPileUp, -1);
  }
  
  
  //cout << "pileup_reweight : " << pileup_reweight << endl;
  weight_trig_diele = weight_trig_diele * weight_1pb * pileup_reweight;
  weight_trig_mu50 = weight_trig_mu50 * weight_1pb * pileup_reweight;
  

  FillHist("signal_eff", 0.5, weight_trig_diele * PDF_weight, 40, 0., 40.); // cutflow - before trigger
  
  if(!PassMETFilter()) return;
  if(!Pass_diele && !Pass_mu50) return;
  
  if(Pass_diele) FillHist("signal_eff", 1.5, weight_trig_diele, 40, 0., 40.); // cutflow - DoublePhoton HLT
  if(Pass_mu50) FillHist("signal_eff", 2.5, weight_trig_mu50, 40, 0., 40.); // cutflow - SingleMuon HLT
  
  // -- Save Electrons and Numbers of them
  double MinPt = 75;
  
    
  std::vector<Electron> electrons_Veto = SelectElectrons(electron_all, param.Electron_Veto_ID, 35., 2.5);
  std::vector<Electron> electrons_Loose = SelectElectrons(electron_all, param.Electron_Loose_ID, param.Electron_MinPt, 2.5);
  std::vector<Electron> electrons_Tight = SelectElectrons(electron_all, param.Electron_Tight_ID, param.Electron_MinPt, 2.5);
  std::vector<Electron> electrons_Tight_all = SelectElectrons(electron_all, param.Electron_Tight_ID, param.Electron_MinPt, 2.5);

  // -- Save Muons and Numbers of them
  std::vector<Muon> muons_Veto  = SelectMuons(muons_all, param.Muon_Veto_ID,  10., 2.4);
  std::vector<Muon> muons_Loose = SelectMuons(muons_all, param.Muon_Loose_ID, 10., 2.4);
  std::vector<Muon> muons_Tight = SelectMuons(muons_all, param.Muon_Tight_ID, 10., 2.4);
  std::vector<Muon> muons_Tight_all = SelectMuons(muons_all, param.Muon_Tight_ID, 10., 2.4);
  
  
  //==== Now Apply pt cut
  electrons_Loose = ElectronApplyPtCut(electrons_Loose, MinPt);
  electrons_Tight = ElectronApplyPtCut(electrons_Tight, MinPt);
  
  muons_Loose     = MuonApplyPtCut(muons_Loose, MinPt);
  muons_Tight     = MuonApplyPtCut(muons_Tight, MinPt);
  
  
  // -- Get Size of Lepton Vectors
  int N_electrons_Tight = 0;
  int N_electrons_Tight_all = 0;
  int N_electrons_Loose = 0;
  int N_electrons_Veto = 0;
  N_electrons_Tight = electrons_Tight.size();
  N_electrons_Tight_all = electrons_Tight_all.size();
  N_electrons_Loose = electrons_Loose.size();
  N_electrons_Veto = electrons_Veto.size();
    
  int N_muons_Tight = 0;
  int N_muons_Tight_all = 0;
  int N_muons_Loose = 0;
  int N_muons_Veto = 0;
  N_muons_Tight = muons_Tight.size();
  N_muons_Tight_all = muons_Tight_all.size();
  N_muons_Loose = muons_Loose.size();
  N_muons_Veto = muons_Veto.size();
  
  
  // -- Save Jets
  std::vector<FatJet>   fatjets         = SelectFatJets(fatjets_all,param.FatJet_ID, 300, 2.7);
  std::vector<Jet>      alljets         = SelectJets(jets_all, param.Jet_ID, 40., 2.7);
  std::vector<Jet>      jets            = JetsVetoLeptonInside(JetsAwayFromFatJet(alljets, fatjets), electrons_Veto, muons_Veto);
  
  int NBJets=0;
  for(unsigned int i=0; i<alljets.size(); i++){
    if(IsBTagged(alljets.at(i), Jet::CSVv2, Jet::Medium, true, 0)) NBJets++;
  }
  
    
  // -- Get Prefire weight
  double prefire_weight = 1.;
  if(!IsDATA) prefire_weight =  L1PrefireReweight_Central;
  
  //cout << "prefire_weight : " << prefire_weight << endl;
  
  
  if(N_electrons_Loose != N_electrons_Veto) return;
  if(N_muons_Loose != N_muons_Veto) return;
  
  // ==== Check Number of veto letpons and assign channel string
  bool trig_pass_for_channel = false;
  TString current_channel;
  if(N_electrons_Veto == 2 && N_muons_Veto == 0){
    current_channel = "DiEle_";
    trig_pass_for_channel = Pass_diele;
  }
  else if(N_muons_Veto == 2 && N_electrons_Veto == 0){
    current_channel = "DiMu_";
    trig_pass_for_channel = Pass_mu50;
  }
  else if(N_muons_Veto == 1 && N_electrons_Veto == 1){
    current_channel = "EMu_";
    trig_pass_for_channel = Pass_mu50;
  }
  else return;
  
  double current_weight = weight_trig_mu50 * prefire_weight;
  if(!IsDATA){
    current_weight *= PDF_weight;
    
    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

    for(unsigned int i = 0; i < electrons_Veto.size(); i++){
      double this_recosf = mcCorr->ElectronReco_SF(electrons_Veto.at(i).scEta(), electrons_Veto.at(i).Pt());
      double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, electrons_Veto.at(i).scEta(), electrons_Veto.at(i).Pt());
      current_weight *= this_recosf*this_idsf;
    }
    
    for(unsigned int i = 0; i < muons_Veto.size(); i++){
      
      double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  muons_Veto.at(i).Eta(), muons_Veto.at(i).MiniAODPt());
      double this_trigsf = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, "Default", muons_Veto);
      current_weight *= this_idsf*this_trigsf;
    }
    for(unsigned  int i = 0; i < muons_Tight.size(); i++){
      double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, muons_Tight.at(i).Eta(), muons_Tight.at(i).MiniAODPt());
      current_weight *= this_isosf;
    }
    
  }

  // ==== Run Charge Blind 
  SR(param, current_channel + syst_flag, trig_pass_for_channel,  current_weight, jets, fatjets, electrons_Tight_all, electrons_Veto, muons_Tight_all, muons_Veto, N_electrons_Tight_all, N_electrons_Veto, N_muons_Tight_all, N_muons_Veto);
    
}


void SR_signal::SR(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto,
		     int N_electron, int N_veto_ele, int N_muon, int N_veto_muon){
  
  //Signal Region, Nbjet = 0, m(ll) > 150 GeV, m(Zp) > 300 GeV
  

  //cout << "[SR] Start" << endl;


  // -- Trigger Pass
  if(!trig_pass) return;

  // -- Fill Lepton class and check N_leptons == 2
  std::vector<Lepton *> leps_electron_veto = MakeLeptonPointerVector(electrons_veto);
  std::vector<Lepton *> leps_muon_veto     = MakeLeptonPointerVector(muons_veto);
  std::vector<Lepton *> Leptons_veto;
  for(unsigned int i=0;i<leps_electron_veto.size(); i++) Leptons_veto.push_back( leps_electron_veto.at(i) );
  for(unsigned int i=0;i<leps_muon_veto.size(); i++) Leptons_veto.push_back( leps_muon_veto.at(i) );
  
  std::vector<Lepton *> leps_electron = MakeLeptonPointerVector(electrons);
  std::vector<Lepton *> leps_muon     = MakeLeptonPointerVector(muons);
  std::vector<Lepton *> Leptons;
  for(unsigned int i=0;i<leps_electron.size(); i++) Leptons.push_back( leps_electron.at(i) );
  for(unsigned int i=0;i<leps_muon.size(); i++) Leptons.push_back( leps_muon.at(i) );
  
  
  if(Leptons.size() > 2) return;

  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 3.5, weight, 40, 0., 40.); // cutflow - DiEle Nlep
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 4.5, weight, 40, 0., 40.); // cutflow - DiMu Nlep
  }
  else if(channel.Contains("EMu")){
    FillHist("signal_eff", 5.5, weight, 40, 0., 40.); // cutflow - EMu Nlep
  }
  else return;

  
  // -- Pt of Leptons > 75 GeV 
  double Lep_1st_Pt, Lep_2nd_Pt;
  Lep_1st_Pt = Leptons_veto.at(0)->Pt();
  Lep_2nd_Pt = Leptons_veto.at(1)->Pt();
  
  
  if(channel.Contains("SS")){
    if(Leptons_veto.at(0)->Charge() != Leptons_veto.at(1)->Charge()) return;
  }
  //else{
  //  if(Leptons_veto.at(0)->Charge() == Leptons_veto.at(1)->Charge()) return;
  //}
  
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 21.5, weight, 40, 0., 40.); // cutflow - DiEle lepton Charge                                                     
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 22.5, weight, 40, 0., 40.); // cutflow - DiMu lepton Charge
  }
  else if(channel.Contains("EMu")){
    FillHist("signal_eff", 23.5, weight, 40, 0., 40.); // cutflow - EMu lepton Charge
  }
  else return;
  
  if(channel.Contains("DiEle")){
    if(Lep_1st_Pt < 210 || Lep_2nd_Pt < 40) return;
  }
  else if(channel.Contains("DiMu")){
    if(Lep_1st_Pt < 55 || Lep_2nd_Pt < 20) return;
  }
  else if(channel.Contains("EMu")){
    if(Lep_1st_Pt < 55 || Lep_2nd_Pt < 55) return;
  }
  else return;
  //if(Lep_1st_Pt < 75 || Lep_2nd_Pt < 75) return;
  
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 6.5, weight, 40, 0., 40.); // cutflow - DiEle lepton Pt 
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 7.5, weight, 40, 0., 40.); // cutflow - DiMu lepton Pt 
  }
  else if(channel.Contains("EMu")){
    FillHist("signal_eff", 8.5, weight, 40, 0., 40.); // cutflow - EMu lepton Pt 
  }
  else return;

  


  //Nbjet == 0 
  std::vector<Jet>      alljets_sub         = JetsVetoLeptonInside(GetJets(param.Jet_ID, 40., 2.7), electrons_veto, muons_veto); //no fatjet veto
  int NBJets=0;
  for(unsigned int i=0; i<alljets_sub.size(); i++){
    if(IsBTagged(alljets_sub.at(i), Jet::CSVv2, Jet::Medium, true, 0)) NBJets++;
  }
  int N_jet = alljets_sub.size();
  //if(NBJets > 0) return;
  TString Region_str = "SR_";

  JSFillHist(Region_str + channel, "N_lepton_" + Region_str + channel, Leptons.size(), weight, 10, 0., 10.);
  JSFillHist(Region_str + channel, "N_lepton_veto_" + Region_str + channel, Leptons_veto.size(), weight, 10, 0., 10.);
  JSFillHist(Region_str + channel, "N_fatjet_" + Region_str + channel, fatjets.size(), weight, 10, 0., 10.);
  JSFillHist(Region_str + channel, "N_jet_" + Region_str + channel, jets.size(), weight, 10, 0., 10.);
  JSFillHist(Region_str + channel, "N_lepton_VS_N_fatjet" + Region_str + channel, Leptons.size(), fatjets.size(), weight, 10, 0., 10., 10, 0., 10.);
  JSFillHist(Region_str + channel, "N_lepton_VS_N_jet" + Region_str + channel, Leptons.size(), jets.size(), weight, 10, 0., 10., 10, 0., 10.);
  FillJetPlots(jets, fatjets, Region_str + channel, weight);
  
  //m(ll) > 150 GeV
  Particle ll = *(Leptons_veto.at(0)) + *(Leptons_veto.at(1));
  double M_ll = ll.M();
  if(M_ll < 150 ) return;
  
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 24.5, weight, 40, 0., 40.); // cutflow - DiEle mll mass
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 25.5, weight, 40, 0., 40.); // cutflow - DiMu mll mass
  }
  else return;

  JSFillHist("SR_" + channel, "Nbjet_SR_" + channel, NBJets, weight, 10, 0., 10.);
  JSFillHist("SR_" + channel, "Njet_SR_" + channel, N_jet, weight, 10, 0., 10.);
  JSFillHist("SR_" + channel, "Nfatjet_SR_" + channel, fatjets.size(), weight, 10, 0., 10.);
  
  
  vector<Particle> Ns = RecoPairN(Leptons, fatjets, jets);
  if(Ns.size() != 2) return;
  TString jetbin_str = Get_N_jet_bin(Leptons, fatjets, jets);
  
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 26.5, weight, 40, 0., 40.); // cutflow - DiEle Njet
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 27.5, weight, 40, 0., 40.); // cutflow - DiMu Njet
  }
  else return;
  
  

  Particle Zp = Ns.at(0) + Ns.at(1);
  if(Ns.at(0).M() < 80 || Ns.at(1).M() < 80) return;
  
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 28.5, weight, 40, 0., 40.); // cutflow - DiEle N mass
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 29.5, weight, 40, 0., 40.); // cutflow - DiMu N mass 
  }
  else return;

  if(Zp.M() < 300) return;
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 18.5, weight, 40, 0., 40.); // cutflow - DiEle SR
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 19.5, weight, 40, 0., 40.); // cutflow - DiMu SR
  }
  else if(channel.Contains("EMu")){
    FillHist("signal_eff", 20.5, weight, 40, 0., 40.); // cutflow - EMu SR
  }
  else return;

  Event ev = GetEvent();
  int nPV = ev.nPV();
  /*
  double pileup_reweight = 1.;
  if(!IsData){
    pileup_reweight = mcCorr.GetPileUpWeightAsSampleName(0, nPileUp);
  }
  JSFillHist(Region_str + channel, "Nvtx_noCorr_" + Region_str + channel, nPV, weight, 1000, 0., 1000.);
  */
  JSFillHist(Region_str + channel, "Nvtx_69p2_" + Region_str + channel, nPV, weight, 1000, 0., 1000.);

  
  
  //TString Region_str = "SR_";
  FillLeptonPlots(Leptons, Region_str + channel, weight);
  JSFillHist(Region_str + channel, "mZp_" + Region_str + channel, Zp.M(), weight, 6000, 0., 6000.);
  JSFillHist(Region_str + channel, "mN_" + Region_str + channel, Ns.at(0).M(), weight, 5000, 0., 5000.);
  JSFillHist(Region_str + channel, "mN_" + Region_str + channel, Ns.at(1).M(), weight, 5000, 0., 5000.);
  FillHNPairPlots(Ns, Region_str + channel, jetbin_str, weight);

  
}


void SR_signal::CR_inv_mll(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto,
		     int N_electron, int N_veto_ele, int N_muon, int N_veto_muon){

  //Control Region (inverted mll cut from SR), Nbjet = 0, m(ll) < 150 GeV, m(Zp) > 300 GeV
  
  //cout << "[CR_inv_mll] Start" << endl;

  
  // -- Trigger Pass
  if(!trig_pass) return;

  // -- Fill Lepton class and check N_leptons == 2 
  std::vector<Lepton *> leps_electron_veto = MakeLeptonPointerVector(electrons_veto);
  std::vector<Lepton *> leps_muon_veto     = MakeLeptonPointerVector(muons_veto);
  std::vector<Lepton *> Leptons_veto;
  for(unsigned int i=0;i<leps_electron_veto.size(); i++) Leptons_veto.push_back( leps_electron_veto.at(i) );
  for(unsigned int i=0;i<leps_muon_veto.size(); i++) Leptons_veto.push_back( leps_muon_veto.at(i) );
  
  std::vector<Lepton *> leps_electron = MakeLeptonPointerVector(electrons);
  std::vector<Lepton *> leps_muon     = MakeLeptonPointerVector(muons);
  std::vector<Lepton *> Leptons;
  for(unsigned int i=0;i<leps_electron.size(); i++) Leptons.push_back( leps_electron.at(i) );
  for(unsigned int i=0;i<leps_muon.size(); i++) Leptons.push_back( leps_muon.at(i) );

  
  if(Leptons.size() > 2) return;

  
  // -- Pt of Leptons > 75 GeV
  double Lep_1st_Pt, Lep_2nd_Pt;
  Lep_1st_Pt = Leptons_veto.at(0)->Pt();
  Lep_2nd_Pt = Leptons_veto.at(1)->Pt();

  //if(Leptons_veto.at(0)->Charge() == Leptons_veto.at(1)->Charge()) return; // OS
  if(Lep_1st_Pt < 75 || Lep_2nd_Pt < 75) return;


  //Nbjet >= 1 
  std::vector<Jet>      alljets_sub         = JetsVetoLeptonInside(GetJets(param.Jet_ID, 40., 2.7), electrons_veto, muons_veto); //no fatjet veto
  int NBJets=0;
  for(unsigned int i=0; i<alljets_sub.size(); i++){
    if(IsBTagged(alljets_sub.at(i), Jet::CSVv2, Jet::Medium, true, 0)) NBJets++;
  }
  
  

  //m(ll) < 150 GeV
  Particle ll = *(Leptons_veto.at(0)) + *(Leptons_veto.at(1));
  double M_ll = ll.M();
  if(M_ll < 55) return;
  
  
  vector<Particle> Ns = RecoPairN(Leptons, fatjets, jets);
  if(Ns.size() != 2) return;
  
  Particle Zp = Ns.at(0) + Ns.at(1);
  if(Zp.M() < 300) return;
  
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 15.5, weight, 40, 0., 40.); // cutflow - DiEle inv mll CR
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 16.5, weight, 40, 0., 40.); // cutflow - DiMu inv mll CR
  }
  else if(channel.Contains("EMu")){
    FillHist("signal_eff", 17.5, weight, 40, 0., 40.); // cutflow - EMu inv mll CR
  }
  else return;
  
  
  TString jetbin_str = Get_N_jet_bin(Leptons, fatjets, jets);
  
  TString Region_str = "CR_inv_mll_";
  JSFillHist(Region_str + channel, "mll_" + Region_str + channel, M_ll, weight, 1000, 0., 1000.);
  FillLeptonPlots(Leptons_veto, Region_str + channel, weight);

  JSFillHist(Region_str + channel, "mZp_" + Region_str + channel, Zp.M(), weight, 6000, 0., 6000.);
  JSFillHist(Region_str + channel, "mN_" + Region_str + channel, Ns.at(0).M(), weight, 5000, 0., 5000.);
  JSFillHist(Region_str + channel, "mN_" + Region_str + channel, Ns.at(1).M(), weight, 5000, 0., 5000.);
 
  FillHNPairPlots(Ns, Region_str + channel, jetbin_str, weight);

}




vector<Particle> SR_signal::RecoPairN(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets){

  vector<Particle> out;
  out.clear();
  
  //==== check lepton size
  //if(lepptrs.size()!=2) return out;
  
  
  
  //==== Use the four leading jets
  if(fatjets.size()==0 && jets.size()>3 && lepptrs.size() == 2){
    // (01)(23), (02)(13), (03)(12)
    
    Particle Dummy_AllJets = jets.at(0)+jets.at(1)+jets.at(2)+jets.at(3);
    
    double mindM = 999999999;
    Particle temp_N[2];

    //==== Loop over, which jet paired to jets.at(0)
    for(int i=1; i<=3; i++){

      Particle TwoJet[2];
      TwoJet[0] = jets.at(0)+jets.at(i);
      TwoJet[1] = Dummy_AllJets-TwoJet[0];
      
      //==== (lep[0]+TwoJet[0])(lep[1]+TwoJet[1])
      Particle N_00 = *(lepptrs.at(0))+TwoJet[0];
      Particle N_11 = *(lepptrs.at(1))+TwoJet[1];
      if( fabs(N_00.M()-N_11.M()) < mindM ){
        mindM = fabs(N_00.M()-N_11.M());
        temp_N[0] = N_00;
        temp_N[1] = N_11;
      }
      
      
      //==== (lep[0]+TwoJet[1])(lep[1]+TwoJet[0])
      Particle N_01 = *(lepptrs.at(0))+TwoJet[1];
      Particle N_10 = *(lepptrs.at(1))+TwoJet[0];
      if( fabs(N_01.M()-N_10.M()) < mindM ){
        mindM = fabs(N_01.M()-N_10.M());
        temp_N[0] = N_01;
        temp_N[1] = N_10;
      }
      
    }
    
    out.push_back(temp_N[0]);
    out.push_back(temp_N[1]);
    
  }
  else if(fatjets.size()==1 && jets.size() > 1){
   
    if(lepptrs.size() == 1){

      Particle temp_N[2];

      FatJet fatjet = fatjets.at(0);
      temp_N[0] = fatjet;
      temp_N[1] = *(lepptrs.at(0)) + jets.at(0) + jets.at(1);

      if(fatjet.DeltaR( *(lepptrs.at(0))) > 1.1) {
	out.push_back(temp_N[0]);
	out.push_back(temp_N[1]);
      }
      
    }
    
    if(lepptrs.size() == 2){
      Particle temp_N[2];

      FatJet fatjet = fatjets.at(0);
      if(fatjet.DeltaR( *(lepptrs.at(0)) ) < fatjet.DeltaR( *(lepptrs.at(1)) )){
	temp_N[0] = AddFatJetAndLepton(fatjet, *(lepptrs.at(0)));
	temp_N[1] = *(lepptrs.at(1)) + jets.at(0) + jets.at(1);
      }
      else{
	temp_N[0] = AddFatJetAndLepton(fatjet, *(lepptrs.at(1)));
	temp_N[1] = *(lepptrs.at(0)) + jets.at(0) + jets.at(1);
      }

      out.push_back(temp_N[0]);
      out.push_back(temp_N[1]);
      
    }
    
    
  }
  else if(fatjets.size()>1){
    
    if(lepptrs.size() == 0){
      Particle temp_N[2];

      temp_N[0] = fatjets.at(0);
      temp_N[1] = fatjets.at(1);

      out.push_back(temp_N[0]);
      out.push_back(temp_N[1]);
    }
    if(lepptrs.size() == 1){
      
      FatJet fatjet = fatjets.at(0);
      Particle temp_N[2];
      if(fatjet.DeltaR( *(lepptrs.at(0)) ) < fatjet.DeltaR( *(lepptrs.at(0)) )){
        temp_N[0] = AddFatJetAndLepton(fatjet,        *(lepptrs.at(0)));
        temp_N[1] = fatjets.at(1);
      }
      else{
        temp_N[0] = fatjet;
        temp_N[1] = AddFatJetAndLepton(fatjets.at(1),        *(lepptrs.at(0)));
      }
    }
    if(lepptrs.size() == 2){
      
      Particle temp_N[2];

      FatJet fatjet = fatjets.at(0); // Leading FatJet this time

      if(fatjet.DeltaR( *(lepptrs.at(0)) ) < fatjet.DeltaR( *(lepptrs.at(1)) )){
	temp_N[0] = AddFatJetAndLepton(fatjet,        *(lepptrs.at(0)));
	temp_N[1] = AddFatJetAndLepton(fatjets.at(1), *(lepptrs.at(1)));
      }
      else{
	temp_N[0] = AddFatJetAndLepton(fatjets.at(1), *(lepptrs.at(0)));
	temp_N[1] = AddFatJetAndLepton(fatjet,        *(lepptrs.at(1)));
      }

      out.push_back(temp_N[0]);
      out.push_back(temp_N[1]);

    }


  }
  else{

  }

  return out;


}


TString SR_signal::Get_N_jet_bin(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets){
  
  TString N_jet_bin = "";

  //==== Use the four leading jets
  if(fatjets.size()==0 && jets.size()>3 && lepptrs.size() == 2){
    N_jet_bin = "0AK8";
  }
  else if(fatjets.size()==1 && jets.size()>1 && lepptrs.size() > 0){
    N_jet_bin = "1AK8";
  }
  else if(fatjets.size()>1){
    N_jet_bin ="2AK8";
  }
  else{

  }
  
  
  return N_jet_bin;

}
SR_signal::SR_signal(){

}

SR_signal::~SR_signal(){

}


