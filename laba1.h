/*!
	\file
	Program is helper with laba 1.1.1
	
        
	//\details
	It read file with data, process them and write result in file.
	The data in the file should be stored in the format of 36 pairs of numbers of current and voltage.
	If input or output file not fond, program print you error.
	If the data file has error, then program print you error in the file.
*/

int Compute (FILE* input, FILE* output, float length, int num, FILE* CSV, float square ,float errorS, float* ro, float* errorRo);
int ReadAndCheck (float U[], float I[], int size, FILE* input);
void Print (float R[], FILE* output, float length);
int CheckData (float U[], float I[]);
void PrintCSV (float U[], float I[], FILE* CSV);
int Diameter (float* s, float* errorS, FILE* diameter);
void Processing (float U[], float I[], float* sumUI, float* sumUU, float* sumII);
float Avg (float sumUI, float sumII, float resistanceAvg);
float RandomError (float resistanceAvg, float sumUU,float sumII, float errorRrandom);
float System (float maxU, float maxI, float resistanceAvg, float errorRsystem);
float TotalErrorR (float errorRrandom, float errorRsystem ,float errorR);
void TotalR (float resistanceAvg, float* rTotal, float* ro, float s, int length);
void TotalErrorRo (float ro, float errorR, float rTotal, float* errorRo, float errorS, int length);
void Print (float ro1, float ro2, float ro3, char* fileoutput, float errorRo1, float errorRo2, float errorRo3, FILE* output);

//=============================================================================

int Laba1 ()
    {
    char fileinput [LengthFilename] = "";
    char fileoutput[LengthFilename] = "";
    char filediameter[LengthFilename] = "";
    char fileCSV[LengthFilename] = "";


    printf ("Please enter name of file with data\n");
    scanf ("%s", fileinput);

    FILE* input  = fopen (fileinput,"r");

    int message = Message (input, fileinput);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);

    
    printf ("\nPlease enter name of file with dimetre\n");
    scanf ("%s", filediameter);

    FILE* diameter  = fopen (filediameter,"r");

    message = Message (diameter, filediameter);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);


    printf ("\nPlease enter name of CSV file wiith sorted data\n");
    scanf ("%s", fileCSV);

    FILE* CSV  = fopen (fileCSV,"w");

    message = Message (CSV, fileCSV);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);  
    
    
    printf ("\nPlease enter name of file with result\n");
    
    scanf ("%s", fileoutput);    

    
    FILE* output = fopen (fileoutput,"w");
	
    message = Message (input, fileoutput);

    assert ((message == 0) || (message == ERROR1));
        
    if (message == ERROR1) return (ERROR1);

    
    float square = 0, errorS = 0;    
    int d = Diameter (&square, &errorS , diameter);
    
    assert ((d == 0) || (d == ERROR2));    
    
    if (d == ERROR2) return (ERROR2);


    float ro1 = 0, errorRo1 = 0, ro2 = 0, errorRo2 = 0, ro3 = 0, errorRo3 = 0;    

    int correct = Compute (input, output, Length1, Number1stMeas, CSV, square, errorS, &ro1, &errorRo1);

    assert (correct >= 0);

    if (correct > 0) return (ERROR2);
   
    
    correct = Compute (input, output, Length2, Number2ndMeas, CSV, square, errorS, &ro2, &errorRo2);

    assert (correct >= 0);

    if (correct > 0) return (ERROR2);


    correct = Compute (input, output, Length3, Number3edMeas, CSV, square, errorS, &ro3, &errorRo3);
    
    assert (correct >= 0);
    
    if (correct > 0) return (ERROR2);

    Print (ro1, ro2, ro3, fileoutput, errorRo1, errorRo2, errorRo3, output);
 
    fclose (input);
    fclose (output);
    fclose (diameter);
    fclose (CSV);    
    
    return (0);
    	
    }

//=============================================================================

