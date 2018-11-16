#MC
#python python/SKFlat.py -a NvtxProfile -i DYJets -n 300 &
python python/SKFlat.py -a NvtxProfile -l ./run/MC_big.txt -n 2 &
python python/SKFlat.py -a NvtxProfile -l ./run/MC_small.txt -n 300 &
python python/SKFlat.py -a NvtxProfile -l ./run/MC_minor.txt -n 30 &

#DATA
#python python/SKFlat.py -a NvtxProfile -i SingleMuon -n 200 &
#python python/SKFlat.py -a NvtxProfile -i DoubleEG -n 200 &