#ifndef THERMISTORTABLES_H_
#define THERMISTORTABLES_H_

#include "Marlin.h"

#define OVERSAMPLENR 16

#if (THERMISTORHEATER_0 == 1) || (THERMISTORHEATER_1 == 1)  || (THERMISTORHEATER_2 == 1) || (THERMISTORBED == 1) //100k bed thermistor

const short temptable_1[][2] PROGMEM = {
{       23*OVERSAMPLENR ,       300     },
{       25*OVERSAMPLENR ,       295     },
{       27*OVERSAMPLENR ,       290     },
{       28*OVERSAMPLENR ,       285     },
{       31*OVERSAMPLENR ,       280     },
{       33*OVERSAMPLENR ,       275     },
{       35*OVERSAMPLENR ,       270     },
{       38*OVERSAMPLENR ,       265     },
{       41*OVERSAMPLENR ,       260     },
{       44*OVERSAMPLENR ,       255     },
{       48*OVERSAMPLENR ,       250     },
{       52*OVERSAMPLENR ,       245     },
{       56*OVERSAMPLENR ,       240     },
{       61*OVERSAMPLENR ,       235     },
{       66*OVERSAMPLENR ,       230     },
{       71*OVERSAMPLENR ,       225     },
{       78*OVERSAMPLENR ,       220     },
{       84*OVERSAMPLENR ,       215     },
{       92*OVERSAMPLENR ,       210     },
{       100*OVERSAMPLENR        ,       205     },
{       109*OVERSAMPLENR        ,       200     },
{       120*OVERSAMPLENR        ,       195     },
{       131*OVERSAMPLENR        ,       190     },
{       143*OVERSAMPLENR        ,       185     },
{       156*OVERSAMPLENR        ,       180     },
{       171*OVERSAMPLENR        ,       175     },
{       187*OVERSAMPLENR        ,       170     },
{       205*OVERSAMPLENR        ,       165     },
{       224*OVERSAMPLENR        ,       160     },
{       245*OVERSAMPLENR        ,       155     },
{       268*OVERSAMPLENR        ,       150     },
{       293*OVERSAMPLENR        ,       145     },
{       320*OVERSAMPLENR        ,       140     },
{       348*OVERSAMPLENR        ,       135     },
{       379*OVERSAMPLENR        ,       130     },
{       411*OVERSAMPLENR        ,       125     },
{       445*OVERSAMPLENR        ,       120     },
{       480*OVERSAMPLENR        ,       115     },
{       516*OVERSAMPLENR        ,       110     },
{       553*OVERSAMPLENR        ,       105     },
{       591*OVERSAMPLENR        ,       100     },
{       628*OVERSAMPLENR        ,       95      },
{       665*OVERSAMPLENR        ,       90      },
{       702*OVERSAMPLENR        ,       85      },
{       737*OVERSAMPLENR        ,       80      },
{       770*OVERSAMPLENR        ,       75      },
{       801*OVERSAMPLENR        ,       70      },
{       830*OVERSAMPLENR        ,       65      },
{       857*OVERSAMPLENR        ,       60      },
{       881*OVERSAMPLENR        ,       55      },
{       903*OVERSAMPLENR        ,       50      },
{       922*OVERSAMPLENR        ,       45      },
{       939*OVERSAMPLENR        ,       40      },
{       954*OVERSAMPLENR        ,       35      },
{       966*OVERSAMPLENR        ,       30      },
{       977*OVERSAMPLENR        ,       25      },
{       985*OVERSAMPLENR        ,       20      },
{       993*OVERSAMPLENR        ,       15      },
{       999*OVERSAMPLENR        ,       10      },
{       1004*OVERSAMPLENR       ,       5       },
{       1008*OVERSAMPLENR       ,       0       } //safety
};
#endif
#if (THERMISTORHEATER_0 == 2) || (THERMISTORHEATER_1 == 2) || (THERMISTORHEATER_2 == 2) || (THERMISTORBED == 2) //200k bed thermistor
const short temptable_2[][2] PROGMEM = {
//200k ATC Semitec 204GT-2
//Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 4.7kohm pullup, voltage divider math, and manufacturer provided temp/resistance
   {1*OVERSAMPLENR, 848},
   {30*OVERSAMPLENR, 300}, //top rating 300C
   {34*OVERSAMPLENR, 290},
   {39*OVERSAMPLENR, 280},
   {46*OVERSAMPLENR, 270},
   {53*OVERSAMPLENR, 260},
   {63*OVERSAMPLENR, 250},
   {74*OVERSAMPLENR, 240},
   {87*OVERSAMPLENR, 230},
   {104*OVERSAMPLENR, 220},
   {124*OVERSAMPLENR, 210},
   {148*OVERSAMPLENR, 200},
   {176*OVERSAMPLENR, 190},
   {211*OVERSAMPLENR, 180},
   {252*OVERSAMPLENR, 170},
   {301*OVERSAMPLENR, 160},
   {357*OVERSAMPLENR, 150},
   {420*OVERSAMPLENR, 140},
   {489*OVERSAMPLENR, 130},
   {562*OVERSAMPLENR, 120},
   {636*OVERSAMPLENR, 110},
   {708*OVERSAMPLENR, 100},
   {775*OVERSAMPLENR, 90},
   {835*OVERSAMPLENR, 80},
   {884*OVERSAMPLENR, 70},
   {924*OVERSAMPLENR, 60},
   {955*OVERSAMPLENR, 50},
   {977*OVERSAMPLENR, 40},
   {993*OVERSAMPLENR, 30},
   {1004*OVERSAMPLENR, 20},
   {1012*OVERSAMPLENR, 10},
   {1016*OVERSAMPLENR, 0},
};

