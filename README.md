# ms2go
Count the numbers of gap and overlap in each trace of miniSEED input.

# Clone
```
$ git clone https://github.com/Cuda-Chen/ms2go.git --recursive
```

# How to Compile and Run
```
$ make
$ ./ms2go [mseedfile]
```

# Reference
The printGapAndOverlap() function originates here:
https://github.com/iris-edu/libmseed/blob/7836b2d7cdf4cff96c02e86e13a1adf628ffb4e1/tracelist.c#L2218
