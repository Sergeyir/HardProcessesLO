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
#include <filesystem>
#include <cmath>
#include <chrono>

#include "LHAPDF/LHAPDF.h"

#include "yaml-cpp/yaml.h"

#include "TFile.h"
#include "TH1D.h"
#include "TRandom.h"

/// LHDAPDF::PDF object; it is used to obtain \alpha_Q, Q1, and Q2 
/// if initialized for the given pdf set
LHAPDF::PDF *pdf;
/// ROOT TRandom object; it is used to randomly select y1, y2, and p_T for MC integration
TRandom rnd;
/// number of integration steps for MC integration
long numberOfIntegrationSteps;

/* @brief Returns random number that can be used as seed for TRandom
 */
unsigned int GetRandomSeed();
/* @brief Calculates d\sigma / d\Omega for all processes id1+id2 -> X + X for the given pT, \sqrt{s'_{NN}}, and y
 *
 * @param[in] id1 id of a 1st parton
 * @param[in] id2 id of a 2nd parton
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] s square root of s'_{NN} (' denotes parton COM frame) [GeV]
 * @param[in] y = y1 - y2
 * @param[out] d\sigma / dp_T dy_1 dy_2
 */
double GetDSigmaDOmega(const int id1, const int id2, const double pT, 
                       const double s, const double y);
/* @brief Calculates d\sigma / dp_T dy_1 dy_2 for the given pT, \sqrt{s'_{NN}}, y1, y2, x1, and x2
 *
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] s square root of s'_{NN} (' denotes parton COM frame) [GeV]
 * @param[in] y1 1st parton rapidity
 * @param[in] y2 2nd parton rapidity
 * @param[in] x1 1st parton x
 * @param[in] x2 2nd parton x 
 * @param[out] d\sigma / dp_T dy_1 dy_2
 */
double GetDSigmaDPTDY1DY2(const double pT, const double s, 
                          const double y1, const double y2,
                          const double x1, const double x2);
/* @brief Calculates x (parton momentum fraction alongside Z axis) for the first parton for the given pT, \sqrt{s_{NN}}, y1, y2
 *
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] sqrtSNN square root of s_{NN} [GeV]
 * @param[in] y1 rapidity of a first parton
 * @param[in] y2 rapidity of a second parton
 * @param[out] x
 */
double GetX1(const double pT, const double sqrtSNN, const double y1, const double y2);
/* @brief Calculates x (parton momentum fraction alongside Z axis) for the second parton for the given pT, \sqrt{s_{NN}}, y1, y2
 *
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] sqrtSNN square root of s_{NN} [GeV]
 * @param[in] y1 rapidity of a first parton
 * @param[in] y2 rapidity of a second parton
 * @param[out] x
 */
double GetX2(const double pT, const double sqrtSNN, const double y1, const double y2);
/* @brief Calculates d\sigma / dp_T for the given pT, \sqrt{s_{NN}} and rapidity range. Integration is be performed over [-absYMax, absYMax] range for both y1 and y2
 *
 * @param[in] pT transverse momentum [GeV/c]
 * @param[in] sqrtSNN square root of s_{NN} [GeV]
 * @param[in] absYMax maximum absolute of a rapidity of a parton
 * @param[in] err statistic uncertainty tied to limited number of integration steps
 * @param[out] d\sigma / dp_T
 */
double GetDSigmaDPT(const double pT, const double sqrtSNN, const double absYMax, double &err);
/* @brief Calculates d\sigma / dp_\DeltaY for the given \Delta y, \sqrt{s_{NN}} and rapidity range. Integration is be performed over [-absYMax, absYMax] range for both y1 and y2; for pT integration is performed from pTHatMin to maximum kinematicaly possible pT value
 *
 * @param[in] deltaY rapidity difference y_1 - y_2
 * @param[in] sqrtSNN square root of s_{NN} [GeV]
 * @param[in] absYMax maximum absolute of a rapidity of a parton
 * @param[in] err statistic uncertainty tied to limited number of integration steps
 * @param[out] d\sigma / dp_T
 */
double GetDSigmaDPT(const double deltaY, const double sqrtSNN, const double absYMax, double &err);

#endif /* CALCULATE_ANALYTIC_HPP */