#endif
#if (THERMISTORHEATER_0 == 3) || (THERMISTORHEATER_1 == 3) || (THERMISTORHEATER_2 == 3) || (THERMISTORBED == 3) //mendel-parts
const short temptable_3[][2] PROGMEM = {
                {1*OVERSAMPLENR,864},
                {21*OVERSAMPLENR,300},
                {25*OVERSAMPLENR,290},
                {29*OVERSAMPLENR,280},
                {33*OVERSAMPLENR,270},
                {39*OVERSAMPLENR,260},
                {46*OVERSAMPLENR,250},
                {54*OVERSAMPLENR,240},
                {64*OVERSAMPLENR,230},
                {75*OVERSAMPLENR,220},
                {90*OVERSAMPLENR,210},
                {107*OVERSAMPLENR,200},
                {128*OVERSAMPLENR,190},
                {154*OVERSAMPLENR,180},
                {184*OVERSAMPLENR,170},
                {221*OVERSAMPLENR,160},
                {265*OVERSAMPLENR,150},
                {316*OVERSAMPLENR,140},
                {375*OVERSAMPLENR,130},
                {441*OVERSAMPLENR,120},
                {513*OVERSAMPLENR,110},
                {588*OVERSAMPLENR,100},
                {734*OVERSAMPLENR,80},
                {856*OVERSAMPLENR,60},
                {938*OVERSAMPLENR,40},
                {986*OVERSAMPLENR,20},
                {1008*OVERSAMPLENR,0},
                {1018*OVERSAMPLENR,-20}
        };

#endif
#if (THERMISTORHEATER_0 == 4) || (THERMISTORHEATER_1 == 4) || (THERMISTORHEATER_2 == 4) || (THERMISTORBED == 4) //10k thermistor
const short temptable_4[][2] PROGMEM = {
   {1*OVERSAMPLENR, 430},
   {54*OVERSAMPLENR, 137},
   {107*OVERSAMPLENR, 107},
   {160*OVERSAMPLENR, 91},
   {213*OVERSAMPLENR, 80},
   {266*OVERSAMPLENR, 71},
   {319*OVERSAMPLENR, 64},
   {372*OVERSAMPLENR, 57},
   {425*OVERSAMPLENR, 51},
   {478*OVERSAMPLENR, 46},
   {531*OVERSAMPLENR, 41},
   {584*OVERSAMPLENR, 35},
   {637*OVERSAMPLENR, 30},
   {690*OVERSAMPLENR, 25},
   {743*OVERSAMPLENR, 20},
   {796*OVERSAMPLENR, 14},
   {849*OVERSAMPLENR, 7},
   {902*OVERSAMPLENR, 0},
   {955*OVERSAMPLENR, -11},
   {1008*OVERSAMPLENR, -35}
};
#endif

#if (THERMISTORHEATER_0 == 5) || (THERMISTORHEATER_1 == 5) || (THERMISTORHEATER_2 == 5) || (THERMISTORBED == 5) //100k ParCan thermistor (104GT-2)
const short temptable_5[][2] PROGMEM = {
// ATC Semitec 104GT-2 (Used in ParCan)
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 4.7kohm pullup, voltage divider math, and manufacturer provided temp/resistance
{(short)(1.06*OVERSAMPLENR), 700},
{(short)(22.19*OVERSAMPLENR), 300},
{(short)(23.66*OVERSAMPLENR), 295},
{(short)(25.25*OVERSAMPLENR), 290},
{(short)(26.98*OVERSAMPLENR), 285},
{(short)(28.85*OVERSAMPLENR), 280},
{(short)(30.89*OVERSAMPLENR), 275},
{(short)(33.11*OVERSAMPLENR), 270},
{(short)(35.54*OVERSAMPLENR), 265},
{(short)(38.18*OVERSAMPLENR), 260},
{(short)(41.06*OVERSAMPLENR), 255},
{(short)(44.22*OVERSAMPLENR), 250},
{(short)(47.67*OVERSAMPLENR), 245},
{(short)(51.45*OVERSAMPLENR), 240},
{(short)(55.6*OVERSAMPLENR), 235},
{(short)(60.15*OVERSAMPLENR), 230},
{(short)(65.16*OVERSAMPLENR), 225},
{(short)(70.66*OVERSAMPLENR), 220},
{(short)(76.72*OVERSAMPLENR), 215},
{(short)(83.39*OVERSAMPLENR), 210},
{(short)(90.75*OVERSAMPLENR), 205},
{(short)(98.86*OVERSAMPLENR), 200},
{(short)(107.8*OVERSAMPLENR), 195},
{(short)(117.67*OVERSAMPLENR), 190},
{(short)(128.55*OVERSAMPLENR), 185},
{(short)(140.56*OVERSAMPLENR), 180},
{(short)(153.79*OVERSAMPLENR), 175},
{(short)(168.36*OVERSAMPLENR), 170},
{(short)(184.4*OVERSAMPLENR), 165},
{(short)(202.01*OVERSAMPLENR), 160},
{(short)(221.32*OVERSAMPLENR), 155},
{(short)(242.43*OVERSAMPLENR), 150},
{(short)(265.45*OVERSAMPLENR), 145},
{(short)(290.45*OVERSAMPLENR), 140},
{(short)(317.48*OVERSAMPLENR), 135},
{(short)(346.56*OVERSAMPLENR), 130},
{(short)(377.66*OVERSAMPLENR), 125},
{(short)(410.69*OVERSAMPLENR), 120},
{(short)(445.51*OVERSAMPLENR), 115},
{(short)(481.9*OVERSAMPLENR), 110},
{(short)(519.56*OVERSAMPLENR), 105},
{(short)(558.15*OVERSAMPLENR), 100},
{(short)(597.23*OVERSAMPLENR), 95},
{(short)(636.36*OVERSAMPLENR), 90},
{(short)(675.02*OVERSAMPLENR), 85},
{(short)(712.73*OVERSAMPLENR), 80},
{(short)(749*OVERSAMPLENR), 75},
{(short)(783.41*OVERSAMPLENR), 70},
{(short)(815.6*OVERSAMPLENR), 65},
{(short)(845.28*OVERSAMPLENR), 60},
{(short)(872.28*OVERSAMPLENR), 55},
{(short)(896.49*OVERSAMPLENR), 50},
{(short)(917.91*OVERSAMPLENR), 45},
{(short)(936.62*OVERSAMPLENR), 40},
{(short)(952.75*OVERSAMPLENR), 35},
{(short)(966.48*OVERSAMPLENR), 30},
{(short)(978.03*OVERSAMPLENR), 25},
{(short)(987.64*OVERSAMPLENR), 20},
{(short)(995.53*OVERSAMPLENR), 15},
{(short)(1001.94*OVERSAMPLENR), 10},
{(short)(1007.09*OVERSAMPLENR), 5},
{(short)(1011.18*OVERSAMPLENR), 0}
};
#endif

