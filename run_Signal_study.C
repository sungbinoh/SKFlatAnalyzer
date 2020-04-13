R__LOAD_LIBRARY(libPhysics.so)
R__LOAD_LIBRARY(libTree.so)
R__LOAD_LIBRARY(libHist.so)
R__LOAD_LIBRARY(./lib/libDataFormats.so)
R__LOAD_LIBRARY(./lib/libAnalyzerTools.so)
R__LOAD_LIBRARY(./lib/libGEScaleSyst.so)
R__LOAD_LIBRARY(./lib/libAnalyzers.so)
R__LOAD_LIBRARY(/cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/lhapdf/6.2.1-fmblme/lib/libLHAPDF.so)


void run_Signal_study(){

  Signal_study m;

  m.SetTreeName("recoTree/SKFlat");
  
  m.MaxEvent = 1000;
  
  //==== For Data Run
  //m.IsDATA = false;
  //m.DataStream = "SingleMuon";
  //m.AddFile("/data9/DATA/SKFlat/Run2Legacy_v3/2018/MC_SkimTree_LRSMHighPt/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/2019_05_19_124859/SKFlatNtuple_2018_MC_11.root")
  
  //==== For MC Run
  m.MCSample = "HNPairToJJJJ_MuMu_ZP2000_N900_WR5000";
  m.IsDATA = false;
  m.xsec = 0.1;
  m.sumW = 50000;
  m.IsFastSim = false;
  m.DataYear = 2016;
  //m.AddFile("/gv0/DATA/SKFlat/Run2Legacy_v4/2017/MC_SkimTree_LRSMHighPt/DYJetsToLL_M-50_HT-70to100_TuneCP5_13TeV-madgraphMLM-pythia8/2020_02_02_011807/SKFlatNtuple_2017_MC_172.root");
  m.AddFile("/gv0/DATA/SKFlat/Run2Legacy_v4/2016/PrivateMC/ZpNN/ZprimetoNN_MuMuJJJJ_Zprime2000_N900_WR5000_NLO/SKFlatNtuple_2016_MC_23.root");
  m.SetOutfilePath("hists.root");
  m.Init();
  m.initializeAnalyzer();
  m.initializeAnalyzerTools();
  m.SwitchToTempDir();
  m.Loop();
  
  m.WriteHist();
  
}

