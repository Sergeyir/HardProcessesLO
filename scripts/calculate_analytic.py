#!/usr/bin/env python3

from typing import Tuple
import os
import sys
import argparse
import time
import math
import yaml
import numpy as np
import lhapdf
import ROOT

# Globals
pdf = None  # LHAPDF PDF object
number_of_integration_steps = 0
rng = None  # numpy.random.Generator


def get_random_seed() -> int:
    """Return random seed"""
    ms = int(time.time() * 1000) # miliseconds
    return ms % 900000000


def get_dsigma_domega(id1: int, id2: int, pT: float, s: float, y: float) -> float:
    """
    To do: define mandelstam variables (t, u) and substitute lower random dummies for them
    (hint: it will be easier if you define cos(Theta) first
    """
    t = rng.uniform(-s, s)
    u = rng.uniform(-s, s)

    # alpha_s at hard scale \mu_F = pT
    alpha_s = pdf.alphasQ2(pT * pT)

    """
    To do: implement other channels below
    """
    # only qq -> qq is shown as an example
    if id1 != 0 and id2 != 0 and id1 == id2:
        # from C++: alphaS*alphaS/(9.*s)*((t*t + s*s)/(u*u) + (s*s + u*u)/(t*t) - 2.*s*s/(3.*u*t));
        return (alpha_s * alpha_s / (9.0 * s)) * ((t * t + s * s) / (u * u) +
                                                (s * s + u * u) / (t * t) -
                                                2.0 * s * s / (3.0 * u * t))
    return 0.


def get_dsigma_dpt_dy1_dy2(pT: float, s: float, y1: float, y2: float, x1: float, x2: float) -> float:
    # Sum over all combinations id1=-5..5, id2=-5..5
    result = 0.0
    for id1 in range(-5, 6):
        for id2 in range(id1, 6):
            # LHAPDF xfxQ2 takes (id, x, Q2)
            fx1 = pdf.xfxQ2((21, id1)[id1 == 0], x1, pT * pT)
            fx2 = pdf.xfxQ2((21, id2)[id2 == 0], x2, pT * pT)
            dsigma_domega = get_dsigma_domega(id1, id2, pT, s, y1 - y2)
            """
            To do: determine measurement units for the following expression
            """
            result += 8.0 * math.pi * pT * fx1 * fx2 * dsigma_domega / s
            """
            To do: implement expression for the result for the case when particles are non-identical
            """
    return result


def get_x1(pT: float, sqrtSNN: float, y1: float, y2: float) -> float:
    return 2.0 * pT / sqrtSNN * math.exp((y1 + y2) / 2.0) * math.cosh((y1 - y2) / 2.0)


def get_x2(pT: float, sqrtSNN: float, y1: float, y2: float) -> float:
    """
    To do: fill in the formula for x2 in the current function
    """
    return 0.5 


def get_dsigma_dpt(pT: float, sqrtSNN: float, absYMax: float) -> Tuple[float, float]:
    # Monte Carlo integration over y1, y2 in [-absYMax, absYMax].
    result = 0.
    # normalization constant for MC integration: equals to the number of succesfull integration steps
    normalization = 0.

    for _ in range(number_of_integration_steps):
        # randomly choosing y1 and y2
        y1 = rng.uniform(-absYMax, absYMax)
        y2 = rng.uniform(-absYMax, absYMax)

        x1 = get_x1(pT, sqrtSNN, y1, y2)
        x2 = get_x2(pT, sqrtSNN, y1, y2)

        """
        To do: introduce a check that tests if x1 and x2 have proper values
        """

        s = (sqrtSNN * sqrtSNN) * x1 * x2

        # only do the following steps after all cuts
        result += get_dsigma_dpt_dy1_dy2(pT, s, y1, y2, x1, x2)
        normalization += 1.

    # without multiplying the result by area we just get average value; 
    # area is needed to obtain from that average value an integral
    # normalization/number_of_integration_steps is needed since not all points
    # correspond to the kinematicaly allowed area, but those that do were selected
    # and counted by the normalization constant
    area = 4.*absYMax*absYMax*float(normalization)/float(number_of_integration_steps)

    # scaling integration sum by the number of succesfull integration steps 
    # and by the integration area
    result *= area/normalization
    # 2 is present since area and MC integration statistical uncertainty 
    # produce the same uncertainty which has to be propagated between them
    err = result / math.sqrt(normalization/2.)
    return result, err