#if (THERMISTORHEATER_0 == 6) || (THERMISTORHEATER_1 == 6) || (THERMISTORHEATER_2 == 6) || (THERMISTORBED == 6) // 100k Epcos thermistor
const short temptable_6[][2] PROGMEM = {
   {1*OVERSAMPLENR, 350},
   {28*OVERSAMPLENR, 250}, //top rating 250C
   {31*OVERSAMPLENR, 245},
   {35*OVERSAMPLENR, 240},
   {39*OVERSAMPLENR, 235},
   {42*OVERSAMPLENR, 230},
   {44*OVERSAMPLENR, 225},
   {49*OVERSAMPLENR, 220},
   {53*OVERSAMPLENR, 215},
   {62*OVERSAMPLENR, 210},
   {71*OVERSAMPLENR, 205}, //fitted graphically
   {78*OVERSAMPLENR, 200}, //fitted graphically
   {94*OVERSAMPLENR, 190},
   {102*OVERSAMPLENR, 185},
   {116*OVERSAMPLENR, 170},
   {143*OVERSAMPLENR, 160},
   {183*OVERSAMPLENR, 150},
   {223*OVERSAMPLENR, 140},
   {270*OVERSAMPLENR, 130},
   {318*OVERSAMPLENR, 120},
   {383*OVERSAMPLENR, 110},
   {413*OVERSAMPLENR, 105},
   {439*OVERSAMPLENR, 100},
   {484*OVERSAMPLENR, 95},
   {513*OVERSAMPLENR, 90},
   {607*OVERSAMPLENR, 80},
   {664*OVERSAMPLENR, 70},
   {781*OVERSAMPLENR, 60},
   {810*OVERSAMPLENR, 55},
   {849*OVERSAMPLENR, 50},
   {914*OVERSAMPLENR, 45},
   {914*OVERSAMPLENR, 40},
   {935*OVERSAMPLENR, 35},
   {954*OVERSAMPLENR, 30},
   {970*OVERSAMPLENR, 25},
   {978*OVERSAMPLENR, 22},
   {1008*OVERSAMPLENR, 3},
   {1023*OVERSAMPLENR, 0}  //to allow internal 0 degrees C
};
#endif

#if (THERMISTORHEATER_0 == 7) || (THERMISTORHEATER_1 == 7) || (THERMISTORHEATER_2 == 7) || (THERMISTORBED == 7) // 100k Honeywell 135-104LAG-J01
const short temptable_7[][2] PROGMEM = {
   {1*OVERSAMPLENR, 941},
   {19*OVERSAMPLENR, 362},
   {37*OVERSAMPLENR, 299}, //top rating 300C
   {55*OVERSAMPLENR, 266},
   {73*OVERSAMPLENR, 245},
   {91*OVERSAMPLENR, 229},
   {109*OVERSAMPLENR, 216},
   {127*OVERSAMPLENR, 206},
   {145*OVERSAMPLENR, 197},
   {163*OVERSAMPLENR, 190},
   {181*OVERSAMPLENR, 183},
   {199*OVERSAMPLENR, 177},
   {217*OVERSAMPLENR, 171},
   {235*OVERSAMPLENR, 166},
   {253*OVERSAMPLENR, 162},
   {271*OVERSAMPLENR, 157},
   {289*OVERSAMPLENR, 153},
   {307*OVERSAMPLENR, 149},
   {325*OVERSAMPLENR, 146},
   {343*OVERSAMPLENR, 142},
   {361*OVERSAMPLENR, 139},
   {379*OVERSAMPLENR, 135},
   {397*OVERSAMPLENR, 132},
   {415*OVERSAMPLENR, 129},
   {433*OVERSAMPLENR, 126},
   {451*OVERSAMPLENR, 123},
   {469*OVERSAMPLENR, 121},
   {487*OVERSAMPLENR, 118},
   {505*OVERSAMPLENR, 115},
   {523*OVERSAMPLENR, 112},
   {541*OVERSAMPLENR, 110},
   {559*OVERSAMPLENR, 107},
   {577*OVERSAMPLENR, 105},
   {595*OVERSAMPLENR, 102},
   {613*OVERSAMPLENR, 99},
   {631*OVERSAMPLENR, 97},
   {649*OVERSAMPLENR, 94},
   {667*OVERSAMPLENR, 92},
   {685*OVERSAMPLENR, 89},
   {703*OVERSAMPLENR, 86},
   {721*OVERSAMPLENR, 84},
   {739*OVERSAMPLENR, 81},
   {757*OVERSAMPLENR, 78},
   {775*OVERSAMPLENR, 75},
   {793*OVERSAMPLENR, 72},
   {811*OVERSAMPLENR, 69},
   {829*OVERSAMPLENR, 66},
   {847*OVERSAMPLENR, 62},
   {865*OVERSAMPLENR, 59},
   {883*OVERSAMPLENR, 55},
   {901*OVERSAMPLENR, 51},
   {919*OVERSAMPLENR, 46},
   {937*OVERSAMPLENR, 41},
   {955*OVERSAMPLENR, 35},
   {973*OVERSAMPLENR, 27},
   {991*OVERSAMPLENR, 17},
   {1009*OVERSAMPLENR, 1},
   {1023*OVERSAMPLENR, 0}  //to allow internal 0 degrees C
};
#endif

