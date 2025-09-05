#include "stdafx.h"
#include "Support.h"
#include "globals.h"
#include "Loging.h"
#include "QalibConstants.h"
#include "FitParameters.h"

extern QalibConstants currentConstants;

HRESULT GetQuadraticDisposition(VARIANT *fitParameters,
								VARIANT *minLimits,
								VARIANT *maxLimits,
								VARIANT *disposition)
{
	HRESULT	hr = S_OK;
	double	tmpFitParameter, 
			tmpMinLimit,
			tmpMaxLimit;
	BSTR	bstr=NULL;
	char	buf[512];

	// Set disposition status flags.
	BSTR statusTrue	 = ::SysAllocString(L"PASS");
	BSTR statusFalse = ::SysAllocString(L"FAIL");

	// Disposition safearray to return.
    SAFEARRAY FAR* psaDisposition = NULL;

	// Safearrays to access passed in VARIANTs.
    SAFEARRAY FAR* psa_fitParameters = NULL;
    SAFEARRAY FAR* psa_minLimits = NULL;
    SAFEARRAY FAR* psa_maxLimits = NULL;

	// Convert passed in VARIANTs, to SAFEARRAYs.
	psa_fitParameters = V_ARRAY(fitParameters);
    psa_minLimits = V_ARRAY(minLimits);
    psa_maxLimits = V_ARRAY(maxLimits);

	//*** define the SAFEARRAYBOUND structure
	SAFEARRAYBOUND sabound[1];

	sabound[0].cElements = 9;

	sabound[0].lLbound = 0;

	psaDisposition = SafeArrayCreate(VT_BSTR, 1, sabound);

	long i=0;

	//*** Put Chi-squared status in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, statusTrue);		
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	i++;
	//*** Put "Correlation coefficient" status label in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, statusTrue);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	i++;
	//*** Put "Bartlett's test" status label in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, statusTrue);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	i++;
	hr = SafeArrayGetElement(psa_fitParameters, &i, &tmpFitParameter);		
	hr = SafeArrayGetElement(psa_minLimits, &i, &tmpMinLimit);		
	hr = SafeArrayGetElement(psa_maxLimits, &i, &tmpMaxLimit);		
	if ((tmpFitParameter < tmpMinLimit) || (tmpFitParameter > tmpMaxLimit))
	{
		bstr = statusFalse;
	}
	else
	{
		bstr = statusTrue;
	}
	//*** Put "Slope" status label in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, bstr);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	i++;
	//*** Put "Slope standard error" status label in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, statusTrue);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	i++;
	hr = SafeArrayGetElement(psa_fitParameters, &i, &tmpFitParameter);		
	hr = SafeArrayGetElement(psa_minLimits, &i, &tmpMinLimit);		
	hr = SafeArrayGetElement(psa_maxLimits, &i, &tmpMaxLimit);		
	if ((tmpFitParameter < tmpMinLimit) || (tmpFitParameter > tmpMaxLimit))
	{
		bstr = statusFalse;
	}
	else
	{
		bstr = statusTrue;
	}
	//*** Put "Anchor" status label in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, bstr);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	i++;
	//*** Put "Anchor standard error" status label in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, statusTrue);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	i++;
	hr = SafeArrayGetElement(psa_fitParameters, &i, &tmpFitParameter);		
	hr = SafeArrayGetElement(psa_minLimits, &i, &tmpMinLimit);		
	hr = SafeArrayGetElement(psa_maxLimits, &i, &tmpMaxLimit);		
	if ((tmpFitParameter < tmpMinLimit) || (tmpFitParameter > tmpMaxLimit))
	{
		bstr = statusFalse;
	}
	else
	{
		bstr = statusTrue;
	}
	//*** Put "Curvature" status label in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, bstr);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	i++;
	//*** Put "Curvature standard error" status label in array to return ***
	hr = SafeArrayPutElement(psaDisposition, &i, statusTrue);
	if (FAILED(hr))
	{
		sprintf(buf,"Function \"GetQuadraticDisposition\", file %s, line %d", __FILE__, __LINE__);
		LogErrorMessage(hr,"Return from function \"SafeArrayPutElement\"",buf);
		return hr;
	}

	// Set the return array's type, array of VT_BSTR.
	// VT_BSTR, strings. Type of data to return in Disposition.
	// Status for each value calculated.
	V_VT(disposition) = VT_ARRAY | VT_BSTR;
	// Set return Disposition array to contain variable's status.
    V_ARRAY(disposition) = psaDisposition;

	return hr;
}

HRESULT SetQuadraticDescriptionLabels(VARIANT *description)
{
	HRESULT			hr=S_OK;
	CStringArray	l_description;
	long			k;
	CString			tmpLabel;
	BSTR			bstr=NULL;

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	//*** define the safearray ***
    SAFEARRAY FAR* psa_description = NULL;

	l_description.Add("Chi Squared");
	l_description.Add("Correlation Coefficient");
	l_description.Add("Homogeneity");
	l_description.Add("Slope");
	l_description.Add("Slope standard error");
	l_description.Add("Anchor");
	l_description.Add("Anchor standard error");
	l_description.Add("Curvature");
	l_description.Add("Curvature standard error");

	// Set size of description array to pass to client.
	sabound[0].cElements = l_description.GetSize();
	sabound[0].lLbound = 0;
	psa_description = SafeArrayCreate(VT_BSTR, 1, sabound);

	k=0;
	// Copy local descriptions to VARIANT to pass to client.
	for (int i=0; i<l_description.GetSize(); i++)
	{
		tmpLabel = l_description.GetAt(i);
		bstr = tmpLabel.AllocSysString();
		hr = SafeArrayPutElement(psa_description, &k, bstr);
		k++;
	}

	// Set the return description array of VT_BSTR.
	// VT_BSTR, strings. Type of data to return in description.
	V_VT(description) = VT_ARRAY | VT_BSTR;
    V_ARRAY(description) = psa_description;

	return S_OK;
}

