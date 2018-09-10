#include "HNWRAnalyzer.h"

void HNWRAnalyzer::initializeAnalyzer(){

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");

  cout << "[HNWRAnalyzer::initializeAnalyzer] RunFake = " << RunFake << endl;
  cout << "[HNWRAnalyzer::initializeAnalyzer] RunCF = " << RunCF << endl;

}

void HNWRAnalyzer::executeEvent(){

  //==========================
  //==== Gen for genmatching
  //==========================

  gens = GetGens();

  //========================
  //==== AnalyzerParameter
  //========================

  AnalyzerParameter param;
  param.Clear();

  param.Name = "HNWR";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNWRTight";
  param.Electron_Tight_RelIso = 0.15;
  param.Electron_Loose_ID = "HNWRLoose";
  param.Electron_Loose_RelIso = 0.6;
  param.Electron_Veto_ID = "HNWRVeto";
  param.Electron_Veto_RelIso = 0.6;
  param.Electron_FR_ID = "HNWR";
  param.Electron_FR_Key = "AwayJetPt40";
  param.Electron_CF_ID = "HNWR";
  param.Electron_CF_Key = "ZToLL";
  param.Electron_UseMini = false;
  param.Electron_UsePtCone = false;
  param.Electron_MinPt = 10.;

  param.Muon_Tight_ID = "HNWRTight";
  param.Muon_Tight_RelIso = 0.15;
  param.Muon_Loose_ID = "HNWRLoose";
  param.Muon_Loose_RelIso = 0.6;
  param.Muon_Veto_ID = "HNWRVeto";
  param.Muon_Veto_RelIso = 0.6;
  param.Muon_FR_ID = "HNWR";
  param.Muon_FR_Key = "AwayJetPt40";
  param.Muon_CF_ID = "HNWR";
  param.Muon_CF_Key = "ZToLL";
  param.Muon_UseMini = false;
  param.Muon_UsePtCone = false;
  param.Muon_MinPt = 10.;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  executeEventFromParameter(param);

}

