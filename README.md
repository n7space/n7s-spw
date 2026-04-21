# n7s-spw

A wrapper for Microchip's spacewire driver for SAMRH71, created as a part of "Model-Based Execution Platform for Space Applications (MBEP)" project, funded by ESA, Contract 4000146882/24/NL/KK.

# Test environment setup

* Prepare test board
 * Using SSH start JLink in one terminal

* Prepare linux machine
 * Execute in terminal `export SAMRH71_REMOTE_GDBSERVER=<test board ip>:2331`

# Executing tests

Initialize submodule `git submodule update --init --recursive`
To execute tests simply execute `make test`