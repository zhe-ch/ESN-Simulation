# ESN_Simulation

The echo state network (ESN),[1][2] is a recurrent neural network with a sparsely connected hidden layer (with typically 1% connectivity). The connectivity and weights of hidden neurons are fixed and randomly assigned. The weights of output neurons can be learned so that the network can (re)produce specific temporal patterns. The main interest of this network is that although its behaviour is non-linear, the only weights that are modified during training are for the synapses that connect the hidden neurons to output neurons. Thus, the error function is quadratic with respect to the parameter vector and can be differentiated easily to a linear system.

This repository contains codes for simulation of a echo-state network (ESN) at the scale of 500/800 hidden nodes.

The simulation is carried out in C.
The implementation is targetting at FPGA, using Vivado HLS to generate the RTL kernel.

[1] Herbert Jaeger and Harald Haas. Harnessing Nonlinearity: Predicting Chaotic Systems and Saving Energy in Wireless Communication. Science 2 April 2004: Vol. 304. no. 5667, pp. 78 – 80

[2] Herbert Jaeger (2007) Echo State Network. 
