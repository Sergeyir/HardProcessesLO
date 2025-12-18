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
                   "inputFileName.yaml numberOfIntegrationSteps" << std::endl;
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

   numberOfIntegrationSteps = std::stol(argv[2]);

   // YAML::Node object reads all data from the .yaml file
   YAML::Node inputFileContents = YAML::LoadFile(argv[1]);

   const std::string pdfSet = inputFileContents["pdfset"].as<std::string>();
   const double pTHatMin = inputFileContents["pthatmin"].as<double>();
   const double absMaxY = inputFileContents["abs_max_y"].as<double>();
   const double sqrtSNN = inputFileContents["energy"].as<double>();

   pdf = LHAPDF::mkPDF(pdfSet);
   rnd.SetSeed(GetRandomSeed());

   TFile outputFile("output/analytic.root", "RECREATE");

   TH1D distrDSigmaDPT("dsigmadpT", "d#sigma/dp_{T}", 1000 + pTHatMin, 0., 100. + pTHatMin);

   const int nBinsX = distrDSigmaDPT.GetXaxis()->GetNbins();
   for (int i = 1; i <= nBinsX; i++)
   {
      // the line below prints progress in percents
      std::cout << static_cast<double>(i)/static_cast<double>(nBinsX)*100. << "%\r";

      double err;
      const double dSigmaDPT = GetDSigmaDPT(distrDSigmaDPT.GetXaxis()->GetBinCenter(i), sqrtSNN, absMaxY, err);

      distrDSigmaDPT.SetBinContent(i, dSigmaDPT);
      distrDSigmaDPT.SetBinError(i, err);
   }

   distrDSigmaDPT.Write();

   std::cout << distrDSigmaDPT.Integral() << std::endl;

   outputFile.Close();

   return 0;
}

double GetDSigmaDOmega(const int id1, const int id2, const double pT, 
                       const double s, const double y)
{
   // to do: define mandelstam variables (t, u) and substitute lower random dummies for them
   // (hint: it will be easier if you define cos(\Theta) first)
   const double t = rnd.Uniform(-s, s);
   const double u = rnd.Uniform(-s, s);
   
   // obtaining alpha_{S} at hard scale, i.e. \mu_F = p_T
   const double alphaS = pdf->alphasQ2(pT*pT);

   // only qq->qq is shown below as an example
   if (id1 != 0 && id2 != 0 && id1 == id2)
   {
      // source: Eq. 51.7 at https://pdg.lbl.gov/2025/reviews/contents_sports.html
      return alphaS*alphaS/(9.*s)*((t*t + s*s)/(u*u) + (s*s + u*u)/(t*t) - 2.*s*s/(3.*u*t));
   }
   // to do: add other d\sigma / d\Omega processes here

   return 0.;
}

double GetDSigmaDPTDY1DY2(const double pT, const double s, 
                          const double y1, const double y2, 
                          const double x1, const double x2)
{
   // result of a sum over all combinations
   double result = 0.;
   
   // iterating over all posible combinations
   for (int id1 = -5; id1 <= 5; id1++)
   {
      for (int id2 = id1; id2 <= 5; id2++)
      {
         // to do : determine measurement units for the following expression
         result += 8.*M_PI*pT*pdf->xfxQ2(((id1 == 0) ? 21 : id1), x1, pT*pT)*
                   pdf->xfxQ2(((id2 == 0) ? 21 : id2), x2, pT*pT)*
                   GetDSigmaDOmega(id1, id2, pT, s, y1 - y2)/s;
         // to do: add an expression to calculate result for non-identical particles
      }
   }
   return result;
}

double GetX1(const double pT, const double sqrtSNN, const double y1, const double y2)
{
   return 2.*pT/sqrtSNN*exp((y1 + y2)/2.)*cosh((y1 - y2)/2.);
}

double GetX2(const double pT, const double sqrtSNN, const double y1, const double y2)
{
   // To do: fill in the formula for x2 in the current function
   return 0.5;
}

