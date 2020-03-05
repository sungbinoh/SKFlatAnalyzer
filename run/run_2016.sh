#Signal
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_signal_2016.txt -y 2016 -q fastq -n 1 &
#python python/SKFlat.py -a Signal_study -l ./run/MC_signal_2016.txt -y 2016 -q fastq -n 1 &
python python/SKFlat.py -a GetEffLumi -l ./run/MC_signal_2016.txt -y 2016 -q fastq -n 1 &


###############
## Skim RUN ###
###############
#MC
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_DY_HT.txt  --skim SkimTree_LRSMHighPt -y 2016 -q fastq -n 30 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_big_2018.txt --skim SkimTree_LRSMHighPt -y 2016 -q fastq -n 30 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_small_2018.txt --skim SkimTree_LRSMHighPt -y 2016 -q fastq -n 30 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_minor_2016.txt --skim SkimTree_LRSMHighPt -y 2016 -q fastq -n 30 &

#DATA
#python python/SKFlat.py -a HN_pair_all -i SingleMuon --skim SkimTree_LRSMHighPt -y 2016 -q fastq -n 20 &
#python python/SKFlat.py -a HN_pair_all -i DoubleEG --skim SkimTree_LRSMHighPt -y 2016 -q fastq -n 20 &

#TEST
#python python/SKFlat.py -a HN_pair_all --skim SkimTree_LRSMHighPt -i DYJets -q fastq -n 30 &


###############
## No Skim ####
###############
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_big.txt -q fastq -n 200 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_small.txt -q fastq -n 200 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_minor.txt -q fastq -n 200 &

#python python/SKFlat.py -a HN_pair_all -i SingleMuon -q fastq -n 200 &
#python python/SKFlat.py -a HN_pair_all -i DoubleEG -q fastq -n 200 &
