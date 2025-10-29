/** 
 *  @file   CalculateAnalytic.hpp
 *  @brief  Contains simplest case that shows how the analytic calculation can be performed for the estimation of the cross section
 *
 *  This file is a part of a project HadronProcessesLO (https://github.com/Sergeyir/HadronProcessesLO).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef CALCULATE_ANALYTIC_HPP
#define CALCULATE_ANALYTIC_HPP

#include <iostream>
#include <vector>
#include <fstream>

#include "LHAPDF/LHAPDF.h"

#include "yaml-cpp/yaml.h"

#include "TFile.h"
#include "TH1D.h"

#include "Constants.hpp"

/* @brief Calculates x for the first parton for the given pT, \sqrt{s_{NN}}, y1, y2
 *
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] sNNSqrt square root of s_{NN}
 * @param[in] y1 rapidity of a first parton
 * @param[in] y2 rapidity of a second parton
 */
double GetX1(const double pT, const double sNNSqrt, const double y1, const double y2);
/* @brief Calculates x for the second parton for the given pT, \sqrt{s_{NN}}, y1, y2
 *
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] sNNSqrt square root of s_{NN}
 * @param[in] y1 rapidity of a first parton
 * @param[in] y2 rapidity of a second parton
 */
double GetX2(const double pT, const double sNNSqrt, const double y1, const double y2);
/* @brief Calculates d\sigma / dp_T for the given pT
 *
 * @param[in] pT transverse momentum [GeV/c]
 */
double GetDSigmaDPT(const double pT);

int main(int argc, char **argv);

#endif /* CALCULATE_ANALYTIC_HPP */
