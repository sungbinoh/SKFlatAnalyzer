#include "HNPairAnalyzer.h"

void HNPairAnalyzer::initializeAnalyzer(){

  RunFake = HasFlag("RunFake");
  RunCF = HasFlag("RunCF");

  cout << "[HNPairAnalyzer::initializeAnalyzer] RunFake = " << RunFake << endl;
  cout << "[HNPairAnalyzer::initializeAnalyzer] RunCF = " << RunCF << endl;

}

void HNPairAnalyzer::executeEvent(){

  AnalyzerParameter param;

  param.Name = "HNPair";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Electron_Tight_ID = "HNPairTight";
  param.Electron_Tight_RelIso = 0.1;
  param.Electron_Loose_ID = "HNPairLoose";
  param.Electron_Loose_RelIso = 0.6;
  param.Electron_Veto_ID = "HNPairVeto";
  param.Electron_Veto_RelIso = 0.6;
  param.Electron_FR_ID = "HNPair_PtCut";
  param.Electron_FR_Key = "AwayJetPt40";
  param.Electron_CF_ID = "HNPairTight";
  param.Electron_CF_Key = "ZToLL";
  param.Electron_UseMini = true;
  param.Electron_MinPt = 75.; // HLT_DoublePhoton70_v

  param.Muon_Tight_ID = "HNPairTight";
  param.Muon_Tight_RelIso = 0.2;
  param.Muon_Loose_ID = "HNPairLoose";
  param.Muon_Loose_RelIso = 0.6;
  param.Muon_Veto_ID = "HNPairVeto";
  param.Muon_Veto_RelIso = 0.6;
  param.Muon_FR_ID = "HNPair_PtCut";
  param.Muon_FR_Key = "AwayJetPt40";
  param.Muon_CF_ID = "HNPairTight";
  param.Muon_CF_Key = "ZToLL";
  param.Muon_UseMini = true;
  param.Muon_MinPt = 55.; // HLT_Mu50_v

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  executeEventFromParameter(param);

}

