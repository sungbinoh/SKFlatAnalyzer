R__LOAD_LIBRARY(libPhysics.so)
R__LOAD_LIBRARY(libTree.so)
R__LOAD_LIBRARY(libHist.so)
R__LOAD_LIBRARY(libDataFormats.so)
R__LOAD_LIBRARY(libAnalyzers.so)

void run_NvtxProfile(){

  NvtxProfile m;

  
  TString outputdir = getenv("OUTPUTDIR");
  
  cout << "[run_NvtxProfile] SetTreeName " << endl;
  m.SetTreeName("recoTree/SKFlat");

  m.MaxEvent = 20;

/*
  //==== For Data Run
  m.IsDATA = true;
  m.DataStream = "SingleMuon";
*/

  //==== For MC Run
  
  cout << "[run_NvtxProfile] Set MCSample " << endl;
  m.MCSample = "DYJets";
  cout << "[run_NvtxProfile] Set IsDATA " << endl;
  m.IsDATA = false;
  cout << "[run_NvtxProfile] AddFile " << endl;
  m.AddFile("root://cms-xrdr.sdfarm.kr:1094///xrd/store/user/jskim/SKFlat/2017/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/SKFlat_v949cand2_1/180925_212138/0000/SKFlatNtuple_2017_MC_1.root");
  //m.AnalyzerCore();
  
  m.Set_MCSample_for_mcCorr();
  cout << "[run_NvtxProfile] SetOutfilePath " << endl;
  m.SetOutfilePath("hists.root");
  
  cout << "[run_NvtxProfile] Init " << endl;
  m.Init();

  cout << "[run_NvtxProfile] initializeAnalyzer " << endl;
  m.initializeAnalyzer();
  cout << "[run_NvtxProfile] Loop " << endl;
  m.Loop();

  
  m.WriteHist();

}

