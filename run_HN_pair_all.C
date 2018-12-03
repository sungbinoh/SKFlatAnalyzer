R__LOAD_LIBRARY(libPhysics.so)
R__LOAD_LIBRARY(libTree.so)
R__LOAD_LIBRARY(libHist.so)
R__LOAD_LIBRARY(./lib/libDataFormats.so)
R__LOAD_LIBRARY(./lib/libAnalyzers.so)


void run_HN_pair_all(){

  HN_pair_all m;

  TString outputdir = getenv("OUTPUTDIR");

  m.SetTreeName("recoTree/SKFlat");

  m.MaxEvent = 100000;
  
  //==== For Data Run
  /*
  m.IsDATA = true;
  m.DataStream = "SingleMuon";
  m.AddFile("root://cms-xrdr.sdfarm.kr:1094///xrd/store/user/jskim/SKFlat/2017/SingleMuon/SKFlat_v949cand2_1_periodF/181004_124732/0000/SKFlatNtuple_2017_DATA_114.root");
  */
  //==== For MC Run
  m.MCSample = "DYJets";
  m.IsDATA = false;
  m.xsec = 5765.4;
  m.sumW = 123584524;
  m.DataYear = 2017;
  m.AddFile("root://cms-xrdr.sdfarm.kr:1094///xrd/store/user/jskim/SKFlat/2017/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/SKFlat_v949cand2_2_MCPDFFIX/181126_102511/0000/SKFlatNtuple_2017_MC_39.root");
  m.SetOutfilePath("hists.root");
  m.Init();
  m.initializeAnalyzerTools();
  m.initializeAnalyzer();
  m.Loop();
  
  m.WriteHist();
  
}