def get_dsigma_d_deltay(delta_y: float, sqrtSNN: float, absYMax: float, pTHatMin: float) -> Tuple[float, float]:
    """
    This is just a functiont prototype The intended purpose of which is to show how to 
    randomly choose pT. If the pT is chosen from the uniform distribution the cross section
    values will jump to much from point to point due to significant contribution coming from
    low pT region (with finite integration calculation steps only few points on this range will be
    selected which results in some points having larger contribution from low pT that the other
    thus the "jumpiness" of a histogram points arises). 
    """

    normalization = 0.
    result = 0.

    for _ in range(number_of_integration_steps):
        # To do : add correct formula for tau
        """
        This is one way to achieve this: to distrubute pT in a way so that 
        there are many more points on low pT
        """
        tau = 1.
        pT = pTHatMin + rnd.Exp(tau)
        """
        In order for the TRandom to work correctly and fast tau needs to be defined so that
        int_{pTHatMIn}^{pTMax} e^{-x/tau) dx = 1
        Where pTMax - maximum kinematicaly possible pT value
        Exponent is chosen since it resembles the form of dSigma/dpT. The closer the resemblance - 
        the better error estimation is performed for finite number of integration steps
        """

        """
        But since the distribution is non-uniform the normalization 
        needs to be explicitly calculated as sum of weig
        """
        normalization += math.exp(-(pT - pTHatMin)/tau) # here exp(-pT/tau) is weight

        # dummy value
        result += 1e-6

    # To do: add normalization by integration volume
    """
    To do: check for other sources of uncertainties and propagate 
    them with this error if you find any
    """
    err = result/sqrt(normalization)

    return result/normalization, err

def calculate_analytic(input_file_name : str) -> int:
    global pdf, number_of_integration_steps, rng

    with open(input_file_name, "r") as f:
        cfg = yaml.safe_load(f)

    pdfset = cfg["pdfset"]
    pTHatMin = float(cfg["pthatmin"])
    absMaxY = float(cfg["abs_max_y"])
    sqrtSNN = float(cfg["energy"])

    pdf = lhapdf.mkPDF(pdfset)
    seed = get_random_seed()
    rng = np.random.default_rng(seed)

    os.system('mkdir -p output')
    output_file = ROOT.TFile("output/analytic.root", "RECREATE")

    # Histogram: mimic TH1D with numpy: bins from 0..100+pTHatMin with 1000+pTHatMin bins (like C++)
    nbins = int(1000 + pTHatMin)
    x_min = 0.0
    x_max = 100.0 + pTHatMin
    distr_dsigma_dpt = ROOT.TH1D("dsigmadpT", "d#sigma/dp_{T}", nbins, x_min, x_max)

    for i in range(1, nbins + 1):
        print(round(float(i)/float(nbins)*100., 2), "%\r", end='', flush=True)
        val, err = get_dsigma_dpt(distr_dsigma_dpt.GetXaxis().GetBinCenter(i), sqrtSNN, absMaxY)
        distr_dsigma_dpt.SetBinContent(i, val)
        distr_dsigma_dpt.SetBinError(i, err)

    distr_dsigma_dpt.Write()

    print(distr_dsigma_dpt.Integral())

    return 0

def main() :
    global number_of_integration_steps
    # parsing input arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", type=str)
    parser.add_argument("-n", "--nsteps", type=int)

    args = parser.parse_args()
    input_file_name = args.input
    number_of_integration_steps = args.nsteps

    if not input_file_name :
        print('\033[1;31mError:\033[0m input file must be specified; use option -i or --input')
        sys(exit(1))

    if number_of_integration_steps == 0 :
        print('\033[1;31mError:\033[0m number of integration steps must be specified; '\
              'use option -n or --nsteps')
        sys(exit(1))

    sys.exit(calculate_analytic(input_file_name))

if __name__ == "__main__":
    main()