/*!
    \brief
    Funcion that process data and print result 
    \param input
    Input file
    \param output
    Output file
    \param length
    Length of wire
    \param num 
    Multiplier for number of measurements
    \param CSV
    Name of .csv file
    \param square 
    Square of wire
    \param errorS
    Value of error of square
    \param ro
    Value of resistivity
    \param errorRo
    Value of Error of resistivity   
*/

int Compute (FILE* input, FILE* output, float length, int num, FILE* CSV, float square ,float errorS, float* ro, float* errorRo)
    {
    assert ((Length1 <= length) && (length <= Length3));

    float U[NMeas], I[NMeas], R[NMeas];
    ZeroArray (U, NMeas);
    ZeroArray (I, NMeas);
    ZeroArray (R, NMeas);
	
    int checkread = ReadAndCheck (U, I, NMeas, input);

    assert (checkread >= 0);
    
    if (checkread > 0)
        {
        printf ("ERROR in line %d\n", checkread + num);
        return (ERROR2); 
        }
    
    int check = CheckData (U, I);	

    assert (check >= 0);

    if (check > 0)
        {
        printf ("ERROR in line %d\n",check + num);
        return (ERROR2); 
        }
		
    Sort (U, NMeas);
    Sort (I, NMeas);
    float maxU = U[NMeas - 1]; 
    float maxI = I[NMeas - 1]; 
    
    float sumII = 0, sumUI = 0, sumUU = 0;
    Processing (U, I, &sumUI, &sumUU, &sumII); 

    float resistanceAvg = 0;
    resistanceAvg = Avg (sumUI, sumII, resistanceAvg);

    assert (resistanceAvg > 0);


    float errorRrandom = 0;   
    errorRrandom = RandomError (resistanceAvg, sumUU, sumII, errorRrandom);

    assert (errorRrandom > 0);

     
    float errorRsystem = 0;
    errorRsystem = System (maxU, maxI, resistanceAvg, errorRsystem);

    assert (errorRsystem > 0);


    float errorR = 0;
    errorR = TotalErrorR (errorRrandom, errorRsystem, errorR);

    assert (errorR > 0);

    float rTotal = 0;
    TotalR (resistanceAvg, &rTotal, ro, square , length);

    assert (rTotal > 0);


    TotalErrorRo (*ro, errorR, rTotal, errorRo, errorS, length);

    assert (ro > 0);

    
    fprintf (output,"\nFor length = %3.0f cm\n",length);
    fprintf (output,"Ravg = %5.3f Om\n", resistanceAvg); 
    fprintf (output,"Rtot = %5.3f Om\n", rTotal);
    fprintf (output,"ErrorRand = %5.3f Om\n", errorRrandom);
    fprintf (output,"ErrorSys = %5.3f Om\n", errorRsystem);
    fprintf (output,"ErrorTot = %5.3f Om\n", errorR);
    fprintf (output,"Ro = %5.2f\t, ErrorRo = %5.2f\n\n", *ro, *errorRo); 


    PrintCSV (U, I, CSV);
	
    return (0);

    }

//=============================================================================

/*!
    \brief
    Funcion that read file 
    \param U[]
    Array with value of voltage
    \param I[] 
    Array with value of current
    \param size 
    Number of value
    \param input 
    Input file
*/ 

int ReadAndCheck (float U[], float I[], int size, FILE* input)
    {
    assert (size > 0);

    char str[1024] = "";

    for (int line = 0; line < size; line++)
        {
        assert ((0 <= line) && (line < size));

        fgets (str, sizeof (str) - 1, input);

        int check = sscanf (str,"%f %f", &U[line], &I[line]);

        assert ((check >= -1) && (check <= 2));

        if (check != 2) return (line + 1);
        }

    return (0);
    }

//=============================================================================

/*!
    \brief
    Funcion that check data
    \param U[]
    Array with value of voltage
    \param I[] 
    Array with value of current
*/ 

