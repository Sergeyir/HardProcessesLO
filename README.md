# Overview

This is a simple projects that helps to study leading order (LO) hard processes in p+p collisions at high energy. The goal of this project is a numerical calculation of $d \sigma/dp_{T}$ and $d \sigma / d \Delta y$ of partons that originate from LO hard processes. The calculation is performed in 3 different ways: 
- **Analytic**: integration of the factorization formula of LO hard p+p interaction
- **PYTHIA8**: acquisition of cross section directly from PYTHIA8 event data
- **PYTHIA8+FASTJET3**: acquisition of cross section by reconstruction of jets with FASTJET3 from PYTHIA8 final state particles

# Requirements

- GNU GCC++17 or newer
- python3 (optional)
- [ROOT6](https://root.cern/)
- [LHAPDF6](https://lhapdf.hepforge.org/)
- [PYTHIA8](https://pythia.org/) compiled with LHAPDF6
- [FASTJET3](https://fastjet.fr/) 

ROOT6, LHAPDF6, PYTHIA8, and FASTJET3 have to be compiled with python3 for the python interface. For more information see [Installation tutorial](INSTALLATION_TUTORIAL.md).

# Analytic calculation

Using the factorization formula for $d \sigma / dy_{1} dy_{2} dp^2_{T}$ for LO hard $p+p$ process from [2](#sources) (Eq.51.42), one can obtain

```math
\frac{d \sigma}{dp^2_T dy_1 dy_2} = \frac{8 \pi p_T}{\hat{s}} \sum_{ijkl} x_{1} f_{i}(x_{1}, \mu_F^2) x_{2} f_{j}(x_{2}, \mu_F^2) \left[ \frac{d \sigma_{ij \rightarrow kl}}{d \Omega} (\hat{s}, \hat{t}, \hat{u}) + \frac{d \sigma_{ij \rightarrow kl}}{d \Omega} (\hat{s}, \hat{u}, \hat{t}) \right]
```

If i and j are identical particle types, the second term $d \sigma_{ij \rightarrow kl} / d\Omega$ should be dropped.

For this project the hard scale is implemented i.e. $\mu_F \approx p_T$

$d \sigma_{ij \rightarrow kl}/d \Omega$ can be found in  in [2](#sources) (Eq. 51.4 - 51.12)

The formulas for the needed cross section can be obtained with kinematic equations from [1](#sources) (Eq.206-211)

```math
\frac{d \sigma}{dp_T} = \int dy_1 \int dy_2 \frac{d \sigma}{dp_T dy_1 dy_2}
```

```math
\frac{d \sigma}{d \Delta y} = \int dp_T \int dy_1 \int dy_2 \frac{d \sigma}{dp_T dy_1 dy_2} \delta(\Delta y - |y_1 - y_2|)
```

# Installation

```sh
git clone https://github.com/Sergeyir/HardProcessesLO
```

Install all requirements and configure environment variables described in [Installation tutorial](INSTALLATION_TUTORIAL.md)

# Sources

1. [Michelangelo L. Mangano "Introduction to QCD"](https://cds.cern.ch/record/454171/files/p53.pdf)
2. [S. Navas et al. (Particle Data Group), Phys. Rev. D 110, 030001 (2024) and 2025 update](https://pdg.lbl.gov/2025/reviews/contents_sports.html)
