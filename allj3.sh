#!/bin/bash
make clean
make lin -j 3
make clean
make lindbhack -j 3
make clean
make linwin -j 3
make clean
make linwindbhack -j 3
make clean
make win32 -j 3
make clean
make win32win -j 3
make clean
make win32dbhack -j 3
make clean
make win32windbhack -j 3
make clean
make win32winbenchmark -j 3
make clean
