#Signal (2017)
#python python/SKFlat.py -a HN_pair_all -l ./run/HN_pair_MM.txt -n 20
#python python/SKFlat.py -a HN_pair_all -l ./run/HN_pair_EE.txt -n 20

#MC
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_big.txt -n 400 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_small.txt -n 300 &
#python python/SKFlat.py -a HN_pair_all -l ./run/MC_minor.txt -n 30 &
#DATA
#python python/SKFlat.py -a HN_pair_all -i SingleMuon -n 200 &
#python python/SKFlat.py -a HN_pair_all -i DoubleEG -n 200 &

#TEST
python python/SKFlat.py -a HN_pair_all -i DYJets -n 200 &