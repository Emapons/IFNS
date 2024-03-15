/*  Autore: Davide Rinarelli
 *  Esperienza: Raggi cosmici
 *  Descrizione:
 *  Note:
 *  > File named (se è mca cambiarlo nel programma): 
 *      rc.txt
 *  > With the following structure:
 *    (int)
 *      4
 *      5
 *      6
 *      ..
 *  > Always qit root before reuse the macro
*/

#include <iostream>
#include <fstream>
#include <math.h>   
#include <string>
#include <sstream>
#include <algorithm> // for std::count() function
#include <vector>

//some library are not userfull
#include "TH1F.h"
#include "TF1.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TGraphPainter.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"
#include "TStyle.h"

using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ostringstream; 

//Debugging with dbg in linux (better then Root)
//gdb --args root.exe -l -b -q yourRootMacro.C
//and then "run"

void spettroRC(){
  //tempo di misura "gate"
  float tau=0.5;
 
  //creation ifstream pointer in heap memory
  ifstream myFile;
    
  //opening file
  //if the file is not found may accur "segmentation violation"
  myFile.open("rc.txt"); //.c_str() for old library string
  
  //count number of bin for each set
  int nBins = 0;  //resetting nBins
  nBins=std::count(std::istreambuf_iterator<char>(myFile), std::istreambuf_iterator<char>(), '\n');
  //go on the start of the file
  myFile.clear( );
  myFile.seekg(0, std::ios::beg );
  
  //creation data array (pointer) (in heap memory)
  float x[nBins];
  
  int n=0;
  if (myFile.is_open()) {
    while ( myFile.good() ) {
      myFile >> x[n];   
      //cout<<"\t"<<" "<<x[n]<<" "<<y[n]<<endl; //decomment this line if you want to see the data
      n++;   
      if (n == nBins) break;
    }
    myFile.close();
  }
  
  //Histogram Creation
  TH1F *myHisto = new TH1F("myHisto", "myHisto", nBins, 0, nBins);   
  //Histogram filling
  for (int j=0;j<nBins;j++) {
    myHisto->SetBinContent(j+1,x[j]);
  }
  
  //possible merger
  //myHisto->Rebin(400);  //dalla scheda
  //ora myHisto ha nBins/4 classi e ognuna contiene gli eventi di 4 adiacenti
  
  //Regression with userdefined function 
  //for more see: "https://root.cern.ch/root/htmldoc/guides/users-guide/FittingHistograms.html#example-of-fit-combining-functions"  -  section 7.4
  TF1* f1 = new TF1("f1", "[0]*TMath::Poisson(x,[1])", 0, nBins);  //poisson function
                                                      //change extremes in case of rebinning
  f1 -> SetParameter(0, 2000);
  f1 -> SetParameter(1, 5);
  myHisto->Fit(f1, "R+");    //"N" is for not drawing the fit
  //Get parameter
  float mu=f1->GetParameter(0);
  float smu=f1->GetParError(0);
  float sg=f1->GetParameter(1);
  float ssg=f1->GetParError(1);
  cout<<endl<<"Media: "<<mu<<"+-"<<smu<<"\tSigma: "<<sg<<"+-"<<ssg<<endl;
  
}
