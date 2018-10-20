/*!
    \file
    Program is helper with laba 1.1.3

        
    //\details
    It read file with data, process them and write result in 3 file.
    User must enter number of measurements
    The data in the file should be stored in the format of number of measurements value of resistance 
    If input or output files not found, program print you error.
*/

/*!
    \brief
    Funcion that read data from file 
    \param R[]
    Array with data of resistance
    \param input
    Input file
    \param size
    Number of measurements   
*/

void Read (float R[], FILE* input, int size);

/*!
    \brief
    Funcion that compute deltaN 
    \param R[]
    Array with data of resistance
    \param deltaR
    (Rmin - Rmax) / N
    \param num
    Value of N
    \param deltaN[] 
    Array of value of deltaN
    \param nMeas
    Number of measurements
*/

void DeltaN (float R[], float deltaR, int num, float deltaN[], int nMeas);

/*!
    \brief
    Funcion that process data
    \param input
    Name of input file
    \param nMeas
    Number of measurements
    \param outputSort
    Name of file with sorted data
    \param outputGraph
    Name of file with data for graph
    \param outputText
    Name of file with result
*/

void Compute2 (FILE* input, int nMeas, FILE* outputSort, FILE* outputGraph, FILE* outputText);

/*!
    \brief
    Funcion that compute omega
    \param deltaN[]
    Array with value deltaN
    \param OmegaN[]
    Array with value omega
    \param nMeas
    Number of measurements
    \param deltaR
    Value deltaR
    \param n
    Value N 
*/

void ComputeOmega (float deltaN[], float OmegaN[], int nMeas, float deltaR, int n);

/*!
    \brief
    Funcion that compute average resistance
    \param rAvg
    Value average resistance
    \param nMeas
    Number of measurements
    \param R[]
    Array with value of resistance
*/

void Avg ( float* rAvg, int nMeas, float R[]);

/*!
    \brief
    Funcion that compute error
    \param R[]
    Array with value of resistance
    \param rAvg
    Value average resistance
    \param error
    Value of error
    \param nMeas
    Number of measurements
*/

void Error (float R[], float rAvg, float* error, int nMeas);

/*!
    \brief
    Funcion that compute gausiana
    \param R[]
    Array with value of resistance
    \param rAvg
    Value average resistance
    \param error
    Value of error
    \param Gausiana[]
    Value of funcion Gaus's
    \param nMeas
    Number of measurements
*/

void ComputeGausiana (float R[], float rAvg, float error, float Gausiana[], int nMeas);

/*!
    \brief
    Funcion that compute probability
    \param R[]
    Array with value of resistance
    \param rAvg
    Value average resistance
    \probab
    Probability of falling into the error
    \param error
    Value of error
    \param nMeas
    Number of measurements
*/

void Probability (float R[], float rAvg, float* probab, float error, int nMeas);

/*!
    Print funcion
*/

void PrintSort (float R[] , FILE* outputSort, int nMeas);

/*!
    Print funcion
*/

void PrintGraph (float R[], float Gausiana[], float OmegaN1[], float OmegaN2[], int nMeas, float deltaR1, float deltaR2, FILE* outputGraph);

/*!
    Print funcion
*/

void PrintText (float rAvg, float error, float deltaR1, float deltaR2, FILE* outputText, float probab1, float probab2, float probab3);

//=============================================================================