HRESULT	GetQuadraticMaxAndMinLimits(LONG	calibrationID,
									VARIANT *minLimits,
									VARIANT *maxLimits,
									CString algorithm,
									VARIANT *error_msg)
{
	HRESULT	hr;
	long	i;

	// Get current maximum value for a double.
	// Used as a hold place for future useage.
	double cur_max_value = currentConstants.GetMaxDoubleValue();
	double cur_min_value = currentConstants.GetMinDoubleValue();

    //*** define the SAFEARRAYBOUND structure
    SAFEARRAYBOUND sabound[1];

	//*** define the safearray ***
    SAFEARRAY FAR* psaMinLimits = NULL;
    SAFEARRAY FAR* psaMaxLimits = NULL;

	// Return array size.
	sabound[0].cElements = 9;
	
	sabound[0].lLbound = 0;

	//*** create the safearrays to return ***
    psaMinLimits = SafeArrayCreate(VT_R8, 1, sabound);
    psaMaxLimits = SafeArrayCreate(VT_R8, 1, sabound);

	// Get upper and lower fit parameter limits from the database.
	hr = GetFitParsLimits(calibrationID, 
						  LowerLimits,
						  UpperLimits,
						  algorithm,
						  numberOfFitParameters,
						  "",
						  error_msg );
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Return from \"GetFitParsLimits\" function","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i=0;
	// -----------------------------------------
	// -------------- CHI-SQUARED --------------
	// -----------------------------------------
	//*** Put Chi-squared MAXIMUM in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &cur_max_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put Chi-squared MAXIMUM in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put Chi-squared MINIMUM in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &cur_min_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put Chi-squared MINIMUM in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i++;
	// ----------------------------------------------------
	// -------------- CORELATION COEFFICIENT --------------
	// ----------------------------------------------------
	//*** Put "Correlation coefficient" MINIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &cur_min_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Correlation coefficient\" MINIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put "Correlation coefficient" MAXIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &cur_max_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Correlation coefficient\" MAXIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i++;
	// -----------------------------------------
	// -------------- HOMOGENEITY --------------
	// -----------------------------------------
	//*** Put "Bartlett's test" MINIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &cur_min_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Bartlett's test\" MINIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put "Bartlett's test" MAXIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &cur_max_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Bartlett's test\" MAXIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i++;
	// -----------------------------------
	// -------------- SLOPE --------------
	// -----------------------------------
	//*** Put "Slope" MINIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &LowerLimits[Slope]);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Slope\" MINIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put "Slope" MAXIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &UpperLimits[Slope]);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Slope\" MAXIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i++;
	//*** Put "Slope standard error" MINIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &cur_min_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Slope standard error\" MINIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put "Slope standard error" MAXIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &cur_max_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Slope standard error\" MAXIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i++;
	// ---------------------------------------
	// -------------- ANCHOR --------------
	// ---------------------------------------
	//*** Put "Anchor" MINIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &LowerLimits[Anchor]);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Intercept\" MINIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put "Anchor" MAXIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &UpperLimits[Anchor]);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Intercept\" MAXIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i++;
	//*** Put "Anchor standard error" MINIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &cur_min_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Intercept standard error\" MINIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put "Anchor standard error" MAXIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &cur_max_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Intercept standard error\" MAXIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i++;
	// ---------------------------------------
	// -------------- CURVATURE --------------
	// ---------------------------------------
	//*** Put "Curvature" MINIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &LowerLimits[Curvature]);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Intercept\" MINIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put "Curvature" MAXIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &UpperLimits[Curvature]);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Intercept\" MAXIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	i++;
	//*** Put "Curvature standard error" MINIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMinLimits, &i, &cur_min_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Intercept standard error\" MINIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}
	//*** Put "Curvature standard error" MAXIMUM value in array to return ***
	hr = SafeArrayPutElement(psaMaxLimits, &i, &cur_max_value);
	if (FAILED(hr))
	{
		LogErrorMessage(hr,"Put \"Intercept standard error\" MAXIMUM value in array to return","GetQuadraticMaxAndMinLimits");
		return hr;
	}

	// Set the return array's type, array of VT_R8.
	// VT_R8, 8-byte real. Type of data to return in MinLimits.
	//*** set up the safearray to export it as an array of reals.  
	V_VT(minLimits) = VT_ARRAY | VT_R8;
	// Set return array to contain minimum limits.
	//*** put the pointer into the VARIANT ***
	V_ARRAY(minLimits) = psaMinLimits;

	// Set the return array's type, array of VT_R8.
	// VT_R8, 8-byte real. Type of data to return in MaxLimits.
	//*** set up the safearray to export it as an array of reals.  
	V_VT(maxLimits) = VT_ARRAY | VT_R8;
	// Set return array to contain requested data.
	//*** put the pointer into the VARIANT ***
	V_ARRAY(maxLimits) = psaMaxLimits;

	return S_OK;
}
