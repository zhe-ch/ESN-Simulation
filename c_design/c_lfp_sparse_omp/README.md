# C Acceleration for Sparse ESN for LFP signal analysis

OpenMP framework is taken advantage to accelerate the C implementation.

This directory contains design files of ESN with sparse interconnections.

The non linear operator TANH is carried out by following three ways:

- Double precision floating point.
- Single precision floating point.
- Look up table (LUT)

The design files for these three cases are stored in ./design_files/

