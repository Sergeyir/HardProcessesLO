/** 
 *  @file  rootlogon.C
 *  @brief Contains options such as include paths and libraries for ROOT cling so that
 *  it can be implemented in the current project
 *
 *  This file is a part of a project HadronProcessesLO (https://github.com/Sergeyir/HadronProcessesLO).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/

#pragma once // root may give error executing rootlogon if include guards are implemented

{
   gInterpreter->AddIncludePath("include");
   gInterpreter->AddIncludePath("yaml-cpp/include");
   gInterpreter->AddIncludePath((static_cast<std::string>(getenv("LHAPDF6_PATH")) + 
                                 "/include").c_str());
   gInterpreter->AddIncludePath((static_cast<std::string>(getenv("PYTHIA8_PATH")) + 
                                 "/include").c_str());
   gInterpreter->AddIncludePath((static_cast<std::string>(getenv("FASTJET3_PATH")) + 
                                 "/include").c_str());
   
   //gSystem->Load((static_cast<std::string>(getenv("PYTHIA8_PATH")) + "/lib/libdl.a").c_str());
   gSystem->Load((static_cast<std::string>(getenv("PYTHIA8_PATH")) + "/lib/libpythia8.so").c_str());
   gSystem->Load((static_cast<std::string>(getenv("LHAPDF6_PATH")) + "/lib/libLHAPDF.so").c_str());
   gSystem->Load((static_cast<std::string>(getenv("FASTJET3_PATH")) + "/lib/libfastjet.so").c_str());
   gSystem->Load((static_cast<std::string>(getenv("FASTJET3_PATH")) + 
                                           "/lib/libfastjettools.so").c_str());

   gSystem->Load("lib/libyaml-cpp.so");
}
