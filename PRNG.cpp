//Mohammad Hemmati 
/******************************************************************************/
/*** Ver 0.2                                                                ***/
/*** Date: 13960219                                                         ***/
/*** Code implemented in GNU C++ compiler (v. 4.6.3),                       ***/
/*** OS: Linux Ubuntu (x86_64)                               		    ***/
/******************************************************************************/

// compile by following command in shell:
//> g++ -o PRNGs.out PRNGs.cpp -Ofast
// execute in shell by:
//> ./PRNGs.out

/* These routine based on Random number generator

   double ran2(long *idum);
   
implemented in the Numerical recipes in C, chapter 7 (ran2)

Long period (> 2 × 10^{18}) random number generator of L. Ecuyer with Bays-Durham shuffle
and added safeguards. Returns a uniform random deviate between 0.0 and 1.0 (exclusive of
the endpoint values). 

***!!! Call with idum a negative integer to initialize; !!!*** thereafter, do not alter
idum between successive deviates in a sequence. RNMX should approximate the largest floating
value that is less than 1.

Visit www.nr.com for the licence.*/

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <ctime>

using namespace std;

//------------------------------------------------------------------------------

/* following routine based on Random number generator

   double ran2(long *idum);
   
implemented in the Numerical recipes in C, chapter 7 (ran2)

Long period (> 2 × 10^{18}) random number generator of L. Ecuyer with Bays-Durham shuffle
and added safeguards. Returns a uniform random deviate between 0.0 and 1.0 (exclusive of
the endpoint values). 

***!!! Call with idum a negative integer to initialize; !!!*** thereafter, do not alter
idum between successive deviates in a sequence. RNMX should approximate the largest floating
value that is less than 1.

Visit www.nr.com for the licence.*/

// This is a internal, 32 bit random number generator with uniform Distribution in range [0..1)
/* note #undef's at end of file */
//////==========================================================================
#define IM1 2147483563 // add our constants
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)


double ran2(long *idum) { 
    int j;
    long k;
    static long idum2=123456789;
    static long iy=0;
    static long iv[NTAB];
    double temp;

    if (*idum <= 0) {
        if (-(*idum) < 1) *idum=1;
        else *idum = -(*idum);
        idum2=(*idum);
        for (j=NTAB+7;j>=0;j--) {
            k=(*idum)/IQ1;
            *idum=IA1*(*idum-k*IQ1)-k*IR1;
            if (*idum < 0) *idum += IM1;
            if (j < NTAB) iv[j] = *idum;
        }
        iy=iv[0];
    }
    k=(*idum)/IQ1;
    *idum=IA1*(*idum-k*IQ1)-k*IR1;
    if (*idum < 0) *idum += IM1;
    k=idum2/IQ2;
    idum2=IA2*(idum2-k*IQ2)-k*IR2;
    if (idum2 < 0) idum2 += IM2;
    j=iy/NDIV;
    iy=iv[j]-idum2;
    iv[j] = *idum;
    if (iy < 1) iy += IMM1;
    if ((temp=AM*iy) > RNMX) return RNMX;
    else return temp;
}
#undef IM1
#undef IM2
#undef AM
#undef IMM1
#undef IA1
#undef IA2
#undef IQ1
#undef IQ2
#undef IR1
#undef IR2
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

//------------------------------------------------------------------------------

// My interface 

// Random number generator seed
long iseed = -36;

// Note: idum in routines argument is random number generator seed if not availible default seed (iseed) is used
// You can initializing random generator seed by usin following function
void Randomize() { // Threadsafe version of Randomize
  iseed = -time(NULL);  
}

// return random number with uniform distribution in range [0..1).
inline double Random() { return ran2(&iseed); }

// return integer random number with uniform distribution in range 0 to N.
inline int Random(int N) { return int(ran2(&iseed)*N); }


// Main routine
int main (int argc, char *argv[]) {
    cout << "Hello" << endl;
    int tedad=10;
    cout << "time(NULL): " << time(NULL) << endl;
    cout << "chand adad random mikhayd? (yek adade integer vared konid)=" ;
    cin >> tedad;
	// ba afzoodane tedad adad random az vorodi mitavan ba ejraye chand bare barname ba tedad mokhtalef yeknavakht shodan tozi ra ba afzashe tedad 
	// moshahede kard.
   
	Randomize();
    
    cout << "random sequence from ran2()\n";
    for (int i=0; i<tedad; i++)
        cout << Random() << endl;
    
    // find distribution of ran2()
    // Init
    int N_bin = 20;                         // Number of bins
    int p[N_bin];                           // bins
    for (int i=0; i<N_bin; i++)
        p[i] = 0;                           // clear bins
    
    // run
    for (int i=0; i<tedad; i++) {
        int i_bin = (int) (Random()*N_bin);
        ++p[i_bin];
    }
    
    // done
    cout << "p[i]\n";
    for (int i=0; i<N_bin; i++)
        cout << p[i] << endl;

    double sum = 0;
    for (int i=0; i<N_bin; i++)
        sum += p[i];
	
    cout << "normalized\n";
    double r = 1./sum;
    for (int i=0; i<N_bin; i++){
        cout << p[i] * r << endl ;
		}
	cout << "sum=" << sum << endl ;
    ofstream temp("temp.txt", ios::out | ios::trunc);
    temp << "set ylabel \"y\"\n"
         << "set xlabel \"x\"\n" 
         << "set yrange [0:1.5] \n"  		//range y ra dar gnuplot moshakhas mikonad
		 << "set ytics 0.5\n"				// tics faseleye beyne do addadi ke namayesh midahad ra taeen mikonad
       	 << "set xtics 0.5\n"
  		 << "set xrange [0:1]\n"
		 << "set term png \n"				//akse png az nemodar tolid mikonad
		 << "set output \"PRNG1.png\" \n"	//akse khoroji ra dar haman poshe be name PRNG1.png zakhire mikonad
  		 << "plot '-' using 1:2 title \"Alef\" with lp lc 3 \n" 
         << "set term x11 \n" << endl;		//baraye sakht file az gnuplot x11 
    double alfa;
    for(int i=0; i<N_bin; i++){
		alfa=(1.0*i)/N_bin;					//dar soal khaste shode mehvar x az 0 ta 1 bashad darnatije bar tedadeshan taghsim kardim ta normalize shan
   		temp << alfa << '\t' << p[i] * r * N_bin << endl;
		}
    
    temp << "EOF\n"
         << "pause -1 \"Hit any key to continue\"" << endl;

    temp.close();

    system("gnuplot temp.txt");
	system("gnome-open PRNG1.png");			//in dastor baraye baz kardan akse sakhte shode dar Ubuntu be kar miayad. dar sayere system amel ha file
											// png sakhte shode baz nemishavad. bayad dasti baz kard. ya in dastor ra taghir dad.
    
    cout << "Finish" << endl;

    return 0;
}
