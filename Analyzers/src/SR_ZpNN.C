#include "SR_ZpNN.h"

void SR_ZpNN::initializeAnalyzer(){

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");
  
  //===============================
  //==== Year Dependent Parameters
  //===============================
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
    TriggerNameForSF_Electron = "ZpNNTrigger";
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
      "HLT_OldMu100_v",
      "HLT_TkMu100_v",
    };
    TriggerNameForSF_Electron = "ZpNNTrigger";
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
    TriggerNameForSF_Electron = "ZpNNTrigger";
    TriggerNameForSF_Muon = "Mu50";
    TriggerSafePt_Electron = 75.;
    TriggerSafePt_Muon = 52.;

  }

  //========================
  //==== B-tagging
  //========================
  jtps.push_back( JetTagging::Parameters(JetTagging::DeepCSV, JetTagging::Medium, JetTagging::mujets, JetTagging::mujets) );
  mcCorr->SetJetTaggingParameters(jtps);  
    
  //===============================
  //==== PileUp Reweight Histrograms
  //===============================
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

void SR_ZpNN::executeEvent(){
  //cout << "===============" << endl;
  //cout << "------------------------[executeEvent] start" << endl;
  
  Event ev_dem = GetEvent();

  //cout << "Get pdf size" << endl;
  double central_PDF = PDFWeights_Scale->at(0);
  //cout << "here?" << endl;
  unsigned int size_scale_err = PDFWeights_Scale->size();
  unsigned int size_PDF_err = PDFWeights_Error->size();
  unsigned int size_renorm_err = PDFWeights_AlphaS->size();
  
  /*
  cout << "=====================" << endl;
  cout << "central_PDF : " << central_PDF << endl;
  cout << "size_scale_err : " << size_scale_err << endl;
  cout << "size_PDF_err : " << size_PDF_err << endl;
  cout << "size_renorm_err : " << size_renorm_err << endl;
  
  cout << "PDFWeights_Error->at(0) : " << PDFWeights_Error->at(0) << endl;
  cout << "PDFWeights_Error->at(26) : " << PDFWeights_Error->at(26) << endl;
  cout << "PDFWeights_Error->at(50) : " << PDFWeights_Error->at(50) << endl;
  cout << "PDFWeights_AlphaS->at(0) : " << PDFWeights_AlphaS->at(0) << endl;
  cout << "PDFWeights_AlphaS->at(1) : " << PDFWeights_AlphaS->at(1) << endl;
  cout << "PDFWeights_Scale->at(2) : " << PDFWeights_Scale->at(2) << endl;
  cout << "proper errors" << endl;
  cout << "PDFWeights_Error->at(26) : " << 2*PDFWeights_Error->at(26) << endl;
  cout << "PDFWeights_Error->at(50) : " << 2*PDFWeights_Error->at(50) << endl;
  cout << "PDFWeights_AlphaS->at(0) : " << 1./9. + PDFWeights_AlphaS->at(0)/0.75 << endl;
  cout << "PDFWeights_AlphaS->at(1) : " << 1./9. + PDFWeights_AlphaS->at(1)/0.75 << endl;
  cout << "PDFWeights_Scale->at(2) : " << 2*PDFWeights_Scale->at(2) << endl;
  
  FillHist("PDFWeights_AlphaS1", PDFWeights_AlphaS->at(0), 1., 100, 0., 2.);
  FillHist("PDFWeights_AlphaS1_modified", -1./4. + 2*PDFWeights_AlphaS->at(0), 1., 100, 0., 2.);
  */


  
  //=========================
  //==== Get PileUp Reweight
  //=========================
  //cout << "Get pileup reweight" << endl;
  N_VTX = nPV;
  if(!IsDATA){
    //==== nPileUp starts from 0
    N_VTX = nPileUp+1;
    //==== but histogram is as a function of nPileUp not nVTX
    int bin_pu = hist_PUReweight->FindBin(nPileUp);
    weight_PU = hist_PUReweight->GetBinContent(bin_pu);

    weight_PU_Up = hist_PUReweight_Up->GetBinContent(bin_pu);
    weight_PU_Down = hist_PUReweight_Down->GetBinContent(bin_pu);
    //cout << "weight_PU : " << weight_PU << endl;
    //cout << "weight_PU_Up: " << weight_PU_Up << endl;
    //cout << "weight_PU_Down : " << weight_PU_Down << endl;

  }

  //========================
  //==== AnalyzerParameter
  //========================
  AnalyzerParameter param;
  param.Clear();

  param.Name = "HNPair";
  
  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HEEP_dZ";
  param.Electron_Loose_ID = "CutBasedLooseNoIso";
  param.Electron_Veto_ID = "CutBasedLooseNoIso";
  param.Electron_ID_SF_Key = "HEEP";
  param.Electron_Trigger_SF_Key = "HEEP";
  param.Electron_MinPt = TriggerSafePt_Electron; // HLT_DoublePhoton70_v
  
  param.Muon_Tight_ID = "POGHighPtWithLooseTrkIso";
  //param.Muon_Loose_ID = "POGHighPt";
  //param.Muon_Veto_ID = "POGHighPt";
  param.Muon_Loose_ID = "POGLoose";
  param.Muon_Veto_ID = "POGLoose";
  param.Muon_MinPt = TriggerSafePt_Muon; // HLT_Mu50_v
  param.Muon_RECO_SF_Key = "HighPtMuonRecoSF";
  param.Muon_ID_SF_Key = "NUM_HighPtID_DEN_genTracks";
  param.Muon_ISO_SF_Key = "NUM_LooseRelTkIso_DEN_HighPtIDandIPCut";
  param.Muon_Trigger_SF_Key = "POGHighPtLooseTrkIso";
  
  param.Jet_ID = "tight";
  param.FatJet_ID = "HN";

  //========================
  //==== PDF Reweight
  //========================
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
  
  //====================================================
  //==== Corrections related with Truth Level Particles
  //====================================================
  TString current_MC = "";
  if(!IsDATA) current_MC = MCSample;
  bool is_TTLL = current_MC.Contains("TTLL");
  bool is_DY = current_MC.Contains("DY") && current_MC.Contains("Jets_MG"); // -- https://github.com/IzaakWN/NanoTreeProducer/blob/master/corrections/RecoilCorrectionTool.py#L95-L109 : available only for LO level DY Samples
  zpt_reweight = 1.;
  top_pt_reweight = 1.;
  if(is_DY){
    vector<Gen> gens = GetGens();
    zpt_reweight = mcCorr->GetOfficialDYReweight(gens);
    //cout << "zpt_reweight : " << zpt_reweight << endl; 
  }
  
  if(is_TTLL){// -- Calculate top pt reweight value
    std::vector<Gen> gens = GetGens();
    double top_pt = 9999.;
    double atop_pt = 9999.;
    bool top_found = false;
    bool atop_found = false;
    for(unsigned int i_gen = 0; i_gen < gens.size(); i_gen++){
      if(gens.at(i_gen).PID() == 6 && gens.at(i_gen).Status() == 22) {
        top_found = true;
	top_pt = gens.at(i_gen).Pt();
      }
      if(gens.at(i_gen).PID() == -6 && gens.at(i_gen).Status() == 22) {
	atop_found = true;
        atop_pt = gens.at(i_gen).Pt();
      }
    }

    if(top_found && atop_found){
      if(top_pt < 800. && atop_pt < 800.){
        double top_weight = exp(0.0615 - 0.0005 * top_pt);
        double atop_weight = exp(0.0615 - 0.0005 * atop_pt);
        top_pt_reweight = top_weight * top_weight;
      }
    }
  }

  //only for signal sample
  /*
  std::vector<Gen> gen_signal = GetGens();
  int N_Bmeson = 0;
  int N_Bquark = 0;
  for(unsigned int i_gen = 0; i_gen < gen_signal.size(); i_gen++){
    int current_pid = gen_signal.at(i_gen).PID();
    int current_status = gen_signal.at(i_gen).Status();
    if(abs(current_pid) > 499 && abs(current_pid) < 600){
      N_Bmeson ++;
    }
    if(abs(current_pid) ==  5 && current_status == 21 && current_status == 23){
      N_Bquark ++;
    }
  }
  cout << "==================" << endl;
  cout << "N_Bmeson : "  << N_Bmeson << endl;
  cout << "N_Bquark : "  << N_Bquark << endl;
  */
  //=======================
  //==== Run Main Function
  //=======================
  Select_syst_objects(param);
  
}