#if (THERMISTORHEATER_0 == 71) || (THERMISTORHEATER_1 == 71) || (THERMISTORHEATER_2 == 71) || (THERMISTORBED == 71) // 100k Honeywell 135-104LAF-J01
// R0 = 100000 Ohm
// T0 = 25 °C
// Beta = 3974
// R1 = 0 Ohm
// R2 = 4700 Ohm
const short temptable_71[][2] PROGMEM = {
   {35*OVERSAMPLENR, 300},
   {51*OVERSAMPLENR, 270},
   {54*OVERSAMPLENR, 265},
   {58*OVERSAMPLENR, 260},
   {59*OVERSAMPLENR, 258},
   {61*OVERSAMPLENR, 256},
   {63*OVERSAMPLENR, 254},
   {64*OVERSAMPLENR, 252},
   {66*OVERSAMPLENR, 250},
   {67*OVERSAMPLENR, 249},
   {68*OVERSAMPLENR, 248},
   {69*OVERSAMPLENR, 247},
   {70*OVERSAMPLENR, 246},
   {71*OVERSAMPLENR, 245},
   {72*OVERSAMPLENR, 244},
   {73*OVERSAMPLENR, 243},
   {74*OVERSAMPLENR, 242},
   {75*OVERSAMPLENR, 241},
   {76*OVERSAMPLENR, 240},
   {77*OVERSAMPLENR, 239},
   {78*OVERSAMPLENR, 238},
   {79*OVERSAMPLENR, 237},
   {80*OVERSAMPLENR, 236},
   {81*OVERSAMPLENR, 235},
   {82*OVERSAMPLENR, 234},
   {84*OVERSAMPLENR, 233},
   {85*OVERSAMPLENR, 232},
   {86*OVERSAMPLENR, 231},
   {87*OVERSAMPLENR, 230},
   {89*OVERSAMPLENR, 229},
   {90*OVERSAMPLENR, 228},
   {91*OVERSAMPLENR, 227},
   {92*OVERSAMPLENR, 226},
   {94*OVERSAMPLENR, 225},
   {95*OVERSAMPLENR, 224},
   {97*OVERSAMPLENR, 223},
   {98*OVERSAMPLENR, 222},
   {99*OVERSAMPLENR, 221},
   {101*OVERSAMPLENR, 220},
   {102*OVERSAMPLENR, 219},
   {104*OVERSAMPLENR, 218},
   {106*OVERSAMPLENR, 217},
   {107*OVERSAMPLENR, 216},
   {109*OVERSAMPLENR, 215},
   {110*OVERSAMPLENR, 214},
   {112*OVERSAMPLENR, 213},
   {114*OVERSAMPLENR, 212},
   {115*OVERSAMPLENR, 211},
   {117*OVERSAMPLENR, 210},
   {119*OVERSAMPLENR, 209},
   {121*OVERSAMPLENR, 208},
   {123*OVERSAMPLENR, 207},
   {125*OVERSAMPLENR, 206},
   {126*OVERSAMPLENR, 205},
   {128*OVERSAMPLENR, 204},
   {130*OVERSAMPLENR, 203},
   {132*OVERSAMPLENR, 202},
   {134*OVERSAMPLENR, 201},
   {136*OVERSAMPLENR, 200},
   {139*OVERSAMPLENR, 199},
   {141*OVERSAMPLENR, 198},
   {143*OVERSAMPLENR, 197},
   {145*OVERSAMPLENR, 196},
   {147*OVERSAMPLENR, 195},
   {150*OVERSAMPLENR, 194},
   {152*OVERSAMPLENR, 193},
   {154*OVERSAMPLENR, 192},
   {157*OVERSAMPLENR, 191},
   {159*OVERSAMPLENR, 190},
   {162*OVERSAMPLENR, 189},
   {164*OVERSAMPLENR, 188},
   {167*OVERSAMPLENR, 187},
   {170*OVERSAMPLENR, 186},
   {172*OVERSAMPLENR, 185},
   {175*OVERSAMPLENR, 184},
   {178*OVERSAMPLENR, 183},
   {181*OVERSAMPLENR, 182},
   {184*OVERSAMPLENR, 181},
   {187*OVERSAMPLENR, 180},
   {190*OVERSAMPLENR, 179},
   {193*OVERSAMPLENR, 178},
   {196*OVERSAMPLENR, 177},
   {199*OVERSAMPLENR, 176},
   {202*OVERSAMPLENR, 175},
   {205*OVERSAMPLENR, 174},
   {208*OVERSAMPLENR, 173},
   {212*OVERSAMPLENR, 172},
   {215*OVERSAMPLENR, 171},
   {219*OVERSAMPLENR, 170},
   {237*OVERSAMPLENR, 165},
   {256*OVERSAMPLENR, 160},
   {300*OVERSAMPLENR, 150},
   {351*OVERSAMPLENR, 140},
   {470*OVERSAMPLENR, 120},
   {504*OVERSAMPLENR, 115},
   {538*OVERSAMPLENR, 110},
   {552*OVERSAMPLENR, 108},
   {566*OVERSAMPLENR, 106},
   {580*OVERSAMPLENR, 104},
   {594*OVERSAMPLENR, 102},
   {608*OVERSAMPLENR, 100},
   {622*OVERSAMPLENR, 98},
   {636*OVERSAMPLENR, 96},
   {650*OVERSAMPLENR, 94},
   {664*OVERSAMPLENR, 92},
   {678*OVERSAMPLENR, 90},
   {712*OVERSAMPLENR, 85},
   {745*OVERSAMPLENR, 80},
   {758*OVERSAMPLENR, 78},
   {770*OVERSAMPLENR, 76},
   {783*OVERSAMPLENR, 74},
   {795*OVERSAMPLENR, 72},
   {806*OVERSAMPLENR, 70},
   {818*OVERSAMPLENR, 68},
   {829*OVERSAMPLENR, 66},
   {840*OVERSAMPLENR, 64},
   {850*OVERSAMPLENR, 62},
   {860*OVERSAMPLENR, 60},
   {870*OVERSAMPLENR, 58},
   {879*OVERSAMPLENR, 56},
   {888*OVERSAMPLENR, 54},
   {897*OVERSAMPLENR, 52},
   {905*OVERSAMPLENR, 50},
   {924*OVERSAMPLENR, 45},
   {940*OVERSAMPLENR, 40},
   {955*OVERSAMPLENR, 35},
   {967*OVERSAMPLENR, 30},
   {970*OVERSAMPLENR, 29},
   {972*OVERSAMPLENR, 28},
   {974*OVERSAMPLENR, 27},
   {976*OVERSAMPLENR, 26},
   {978*OVERSAMPLENR, 25},
   {980*OVERSAMPLENR, 24},
   {982*OVERSAMPLENR, 23},
   {984*OVERSAMPLENR, 22},
   {985*OVERSAMPLENR, 21},
   {987*OVERSAMPLENR, 20},
   {995*OVERSAMPLENR, 15},
   {1001*OVERSAMPLENR, 10},
   {1006*OVERSAMPLENR, 5},
   {1010*OVERSAMPLENR, 0},
};
#endif

