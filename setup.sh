export SKFlat_WD=`pwd`
export OUTPUTDIR=$SKFlat_WD/outputs/
export SKFlat_LIB_PATH=$SKFlat_WD/lib/
mkdir -p $SKFlat_LIB_PATH

export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$SKFlat_WD/include/:$SKFlat_WD/src/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SKFlat_LIB_PATH

export SKFlatV="v946p1_3"
mkdir -p $SKFlat_WD/data/$SKFlatV

export SKFlatSEDir="/xrootd/store/user/$USER/SKFlatOutput/"
export SKFlatLogEmail=''

if [[ $HOSTNAME == *"ui10.sdfarm.kr"* ]]; then
  echo "Working on KISTI"
  export SKFlatRunlogDir="/cms/scratch/$USER/SKFlatRunlog/"
  export SKFlatOutputDir="/cms/scratch/$USER/SKFlatOutput/"
  export DATA_DIR=data/$SKFlatV

  export CMS_PATH=/cvmfs/cms.cern.ch
  source $CMS_PATH/cmsset_default.sh
  export SCRAM_ARCH=slc6_amd64_gcc630
  cd /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/src/
  eval `scramv1 runtime -sh`
  cd -
  source /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/external/slc6_amd64_gcc630/bin/thisroot.sh

else
  echo "Working on 42cluster"
  export SKFlatRunlogDir="/data7/Users/$USER/SKFlatRunlog/"
  export SKFlatOutputDir="/data7/Users/$USER/SKFlatOutput/"
  export DATA_DIR=$SKFlat_WD/data/$SKFlatV

  export CMS_PATH=/cvmfs/cms.cern.ch
  source $CMS_PATH/cmsset_default.sh
  export SCRAM_ARCH=slc6_amd64_gcc630
  cd /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/src/
  eval `scramv1 runtime -sh`
  cd -
  source /cvmfs/cms.cern.ch/slc6_amd64_gcc630/cms/cmssw/CMSSW_9_4_4/external/slc6_amd64_gcc630/bin/thisroot.sh

fi
export MYBIN=$SKFlat_WD/bin/
export PATH=${MYBIN}:${PATH}


source $SKFlat_WD/bin/BashColorSets.sh
python python/PrintToDoLists.py
source $SKFlat_WD/tmp/ToDoLists.sh
rm $SKFlat_WD/tmp/ToDoLists.sh

