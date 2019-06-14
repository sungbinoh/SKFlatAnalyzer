#Signal
#python python/SKFlat.py -a HN_pair_all -l ./run/HN_pair_MM.txt -q fastq -n 1 &
#python python/SKFlat.py -a HN_pair_all -l ./run/HN_pair_EE.txt -q fastq -n 1 &

#python python/SKFlat.py -a SR_signal -l ./run/HN_pair_MM.txt -q fastq -n 1 &
#python python/SKFlat.py -a SR_signal -l ./run/HN_pair_EE.txt -q fastq -n 1 &

#python python/SKFlat.py -a SR_signal -i HNPairToJJJJ_MuMu_ZP4800_N2300_WR5000 -n 10 &
#python python/SKFlat.py -a SR_signal -i HNPairToJJJJ_EE_ZP4800_N2300_WR5000 -n 10 &
#python python/SKFlat.py -a SR_signal -i HNPairToJJJJ_EE_ZP4800_N500_WR5000 -n 10 &
#python python/SKFlat.py -a SR_signal -i HNPairToJJJJ_EE_ZP4800_N1200_WR5000 -n 10 &
#python python/SKFlat.py -a SR_signal -i HNPairToJJJJ_MuMu_ZP4800_N1200_WR5000 -n 10 &

###############
## Skim RUN ###
###############
#MC
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_big_2018.txt --skim SkimTree_LRSMHighPt -y 2018 -q fastq -n 10 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_small_2018.txt --skim SkimTree_LRSMHighPt -y 2018 -q fastq -n 10 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_minor_2018.txt --skim SkimTree_LRSMHighPt -y 2018 -q fastq -n 10 &

#DATA
#python python/SKFlat.py -a HN_pair_all -i SingleMuon --skim SkimTree_LRSMHighPt -y 2018 -q fastq -n 10 &
#python python/SKFlat.py -a HN_pair_all -i EGamma --skim SkimTree_LRSMHighPt -y 2018 -q fastq -n 10 &

#TEST
python python/SKFlat.py -a HN_pair_all -i WWW --skim SkimTree_LRSMHighPt -y 2018 -q fastq -n 10 &
#python python/SKFlat.py -a HN_pair_all -i ZZ_pythia --skim SkimTree_LRSMHighPt -y 2018 -q fastq -n 10 &



###############
## No Skim ####
###############
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_big.txt -q fastq -n 200 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_small.txt -q fastq -n 200 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_minor.txt -q fastq -n 200 &

#python python/SKFlat.py -a HN_pair_all -i SingleMuon -q fastq -n 200 &
#python python/SKFlat.py -a HN_pair_all -i DoubleEG -q fastq -n 200 &