#if (THERMISTORHEATER_0 == 8) || (THERMISTORHEATER_1 == 8) || (THERMISTORHEATER_2 == 8) || (THERMISTORBED == 8)
// 100k 0603 SMD Vishay NTCS0603E3104FXT (4.7k pullup)
const short temptable_8[][2] PROGMEM = {
   {1*OVERSAMPLENR, 704},
   {54*OVERSAMPLENR, 216},
   {107*OVERSAMPLENR, 175},
   {160*OVERSAMPLENR, 152},
   {213*OVERSAMPLENR, 137},
   {266*OVERSAMPLENR, 125},
   {319*OVERSAMPLENR, 115},
   {372*OVERSAMPLENR, 106},
   {425*OVERSAMPLENR, 99},
   {478*OVERSAMPLENR, 91},
   {531*OVERSAMPLENR, 85},
   {584*OVERSAMPLENR, 78},
   {637*OVERSAMPLENR, 71},
   {690*OVERSAMPLENR, 65},
   {743*OVERSAMPLENR, 58},
   {796*OVERSAMPLENR, 50},
   {849*OVERSAMPLENR, 42},
   {902*OVERSAMPLENR, 31},
   {955*OVERSAMPLENR, 17},
   {1008*OVERSAMPLENR, 0}
};
#endif
#if (THERMISTORHEATER_0 == 9) || (THERMISTORHEATER_1 == 9) || (THERMISTORHEATER_2 == 9) || (THERMISTORBED == 9)
// 100k GE Sensing AL03006-58.2K-97-G1 (4.7k pullup)
const short temptable_9[][2] PROGMEM = {
	{1*OVERSAMPLENR, 936},
	{36*OVERSAMPLENR, 300},
	{71*OVERSAMPLENR, 246},
	{106*OVERSAMPLENR, 218},
	{141*OVERSAMPLENR, 199},
	{176*OVERSAMPLENR, 185},
	{211*OVERSAMPLENR, 173},
	{246*OVERSAMPLENR, 163},
	{281*OVERSAMPLENR, 155},
	{316*OVERSAMPLENR, 147},
	{351*OVERSAMPLENR, 140},
	{386*OVERSAMPLENR, 134},
	{421*OVERSAMPLENR, 128},
	{456*OVERSAMPLENR, 122},
	{491*OVERSAMPLENR, 117},
	{526*OVERSAMPLENR, 112},
	{561*OVERSAMPLENR, 107},
	{596*OVERSAMPLENR, 102},
	{631*OVERSAMPLENR, 97},
	{666*OVERSAMPLENR, 92},
	{701*OVERSAMPLENR, 87},
	{736*OVERSAMPLENR, 81},
	{771*OVERSAMPLENR, 76},
	{806*OVERSAMPLENR, 70},
	{841*OVERSAMPLENR, 63},
	{876*OVERSAMPLENR, 56},
	{911*OVERSAMPLENR, 48},
	{946*OVERSAMPLENR, 38},
	{981*OVERSAMPLENR, 23},
	{1005*OVERSAMPLENR, 5},
	{1016*OVERSAMPLENR, 0}
};
#endif
#if (THERMISTORHEATER_0 == 10) || (THERMISTORHEATER_1 == 10) || (THERMISTORHEATER_2 == 10) || (THERMISTORBED == 10)
// 100k RS thermistor 198-961 (4.7k pullup)
const short temptable_10[][2] PROGMEM = {
   {1*OVERSAMPLENR, 929},
   {36*OVERSAMPLENR, 299},
   {71*OVERSAMPLENR, 246},
   {106*OVERSAMPLENR, 217},
   {141*OVERSAMPLENR, 198},
   {176*OVERSAMPLENR, 184},
   {211*OVERSAMPLENR, 173},
   {246*OVERSAMPLENR, 163},
   {281*OVERSAMPLENR, 154},
   {316*OVERSAMPLENR, 147},
   {351*OVERSAMPLENR, 140},
   {386*OVERSAMPLENR, 134},
   {421*OVERSAMPLENR, 128},
   {456*OVERSAMPLENR, 122},
   {491*OVERSAMPLENR, 117},
   {526*OVERSAMPLENR, 112},
   {561*OVERSAMPLENR, 107},
   {596*OVERSAMPLENR, 102},
   {631*OVERSAMPLENR, 97},
   {666*OVERSAMPLENR, 91},
   {701*OVERSAMPLENR, 86},
   {736*OVERSAMPLENR, 81},
   {771*OVERSAMPLENR, 76},
   {806*OVERSAMPLENR, 70},
   {841*OVERSAMPLENR, 63},
   {876*OVERSAMPLENR, 56},
   {911*OVERSAMPLENR, 48},
   {946*OVERSAMPLENR, 38},
   {981*OVERSAMPLENR, 23},
   {1005*OVERSAMPLENR, 5},
   {1016*OVERSAMPLENR, 0}
};
#endif