void HNPairAnalyzer::executeEventFromParameter(AnalyzerParameter param){

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

  bool PassDiPhoton = ev.PassTrigger("HLT_DoublePhoton70_v");
  bool PassSingleMuon = ev.PassTrigger("HLT_Mu50_v");

  //==========================
  //==== Gen for genmatching
  //==========================

  vector<Gen> gens = GetGens();

  //==============
  //==== Leptons
  //==============

  double MinPt = 75;

  std::vector<Electron> Veto_electrons  = GetElectrons(param.Electron_Veto_ID, MinPt, 2.4);
  std::vector<Muon>     Veto_muons      = MuonWithoutGap(GetMuons(param.Muon_Veto_ID, MinPt, 2.4));

  //==== X_MinPt should be same as the pt cut used in the FR calculation
  std::vector<Electron> Loose_electrons = ElectronPromptOnly(GetElectrons(param.Electron_Loose_ID, param.Electron_MinPt, 2.4), gens);
  std::vector<Muon>     Loose_muons     = MuonPromptOnly(MuonWithoutGap(GetMuons(param.Muon_Loose_ID, param.Muon_MinPt, 2.4)), gens);
  std::vector<Electron> Tight_electrons;
  std::vector<Muon>     Tight_muons;

  for(unsigned int i=0; i<Loose_electrons.size(); i++){

    //==== Calculate pt-cone
    double ThisRelIso = Loose_electrons.at(i).RelIso();
    if(param.Electron_UseMini) ThisRelIso = Loose_electrons.at(i).MiniRelIso();
    double ptcone = Loose_electrons.at(i).CalcPtCone(ThisRelIso, param.Electron_Tight_RelIso);
    Loose_electrons.at(i).SetPtCone( ptcone );

    //==== Is Tight?
    if(Loose_electrons.at(i).PassID(param.Electron_Tight_ID)) Tight_electrons.push_back( Loose_electrons.at(i) );

  }
  for(unsigned int i=0; i<Loose_muons.size(); i++){

    //==== Calculate pt-cone
    double ThisRelIso = Loose_muons.at(i).RelIso();
    if(param.Muon_UseMini) ThisRelIso = Loose_muons.at(i).MiniRelIso();
    double ptcone = Loose_muons.at(i).CalcPtCone(ThisRelIso, param.Muon_Tight_RelIso);
    Loose_muons.at(i).SetPtCone( ptcone );

    //==== Is Tight?
    if(Loose_muons.at(i).PassID(param.Muon_Tight_ID)) Tight_muons.push_back( Loose_muons.at(i) );

  }

  //==== If fake, Replace pt by pt-cone
  if(RunFake){
    Loose_electrons = ElectronUsePtCone(Loose_electrons);
    Loose_muons     = MuonUsePtCone(Loose_muons);
    Tight_electrons = ElectronUsePtCone(Tight_electrons);
    Tight_muons     = MuonUsePtCone(Tight_muons);
  }

  //==== Now Apply pt cut
  Loose_electrons = ElectronApplyPtCut(Loose_electrons, MinPt);
  Loose_muons     = MuonApplyPtCut(Loose_muons, MinPt);
  Tight_electrons = ElectronApplyPtCut(Tight_electrons, MinPt);
  Tight_muons     = MuonApplyPtCut(Tight_muons, MinPt);

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

  std::vector<FatJet>   fatjets         = GetFatJets(param.FatJet_ID, 300, 2.7);

  std::vector<Jet>      alljets         = GetJets(param.Jet_ID, 40., 2.7);
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
    "DiPhoton", // at least two electrons
    "SingleMuon", // at least two muons
    "EMuMethod_SingleMuon", // EMu method
  };
  std::vector< bool > PassTriggers = {
    PassDiPhoton   && (Loose_electrons.size()>=2),
    PassSingleMuon && (Loose_muons.size()>=2),
    PassSingleMuon && (Loose_electrons.size()==1) && (Loose_muons.size()==1),
  };

  //=================
  //==== Start loop
  //=================

  for(unsigned int it_Suffix=0; it_Suffix<Suffixs.size(); it_Suffix++){

    TString Suffix = Suffixs.at(it_Suffix);

    bool RunEMu = false;
    if(Suffix=="EMuMethod_SingleMuon"){
      RunEMu = true;
    }

    bool PassTrigger = PassTriggers.at(it_Suffix);

    if(!( PassTrigger )) continue;

    if(this->IsDATA){
      if(this->DataStream == "DoubleEG"){
        if(!( Suffix.Contains("DiPhoton") )) continue;
      }
      else if(this->DataStream == "SingleMuon"){
        if(!( Suffix.Contains("SingleMuon") )) continue;
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

    vector<Particle> Ns = RecoPairN(leps, fatjets, jets);

    int FatJetConfig = fatjets.size();
    if(FatJetConfig>=2) FatJetConfig = 2;
    TString this_regionFATJET = "FatJetConfig"+TString::Itoa(FatJetConfig,10);

    if(IsDATA){

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

    //==== Two Lepton; Use IsTwoLeptonEvent

    bool IsTwoLeptonEvent = false;
    if(Suffix.Contains("DiPhoton")){
      IsTwoLeptonEvent = (Loose_electrons.size()==2) && (Loose_muons.size()==0) && (!HasCFElectron);
    }
    else if(Suffix.Contains("SingleMuon")){
      IsTwoLeptonEvent = (Loose_electrons.size()==0) && (Loose_muons.size()==2);
    }
    if(RunEMu) IsTwoLeptonEvent = true;

    bool IsOS = false;

    Particle ZCand_IsTwoLeptonEvent = (*leps[0])+(*leps[1]); // Only works for two lepton case
    IsOS = (leps[0]->Charge() != leps[1]->Charge()); // Only works for two lepton case
    if(RunCF){
      if(!IsOS) continue;
      IsOS = !IsOS;
    }

    double this_MT_el=-999., this_MT_mu = -999., this_MT_close=-999.;
    double MT_el = -999., MT_mu = -999., MT_close = -999.;
    if(IsTwoLeptonEvent){
      TMP_map_bool_To_Region["SR"] = (ZCand_IsTwoLeptonEvent.M() > 150.) && ( Ns.size()==2 ) && ( (Ns.at(0)+Ns.at(1)).M() > 300. );
      TMP_map_bool_To_Region["CR1"] = (ZCand_IsTwoLeptonEvent.M() < 150.);
      TMP_map_bool_To_Region["CR1_mll55"] = (ZCand_IsTwoLeptonEvent.M() < 150.) && (ZCand_IsTwoLeptonEvent.M() > 55.); // FIXME No DY10-50 yet
      TMP_map_bool_To_Region["CR2"] = IsOnZ(ZCand_IsTwoLeptonEvent.M(), 10.);
      TMP_map_bool_To_Region["CR3"] = (jets.size()>1) && (NBJets>=1) && (METv.Pt() > 40.);

      if(Suffix=="EMuMethod_SingleMuon" && jets.size()>=2){
        TMP_map_bool_To_Region["CR4"] = true;

        MT_el = MT(Loose_electrons.at(0) ,METv);
        MT_mu = MT(Loose_muons.at(0) ,METv);

        TString whichlepfake = "ElFake";
        MT_close = MT_el;
        if( fabs(MT_el-M_W) < fabs(MT_mu-M_W) ){
          whichlepfake = "MuFake";
          MT_close = MT_mu;
        }
        if( fabs(MT_close-M_W) < 20. ){
          TMP_map_bool_To_Region["CR4_"+whichlepfake] = true;
        }

      }

    }

    for(std::map<TString, bool>::iterator it_map = TMP_map_bool_To_Region.begin(); it_map != TMP_map_bool_To_Region.end(); it_map++){
      TString this_region = it_map->first;

      if(it_map->second){
        if(IsOS) map_bool_To_Region[this_region+"_OS"] = true;
        else     map_bool_To_Region[this_region+"_SS"] = true;
      }

    }

    //==== Z + fake

    bool IsZPairPlusOneDifferentLepton = false;
    Particle ZCand_IsZPairPlusOneDifferentLepton;
    if(Suffix.Contains("DiPhoton")){
      if( (Loose_electrons.size()==2) && (Loose_muons.size()==1) ){
        if( IsOnZ( (Loose_electrons.at(0)+Loose_electrons.at(1)).M(), 10. ) ){
          IsZPairPlusOneDifferentLepton = true;
          ZCand_IsZPairPlusOneDifferentLepton = Loose_electrons.at(0)+Loose_electrons.at(1);
        }
      }
    }
    else if(Suffix.Contains("SingleMuon")){
      if( (Loose_electrons.size()==1) && (Loose_muons.size()==2) ){
        if( IsOnZ( (Loose_muons.at(0)+Loose_muons.at(1)).M(), 10. ) ){
          IsZPairPlusOneDifferentLepton = true;
          ZCand_IsZPairPlusOneDifferentLepton = Loose_muons.at(0)+Loose_muons.at(1);
        }
      }
    }

    if(IsZPairPlusOneDifferentLepton){
      map_bool_To_Region["CR5"] = true;
    }



    //==== Start boolean loop

    for(std::map<TString, bool>::iterator it_map = map_bool_To_Region.begin(); it_map != map_bool_To_Region.end(); it_map++){

      TString this_region = it_map->first;
      this_region = (param.Name)+"_"+Suffix+"_"+this_region;

      if(it_map->second){

        FillLeptonPlots(leps, this_region, weight);
        FillJetPlots(jets, fatjets, this_region, weight);

        JSFillHist(this_region, "NEvent_"+this_region, 0., weight, 1, 0., 1.);
        JSFillHist(this_region, "MET_"+this_region, METv.Pt(), weight, 500, 0., 500.);
        JSFillHist(this_region, "METphi_"+this_region, METv.Phi(), weight, 60, -3., 3.);

        JSFillHist(this_region, "HT_"+this_region, HT, weight, 1000, 0., 1000.);

        JSFillHist(this_region, "FatJet_Size_"+this_region, fatjets.size(), weight, 10, 0., 10.);
        JSFillHist(this_region, "Jet_Size_"+this_region, jets.size(), weight, 10, 0., 10.);

        if(IsTwoLeptonEvent){
          JSFillHist(this_region, "ZCand_Mass_"+this_region, ZCand_IsTwoLeptonEvent.M(), weight, 1000, 0., 1000.);
          JSFillHist(this_region, "dR_ll_"+this_region, (*leps[0]).DeltaR( (*leps[1]) ), weight, 100, 0., 10.);
        }
        if(IsZPairPlusOneDifferentLepton){
          JSFillHist(this_region, "ZCand_Mass_"+this_region, ZCand_IsZPairPlusOneDifferentLepton.M(), weight, 1000, 0., 1000.);
        }

        if(this_region.Contains("CR4")){
          JSFillHist(this_region, "MT_el_"+this_region, MT_el, weight, 500, 0., 500.);
          JSFillHist(this_region, "MT_mu_"+this_region, MT_mu, weight, 500, 0., 500.);
          JSFillHist(this_region, "MT_close_"+this_region, MT_close, weight, 500, 0., 500.);
        }


        if(Ns.size()==2){

          JSFillHist(this_region, "N_Mass_"+this_region, Ns.at(0).M(), weight, 500, 0., 5000);
          JSFillHist(this_region, "N_Mass_"+this_region, Ns.at(1).M(), weight, 500, 0., 5000);
          JSFillHist(this_region, "ZP_Mass_"+this_region, (Ns.at(0)+Ns.at(1)).M(), weight, 800, 0., 8000);

          JSFillHist(this_region+"_"+this_regionFATJET, "N_Mass_"+this_region+"_"+this_regionFATJET, Ns.at(0).M(), weight, 500, 0., 5000);
          JSFillHist(this_region+"_"+this_regionFATJET, "N_Mass_"+this_region+"_"+this_regionFATJET, Ns.at(1).M(), weight, 500, 0., 5000);
          JSFillHist(this_region+"_"+this_regionFATJET, "ZP_Mass_"+this_region+"_"+this_regionFATJET, (Ns.at(0)+Ns.at(1)).M(), weight, 800, 0., 8000);


        }

      }

    } // END Loop map_bool_To_Region



  } // END Loop Suffix

}

vector<Particle> HNPairAnalyzer::RecoPairN(vector<Lepton *> lepptrs, vector<FatJet> fatjets, vector<Jet> jets){

  vector<Particle> out;

  //==== check lepton size
  if(lepptrs.size()!=2) return out;

  //==== Use the four leading jets
  if(fatjets.size()==0 && jets.size()>3){
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
  else if(fatjets.size()==1 && jets.size()>1){
    // AddFatJetAndLepton
    // Particle N_00 = TwoJet[0]+*(lepptrs.at(0));
    //  Particle N_11 = TwoJet[1]+*(lepptrs.at(1));

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
  else if(fatjets.size()>1){

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
  else{

  }

  return out;

}
/*
void HNPairAnalyzer::FillLeptonPlots(){

}
*/
HNPairAnalyzer::HNPairAnalyzer(){

  RunFake = false;

}

HNPairAnalyzer::~HNPairAnalyzer(){

}














