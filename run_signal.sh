:<<END
python python/SKFlat.py -a GetEffLumi -y 2017 -i HNPairToJJJJ_MuMu_ZP3400_N200_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP3400_N200_WR5000_2017 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2017 -i HNPairToJJJJ_MuMu_ZP3400_N400_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP3400_N400_WR5000_2017 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2017 -i HNPairToJJJJ_MuMu_ZP3400_N600_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP3400_N600_WR5000_2017 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2017 -i HNPairToJJJJ_MuMu_ZP3400_N800_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP3400_N800_WR5000_2017 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2017 -i HNPairToJJJJ_MuMu_ZP4600_N1000_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP4600_N1000_WR5000_2017 -n 1 &

python python/SKFlat.py -a GetEffLumi -y 2018 -i HNPairToJJJJ_MuMu_ZP400_N150_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP400_N150_WR5000_2018 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2018 -i HNPairToJJJJ_MuMu_ZP1600_N750_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP1600_N750_WR5000_2018 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2018 -i HNPairToJJJJ_MuMu_ZP2600_N1250_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP2600_N1250_WR5000_2018 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2018 -i HNPairToJJJJ_MuMu_ZP2800_N1350_WR5000  -q fastq --batchname HNPairToJJJJ_MuMu_ZP2800_N1350_WR5000_2018 -n 1 &
END
python python/SKFlat.py -a GetEffLumi -y 2018 -i HNPairToJJJJ_EE_ZP400_N150_WR5000  -q fastq --batchname HNPairToJJJJ_EE_ZP400_N150_WR5000_2018 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2018 -i HNPairToJJJJ_EE_ZP1600_N750_WR5000  -q fastq --batchname HNPairToJJJJ_EE_ZP1600_N750_WR5000_2018 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2018 -i HNPairToJJJJ_EE_ZP2600_N1250_WR5000  -q fastq --batchname HNPairToJJJJ_EE_ZP2600_N1250_WR5000_2018 -n 1 &
python python/SKFlat.py -a GetEffLumi -y 2018 -i HNPairToJJJJ_EE_ZP2800_N1350_WR5000  -q fastq --batchname HNPairToJJJJ_EE_ZP2800_N1350_WR5000_2018 -n 1 &

#python python/SKFlat.py -a SR_ZpNN -l ./run/MC_signal_2016.txt -y 2016 -q fastq --batchname ZpNN_signal_2016 -n 1 &
#python python/SKFlat.py -a SR_ZpNN -l ./run/MC_signal_2017.txt -y 2017 -q fastq --batchname ZpNN_signal_2017 -n 1 &
#python python/SKFlat.py -a SR_ZpNN -l ./run/MC_signal_2018.txt -y 2018 -q fastq --batchname ZpNN_signal_2018 -n 1 &