int Laba2 ()
	{
    char fileinput [LengthFilename] = "";
    char fileText [LengthFilename] = "";
    char fileGraph [LengthFilename] = "";
    char fileSort [LengthFilename] = "";    
	
    printf ("Please enter name of file with data\n");
    scanf ("%s", fileinput);


    FILE* input  = fopen (fileinput,"r");
    
    int message = Message (input, fileinput);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);

    int nMeas = 0;
    printf ("\nPlease enter number of measurements\n");
    scanf ("%d", &nMeas);


    printf ("\nPlease enter name of CSV file wiith sorted data\n");
    scanf ("%s", fileSort);

    FILE* outputSort = fopen (fileSort,"w");

    message = Message (outputSort, fileSort);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);  
    
    
    printf ("\nPlease enter name of CSV file with result for graph\n");
    scanf ("%s", fileGraph);

    FILE* outputGraph  = fopen (fileGraph,"w");

    message = Message (outputGraph, fileGraph);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);

    
    printf ("\nPlease enter name of CSV file with result for graph\n");
    scanf ("%s", fileText);

    FILE* outputText  = fopen (fileText,"w");

    message = Message (outputText, fileText);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);

    Compute2 (input, nMeas, outputSort, outputGraph, outputText);
	
    fclose (input);
    fclose (outputSort);
    fclose (outputText);
    fclose (outputGraph);   

    } 

//=============================================================================

void Compute2 (FILE* input, int nMeas, FILE* outputSort, FILE* outputGraph, FILE* outputText) 
    {
    float deltaN1[N1], deltaN2[N2], OmegaN1[N1], OmegaN2[N2];
    float* R = (char*) calloc (nMeas, sizeof (char));
    float* Gausiana = (char*) calloc (nMeas, sizeof (char));
	
    ZeroArray (deltaN1, N1);
    ZeroArray (deltaN2, N2);
    ZeroArray (R, nMeas);
    ZeroArray (OmegaN1, N1);
    ZeroArray (OmegaN2, N2);
    ZeroArray (Gausiana, nMeas);
    Read (R, input, nMeas);
    Sort (R, nMeas);
    float rAvg = 0; 
    float rMax = R[nMeas - 1];
    float rMin = R[0];
    Avg (&rAvg, nMeas, R);
 
    float deltaR1 = (rMax - rMin) / N1;
    float deltaR2 = (rMax - rMin) / N2;

    for (int num = 0; num < N1; num++)
        {
        assert ((0 <= num) && (num < N1));

        DeltaN (R, deltaR1, num, deltaN1, nMeas);
        }

    for (int num = 0; num < N2; num++)
        {
        assert ((0 <= num) && (num < N2));

        DeltaN (R, deltaR2, num, deltaN2, nMeas);
        } 
 

    deltaN1[N1 - 1] += 1;
    deltaN2[N2 - 2] += 1;
    
    ComputeOmega (deltaN1, OmegaN1, nMeas, deltaR1, N1);
    ComputeOmega (deltaN2, OmegaN2, nMeas, deltaR2, N2);


    float error = 0;

    Error (R, rAvg, &error, nMeas); 
        
    float probab1 = 0, probab2 = 0, probab3 = 0;
    
    Probability (R, rAvg, &probab1, error, nMeas);
    Probability (R, rAvg, &probab2, 2 * error, nMeas);          
    Probability (R, rAvg, &probab3, 3 * error, nMeas);

    ComputeGausiana (R, rAvg, error, Gausiana, nMeas);
    
    PrintSort (R , outputSort, nMeas);
    PrintGraph (R, Gausiana, OmegaN1, OmegaN2, nMeas, deltaR1, deltaR2, outputGraph);
    PrintText (rAvg, error, deltaR1, deltaR2, outputText, probab1, probab2, probab3); 

    printf ("Progaram finished successfully");
    
}

//=============================================================================

void DeltaN (float R[], float deltaR, int num, float deltaN[], int nMeas)
    {
    for (int i = 0; i < nMeas; i++)
        {
        assert ((0 <= i) && (i < nMeas)); 

        if ((R[0] + num * deltaR <= R[i]) && (R[0] + (num + 1) * deltaR > R[i])) deltaN[num] += 1; 
        }
    } 

//=============================================================================

void Read (float R[], FILE* input, int size)
    {
    assert (size > 0);
    
    for (int i = 0; i < size; i++) fscanf (input, "%f", &R[i]);
    }

//=============================================================================

void ComputeOmega (float deltaN[], float OmegaN[], int nMeas, float deltaR, int n) 
    {    
    for (int i = 0; i < n; i++)
        {
        assert ((0 <= i) && (i < n));

        OmegaN[i] = deltaN[i] / (nMeas * deltaR);
        
        assert (OmegaN[i] >= 0);    
        }
    }

