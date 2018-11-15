#ifndef SkimTree_LRSMHighPt_h
#define SkimTree_LRSMHighPt_h

#include "AnalyzerCore.h"

class SkimTree_LRSMHighPt : public AnalyzerCore {

public:

  void initializeAnalyzer();
  void executeEventFromParameter(AnalyzerParameter param);
  void executeEvent();

  SkimTree_LRSMHighPt();
  ~SkimTree_LRSMHighPt();

  TTree *newtree;

  vector<string> *temp_HLT_TriggerName;
  void WriteHist();

};



#endif