#if (THERMISTORHEATER_0 == 51) || (THERMISTORHEATER_1 == 51) || (THERMISTORHEATER_2 == 51) || (THERMISTORBED == 51)
// 100k EPCOS (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee.
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: Twice the resolution and better linearity from 150C to 200C
const short temptable_51[][2] PROGMEM = {
   {1*OVERSAMPLENR, 350},
   {190*OVERSAMPLENR, 250}, //top rating 250C
   {203*OVERSAMPLENR, 245},
   {217*OVERSAMPLENR, 240},
   {232*OVERSAMPLENR, 235},
   {248*OVERSAMPLENR, 230},
   {265*OVERSAMPLENR, 225},
   {283*OVERSAMPLENR, 220},
   {302*OVERSAMPLENR, 215},
   {322*OVERSAMPLENR, 210},
   {344*OVERSAMPLENR, 205},
   {366*OVERSAMPLENR, 200},
   {390*OVERSAMPLENR, 195},
   {415*OVERSAMPLENR, 190},
   {440*OVERSAMPLENR, 185},
   {467*OVERSAMPLENR, 180},
   {494*OVERSAMPLENR, 175},
   {522*OVERSAMPLENR, 170},
   {551*OVERSAMPLENR, 165},
   {580*OVERSAMPLENR, 160},
   {609*OVERSAMPLENR, 155},
   {638*OVERSAMPLENR, 150},
   {666*OVERSAMPLENR, 145},
   {695*OVERSAMPLENR, 140},
   {722*OVERSAMPLENR, 135},
   {749*OVERSAMPLENR, 130},
   {775*OVERSAMPLENR, 125},
   {800*OVERSAMPLENR, 120},
   {823*OVERSAMPLENR, 115},
   {845*OVERSAMPLENR, 110},
   {865*OVERSAMPLENR, 105},
   {884*OVERSAMPLENR, 100},
   {901*OVERSAMPLENR, 95},
   {917*OVERSAMPLENR, 90},
   {932*OVERSAMPLENR, 85},
   {944*OVERSAMPLENR, 80},
   {956*OVERSAMPLENR, 75},
   {966*OVERSAMPLENR, 70},
   {975*OVERSAMPLENR, 65},
   {982*OVERSAMPLENR, 60},
   {989*OVERSAMPLENR, 55},
   {995*OVERSAMPLENR, 50},
   {1000*OVERSAMPLENR, 45},
   {1004*OVERSAMPLENR, 40},
   {1007*OVERSAMPLENR, 35},
   {1010*OVERSAMPLENR, 30},
   {1013*OVERSAMPLENR, 25},
   {1015*OVERSAMPLENR, 20},
   {1017*OVERSAMPLENR, 15},
   {1018*OVERSAMPLENR, 10},
   {1019*OVERSAMPLENR, 5},
   {1020*OVERSAMPLENR, 0},
   {1021*OVERSAMPLENR, -5}
};
#endif

#if (THERMISTORHEATER_0 == 52) || (THERMISTORHEATER_1 == 52) || (THERMISTORHEATER_2 == 52) || (THERMISTORBED == 52) 
// 200k ATC Semitec 204GT-2 (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const short temptable_52[][2] PROGMEM = {
   {1*OVERSAMPLENR, 500},
   {125*OVERSAMPLENR, 300}, //top rating 300C
   {142*OVERSAMPLENR, 290},
   {162*OVERSAMPLENR, 280},
   {185*OVERSAMPLENR, 270},
   {211*OVERSAMPLENR, 260},
   {240*OVERSAMPLENR, 250},
   {274*OVERSAMPLENR, 240},
   {312*OVERSAMPLENR, 230},
   {355*OVERSAMPLENR, 220},
   {401*OVERSAMPLENR, 210},
   {452*OVERSAMPLENR, 200},
   {506*OVERSAMPLENR, 190},
   {563*OVERSAMPLENR, 180},
   {620*OVERSAMPLENR, 170},
   {677*OVERSAMPLENR, 160},
   {732*OVERSAMPLENR, 150},
   {783*OVERSAMPLENR, 140},
   {830*OVERSAMPLENR, 130},
   {871*OVERSAMPLENR, 120},
   {906*OVERSAMPLENR, 110},
   {935*OVERSAMPLENR, 100},
   {958*OVERSAMPLENR, 90},
   {976*OVERSAMPLENR, 80},
   {990*OVERSAMPLENR, 70},
   {1000*OVERSAMPLENR, 60},
   {1008*OVERSAMPLENR, 50},
   {1013*OVERSAMPLENR, 40},
   {1017*OVERSAMPLENR, 30},
   {1019*OVERSAMPLENR, 20},
   {1021*OVERSAMPLENR, 10},
   {1022*OVERSAMPLENR, 0}
};
#endif

#if (THERMISTORHEATER_0 == 55) || (THERMISTORHEATER_1 == 55) || (THERMISTORHEATER_2 == 55) || (THERMISTORBED == 55) 
// 100k ATC Semitec 104GT-2 (Used on ParCan) (WITH 1kohm RESISTOR FOR PULLUP, R9 ON SANGUINOLOLU! NOT FOR 4.7kohm PULLUP! THIS IS NOT NORMAL!)
// Verified by linagee. Source: http://shop.arcol.hu/static/datasheets/thermistors.pdf
// Calculated using 1kohm pullup, voltage divider math, and manufacturer provided temp/resistance
// Advantage: More resolution and better linearity from 150C to 200C
const short temptable_55[][2] PROGMEM = {
   {1*OVERSAMPLENR, 500},
   {76*OVERSAMPLENR, 300},
   {87*OVERSAMPLENR, 290},
   {100*OVERSAMPLENR, 280},
   {114*OVERSAMPLENR, 270},
   {131*OVERSAMPLENR, 260},
   {152*OVERSAMPLENR, 250},
   {175*OVERSAMPLENR, 240},
   {202*OVERSAMPLENR, 230},
   {234*OVERSAMPLENR, 220},
   {271*OVERSAMPLENR, 210},
   {312*OVERSAMPLENR, 200},
   {359*OVERSAMPLENR, 190},
   {411*OVERSAMPLENR, 180},
   {467*OVERSAMPLENR, 170},
   {527*OVERSAMPLENR, 160},
   {590*OVERSAMPLENR, 150},
   {652*OVERSAMPLENR, 140},
   {713*OVERSAMPLENR, 130},
   {770*OVERSAMPLENR, 120},
   {822*OVERSAMPLENR, 110},
   {867*OVERSAMPLENR, 100},
   {905*OVERSAMPLENR, 90},
   {936*OVERSAMPLENR, 80},
   {961*OVERSAMPLENR, 70},
   {979*OVERSAMPLENR, 60},
   {993*OVERSAMPLENR, 50},
   {1003*OVERSAMPLENR, 40},
   {1010*OVERSAMPLENR, 30},
   {1015*OVERSAMPLENR, 20},
   {1018*OVERSAMPLENR, 10},
   {1020*OVERSAMPLENR, 0}
};
#endif