//=============================================================================

void Avg ( float* rAvg, int nMeas, float R[])
    {
    float sumR = 0;

    assert (nMeas > 0);
    
    for (int i = 0; i < nMeas; i++)
        {
        assert ((0 <= i) && (i < nMeas));

        sumR += R[i];
        }
    *rAvg = sumR / nMeas;
    }

//=============================================================================

void Error (float R[], float rAvg, float* error, int nMeas) 
        {
        float sum = 0 ;    
        for (int i = 0; i < nMeas; i++)
            {
            sum += (R[i] - rAvg) * (R[i] - rAvg);
            }  
       
        *error = sqrt (sum / nMeas);        
        }

//=============================================================================

void ComputeGausiana (float R[], float rAvg, float error, float Gausiana[], int nMeas)
    {
    for (int i = 0; i < nMeas; i++)
        {
        assert ((0 <= i) && (i < nMeas));

        Gausiana[i] = pow (e, -1 * (R[i] - rAvg) * (R[i] - rAvg)/ (2 * error * error)) / (sqrt (2* Pi) * error);
        }
    }

//=============================================================================

void Probability (float R[], float rAvg, float* probab, float error, int nMeas)          
    {
    int sum = 0;
    for (int i = 0; i < nMeas; i++)
        {
        assert ((0 <= i) && (i < nMeas));

        if ((R[i] >= rAvg - error) && (R[i] <= rAvg + error)) sum += 1 ;
        }
    
    *probab = sum * 100 / nMeas;   
    }

//=============================================================================

void PrintGraph (float R[], float Gausiana[], float OmegaN1[], float OmegaN2[], int nMeas, float deltaR1, float deltaR2, FILE* outputGraph)
    {
    fprintf (outputGraph, "R Gausiana\n");
    
    for (int i = 0; i < nMeas; i++)
        {
        assert ((0 <= i) && (i < nMeas));
    
        fprintf (outputGraph, "%f %f\n", R[i], Gausiana[i]);
    
        }

    fprintf (outputGraph, "N = 20\n");
    fprintf (outputGraph, "R Omega\n");
    
    for (int i = 0; i < N1; i++)
        {
        assert ((0 <= i) && (i < N1));

        fprintf (outputGraph, "%f %f\n", OmegaN1[i], R[0] + deltaR1 * i); 
        }
        
    fprintf (outputGraph, "N = 10\n");
    fprintf (outputGraph, "R Omega\n");
    
    for (int i = 0; i < N2; i++)
        {
        assert ((0 <= i) && (i < N2));

        fprintf (outputGraph, "%f %f\n", OmegaN2[i], R[0] + deltaR2 * i); 
        }
    }
    
void PrintSort (float R[] , FILE* outputSort, int nMeas)
    {
    for (int i = 0; i < nMeas; i += 9)
        {
        assert ((0 <= i) && (i < nMeas));

        fprintf (outputSort, "%f %f %f %f %f %f %f %f %f\n", R[i], R[i + 1], R[i + 2], R[i + 3], R[i + 4], R[i + 5], R[i + 6], R[i + 7], R[i + 8]);
        } 
    
    }

void PrintText (float rAvg, float error, float deltaR1, float deltaR2, FILE* outputText, float probab1, float probab2, float probab3)
    {
    fprintf (outputText, "Ravg = %.3f\n", rAvg);
    fprintf (outputText, "error = %.3f\n", error);
    fprintf (outputText, "deltaR1 = %.3f\n", deltaR1);
    fprintf (outputText, "deltaR2 = %.3f\n", deltaR2);
    fprintf (outputText, " Probability of falling into the interval +- error%.3f\n", probab1);
    fprintf (outputText, " Probability of falling into the interval +- 2 * error%.3f\n", probab2);
    fprintf (outputText, " Probability of falling into the interval +- 3 * error%.3f\n", probab3);
    
    }

