#include "ExampleRun.h"

ExampleRun::ExampleRun(){

  hoe_max = -1.;

}
ExampleRun::~ExampleRun(){

}

void ExampleRun::executeEvent(){


  AnalyzerParameter param;

  param.Name = "HNPair";

  param.MCCorrrectionIgnoreNoHist = true;

  param.Muon_Tight_ID = "POGTight";
  param.Muon_Tight_RelIso = 0.15;

  param.Jet_ID = "HN";
  param.FatJet_ID = "HN";

  executeEventFromParameter(param);

}

void ExampleRun::executeEventFromParameter(AnalyzerParameter param){

  Event ev = GetEvent();

/*

  //==== Electron Custom-CutBased test

  FillHist("nPV", ev.nPV(), 1., 50, 0., 50.);

  std::vector<Electron> electrons_all = GetAllElectrons();
  std::vector<Muon>     muons_all     = GetAllMuons();

  //==== Comparing POG CutBased and Mine

  for(unsigned int i=0; i<electrons_all.size(); i++){

    Electron el = electrons_all.at(i);

    if(fabs(el.scEta())>2.5) continue;

    bool POGLoose = el.passLooseID();
    bool MyPOGLoose = el.Pass_CutBasedLoose();

    bool POGVeto = el.passVetoID();
    bool MyPOGVeto = el.Pass_CutBasedVeto();

    if(POGLoose)   FillHist("POGLoose", 0., 1., 2, 0., 2.);
    if(MyPOGLoose) FillHist("POGLoose", 1., 1., 2, 0., 2.);
    if(POGVeto)   FillHist("POGVeto", 0., 1., 2, 0., 2.);
    if(MyPOGVeto) FillHist("POGVeto", 1., 1., 2, 0., 2.);


    if( POGLoose && !MyPOGLoose ){
      FillHist("PassPOGLoose__NotMyPOGLoose", 0., 1., 1, 0., 1.);


      cout << "----------------------------------------------------" << endl;
      cout << "|scEta| = " << el.scEta() << endl;
      cout << "Energy_PostCorr = " << el.E() << endl;
      cout << "Energy_PreCorr = " << el.UncorrE() << endl;
      cout << "full5x5 = " << el.full5x5_sigmaIetaIeta() << endl;
      cout << "|dEtaSeed| = " << fabs(el.dEtaSeed()) << endl;
      cout << "|dPhiIn| = " << fabs(el.dPhiIn()) << endl;
      cout << "H/E = " << el.HoverE() << endl;
      cout << "--> cut = " << 0.05 + 1.12/el.UncorrE() + 0.0368*el.Rho()/el.UncorrE() << endl;
      cout << "RelIso = " << el.RelIso() << endl;
      cout << "|1/E-1/p| = " << fabs(el.InvEminusInvP()) << endl;
      cout << "NMissHist = " << el.NMissingHits() << endl;
      cout << "PassConv = " << el.PassConversionVeto() << endl;



      if(el.HoverE() > hoe_max){
        hoe_max = el.HoverE();
        cout << "#######################" << endl;
        cout << "H/E max = " << hoe_max << endl;
        cout << "#######################" << endl;
      }


    }

    if( !POGLoose && MyPOGLoose ){
      FillHist("PassMyPOGLoose__NotPOGLoose", 0., 1., 1, 0., 1.);

      cout << "### |scEta| = " << el.scEta() << " ###" << endl;
      cout << "full5x5 = " << el.full5x5_sigmaIetaIeta() << endl;
      cout << "|dEtaSeed| = " << fabs(el.dEtaSeed()) << endl;
      cout << "|dPhiIn| = " << fabs(el.dPhiIn()) << endl;
      cout << "H/E = " << el.HoverE() << endl;
      cout << "--> cut = " << 0.05 + 1.12/el.UncorrE() + 0.0368*el.Rho()/el.UncorrE() << endl;
      cout << "RelIso = " << el.RelIso() << endl;
      cout << "|1/E-1/p| = " << fabs(el.InvEminusInvP()) << endl;
      cout << "NMissHist = " << el.NMissingHits() << endl;
      cout << "PassConv = " << el.PassConversionVeto() << endl;

    }


    if( POGVeto && !MyPOGVeto ){
      FillHist("PassPOGVeto__NotMyPOGVeto", 0., 1., 1, 0., 1.);
    }
    if( !POGVeto && MyPOGVeto ){
      FillHist("PassMyPOGVeto__NotPOGVeto", 0., 1., 1, 0., 1.);
    }

  } // END Electron Loop
*/

/*
  //==== Fake Rate

  double MinPt = 40;

  std::vector<Electron> Veto_electrons  = GetElectrons(param.Electron_Veto_ID, MinPt, 2.4);
  std::vector<Muon>     Veto_muons      = MuonWithoutGap(GetMuons(param.Muon_Veto_ID, MinPt, 2.4));
  int n_Veto_leptons = Veto_electrons.size()+Veto_muons.size();

  std::vector<Electron> Loose_electrons = GetElectrons(param.Electron_Loose_ID, MinPt, 2.4);
  std::vector<Muon>     Loose_muons     = MuonWithoutGap(GetMuons(param.Muon_Loose_ID, MinPt, 2.4));

  std::vector<Electron> Tight_electrons = GetElectrons(param.Electron_Tight_ID, MinPt, 2.4);
  std::vector<Muon>     Tight_muons     = MuonWithoutGap(GetMuons(param.Muon_Tight_ID, MinPt, 2.4));

  std::vector<Lepton *> leps_electron = MakeLeptonPointerVector(Loose_electrons, param.Electron_Tight_RelIso, param.Electron_UseMini);
  std::vector<Lepton *> leps_muon     = MakeLeptonPointerVector(Loose_muons, param.Muon_Tight_RelIso, param.Muon_UseMini);
  std::vector<Lepton *> leps;
  for(unsigned int i=0;i<leps_electron.size(); i++) leps.push_back( leps_electron.at(i) );
  for(unsigned int i=0;i<leps_muon.size(); i++) leps.push_back( leps_muon.at(i) );

  bool UseEvent = false;
  if(Loose_electrons.size()==2 && Loose_muons.size()==0){
    if(Loose_electrons.at(0).Charge()==Loose_electrons.at(1).Charge()){
      UseEvent = true;
    }
  }
  if(Loose_electrons.size()==0 && Loose_muons.size()==2){
    if(Loose_muons.at(0).Charge()==Loose_muons.at(1).Charge()){
      UseEvent = true;
    }
  }

  if(UseEvent){

    double mll = ( *(leps.at(0))+*(leps.at(1)) ).M();
    if(mll>120){
      double this_fr_weight = fakeEst.GetWeight(leps, param);
      if(fakeEst.HasLooseLepton) FillHist("hist_exp", 0., this_fr_weight, 1, 0., 1.);
      else                       FillHist("hist_obs", 0., 1., 1, 0., 1.);
    }
  }
*/

/*
  //==== Collineaer DiMuon

  bool PassTrigger = ev.PassTrigger("HLT_IsoMu27_v");
  if(!PassTrigger) return;
  std::vector<Muon>     Tight_muons     = GetMuons(param.Muon_Tight_ID, 10., 2.4);
  std::vector<Lepton *> leps;
  leps = MakeLeptonPointerVector(Tight_muons);

  if(Tight_muons.size()==0) return;
  for(unsigned int i=0; i<Tight_muons.size()-1; i++){
    Muon mu1 = Tight_muons.at(i);
    
    for(unsigned int j=i+1; j<Tight_muons.size(); j++){

      Muon mu2 = Tight_muons.at(j);

      TString chageconf = "OS";
      if(mu1.Charge()==mu2.Charge()) chageconf = "SS";

      double dRmuons = mu1.DeltaR(mu2);
      if(dRmuons < 0.1){

        JSFillHist(chageconf, "DeltaR_"+chageconf, dRmuons, 1, 100, 0., 0.1);

        double m_ll = (mu1+mu2).M();
        JSFillHist(chageconf, "m_ll_Wide_"+chageconf, m_ll, 1, 200, 0., 200.);
        JSFillHist(chageconf, "m_ll_Fine_"+chageconf, m_ll, 1, 200, 0., 20.);

      }


    }

  }
*/

  vector<Electron> electrons = GetElectrons("HNWRLoose", 10., 2.5);
  for(unsigned int i=0; i<electrons.size(); i++){

    Electron el = electrons.at(i);

    FillHist("RelIso", el.RelIso(), 1, 100, 0., 1.);
    JSFillHist("Electron", "Electron_RelIso", el.RelIso(), 1, 100, 0., 1.);

  }





}