void HNWRAnalyzer::executeEventFromParameter(AnalyzerParameter param){

  //=============
  //==== No Cut
  //=============

  FillHist("NoCut_"+param.Name, 0., 1., 1, 0., 1.);

  //========================
  //==== Event selecitions
  //========================

  if(!PassMETFilter()) return;

  Event ev = GetEvent();
  Particle METv = ev.GetMETVector();

  bool PassSingleMuon = ev.PassTrigger("HLT_IsoMu27_v");
  bool PassSingleElectron = ev.PassTrigger("HLT_Ele35_WPTight_Gsf_v");

  std::vector<Electron> Veto_electrons  = GetElectrons(param.Electron_Veto_ID, param.Electron_MinPt, 2.5);
  std::vector<Muon>     Veto_muons      = GetMuons(param.Muon_Veto_ID, param.Muon_MinPt, 2.4);

  std::vector<Electron> Loose_electrons = ElectronPromptOnly(GetElectrons(param.Electron_Loose_ID, param.Electron_MinPt, 2.5), gens);
  std::vector<Muon>     Loose_muons     = MuonPromptOnly(GetMuons(param.Muon_Loose_ID, param.Muon_MinPt, 2.4), gens);
  std::vector<Electron> Tight_electrons;
  std::vector<Muon>     Tight_muons;

  for(unsigned int i=0; i<Loose_electrons.size(); i++){

    //==== Is Tight?
    if(Loose_electrons.at(i).PassID(param.Electron_Tight_ID)) Tight_electrons.push_back( Loose_electrons.at(i) );

  }
  for(unsigned int i=0; i<Loose_muons.size(); i++){

    //==== Is Tight?
    if(Loose_muons.at(i).PassID(param.Muon_Tight_ID)) Tight_muons.push_back( Loose_muons.at(i) );

  }

  int n_Veto_leptons = Veto_electrons.size()+Veto_muons.size();
  int n_Loose_leptons = Loose_electrons.size()+Loose_muons.size();
  int n_Tight_leptons = Tight_electrons.size()+Tight_muons.size();

  bool HasCFElectron = false;
  if(!IsDATA){
    for(unsigned int i=0; i<Loose_electrons.size(); i++){
      Gen genlep = GetGenMatchedLepton(Loose_electrons.at(i) , gens);
      if(genlep.Charge()!=Loose_electrons.at(i).Charge()){
        HasCFElectron = true;
        break;
      }
    }
  }

  bool NoExtraLepton = (n_Veto_leptons==n_Loose_leptons);
  bool IsAllTight = (n_Loose_leptons==n_Tight_leptons);

  //==== Veto Extra Lepton
  if(!NoExtraLepton) return;

  //==== Loose sample or not
  if(RunFake){
    if(IsAllTight) return;
  }
  else{
    if(!IsAllTight) return;
  }

  //===========
  //==== Jets
  //===========

  std::vector<FatJet>   fatjets         = GetFatJets(param.FatJet_ID, 200, 2.7);

  std::vector<Jet>      alljets         = GetJets(param.Jet_ID, 20., 2.7);
  std::vector<Jet>      jets            = JetsVetoLeptonInside(JetsAwayFromFatJet(alljets, fatjets), Veto_electrons, Veto_muons);

  int NBJets=0;
  for(unsigned int i=0; i<alljets.size(); i++){
    if(alljets.at(i).IsTagged(Jet::CSVv2, Jet::Medium)) NBJets++;
  }

  //==============
  //==== Sum Pts
  //==============

  double HT(0.);
  for(unsigned int i=0; i<jets.size(); i++){
    HT += jets.at(i).Pt();
  }
  for(unsigned int i=0; i<fatjets.size(); i++){
    HT += fatjets.at(i).Pt();
  }

  //====================================================
  //==== Based on which trigger is fired
  //==== - Make sure, there is no duplicated events
  //====   between Suffixs. Apply exculsive selections
  //====   in map_bool_To_Region
  //====================================================

  std::vector< TString > Suffixs = {
    "SingleMuon", // at least two muons
    "SingleElectron", // at least two muons
  };
  std::vector< bool > PassTriggers = {
    PassSingleMuon && (Loose_electrons.size()==0) && (Loose_muons.size()>=1),
    PassSingleMuon && (Loose_electrons.size()>=1) && (Loose_muons.size()==0),
  };

  //=================
  //==== Start loop
  //=================

  for(unsigned int it_Suffix=0; it_Suffix<Suffixs.size(); it_Suffix++){

    TString Suffix = Suffixs.at(it_Suffix);
    if( !PassTriggers.at(it_Suffix) ) continue;

    if(Suffix.Contains("SingleMuon")){
      if( Loose_muons.at(0).Pt() < 29. ) continue;
    }
    else if(Suffix.Contains("SingleElectron")){
      if( Loose_electrons.at(0).Pt() < 38. ) continue;
    }
    else{

    }

    if(this->IsDATA){
      if(this->DataStream == "SingleMuon"){
        if(!( Suffix.Contains("SingleMuon") )) continue;
      }
      else if(this->DataStream == "SingleElectron"){
        if(!( Suffix.Contains("SingleElectron") )) continue;
      }
      else{

      }
    }

    std::vector<Lepton *> leps_el, leps_mu;
    std::vector<Lepton *> leps;
    leps_el = MakeLeptonPointerVector(Loose_electrons);
    leps_mu = MakeLeptonPointerVector(Loose_muons);
    for(unsigned int i=0; i<leps_el.size(); i++) leps.push_back( leps_el.at(i) );
    for(unsigned int i=0; i<leps_mu.size(); i++) leps.push_back( leps_mu.at(i) );

    double weight = 1.;
    if(!IsDATA){
      weight *= weight_norm_1invfb*ev.GetTriggerLumi("Full")*ev.MCweight();

      mcCorr.IgnoreNoHist = param.MCCorrrectionIgnoreNoHist;

      for(unsigned int i=0; i<Loose_electrons.size(); i++){
        double this_recosf = mcCorr.ElectronReco_SF(Loose_electrons.at(i).scEta(),Loose_electrons.at(i).Pt());
        double this_idsf = mcCorr.ElectronID_SF(param.Electron_ID_SF_Key, Loose_electrons.at(i).scEta(), Loose_electrons.at(i).Pt());
        weight *= this_recosf*this_idsf;
      }
      for(unsigned int i=0; i<Loose_muons.size(); i++){
        double this_idsf  = mcCorr.MuonID_SF (param.Muon_ID_SF_Key,  Loose_muons.at(i).Eta(), Loose_muons.at(i).MiniAODPt());
        double this_isosf = mcCorr.MuonISO_SF(param.Muon_ISO_SF_Key, Loose_muons.at(i).Eta(), Loose_muons.at(i).MiniAODPt());
        double this_trigsf = mcCorr.MuonTrigger_SF(param.Muon_Trigger_SF_Key, "IsoMu27", Loose_muons);

        weight *= this_idsf*this_isosf*this_trigsf;
      }

    }
    else{

      if(RunFake){
        weight = fakeEst.GetWeight(leps, param);
        if(! (fakeEst.HasLooseLepton) ){
          cout << "--> WTF" << endl;
          exit(EXIT_FAILURE);
        }
      }

      if(RunCF){
        weight = cfEst.GetWeight(leps, param);
      }

    }

    std::map<TString, bool> TMP_map_bool_To_Region; // For SS/OS
    std::map<TString, bool> map_bool_To_Region;

    //==== One Lepton; Use IsOneLeptonEvent
    bool IsOneLeptonEvent = false;
    bool IsOneLeptonEvent_FatJet = false;
    if(leps.size()==1){

      if(fatjets.size()>=1){
        map_bool_To_Region["SingleLepton_WithFatJet"] = true;
        IsOneLeptonEvent_FatJet = true;
      }

    }

    //==== Two Lepton; Use IsTwoLeptonEvent
    bool IsTwoLeptonEvent = false;
    bool IsTwoLeptonEvent_TwoJet = false;
    bool IsOS = false;
    Particle ZCand_IsTwoLeptonEvent;
    if(leps.size()==2){
      IsTwoLeptonEvent = true;

      ZCand_IsTwoLeptonEvent = (*leps[0])+(*leps[1]); // Only works for two lepton case
      IsOS = (leps[0]->Charge() != leps[1]->Charge()); // Only works for two lepton case
      if(RunCF){
        if(!IsOS) continue;
        IsOS = !IsOS;
      }

      TMP_map_bool_To_Region["DiLepton"] = true;
      if(jets.size()>=2){
        TMP_map_bool_To_Region["DiLepton_WithTwoJet"] = true;
        IsTwoLeptonEvent_TwoJet = true;
      }

      for(std::map<TString, bool>::iterator it_map = TMP_map_bool_To_Region.begin(); it_map != TMP_map_bool_To_Region.end(); it_map++){
        TString this_region = it_map->first;

        if(it_map->second){
          if(IsOS) map_bool_To_Region[this_region+"_OS"] = true;
          else     map_bool_To_Region[this_region+"_SS"] = true;
        }

      }

    }

    for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

      TString this_region = it_map->first;
      this_region = (param.Name)+"_"+Suffix+"_"+this_region;

      if(it_map->second){

        JSFillHist(this_region, "NEvent_"+this_region, 0., weight, 1, 0., 1.);
        JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 1000., 0., 1000.);

        JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

        if(IsOneLeptonEvent_FatJet){
          JSFillHist(this_region, "lJ_Mass_"+this_region, ((*leps.at(0))+fatjets.at(0)).M(), weight, 2000, 0., 2000.);
        }
        if(IsTwoLeptonEvent){
          JSFillHist(this_region, "ZCand_Mass_"+this_region, ZCand_IsTwoLeptonEvent.M(), weight, 2000, 0., 2000.);
          if(IsTwoLeptonEvent_TwoJet){
            JSFillHist(this_region, "lljj_Mass_"+this_region, (ZCand_IsTwoLeptonEvent+jets.at(0)+jets.at(1)).M(), weight, 2000, 0., 2000.);
          }
        }

        FillLeptonPlots(leps, this_region, weight);

      } // END if(pass Region)

    } // END Loop mapRegionBool

  } // END Loop Suffixs



}

HNWRAnalyzer::HNWRAnalyzer(){

  RunFake = false;

}

HNWRAnalyzer::~HNWRAnalyzer(){

}