#if (THERMISTORHEATER_0 == 60) || (THERMISTORHEATER_1 == 60) || (THERMISTORHEATER_2 == 60) || (THERMISTORBED == 60) // Maker's Tool Works Kapton Bed Thermister
const short temptable_60[][2] PROGMEM = {
   {51*OVERSAMPLENR, 272},
   {61*OVERSAMPLENR, 258},
   {71*OVERSAMPLENR, 247},
   {81*OVERSAMPLENR, 237},
   {91*OVERSAMPLENR, 229},
   {101*OVERSAMPLENR, 221},
   {131*OVERSAMPLENR, 204},
   {161*OVERSAMPLENR, 190},
   {191*OVERSAMPLENR, 179},
   {231*OVERSAMPLENR, 167},
   {271*OVERSAMPLENR, 157},
   {311*OVERSAMPLENR, 148},
   {351*OVERSAMPLENR, 140},
   {381*OVERSAMPLENR, 135},
   {411*OVERSAMPLENR, 130},
   {441*OVERSAMPLENR, 125},
   {451*OVERSAMPLENR, 123},
   {461*OVERSAMPLENR, 122},
   {471*OVERSAMPLENR, 120},
   {481*OVERSAMPLENR, 119},
   {491*OVERSAMPLENR, 117},
   {501*OVERSAMPLENR, 116},
   {511*OVERSAMPLENR, 114},
   {521*OVERSAMPLENR, 113},
   {531*OVERSAMPLENR, 111},
   {541*OVERSAMPLENR, 110},
   {551*OVERSAMPLENR, 108},
   {561*OVERSAMPLENR, 107},
   {571*OVERSAMPLENR, 105},
   {581*OVERSAMPLENR, 104},
   {591*OVERSAMPLENR, 102},
   {601*OVERSAMPLENR, 101},
   {611*OVERSAMPLENR, 100},
   {621*OVERSAMPLENR, 98},
   {631*OVERSAMPLENR, 97},
   {641*OVERSAMPLENR, 95},
   {651*OVERSAMPLENR, 94},
   {661*OVERSAMPLENR, 92},
   {671*OVERSAMPLENR, 91},
   {681*OVERSAMPLENR, 90},
   {691*OVERSAMPLENR, 88},
   {701*OVERSAMPLENR, 87},
   {711*OVERSAMPLENR, 85},
   {721*OVERSAMPLENR, 84},
   {731*OVERSAMPLENR, 82},
   {741*OVERSAMPLENR, 81},
   {751*OVERSAMPLENR, 79},
   {761*OVERSAMPLENR, 77},
   {771*OVERSAMPLENR, 76},
   {781*OVERSAMPLENR, 74},
   {791*OVERSAMPLENR, 72},
   {801*OVERSAMPLENR, 71},
   {811*OVERSAMPLENR, 69},
   {821*OVERSAMPLENR, 67},
   {831*OVERSAMPLENR, 65},
   {841*OVERSAMPLENR, 63},
   {851*OVERSAMPLENR, 62},
   {861*OVERSAMPLENR, 60},
   {871*OVERSAMPLENR, 57},
   {881*OVERSAMPLENR, 55},
   {891*OVERSAMPLENR, 53},
   {901*OVERSAMPLENR, 51},
   {911*OVERSAMPLENR, 48},
   {921*OVERSAMPLENR, 45},
   {931*OVERSAMPLENR, 42},
   {941*OVERSAMPLENR, 39},
   {951*OVERSAMPLENR, 36},
   {961*OVERSAMPLENR, 32},
   {981*OVERSAMPLENR, 23},
   {991*OVERSAMPLENR, 17},
   {1001*OVERSAMPLENR, 9},
   {1008*OVERSAMPLENR, 0},
};
#endif
#if (THERMISTORHEATER_0 == 11) || (THERMISTORHEATER_1 == 11) || (THERMISTORHEATER_2 == 11) || (THERMISTORBED == 11)
// 100k  MF58 // UTILIZADo por el Argento //Ultima actualizacion 24-feb-2014
const short temptable_11[][2] PROGMEM = {
    {     380,       300     }, // r=   112 adc=23.74
    {     409,       295     }, // r=   120 adc=25.56
    {     441,       290     }, // r=   130 adc=27.56
    {     476,       285     }, // r=   141 adc=29.73
    {     514,       280     }, // r=   152 adc=32.11
    {     556,       275     }, // r=   165 adc=34.72
    {     601,       270     }, // r=   179 adc=37.57
    {     651,       265     }, // r=   195 adc=40.70
    {     706,       260     }, // r=   212 adc=44.13
    {     766,       255     }, // r=   231 adc=47.90
    {     833,       250     }, // r=   252 adc=52.04
    {     905,       245     }, // r=   275 adc=56.59
    {     985,       240     }, // r=   301 adc=61.59
    {    1073,       235     }, // r=   330 adc=67.09
    {    1170,       230     }, // r=   362 adc=73.15
    {    1277,       225     }, // r=   398 adc=79.82
    {    1395,       220     }, // r=   438 adc=87.17
    {    1524,       215     }, // r=   483 adc=95.25
    {    1667,       210     }, // r=   533 adc=104.16
    {    1823,       205     }, // r=   589 adc=113.96
    {    1996,       200     }, // r=   653 adc=124.74
    {    2185,       195     }, // r=   724 adc=136.59
    {    2394,       190     }, // r=   805 adc=149.61
    {    2622,       185     }, // r=   897 adc=163.88
    {    2872,       180     }, // r=  1000 adc=179.50
    {    3145,       175     }, // r=  1118 adc=196.58
    {    3443,       170     }, // r=  1252 adc=215.18
    {    3767,       165     }, // r=  1405 adc=235.41
    {    4117,       160     }, // r=  1580 adc=257.32
    {    4495,       155     }, // r=  1780 adc=280.96
    {    4902,       150     }, // r=  2009 adc=306.36
    {    5336,       145     }, // r=  2273 adc=333.51
    {    5798,       140     }, // r=  2578 adc=362.36
    {    6286,       135     }, // r=  2930 adc=392.85
    {    6797,       130     }, // r=  3338 adc=424.83
    {    7330,       125     }, // r=  3812 adc=458.12
    {    7880,       120     }, // r=  4363 adc=492.50
    {    8443,       115     }, // r=  5007 adc=527.70
    {    9014,       110     }, // r=  5761 adc=563.39
    {    9588,       105     }, // r=  6646 adc=599.24
    {   10158,       100     }, // r=  7688 adc=634.87
    {   10719,        95     }, // r=  8917 adc=669.91
    {   11264,        90     }, // r= 10373 adc=704.01
    {   11789,        85     }, // r= 12102 adc=736.84
    {   12289,        80     }, // r= 14162 adc=768.09
    {   12760,        75     }, // r= 16623 adc=797.51
    {   13199,        70     }, // r= 19575 adc=824.93
    {   13603,        65     }, // r= 23127 adc=850.21
    {   13973,        60     }, // r= 27415 adc=873.28
    {   14306,        55     }, // r= 32611 adc=894.14
    {   14605,        50     }, // r= 38931 adc=912.80
    {   14870,        45     }, // r= 46648 adc=929.36
    {   15103,        40     }, // r= 56107 adc=943.93
    {   15306,        35     }, // r= 67749 adc=956.64
    {   15482,        30     }, // r= 82138 adc=967.63
    {   15633,        25     }, // r=100000 adc=977.08
    {   15762,        20     }, // r=122272 adc=985.13
    {   15871,        15     }, // r=150172 adc=991.95
    {   15963,        10     }, // r=185290 adc=997.69
    {   16040,         5     }, // r=229714 adc=1002.49
    {   16104,         0     }, // r=286200 adc=1006.47
};
#endif

