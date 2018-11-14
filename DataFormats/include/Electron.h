#ifndef Electron_h
#define Electron_h

#include "Lepton.h"
#include "TString.h"

class Electron: public Lepton{

public:

  Electron();
  ~Electron();

  void SetSC(double sceta, double scphi, double sce);
  inline double scEta() const { return j_scEta; }
  inline double scPhi() const { return j_scPhi; }
  inline double scE() const { return j_scE; }
  void SetMVA(double mvaiso, double mvanoiso);
  inline double MVAIso() const { return j_mvaiso; }
  inline double MVANoIso() const { return j_mvanoiso; }

  void SetUncorrE(double une);
  inline double UncorrE() const { return j_EnergyUnCorr; }
  inline double UncorrPt() const { return Pt() * j_EnergyUnCorr/E(); }

  void SetPassConversionVeto(bool b);
  inline int PassConversionVeto() const { return j_passConversionVeto; }
  void SetNMissingHits(int n);
  inline int NMissingHits() const { return j_NMissingHits; };

  enum EtaRegion{
    IB, OB, GAP, EC
  };
  EtaRegion etaRegion(){
    double sceta = fabs(scEta());
    if( sceta < 0.8 ) return IB;
    else if( sceta < 1.444 ) return OB;
    else if( sceta < 1.566 ) return GAP;
    else return EC;
  }
  
  void SetCutBasedIDVariables(double Full5x5_sigmaIetaIeta, double dEtaSeed, double dPhiIn, double HoverE, double InvEminusInvP);
  double Full5x5_sigmaIetaIeta() const { return j_Full5x5_sigmaIetaIeta; }
  double dEtaSeed() const { return j_dEtaSeed; }
  double dPhiIn() const { return j_dPhiIn; }
  double HoverE() const { return j_HoverE; }
  double InvEminusInvP() const { return j_InvEminusInvP; }

  void SetPOGIDs(std::vector<bool> bs);
  inline bool passVetoID() const {return j_passVetoID;}
  inline bool passLooseID() const {return j_passLooseID;}
  inline bool passMediumID() const {return j_passMediumID;}
  inline bool passTightID() const {return j_passTightID;}
  inline bool passMVAID_noIso_WP80() const {return j_passMVAID_noIso_WP80;}
  inline bool passMVAID_noIso_WP90() const {return j_passMVAID_noIso_WP90;}
  inline bool passMVAID_iso_WP80() const {return j_passMVAID_iso_WP80;}
  inline bool passMVAID_iso_WP90() const {return j_passMVAID_iso_WP90;}
  inline bool passHEEPID() const {return j_passHEEPID;}
  bool Pass_SUSYMVAWP(TString wp);
  bool Pass_SUSYTight();
  bool Pass_SUSYLoose();
  bool Pass_HEEP_OR_LooseNoIso();
  bool Pass_HEEP_dZ();
  bool Pass_HNPairTight();
  bool Pass_HNPairLoose();
  bool Pass_HNPairLooseNoIP();
  bool Pass_HNPairVeto();
  bool Pass_HNWRTight();
  bool Pass_HNWRLoose();
  bool Pass_HNWRVeto();

  void SetRelPFIso_Rho(double r);
  double EA();

  //==== ID
  bool PassID(TString ID);
  bool Pass_TESTID();

  bool Pass_CutBasedLooseNoIso();
  bool Pass_CutBasedVetoNoIso();
  bool Pass_CutBasedLoose();
  bool Pass_CutBasedVeto();
  void SetRho(double r);
  inline double Rho() const { return j_Rho; }

private:

  double j_scEta,j_scPhi, j_scE;
  double j_mvaiso, j_mvanoiso;
  double j_EnergyUnCorr;
  bool j_passConversionVeto;
  int j_NMissingHits;
  double j_Full5x5_sigmaIetaIeta, j_dEtaSeed, j_dPhiIn, j_HoverE, j_InvEminusInvP;
  bool j_passVetoID;
  bool j_passLooseID;
  bool j_passMediumID;
  bool j_passTightID;
  bool j_passMVAID_noIso_WP80;
  bool j_passMVAID_noIso_WP90;
  bool j_passMVAID_iso_WP80;
  bool j_passMVAID_iso_WP90;
  bool j_passHEEPID;

  double j_RelPFIso_Rho;

  double j_Rho;

  ClassDef(Electron,1)

};

#endif