void SR_ZpNN::Select_syst_objects(AnalyzerParameter param){
  
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
  
  
  // ==== Save Muons with TuneP
  std::vector<Muon> muons_all;
  if(!IsData) muons_all = UseTunePMuon( GetAllMuons() );   else muons_all = UseTunePMuon( GetAllMuons() );
  std::vector<Muon> muons_all_Scale_Up;
  if(!IsData) muons_all_Scale_Up = ScaleMuons(muons_all, 1);              else muons_all_Scale_Up = UseTunePMuon( GetAllMuons() );
  std::vector<Muon> muons_all_Scale_Down;
  if(!IsData) muons_all_Scale_Down = ScaleMuons(muons_all, -1);           else muons_all_Scale_Down = UseTunePMuon( GetAllMuons() );
  std::vector<Muon> muons_all_Smear_Up;   
  //if(!IsData) muons_all_Smear_Up = SmearTunePMuons(muons_all, 1);              else muons_all_Smear_Up = UseTunePMuon( GetAllMuons() );
  muons_all_Smear_Up = UseTunePMuon( GetAllMuons() );
  std::vector<Muon> muons_all_Smear_Down; 
  //if(!IsData) muons_all_Smear_Down = SmearTunePMuons(muons_all, -1);           else muons_all_Smear_Down = UseTunePMuon( GetAllMuons() );
  muons_all_Smear_Down = UseTunePMuon( GetAllMuons() );

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


  // ==== Get FatJets selection with syst flags & Save in to a vector of vectors
  std::vector<FatJet> FatJets_all = puppiCorr->Correct( GetAllFatJets() );
  std::vector<FatJet> FatJets_all_Scale_Up   = ScaleFatJets(puppiCorr->Correct( GetAllFatJets() ),  1);
  std::vector<FatJet> FatJets_all_Scale_Down = ScaleFatJets(puppiCorr->Correct( GetAllFatJets() ), -1);
  std::vector<FatJet> FatJets_all_Res_Up;
  if(!IsData) FatJets_all_Res_Up = SmearFatJets(  puppiCorr->Correct( GetAllFatJets() ),  1);
  else FatJets_all_Res_Up = puppiCorr->Correct( GetAllFatJets() );
  std::vector<FatJet> FatJets_all_Res_Down;
  if(!IsData) FatJets_all_Res_Down = SmearFatJets(  puppiCorr->Correct( GetAllFatJets() ), -1);
  else FatJets_all_Res_Down = puppiCorr->Correct( GetAllFatJets() );
  
  std::vector<FatJet> FatJets_all_SDMass_Scale_Up;
  if(!IsData) FatJets_all_SDMass_Scale_Up = ScaleSDMassFatJets(  puppiCorr->Correct( GetAllFatJets() ),  1);
  else FatJets_all_SDMass_Scale_Up = puppiCorr->Correct( GetAllFatJets() );
  
  std::vector<FatJet> FatJets_all_SDMass_Scale_Down;
  if(!IsData) FatJets_all_SDMass_Scale_Down = ScaleSDMassFatJets(  puppiCorr->Correct( GetAllFatJets() ),  -1);
  else FatJets_all_SDMass_Scale_Down = puppiCorr->Correct( GetAllFatJets() );
  
  std::vector<FatJet> FatJets_all_SDMass_Res_Up;
  if(!IsData) FatJets_all_SDMass_Res_Up = SmearSDMassFatJets(  puppiCorr->Correct( GetAllFatJets() ),  1);
  else FatJets_all_SDMass_Res_Up = puppiCorr->Correct( GetAllFatJets() );
  
  std::vector<FatJet> FatJets_all_SDMass_Res_Down;
  if(!IsData) FatJets_all_SDMass_Res_Down = SmearSDMassFatJets(  puppiCorr->Correct( GetAllFatJets() ),  -1);
  else FatJets_all_SDMass_Res_Down = puppiCorr->Correct( GetAllFatJets() );

  //puppiCorr;
  
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
    

  // ==== Define Systematic flag strings and order
  const int N_systs = 41;
  TString syst_flags[N_systs] = {"central", 
				 "ElectronScaleUp", "ElectronScaleDown", "ElectronSmearUp", "ElectronSmearDown", 
				 "MuonScaleUp", "MuonScaleDown", "MuonSmearUp", "MuonSmearDown",
				 "JetsScaleUp", "JetsScaleDown", "JetsResUp", "JetsResDown", 
				 "SD_JMS_Up", "SD_JMS_Down", "SD_JMR_Up", "SD_JMR_Down", 
				 "PDFNormUp", "PDFNormDown", "PDFScaleUp", "PDFScaleDown", 
				 "PUReweight_Up", "PUReweight_Down",
				 "Prefire_Up", "Prefire_Down",
				 "MuonRecoSFUp", "MuonRecoSFDown",
				 "MuonIDSFUp", "MuonIDSFDown",
				 "MuonISOSFUp", "MuonISOSFDown",
				 "MuonTriggerSFUp", "MuonTriggerSFDown",
				 "ElectronRecoSFUp", "ElectronRecoSFDown",
				 "ElectronIDSFUp", "ElectronIDSFDown",
				 "ElectronTriggerSFUp", "ElectronTriggerSFDown",
				 "ZPtRwUp", "ZPtRwDown",
  };
  
  int electron_index[N_systs] = {0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int muon_index[N_systs]     = {0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int jet_index[N_systs]      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int fatjet_index[N_systs]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  int PDF_index[N_systs]      = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  //double nnpdf_nnlo_pdf = GetPDFWeight("NNPDF31_nnlo_hessian_pdfas", 0);
  //double ct14_nlo_pdf = GetPDFWeight("CT14nlo", 0);
  //double pdf_ratio = ct14_nlo_pdf / nnpdf_nnlo_pdf;
  double pdf_ratio = 1.;
  
  for(int i_syst = 0; i_syst < N_systs; i_syst++){
    //executeEventFromParameter(  param, electrons_all_syst.at(electron_index[i_syst]), muons_all_syst.at(muon_index[i_syst]), Jets_all_syst.at(jet_index[i_syst]), FatJets_all_syst.at(fatjet_index[i_syst]), syst_flags[i_syst], PDF_weight_syst.at(PDF_index[i_syst])  ); 
    executeEventFromParameter(  param, electrons_all_syst.at(electron_index[i_syst]), muons_all_syst.at(muon_index[i_syst]), Jets_all_syst.at(jet_index[i_syst]), FatJets_all_syst.at(fatjet_index[i_syst]), syst_flags[i_syst], pdf_ratio  );
  }
  
}

void SR_ZpNN::executeEventFromParameter(AnalyzerParameter param, std::vector<Electron> electron_all, std::vector<Muon> muons_all, std::vector<Jet> jets_all, std::vector<FatJet> fatjets_all, TString syst_flag, double PDF_weight){
  //fatjets_all
  //cout << "[executeEventFromParameter] syst_flag : " << syst_flag << endl;
  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  // -- Get Gen level info
  //vector<Gen> gens = GetGens();

  // -- HLT
  bool Pass_diele = ev.PassTrigger(Triggers_Electron);
  bool Pass_mu50 = ev.PassTrigger(Triggers_Muon);

  // -- Set weight for Normalization 
  double weight_1pb = 1.;
  double weight_trig_diele = 1.;
  double weight_trig_mu50 = 1.;
  double pileup_reweight = 1.;

  if(!IsData){
    weight_1pb = weight_norm_1invpb * ev.MCweight();
    weight_trig_diele = ev.GetTriggerLumi("Full");
    weight_trig_mu50 = ev.GetTriggerLumi("Full");
    pileup_reweight = weight_PU;
    if(syst_flag.Contains("PUReweight_Up")) pileup_reweight = weight_PU_Up;
    if(syst_flag.Contains("PUReweight_Down")) pileup_reweight = weight_PU_Down;
  }
  
  //cout << "[executeEventFromParameter] MCSample : " << MCSample << endl;
  TString current_MC = "";
  if(!IsDATA) current_MC = MCSample;
  // -- Corrections Based on Truth Level Particles
  bool is_TTLL = current_MC.Contains("TTLL");
  bool is_DY = current_MC.Contains("DYJets_MG"); // -- https://github.com/IzaakWN/NanoTreeProducer/blob/master/corrections/RecoilCorrectionTool.py#L95-L109 : available only for LO level DY samples

  //cout << "[executeEventFromParameter pileup_reweight : " << pileup_reweight << endl;
  weight_trig_diele = weight_trig_diele * weight_1pb * pileup_reweight;
  weight_trig_mu50 = weight_trig_mu50 * weight_1pb * pileup_reweight;

  FillHist("signal_eff", 0.5, weight_trig_diele, 40, 0., 40.); // cutflow - before trigger

  if(!PassMETFilter()) return;
  //cout << "[executeEventFromParameter] Before trigger" << endl;
  if(!Pass_diele && !Pass_mu50) return;
  //cout << "[executeEventFromParameter] After trigger" << endl;

  if(Pass_diele) FillHist("signal_eff", 1.5, weight_trig_diele, 40, 0., 40.); // cutflow - DoublePhoton HLT
  if(Pass_mu50) FillHist("signal_eff", 2.5, weight_trig_mu50, 40, 0., 40.); // cutflow - SingleMuon HLT

  // -- Save Electrons and Numbers of them
  double MinPt = 75.;  
  if(DataYear==2016) MinPt = 65.;
  std::vector<Electron> electrons_Veto = SelectElectrons(electron_all, param.Electron_Veto_ID, 35., 2.5);
  std::vector<Electron> electrons_Loose = SelectElectrons(electron_all, param.Electron_Loose_ID, param.Electron_MinPt, 2.5);
  std::vector<Electron> electrons_Tight = SelectElectrons(electron_all, param.Electron_Tight_ID, param.Electron_MinPt, 2.5);
  std::vector<Electron> electrons_Tight_all = SelectElectrons(electron_all, param.Electron_Tight_ID, param.Electron_MinPt, 2.5);

  // -- Save Muons and Numbers of them
  std::vector<Muon> muons_Veto  = SelectMuons(muons_all, param.Muon_Veto_ID,  10., 2.4);
  std::vector<Muon> muons_Loose = SelectMuons(muons_all, param.Muon_Loose_ID, 10., 2.4);
  std::vector<Muon> muons_Tight = SelectMuons(muons_all, param.Muon_Tight_ID, param.Muon_MinPt, 2.4);
  std::vector<Muon> muons_Tight_all = SelectMuons(muons_all, param.Muon_Tight_ID, param.Muon_MinPt, 2.4);

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
    Jet this_jet = alljets.at(i);
    double this_discr = this_jet.GetTaggerResult(JetTagging::DeepCSV);
    if( mcCorr->IsBTagged_2a(jtps.at(0), this_jet) ) NBJets++; //2a
  }
  /*
  if(syst_flag.Contains("central")){
    cout << "NBJets : " << NBJets << endl;
    cout << "=====================" << endl;
  }
  */
  // -- Get Prefire weight
  //std::vector<Photon> photons            = GetPhotons("passMediumID", 20., 3.0);
  //std::vector<Jet>    jets_prefire       = JetsAwayFromPhoton(GetJets("tight", 40., 3.5), photons, 0.4);
  double prefire_weight = 1.;
  if(!IsDATA && DataYear != 2018) {
    prefire_weight = L1PrefireReweight_Central;
    if(syst_flag.Contains("Prefire_Up")) prefire_weight = L1PrefireReweight_Up;
    if(syst_flag.Contains("Prefire_Down")) prefire_weight= L1PrefireReweight_Down;
  }
  //cout << "[executeEventFromParameter] prefire_weight : " << prefire_weight << endl;
  //cout << "------------------------" << endl;
  //cout << "[executeEventFromParameter] N_electrons_Loose = " << N_electrons_Loose << ", N_electrons_Veto = " << N_electrons_Veto << endl;
  //cout << "[executeEventFromParameter] N_muons_Loose = " <<N_muons_Loose << ", N_muons_Veto = " <<N_muons_Veto << endl;
  
  if(N_electrons_Loose != N_electrons_Veto) return;
  if(N_muons_Loose != N_muons_Veto) return;
  
  //cout << "[executeEventFromParameter] Passed N loose = N veto cut " << endl;

  
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
  
  //cout << "[executeEventFromParameter] current_channel : " << current_channel << endl;

  double current_weight = weight_trig_mu50;// * prefire_weight;// FIXME
  //cout << "norm_prefire current_weight : " << current_weight << endl;

  int syst_MuonRecoSF= 0;
  int syst_MuonIDSF = 0;
  int syst_MuonISOSF = 0;
  int syst_MuonTriggerSF = 0;
  int syst_ElectronRecoSF = 0;
  int syst_ElectronIDSF = 0;
  int syst_ElectronTriggerSF = 0;
  int syst_ZPtRwUp = 0;
  // -- Set syst int
  if(syst_flag.Contains("MuonRecoSFUp")) syst_MuonRecoSF = 1;
  if(syst_flag.Contains("MuonIDSFUp")) syst_MuonIDSF = 1;
  if(syst_flag.Contains("MuonISOSFUp")) syst_MuonISOSF = 1;
  if(syst_flag.Contains("MuonTriggerSFUp")) syst_MuonTriggerSF = 1;
  if(syst_flag.Contains("ElectronRecoSFUp")) syst_ElectronRecoSF = 1;
  if(syst_flag.Contains("ElectronIDSFUp")) syst_ElectronIDSF = 1;
  if(syst_flag.Contains("ElectronTriggerSFUp")) syst_ElectronTriggerSF = 1;
  if(syst_flag.Contains("ZPtRwUp")) syst_ZPtRwUp = 1;

  if(syst_flag.Contains("MuonRecoSFDown")) syst_MuonRecoSF = -1;
  if(syst_flag.Contains("MuonIDSFDown")) syst_MuonIDSF = -1;
  if(syst_flag.Contains("MuonISOSFDown")) syst_MuonISOSF = -1;
  if(syst_flag.Contains("MuonTriggerSFDown")) syst_MuonTriggerSF = -1;
  if(syst_flag.Contains("ElectronRecoSFDown")) syst_ElectronRecoSF = -1;
  if(syst_flag.Contains("ElectronIDSFDown")) syst_ElectronIDSF = -1;
  if(syst_flag.Contains("ElectronTriggerSFDown")) syst_ElectronTriggerSF = -1;
  if(syst_flag.Contains("ZPtRwDown")) syst_ZPtRwUp = -1;

  if(!IsDATA){
    current_weight = current_weight * PDF_weight;
    
    mcCorr->IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

    for(unsigned int i = 0; i < electrons_Veto.size(); i++){
      double this_recosf = mcCorr->ElectronReco_SF(electrons_Veto.at(i).scEta(), electrons_Veto.at(i).Pt(), syst_ElectronRecoSF);
      //double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, electrons_Veto.at(i).scEta(), electrons_Veto.at(i).Pt(), syst_ElectronIDSF);
      
      current_weight *= this_recosf;//*this_idsf;
    }
    //cout << "electron sf current_weight : " << current_weight << endl;
    for(unsigned int i = 0; i < electrons_Tight.size(); i++){
      double this_idsf = mcCorr->ElectronID_SF(param.Electron_ID_SF_Key, electrons_Tight.at(i).scEta(), electrons_Tight.at(i).Pt(), syst_ElectronIDSF);
      current_weight *= this_idsf;
    }

    for(unsigned int i = 0; i < muons_Veto.size(); i++){
      double MiniAODP = sqrt( muons_Veto.at(i).MiniAODPt() * muons_Veto.at(i).MiniAODPt() + muons_Veto.at(i).Pz() * muons_Veto.at(i).Pz() );

      double this_recosf = mcCorr->MuonReco_SF(param.Muon_RECO_SF_Key, muons_Veto.at(i).Eta(), MiniAODP, syst_MuonRecoSF);
      double this_idsf  = mcCorr->MuonID_SF (param.Muon_ID_SF_Key,  muons_Veto.at(i).Eta(), muons_Veto.at(i).MiniAODPt(), syst_MuonIDSF);
      //double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, muons_Veto.at(i).Eta(), muons_Veto.at(i).MiniAODPt(), syst_MuonISOSF);

      current_weight *= this_recosf * this_idsf;// * this_isosf;
    }
    //cout << "muon sf current_weight : " << current_weight << endl;

    for(unsigned  int i = 0; i < muons_Tight.size(); i++){
      double this_isosf = mcCorr->MuonISO_SF(param.Muon_ISO_SF_Key, muons_Tight.at(i).Eta(), muons_Tight.at(i).MiniAODPt());
      current_weight *= this_isosf;
    }
    
    //double trigger_sf_SingleElectron = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, TriggerNameForSF_Electron, electrons_Veto, 0);
    double trigger_sf_DoublePhoton = 1.;
    //cout << "syst_flag : " << syst_flag<< endl;

    if(electrons_Tight.size() > 0) trigger_sf_DoublePhoton = mcCorr->ElectronTrigger_SF(param.Electron_Trigger_SF_Key, TriggerNameForSF_Electron, electrons_Tight, syst_ElectronTriggerSF);
    double trigger_sf_SingleMuon = 1.;
    if(muons_Tight.size() > 0) trigger_sf_SingleMuon = mcCorr->MuonTrigger_SF(param.Muon_Trigger_SF_Key, TriggerNameForSF_Muon, muons_Tight, syst_MuonTriggerSF);
    
    current_weight *= trigger_sf_SingleMuon * trigger_sf_DoublePhoton;// *trigger_sf_SingleElectron;
    //cout << "muons_Tight.size() : " << muons_Tight.size() << endl;
    //cout << "trigger_sf_SingleMuon : " << trigger_sf_SingleMuon << endl;
    //cout << "trig sf current_weight : " << current_weight << endl;

  }

  current_weight *= top_pt_reweight;
  //cout << "current_weight : " << current_weight << endl;
  
  SR(param, current_channel + "DYreweight_" + syst_flag, trig_pass_for_channel,  current_weight * prefire_weight, jets, fatjets, electrons_Tight_all, electrons_Veto, muons_Tight_all, muons_Veto, N_electrons_Tight_all, N_electrons_Veto, N_muons_Tight_all, N_muons_Veto);
  SR(param, current_channel + "DYreweight_NoPFweight" + syst_flag, trig_pass_for_channel,  current_weight, jets, fatjets, electrons_Tight_all, electrons_Veto, muons_Tight_all, muons_Veto, N_electrons_Tight_all, N_electrons_Veto, N_muons_Tight_all, N_muons_Veto);

}

void SR_ZpNN::SR(AnalyzerParameter param, TString channel, bool trig_pass, double weight, std::vector<Jet> jets, std::vector<FatJet> fatjets, std::vector<Electron> electrons, std::vector<Electron> electrons_veto, std::vector<Muon> muons, std::vector<Muon> muons_veto,
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
    if(Lep_1st_Pt < 75 || Lep_2nd_Pt < 75) return;
  }
  else if(channel.Contains("DiMu")){
    if(Lep_1st_Pt < 55 || Lep_2nd_Pt < 55) return;
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
    Jet this_jet = alljets_sub.at(i);
    double this_discr = this_jet.GetTaggerResult(JetTagging::DeepCSV);
    if( mcCorr->IsBTagged_2a(jtps.at(0), this_jet) ) NBJets++; //2a
  }
  int N_jet = alljets_sub.size();
  //if(NBJets > 0) return;
  TString Region_str = "SR_";

  // -- m(ll) > 150 GeV
  Particle ll = *(Leptons_veto.at(0)) + *(Leptons_veto.at(1));
  double M_ll = ll.M();
  if(M_ll < 150 ) return;
  
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 24.5, weight, 40, 0., 40.); // cutflow - DiEle mll mass
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 25.5, weight, 40, 0., 40.); // cutflow - DiMu mll mass
  }
  else if(channel.Contains("EMu")){
  }  
  else return;

  vector<Particle> Ns = RecoPairN(Leptons, fatjets, jets);
  if(Ns.size() != 2) return;
  TString jetbin_str = Get_N_jet_bin(Leptons, fatjets, jets);
  
  if(channel.Contains("DiEle")){
    FillHist("signal_eff", 26.5, weight, 40, 0., 40.); // cutflow - DiEle Njet
  }
  else if(channel.Contains("DiMu")){
    FillHist("signal_eff", 27.5, weight, 40, 0., 40.); // cutflow - DiMu Njet
  }
  else if(channel.Contains("EMu")){
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
  else if(channel.Contains("EMu")){
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

  JSFillHist(Region_str + channel, "mZp_" + Region_str + channel, Zp.M(), weight, 6000, 0., 6000.);
  FillHNPairPlots(Ns, Region_str + channel, jetbin_str, weight);
  
  if(channel.Contains("central")){
    double central_PDF = PDFWeights_Scale->at(0);
    unsigned int size_scale_err = PDFWeights_Scale->size();
    unsigned int size_PDF_err = PDFWeights_Error->size();
    unsigned int size_renorm_err = PDFWeights_AlphaS->size();
    /*
    cout << "=====================" << endl; 
    cout << "central_PDF : " << central_PDF << endl;
    cout << "size_scale_err : " << size_scale_err << endl;
    cout << "size_PDF_err : " << size_PDF_err << endl;
    cout << "size_renorm_err : " << size_renorm_err << endl;
    
    cout << "PDFWeights_Error->at(0) : " << PDFWeights_Error->at(0) << endl;
    cout << "PDFWeights_Error->at(26) : " << PDFWeights_Error->at(26) << endl;
    cout << "PDFWeights_Error->at(50) : " << PDFWeights_Error->at(50) << endl;
    cout << "PDFWeights_AlphaS->at(1) : " << PDFWeights_AlphaS->at(1) << endl;
    cout << "PDFWeights_Scale->at(2) : " << PDFWeights_Scale->at(2) << endl;
    */
    // -- Save Hessian PDF errors
    for(unsigned int i_err = 1; i_err < PDFWeights_Error->size(); i_err++){
      TString current_error = "Hessian_" + TString::Itoa(i_err, 10);
      double current_PDF_weight = PDFWeights_Error->at(i_err) / PDFWeights_Error->at(0);
      if(DataYear==2016 || DataYear==2017) current_PDF_weight = 2.0 * current_PDF_weight;
      
      JSFillHist(Region_str + channel, "mZp_" + Region_str + channel + "_" + current_error, Zp.M(), weight * current_PDF_weight, 6000, 0., 6000.);
      FillHNPairPlots(Ns, Region_str + channel, jetbin_str, current_error, weight * current_PDF_weight);
    }
    
    // -- Save Alpha_S errors
    for(unsigned int i_err = 0; i_err < PDFWeights_AlphaS->size(); i_err++){
      TString current_error = "AlphaS_" + TString::Itoa(i_err, 10);
      double current_PDF_weight = PDFWeights_AlphaS->at(i_err) / PDFWeights_Error->at(0);
      if(DataYear==2016 || DataYear==2017) current_PDF_weight = -1./4. + 2.0 * current_PDF_weight;
      JSFillHist(Region_str + channel, "mZp_" + Region_str + channel + "_" + current_error, Zp.M(), weight * current_PDF_weight, 6000, 0., 6000.);
      FillHNPairPlots(Ns, Region_str + channel, jetbin_str, current_error, weight * current_PDF_weight);
    }
    
    // -- Save Scale errors
    for(unsigned int i_err = 1; i_err < PDFWeights_Scale->size(); i_err++){
      TString current_error = "Scale_" + TString::Itoa(i_err, 10);
      double current_PDF_weight = PDFWeights_Scale->at(i_err) / PDFWeights_Error->at(0);
      if(DataYear==2016 || DataYear==2017) current_PDF_weight = 2.0 * current_PDF_weight;
      JSFillHist(Region_str + channel, "mZp_" + Region_str + channel + "_" + current_error, Zp.M(), weight * current_PDF_weight, 6000, 0., 6000.);
      FillHNPairPlots(Ns, Region_str + channel, jetbin_str, current_error, weight * current_PDF_weight);
    }
  }
  
  
}

vector<Particle> SR_ZpNN::RecoPairN(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets){

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


TString SR_ZpNN::Get_N_jet_bin(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets){
  
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

vector<FatJet> SR_ZpNN::Fatjet_Z_clean(vector<FatJet> fatjets, Particle ll){
  std::vector<FatJet> out;
  
  for(unsigned int i=0; i<fatjets.size(); i++){
    if(fatjets.at(i).DeltaR(ll) > 0.8){
      out.push_back( fatjets.at(i) );
    }
  }

  return out;
}

SR_ZpNN::SR_ZpNN(){

}

SR_ZpNN::~SR_ZpNN(){

}


