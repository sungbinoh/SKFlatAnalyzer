#Signal
#python python/SKFlat.py -a HN_pair_all -l ./run/HN_pair_MM.txt -q fastq -n 1 &
#python python/SKFlat.py -a HN_pair_all -l ./run/HN_pair_EE.txt -q fastq -n 1 &

python python/SKFlat.py -a GetEffLumi -l ./run/HN_pair_EE.txt -y 2016 -n 1 &  
#python python/SKFlat.py -a GetEffLumi -l ./run/HN_pair_MM.txt -y 2016 -n 1 &

#python python/SKFlat.py -a SR_signal -l ./run/HN_pair_MM.txt -q fastq -n 1 &
#python python/SKFlat.py -a SR_signal -l ./run/HN_pair_EE.txt -q fastq -n 1 &


#python python/SKFlat.py -a HN_pair_all -i HNPairToJJJJ_EE_ZP4800_N2300_WR5000 -y 2016 -n 10 &

