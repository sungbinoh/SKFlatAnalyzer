#MC
#python python/SKFlat.py -a NvtxProfile -i DYJets -n 300 &
#python python/SKFlat.py -a NvtxProfile -l ./run/MC_big.txt -n 300 &
#python python/SKFlat.py -a NvtxProfile -l ./run/MC_small.txt -n 300 &
#python python/SKFlat.py -a NvtxProfile -l ./run/MC_minor.txt -n 30 &

#DATA
#python python/SKFlat.py -a NvtxProfile -i SingleMuon -n 200 &
#python python/SKFlat.py -a NvtxProfile -i DoubleEG -n 200 &

#python python/SKFlat.py -a NvtxProfile -i WW_pythia -n 100 &
#python python/SKFlat.py -a NvtxProfile -i WZ_pythia -n 100 &
#python python/SKFlat.py -a NvtxProfile -i ZZ_pythia -n 100 &

python python/SKFlat.py -a NvtxProfile -i TTLJ_powheg -n 100 &
python python/SKFlat.py -a NvtxProfile -i TTJJ_powheg -n 100 &