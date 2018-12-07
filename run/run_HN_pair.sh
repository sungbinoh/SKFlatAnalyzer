#Signal (2017)
python python/SKFlat.py -a HN_pair_all -l ./run/HN_pair_MM.txt -q fastq -n 1
python python/SKFlat.py -a HN_pair_all -l ./run/HN_pair_EE.txt -q fastq -n 1

#MC
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_big.txt --skim SkimTree_LRSMHighPt -q fastq -n 30
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_small.txt --skim SkimTree_LRSMHighPt -q fastq -n 30
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_minor.txt --skim SkimTree_LRSMHighPt -q fastq -n 30

#DATA
#python python/SKFlat.py -a HN_pair_all -i SingleMuon --skim SkimTree_LRSMHighPt -q fastq -n 20
#python python/SKFlat.py -a HN_pair_all -i DoubleEG --skim SkimTree_LRSMHighPt -q fastq -n 20

#TEST
#python python/SKFlat.py -a HN_pair_all --skim SkimTree_LRSMHighPt -i DYJets -q fastq -n 1
