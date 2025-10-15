#!/usr/bin/env python3
import os
import sys
import argparse

import yaml
import ROOT

import pythia8
import fastjet

def generate_data(input_file_name : str, n_events : int) -> int :

    if not os.path.isfile(input_file_name):
        print(f"\033[1;31mError:\033[0m file {input_file_name} was not found")
        return 1

    with open(input_file_name, "r") as f:
        input_file = yaml.safe_load(f)

    pTHatMin = float(input_file["pthatmin"])
    energyCM = float(input_file["energy"])
    pdfset = str(input_file["pdfset"])
    fastjet_r = float(input_file["fastjet_r"])

    # Create and configure Pythia
    pythia = pythia8.Pythia()
    pythia.readString("HardQCD:all = on")
    pythia.readString(f"Beams:eCM = {energyCM}")
    pythia.readString(f"PhaseSpace:pTHatMin = {pTHatMin}")
    pythia.readString(f"PDF:pSet = {pdfset}")
    # pythia.readString("Print:quiet = on")

    pythia.init()

    os.makedirs("output", exist_ok=True)
    output_file = ROOT.TFile("output/generated.root", "RECREATE")

    nbins = 1000 + int(pTHatMin)
    distrHardProcessPartonsPT = ROOT.TH1D("Hard process outgoing parton pT", "p_{T}", 
                                          nbins, 0.0, 100.0 + pTHatMin)
    distrJetsPT = ROOT.TH1D("Jet pT", "p_{T}", nbins, 0.0, 100.0 + pTHatMin)

    jetdef = fastjet.JetDefinition(fastjet.antikt_algorithm, fastjet_r)

    for i in range(n_events):

        if not pythia.next():
            continue

        particles = []
        eventWeight = pythia.infoPython().weight()

        for j in range(pythia.event.size()):
            part = pythia.event[j]
            if part.isFinal():
                if abs(part.charge()) != 1:
                    continue
                pj = fastjet.PseudoJet(part.px(), part.py(), part.pz(), part.e())
                particles.append(pj)
            else:
                if part.status() != -23:
                    continue
                if not part.isParton():
                    continue
                distrHardProcessPartonsPT.Fill(part.pT(), eventWeight)

        if len(particles) == 0:
            continue

        cs = fastjet.ClusterSequence(particles, jetdef)
        inclusive_jets = cs.inclusive_jets(pTHatMin)

        for jet in inclusive_jets:
            distrJetsPT.Fill(jet.pt(), eventWeight)

    print(pythia.infoPython().sigmaGen())

    distrHardProcessPartonsPT.Write()
    distrJetsPT.Write()
    output_file.Close()

    return 0

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--input", type=str)
    parser.add_argument("-n", "--nevents", type=int)

    args = parser.parse_args()
    input_file_name = args.input
    n_events = args.nevents

    sys.exit(generate_data(input_file_name, n_events))
