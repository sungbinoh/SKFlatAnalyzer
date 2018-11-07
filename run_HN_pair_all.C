#include "HN_pair_all.C"

void run_HN_pair_all(){

  HN_pair_all m;

  TString outputdir = getenv("OUTPUTDIR");

  m.SetTreeName("recoTree/SKFlat");

  m.MaxEvent = 10000;

  
  //==== For Data Run
  /*
  m.IsDATA = true;
  m.DataStream = "SingleMuon";
  m.AddFile("root://cms-xrdr.sdfarm.kr:1094///xrd/store/user/jskim/SKFlat/2017/SingleMuon/SKFlat_v949cand2_1_periodF/181004_124732/0000/SKFlatNtuple_2017_DATA_114.root");
  */
  //==== For MC Run

  m.MCSample = "DYJets";
  //m.MCSample = "ttW";
  m.IsDATA = false;
  m.AddFile("root://cms-xrdr.sdfarm.kr:1094///xrd/store/user/jskim/SKFlat/2017/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/SKFlat_v949cand2_1/180925_212138/0000/SKFlatNtuple_2017_MC_1.root");
  //m.AddFile("root://cms-xrdr.sdfarm.kr:1094///xrd/store/user/jskim/SKFlat/2017/ttWJets_TuneCP5_13TeV_madgraphMLM_pythia8/SKFlat_v949cand2_1/181018_142138/0000/SKFlatNtuple_2017_MC_132.root"); // ttW

  m.SetOutfilePath("hists.root");

  m.Init();

  m.initializeAnalyzer();
  m.Loop();

  m.WriteHist();

}

