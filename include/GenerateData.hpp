/** 
 *  @file   GenerateData.hpp
 *  @brief  Contains simplest case that shows how the data can be generated with PYTHIA8 and LHAPDF6 and processed with FASTJET3 for jets selection 
 *
 *  This file is a part of a project HadronProcessesLO (https://github.com/Sergeyir/HadronProcessesLO).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef GENERATE_DATA_HPP
#define GENERATE_DATA_HPP

#include <iostream>
#include <vector>
#include <fstream>

#include "LHAPDF/LHAPDF.h"

#include "Pythia8/Pythia.h"

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"

#include "yaml-cpp/yaml.h"

#include "TFile.h"
#include "TH1D.h"

#include "Constants.hpp"

int main(int argc, char **argv);

#endif /* GENERATE_DATA_HPP */
