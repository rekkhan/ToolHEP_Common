# Tool_Converter

This package provides tools to convert files to usual format that can be used easily in analysis.
Currently there is only the converter for LHE to ROOT available.

# Execute

Simply use:
```
root -l -b -q  macroname.C
```

To run the program in interactive mode, or:
```
root -l -b -q  macroname.C  &> log.txt &
```

To run it in background.

E.g:
```
root -l -b -q  Code_LHEtoROOT.C  &> logLHEtoROOT.txt &
```