int CheckData (float U[], float I[])
    {	
    for (int line = 0; line < NMeas; line++)
        {
	assert ((0 <= line) && (line < NMeas));

	if ((U[line] < 0) || ((I[line] > 0.0 - Tolerance) && (I[line] < 0.0 + Tolerance)) || (U[line] > MaxU + Tolerance) || (I[line] > MaxI + Tolerance))
	    {  
	    return (line + 1);
	    }
        }

    return (0);
    }
    
//=============================================================================

/*!
    \brief
    Funcion that count square of wire
    \details Funcion read file with value of diameter and process data
    \param S 
    Value square of wire
    \param errorS 
    Value error for square
    \param diameter
    File with data of diameter   
*/

int Diameter (float* s, float* errorS, FILE* diameter)
    {	
    float D[NumD] = {} ;
    float sumD = 0, dAvg = 0;
    
    char str[1024] = "";
	
    for (int line = 0; line < NumD; line++)
	{
        fgets (str, sizeof (str) - 1, diameter);

	int check = sscanf (str,"%f", &D[line]);

        assert (check >= -1);        

        if (check != 1)
            {
            printf ("ERROR in file with data of diameter on line №%d",check + 1);
            return (ERROR2);
            }        
            
	sumD += D[line];
	}
	
	dAvg = sumD / NumD;
	*s = Pi * dAvg * dAvg * 100 / 4;
	*errorS = 2 * ErrorD / dAvg;

	assert ((*s > 0) && (*errorS > 0));
    
    	return (0);
    	}

//=============================================================================

/*!
    \brief
    Funcion that print sorted data in .csv file
    \param U[]
    Array with data of voltage
    \param I[]
    Array with data of current
    \param CSV
    Output file
*/

void PrintCSV (float U[], float I[], FILE* CSV)
	{
	for (int i = 0; i < NMeas; i++)
	    {
	    assert ((0 <= i) && (i < NMeas));
		
	    fprintf (CSV,"%4.1f %5.2f\n", U[i], I[i]);
	    }
    	fprintf (CSV,"\n");
        }



//=============================================================================

/*!
    \brief
    Funcion that count summ of U^2, I^2, U * I 
    \param U[]
    Array with data of voltage
    \param I[]
    Array with data of current
    \param sumUI 
    ammount of multiplication current and voltage
    \param sumII 
    ammount of current^2
    \param resistanceAvg 
    Averadge value resistance 
*/

void Processing (float U[], float I[], float* sumUI, float* sumUU, float* sumII)
    {
    for (int i = 0; i < NMeas; i++)                                       
        {
	assert ((0 <= i) && (i < NMeas));    

	*sumUI += U[i] * I[i];
	*sumII += I[i] * I[i];
	*sumUU += U[i] * U[i]; 
	}
    }

//=============================================================================

/*!
    \brief
    Funcion that count average resistance of wire 
    \details Funcion uses the lest squares method for obtain average resistance
    \param sumUI 
    ammount of multiplication current and voltage
    \param sumII 
    ammount of current^2
    \param resistanceAvg 
    Averadge value resistance 
*/

float Avg (float sumUI, float sumII, float resistanceAvg)
    {
    assert ((sumUI - Tolerance > 0) && (sumII - Tolerance > 0));  

    resistanceAvg = sumUI / sumII;

    assert ((0 < resistanceAvg) && (resistanceAvg < MaxResistance));

    return (resistanceAvg); 
    }

//=============================================================================

/*!
    \brief
    Funcion that count random error for resistance of wire  
    \param sumUU 
    ammount of voltage^2
    \param sumII 
    ammount of current^2
    \param resistanceAvg 
    Averadge value resistance
    \param errorRrandom
    Value of random error for resistance 
*/
float RandomError (float resistanceAvg, float sumUU,float sumII, float errorRrandom)
    {
    assert ((resistanceAvg > 0) && (sumUU > 0) && (sumII > 0)); 

    float sqrtN = sqrt(NMeas);
    errorRrandom = (sqrt((sumUU / sumII) - (resistanceAvg * resistanceAvg))) / sqrtN;

    assert ((0 < errorRrandom) && (errorRrandom < MaxError));

    return (errorRrandom);
    }

