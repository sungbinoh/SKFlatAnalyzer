R__LOAD_LIBRARY(libPhysics.so)
R__LOAD_LIBRARY(libTree.so)
R__LOAD_LIBRARY(libHist.so)
R__LOAD_LIBRARY(./lib/libDataFormats.so)
R__LOAD_LIBRARY(./lib/libAnalyzerTools.so)
R__LOAD_LIBRARY(./lib/libAnalyzers.so)
//R__LOAD_LIBRARY(./lib/libLHAPDF.so)
R__LOAD_LIBRARY(/cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/lhapdf/6.2.1-fmblme/lib/libLHAPDF.so)

void run_HN_pair_all(){

  HN_pair_all m;

  //TString outputdir = getenv("OUTPUTDIR");
  
  m.SetTreeName("recoTree/SKFlat");
  
  m.MaxEvent = 1000;
  
  //==== For Data Run
  //m.IsDATA = false;
  //m.DataStream = "SingleMuon";
  //m.AddFile("/data9/DATA/SKFlat/Run2Legacy_v3/2018/MC_SkimTree_LRSMHighPt/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/2019_05_19_124859/SKFlatNtuple_2018_MC_11.root")
  
  //==== For MC Run
  m.MCSample = "DYJets_MG";
  m.IsDATA = false;
  m.xsec = 6225.42;
  m.sumW = 100026541;
  m.IsFastSim = false;
  m.DataYear = 2018;
  m.AddFile("/data9/DATA/SKFlat/Run2Legacy_v3/2018/MC_SkimTree_LRSMHighPt/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/2019_05_19_124859/SKFlatNtuple_2018_MC_11.root");
  m.SetOutfilePath("hists.root");
  m.Init();
  m.initializeAnalyzerTools();
  m.initializeAnalyzer();
  //m.SwitchToTempDir();
  m.Loop();
  
  m.WriteHist();
  
}

