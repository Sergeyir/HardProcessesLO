/** 
 *  @file   Constants.hpp
 *  @brief  Contains constants that are used for different purposes 
 *
 *  This file is a part of a project HadronProcessesLO (https://github.com/Sergeyir/HadronProcessesLO).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

/* @namespace COLOR
 *
 * @brief Contains different colors that can be used to change the text printed in terminal
 */
namespace COLOR
{
   /// default color in terminal (aka white)
   const std::string RESET = "\033[0m";
   /// black text
   const std::string BLACK = "\033[30m";
   /// red text
   const std::string RED ="\033[31m";
   /// green text
   const std::string GREEN = "\033[32m";
   /// yellow text
   const std::string YELLOW = "\033[33m";
   /// blue text
   const std::string BLUE = "\033[34m";
   /// magenta text
   const std::string MAGENTA = "\033[35m";
   /// cyan text
   const std::string CYAN = "\033[36m";
   /// white text
   const std::string WHITE = "\033[37m";

   /// bold black text
   const std::string BOLD_BLACK = "\033[1m\033[30m";
   /// bold red text
   const std::string BOLD_RED = "\033[1m\033[31m";
   /// bold green text
   const std::string BOLD_GREEN = "\033[1m\033[32m";
   /// bold yellow text
   const std::string BOLD_YELLOW = "\033[1m\033[33m";
   /// bold blue text
   const std::string BOLD_BLUE = "\033[1m\033[34m";
   /// bold magenta text
   const std::string BOLD_MAGENTA = "\033[1m\033[35m";
   /// bold cyan text
   const std::string BOLD_CYAN = "\033[1m\033[36m";
   /// bold white text
   const std::string BOLD_WHITE = "\033[1m\033[37m";
}

#endif /* CONSTANTS_HPP */
