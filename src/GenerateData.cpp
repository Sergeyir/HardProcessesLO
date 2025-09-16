/** 
 *  @file   GenerateData.cpp
 *  @brief  Contains simplest case that shows how the data can be generated with PYTHIA8 and LHAPDF6 and processed with FASTJET3 for jets selection 
 *
 *  This file is a part of a project HadronProcessesLO (https://github.com/Sergeyir/HadronProcessesLO).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef GENERATE_DATA_CPP
#define GENERATE_DATA_CPP

#include "GenerateData.hpp"

int main(int argc, char **argv)
{
   if (argc != 3) 
   {
      std::cout << COLOR::BOLD_RED << "Error: " << COLOR::RESET << 
                   "Expected 2 parameters while " + std::to_string(argc) + " "\
                   "parameter(s) were provided \n Usage: bin/GenerateData "\
                   "inputFileName.yaml numberOfEvents" << std::endl;
      return 1;
   }

   std::ifstream inputFile(argv[1]);

   if (!inputFile.is_open())
   {
      std::cout << COLOR::BOLD_RED << "Error: " << COLOR::RESET <<
                   "file " << argv[1] << " was not found" << std::endl;
      return 1;
   }

   YAML::Node inputFileContents = YAML::LoadFile(argv[1]);

   Pythia8::Pythia pythia;

   pythia.readString("Beams:eCM = " + inputFileContents["energy"].as<std::string>());
   pythia.readString("HardQCD:all = on");
   //pythia.readString("PDF:pSet = " + inputFileContents["pdfset"].as<std::string>());
   pythia.readString("PhaseSpace:pTHatMin = " + inputFileContents["pthatmin"].as<std::string>());

	pythia.init();

	for (long i = 0; i < std::stoi(argv[2]); i++)
   {
		if (!pythia.next()) continue;

		for (int j = 0; j < pythia.event.size(); j++)
      {
         if (pythia.event[j].isFinal())
         {
            std::cout << pythia.event[j].id() << std::endl;
         }
      }
   }

   return 0;
}

#endif /* GENERATE_DATA_CPP */
