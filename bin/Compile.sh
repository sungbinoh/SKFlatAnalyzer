#!/bin/bash

mv $SKFlat_LIB_PATH/*.pcm src/
mv $SKFlat_LIB_PATH/*.d src/
mv $SKFlat_LIB_PATH/*.so src/

for cy in SKFlatValidation GetEffLumi CalcCFRate CalcFakeRate GenStudy HNPairAnalyzer ExampleRun HNWRAnalyzer
do
  root -l -b -q src/$cy.C+
  mv src/$cy"_C_ACLiC_dict_rdict.pcm" $SKFlat_LIB_PATH/
  mv src/$cy"_C.d" $SKFlat_LIB_PATH/
  mv src/$cy"_C.so" $SKFlat_LIB_PATH/
done