#define _TT_NAME(_N) temptable_ ## _N
#define TT_NAME(_N) _TT_NAME(_N)

#ifdef THERMISTORHEATER_0
# define HEATER_0_TEMPTABLE TT_NAME(THERMISTORHEATER_0)
# define HEATER_0_TEMPTABLE_LEN (sizeof(HEATER_0_TEMPTABLE)/sizeof(*HEATER_0_TEMPTABLE))
#else
# ifdef HEATER_0_USES_THERMISTOR
#  error No heater 0 thermistor table specified
# else  // HEATER_0_USES_THERMISTOR
#  define HEATER_0_TEMPTABLE NULL
#  define HEATER_0_TEMPTABLE_LEN 0
# endif // HEATER_0_USES_THERMISTOR
#endif

//Set the high and low raw values for the heater, this indicates which raw value is a high or low temperature
#ifndef HEATER_0_RAW_HI_TEMP
# ifdef HEATER_0_USES_THERMISTOR   //In case of a thermistor the highest temperature results in the lowest ADC value
#  define HEATER_0_RAW_HI_TEMP 0
#  define HEATER_0_RAW_LO_TEMP 16383
# else                          //In case of an thermocouple the highest temperature results in the highest ADC value
#  define HEATER_0_RAW_HI_TEMP 16383
#  define HEATER_0_RAW_LO_TEMP 0
# endif
#endif

#ifdef THERMISTORHEATER_1
# define HEATER_1_TEMPTABLE TT_NAME(THERMISTORHEATER_1)
# define HEATER_1_TEMPTABLE_LEN (sizeof(HEATER_1_TEMPTABLE)/sizeof(*HEATER_1_TEMPTABLE))
#else
# ifdef HEATER_1_USES_THERMISTOR
#  error No heater 1 thermistor table specified
# else  // HEATER_1_USES_THERMISTOR
#  define HEATER_1_TEMPTABLE NULL
#  define HEATER_1_TEMPTABLE_LEN 0
# endif // HEATER_1_USES_THERMISTOR
#endif

//Set the high and low raw values for the heater, this indicates which raw value is a high or low temperature
#ifndef HEATER_1_RAW_HI_TEMP
# ifdef HEATER_1_USES_THERMISTOR   //In case of a thermistor the highest temperature results in the lowest ADC value
#  define HEATER_1_RAW_HI_TEMP 0
#  define HEATER_1_RAW_LO_TEMP 16383
# else                          //In case of an thermocouple the highest temperature results in the highest ADC value
#  define HEATER_1_RAW_HI_TEMP 16383
#  define HEATER_1_RAW_LO_TEMP 0
# endif
#endif

#ifdef THERMISTORHEATER_2
# define HEATER_2_TEMPTABLE TT_NAME(THERMISTORHEATER_2)
# define HEATER_2_TEMPTABLE_LEN (sizeof(HEATER_2_TEMPTABLE)/sizeof(*HEATER_2_TEMPTABLE))
#else
# ifdef HEATER_2_USES_THERMISTOR
#  error No heater 2 thermistor table specified
# else  // HEATER_2_USES_THERMISTOR
#  define HEATER_2_TEMPTABLE NULL
#  define HEATER_2_TEMPTABLE_LEN 0
# endif // HEATER_2_USES_THERMISTOR
#endif

//Set the high and low raw values for the heater, this indicates which raw value is a high or low temperature
#ifndef HEATER_2_RAW_HI_TEMP
# ifdef HEATER_2_USES_THERMISTOR   //In case of a thermistor the highest temperature results in the lowest ADC value
#  define HEATER_2_RAW_HI_TEMP 0
#  define HEATER_2_RAW_LO_TEMP 16383
# else                          //In case of an thermocouple the highest temperature results in the highest ADC value
#  define HEATER_2_RAW_HI_TEMP 16383
#  define HEATER_2_RAW_LO_TEMP 0
# endif
#endif

#ifdef THERMISTORBED
# define BEDTEMPTABLE TT_NAME(THERMISTORBED)
# define BEDTEMPTABLE_LEN (sizeof(BEDTEMPTABLE)/sizeof(*BEDTEMPTABLE))
#else
# ifdef BED_USES_THERMISTOR
#  error No bed thermistor table specified
# endif // BED_USES_THERMISTOR
#endif

//Set the high and low raw values for the heater, this indicates which raw value is a high or low temperature
#ifndef HEATER_BED_RAW_HI_TEMP
# ifdef BED_USES_THERMISTOR   //In case of a thermistor the highest temperature results in the lowest ADC value
#  define HEATER_BED_RAW_HI_TEMP 0
#  define HEATER_BED_RAW_LO_TEMP 16383
# else                          //In case of an thermocouple the highest temperature results in the highest ADC value
#  define HEATER_BED_RAW_HI_TEMP 16383
#  define HEATER_BED_RAW_LO_TEMP 0
# endif
#endif

#endif //THERMISTORTABLES_H_
