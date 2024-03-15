/*  Autore: Davide Rinarelli
 *  Esperienza: Optical Cross Talk
 *  Descrizione:
 *  Note:
 *  > Files named: 
 *      oct0.txt
 *      oct1.txt
 *      oct2.txt
 *      ...
 *  > With the following structure:
 *  (float) (int)
 *      1   4
 *      2   5
 *      3   6
 *     ..   ..
 *  > Always qit root before reuse the macro
*/

#include <iostream>
#include <fstream>
#include <math.h>   
#include <string>
#include <sstream>
#include <algorithm> // for std::count() function

//some library are not userfull
#include "TH1F.h"
#include "TF1.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TGraphErrors.h"
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

//Definition of the Posisson function
Double_t fpoisson(Double_t *x,Double_t *par) {
  return par[0]*TMath::Poisson(x[0],par[1]);
}

void oct() {
  //Number of file
  int count=2; //set with (number of file - 1)
  //tempo di misura "gate"
  float tau=0.5;

  unsigned int nBins = 0; //nBins for each set
  unsigned int n=0; //for reading file
  double mu[count], sg[count], smu[count], ssg[count]; //variable for the last fit
  
  /*
   * Part 1 
   * For Cycle
   * new version
  */
  for (int i=0; i<count; i++) {
    
    //creation ifstream pointer in heap memory
    ifstream *myFile = new ifstream;
    
    //opening file
    //if the file is not found may accur "segmentation violation"
    ostringstream filename;
    filename<<"oct"<<i<<".txt";
    myFile->open(filename.str().c_str()); //.c_str() for old library string
    
    //count number of bin for each set
    nBins = 0;  //resetting nBins
    nBins=std::count(std::istreambuf_iterator<char>(*myFile), std::istreambuf_iterator<char>(), '\n');
    //go on the start of the file
    myFile->clear( );
    myFile->seekg(0, std::ios::beg );
    
    //creation data array (pointer) (in heap memory)
    float *x = new float[nBins];
    int *y = new int[nBins];
    
    cout<<"Set "<<i+1<<", con nBins "<<nBins<<" :"<<endl;
    n=0;
    if (myFile->is_open()) {
      while ( myFile->good() ) {
        *myFile >> x[n] >> y[n];   
        //cout<<"\t"<<" "<<x[n]<<" "<<y[n]<<endl; //decomment this line if you want to see the data
        n++;   
        if (n == nBins) break;
      }
      myFile->close();
    }
    
    //Histogram Creation
    TH1F *myHisto = new TH1F("myHisto", "myHisto", nBins, x[0], x[nBins-1]);   
    //Histogram filling
    for (int j=0;j<nBins;j++) {
      myHisto->SetBinContent(j+1,y[j]);
    }
    
    //possible merger
    //myHisto->Rebin(4);  //create a new array before for cycle
    //ora myHisto ha nBins/4 classi e ognuna contiene gli eventi di 4 adiacenti
    
    //Regression with userdefined function 
    //for more see: "https://root.cern.ch/root/htmldoc/guides/users-guide/FittingHistograms.html#example-of-fit-combining-functions"  -  section 7.4
    TF1* f1 = new TF1("f1", "gaus", x[0], x[nBins-1]);  //for Posison function sobstitute "gauss" with fpoisson
                                                        //change extremes in case of rebinning
    myHisto->Fit(f1, "NR+");    //"N" is fornot drawing the fit
    //Get parameter
    mu[i]=f1->GetParameter(1);
    smu[i]=f1->GetParError(1);
    sg[i]=f1->GetParameter(2);
    ssg[i]=f1->GetParError(2);
    cout<<endl<<"Media: "<<mu[i]<<"+-"<<smu[i]<<"\tSigma: "<<sg[i]<<"+-"<<ssg[i]<<endl;
    
    //free heap memory
    delete myFile;
    delete [] x;
    delete [] y;
    delete myHisto;
    delete f1;
  
  }//end for cycle
 
  /*
   * Part 2
  */
  //now we have all the mean and the std deviation in mu[], smu[], sg[], ssg[]
  cout<<endl<<"****************************************"<<endl<<"Final Result:"<<endl;
  //rate calc
  float r[count];
  float sr[count];
  //Per "count" misure
  float ssr[count];

  for (int i =0; i<count; i++) {
    r[i] = mu[i] / tau;                 //calcolo rate come scritto su schede
    sr[i] = std::sqrt(mu[i]) / tau;     //calcolo incertezza rate scritto su schede
    ssr[i] = sr[i] / std::sqrt(count);  //calcolo incertezza definito sulla slide 31 (non chiaro il significato) 
    cout<<"Set "<<i<<"\tr = ("<<r[i]<<"+-"<<sr[i]<<")\t"<<"??? incertezza su ssr=sqrt(sr)/count (per tante misure): ssr = "<<ssr[i]<<endl; 
  }
  cout<<"<r> = ("<<"??"<<"+-"<<"??"<<")\t???"<<endl;
   
  /*
   * Part 3
   * Fitting mean in function of the temperature
  */
  cout<<endl<<"****************************************"<<endl<<"Final Result:"<<endl;
  //import file temperature
  ifstream *myFile = new ifstream;
  myFile->open("octTmp.txt");//, std::ifstream::in);
  
  //reading file
  float x[count];
  n=0;
  if (myFile->is_open()) {
    while ( myFile->good() ) {
      *myFile >> x[n];   
      //cout<<"\t"<<" "<<x[n]<<" "<<y[n]<<endl; //decomment this line if you want to see the data
      n++;  
      if (n == count) break;
    }
    myFile->close();
  }
  
  //create error array
  float sx[count];
  for (int i=0; i<count; i++) {
    sx[i] = 1;                    //modify error
  }

  //Canvas creation 
  TCanvas *c1 = new TCanvas("c1","Canvas",200,10,700,500);
  gStyle -> SetOptFit(1111);  //(#include "TStyle.h")
  //c1->SetFillColor(42);
  c1 -> SetGrid();

  //Plotting points
  TGraphErrors *gr = new TGraphErrors(count, x, r, sx, sr); // | numero di punti | x | y | sx | sy |
  gr -> SetTitle("r(T)");
  gr -> SetMarkerColor(4);
  gr -> SetMarkerStyle(21);
  gr -> GetXaxis() -> SetTitle("T [°C]");
  gr -> GetYaxis() -> SetTitle("r [conteggi/ms]");
  
  //userdefined function for fitting
  TF1 *f1 = new TF1("f1", "[0]+x*[1]", x[0], x[count-1]); //modify the fitting function
  gr -> Fit(f1);
  gr -> Draw("ALP");
  
  //Legend drawing
  TLegend *leg = new TLegend(0.11,0.75,0.5,0.89);
  leg -> SetTextSize(0.03);
  leg -> SetHeader("Legend:");
  leg -> AddEntry(f1, "Fit function: [0] + x*[1]");
  //leg -> SetBorderSize(1);
  leg -> Draw();

}
