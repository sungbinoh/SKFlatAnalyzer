void make_masses(int xxx=0){

  TString channel = "MM";
  if(xxx==1) channel = "EE";

  TString channel2 = "MuMu";
  if(xxx==1) channel2 = "EE";

  TString outfile = "masses.txt";

  string elline;
  ifstream in("HN_pair_MM.txt");
  ofstream out(outfile);
  
  
  vector<TString> masses;
  while(getline(in,elline)){
    std::istringstream is( elline );
    TString mass;
    is >> mass;
    TObjArray *tx = mass.Tokenize("_");

    TString mZp = ((TObjString *)(tx->At(2)))->String();
    TString mN = ((TObjString *)(tx->At(3)))->String();
    mZp.Remove(0,2);
    mN.Remove(0,1);
    
    //TString current_line = "SKZprimetoNN_WR_" + channel2 + "_Z" + mZp + "_N" + mN + "_cat_v8-0-8";
    out << "ZP" + mZp + "_N" + mN << endl;
    //out << current_line << endl;
  }


}
