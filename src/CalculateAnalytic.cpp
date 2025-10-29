/** 
 *  @file   CalculateAnalytic.cpp
 *  @brief  Contains simplest case that shows how the analytic calculation can be performed for the estimation of the cross section
 *
 *  This file is a part of a project HadronProcessesLO (https://github.com/Sergeyir/HadronProcessesLO).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef CALCULATE_ANALYTIC_CPP
#define CALCULATE_ANALYTIC_CPP

#include "CalculateAnalytic.hpp"

int main(int argc, char **argv)
{
   // printing info on usage and exiting program if number of parameters is incorrect
   if (argc != 3) 
   {
      std::cout << COLOR::BOLD_RED << "Error: " << COLOR::RESET << 
                   "Expected 2 parameters while " + std::to_string(argc - 1) + " "\
                   "parameter(s) were provided \n Usage: bin/CalculateAnalytic "\
                   "inputFileName.yaml numberOfMCTries" << std::endl;
      std::cout << "[" << COLOR::BOLD_GREEN << "INFO" << COLOR::RESET << "] " << 
                   " input file example is located in input directory" << std::endl;
      return 1;
   }

   // input .yaml file; we specified argv[1] to be the name of the .yaml file
   std::ifstream inputFile(argv[1]);
   // checking if that file exists; if it doesn't exist error is printed and the program is stopped
   if (!inputFile.is_open())
   {
      std::cout << COLOR::BOLD_RED << "Error: " << COLOR::RESET <<
                   "file " << argv[1] << " was not found" << std::endl;
      return 1;
   }

   // YAML::Node object reads all data from the .yaml file
   YAML::Node inputFileContents = YAML::LoadFile(argv[1]);

   return 0;
}

#endif /* CALCULATE_ANALYTIC_CPP */