//=============================================================================

/*!
    \brief
    Funcion that count systematic error for resistance of wire
    \param maxU 
    Max value of voltage 
    \param maxI 
    Max value of current 
    \param resistanceAvg 
    Averadge value resistance
    \param errorRsystem
    Value of systematic error for resistance   
*/

float System (float maxU, float maxI, float resistanceAvg, float errorRsystem)
    {
    assert ((maxU > 0) && (maxI > 0));	
    errorRsystem = resistanceAvg * sqrt((ErrorU / maxU) * (ErrorU / maxU) + (ErrorI / maxI) * (ErrorI / maxI));
	
    assert ((0 < errorRsystem) && (errorRsystem < MaxError));

    return (errorRsystem);
    }

//=============================================================================

/*!
    \brief
    Funcion that count total error for resistance of wire
    \param errorRrandom 
    Value of random error for resistance 
    \param errorRsystem
    Value of systematic error for resistance 
    \param errorR 
    Total value error for resistance
*/
float TotalErrorR (float errorRrandom, float errorRsystem ,float errorR)
    {
    assert ((errorRrandom > 0) && ( errorRsystem > 0));

    errorR = sqrt(errorRrandom * errorRrandom + errorRsystem * errorRsystem);

    assert (errorR > 0);

    return (errorR);
    }

//=============================================================================

/*!
    \brief
    Funcion that count total resistance and resistivity of wire (l=20cm)  
    \param resistanceAvg 
    Averadge value resistance
    \param rTotal 
    Total resistance of wire
    \param ro
    Value resistivity of wire
    \param s
    Value square of wire
    \param length 
    Length of wire
*/
void TotalR (float resistanceAvg, float* rTotal, float* ro, float s, int length)
    {
    assert ((resistanceAvg > 0) && (s > 0) && (length > 0));

   *rTotal = resistanceAvg + (resistanceAvg * resistanceAvg) / ResistanceU;
   *ro = *rTotal * s / length;

    assert ((rTotal > 0) && (ro > 0));
    }

//=============================================================================

/*!
    \brief
    Funcion that count total error for resistivity of wire (l=20cm)  
    \param ro
    Value resistivity of wire
    \param errorR
    Total value error for resistance of wire
    \param Rtotal 
    Total value resistance of wire
    \param errorRo
    Value error for resistivity
    \param errorS
    Value error for square of wire
    \param length
    Length of wire (cm)
*/

void TotalErrorRo (float ro, float errorR, float rTotal, float* errorRo, float errorS, int length)
    {
    assert ((ro > 0) && (errorR > 0) && (rTotal > 0) && (errorS > 0) && (length > 0));
    
    *errorRo = ro * sqrt((errorR / rTotal) * (errorR / rTotal) + (errorS * errorS) + (ErrorL / length) * (ErrorL / length));

    assert ( errorRo > 0);	
    }

//=============================================================================

void Print (float ro1, float ro2, float ro3, char* fileoutput, float errorRo1, float errorRo2, float errorRo3, FILE* output)
    {
    assert ((ro1 > 0) && (ro2 > 0) && (ro3 > 0) && (fileoutput != "\0") && (errorRo1 > 0) && (errorRo2 > 0) && (errorRo3 > 0) && (output != NULL));

    float roTotal = (ro1 + ro2 + ro3)/3;
    float errorRoTotal = (errorRo1 + errorRo2 + errorRo3)/3;

    printf ("\nResult saved in file '%s'\n", fileoutput);

    fprintf (output,"\n\nTotal value resistivity (%3.2f+-%3.2f) * 10^(-4)Om * cm", roTotal, errorRoTotal); 
    }