double GetDSigmaDPT(const double pT, const double sqrtSNN, const double absYMax, double &err)
{
   // result of MC integration
   double result = 0.;
   // normalization constant for MC integration: equals to the number of succesfull integration steps
   long normalization = 0;

   for (long i = 0; i < numberOfIntegrationSteps; i++)
   {
      // randomly choosing y1 and y2
      const double y1 = rnd.Uniform(-absYMax, absYMax);
      const double y2 = rnd.Uniform(-absYMax, absYMax);

      // calculating x1 and x2
      const double x1 = GetX1(pT, sqrtSNN, y1, y2);
      const double x2 = GetX2(pT, sqrtSNN, y1, y2);

      // to do: add a check that tests whether x1 and x2 are within kinematicaly possible range

      // calculating \sqrt{\hat{s}}, i.e. the center of mass energy of 2 partons
      const double s = sqrtSNN*sqrtSNN*x1*x2;

      // only do the following calculations after all cuts
      result += GetDSigmaDPTDY1DY2(pT, s, y1, y2, x1, x2);
      // normalization constant is a sum of weights; for uniform distribution all weights are 1
      normalization++;
   }

   if (normalization == 0) 
   {
      err = 0.;
      return 0.;
   }

   // without multiplying the result by area we just get average value; 
   // area is needed to obtain from that average value an integral
   // normalization/numberOfIntegrationSteps is needed since not all points
   // correspond to the kinematicaly allowed area, but those that do were selected
   // and counted by the normalization constant
   const double area = 4*absYMax*absYMax*static_cast<double>(normalization)/
                       static_cast<double>(numberOfIntegrationSteps);

   // scaling integration sum by the number of succesfull 
   // integration steps and by the integration area
   result *= area/static_cast<double>(normalization);

   // 2 is present since area and MC integration statistical uncertainty 
   // produce the same uncertainty which has to be propagated between them
   err = result/sqrt(normalization/2.);

   return result;
}

double GetDSigmaDDeltaY(const double deltaY, const double sqrtSNN, const double absYMax, const double pTHatMin, double &err)
{
   // This is just a functiont prototype The intended purpose of which is to show how to 
   // randomly choose pT. If the pT is chosen from the uniform distribution the cross section
   // values will jump to much from point to point due to significant contribution coming from
   // low pT region (with finite integration calculation steps only few points on this range will be
   // selected which results in some points having larger contribution from low pT that the other
   // thus the "jumpiness" of a histogram points arises). 

   double normalization = 0.;
   double result = 0.;

   for (long i = 0; i < numberOfIntegrationSteps; i++)
   {
      // To do : add correct formula for tau
      // This is one way to achieve this: to distrubute pT in a way so that 
      // there are many more points on low pT
      const double tau = 1.;
      const double pT = pTHatMin + rnd.Exp(tau);
      // In order for the TRandom to work correctly and fast tau needs to be defined so that
      // \int_{pTHatMIn}^{pTMax} e^{-x/tau) dx = 1
      // Where pTMax - maximum kinematicaly possible pT value
      // Exponent is chosen since it resembles the form of d\Sigma/dpT. The closer the resemblance - 
      // the better error estimation is performed for finite number of integration steps

      // But since the distribution is non-uniform the normalization 
      // needs to be explicitly calculated as sum of weights
      normalization += exp(-(pT - pTHatMin)/tau); // here exp(-pT/tau) is weight

      // dummy value
      result += 1e-6;
   }
   // To do: add normalization by integration volume

   // To do: check for other sources of uncertainties and propagate 
   // them with this error if you find any
   err = result/sqrt(normalization);

   return result/normalization;
}


unsigned int GetRandomSeed()
{
	auto now = std::chrono::high_resolution_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
	auto epoch = now_ms.time_since_epoch();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	return static_cast<unsigned int>(duration.count() % 900000000);
}

#endif /* CALCULATE_ANALYTIC_CPP */
