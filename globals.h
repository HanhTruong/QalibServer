/*******************************************************************

FILENAME:	globals.h

--------------------------------------------------------------------
MODULE DESCRIPTION:
	Global variables.

--------------------------------------------------------------------
GLOBAL FUNCTIONS:

--------------------------------------------------------------------
COMPILER: Visual C++ 6.0	INVOCATION: Build ->Compile	globals.h

--------------------------------------------------------------------
$HISTORY: $

*******************************************************************/

enum {Slope, Intercept, Top, Bottom};
enum {Anchor=1, Curvature};

extern VD_NONLINFITOPTIONS Opt;

extern ui       sizex;
extern double	ccoe;
extern ui		poolsizeslen;
extern liVector	PoolSizes;

extern dVector	assignedCoef;
extern ui		numberofsamples;

extern dVector	XExp, YExp, Weights;

extern dVector	UpperLimits;	// impose upper limits on parameters
extern dVector	LowerLimits;	// impose lower limits on parameters
extern dMatrix	Covar;			// covariance matrix.

extern dVector	FitParameters;			// Fit parameters.
extern iVector	FitParameterStatus;		// Status of fit parameters, for OptiVec library.
extern ui		numberOfFitParameters;	// number of parameters to be evaluated.
