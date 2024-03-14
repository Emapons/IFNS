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
  //........
  //........
  //........


  /*
   * Part 2 
   * Fit
   * Nel caso in cui potesse essere utile per un'altra analisi dati (in questo caso naturalmente non ha senso)
  */
  /* 
  //Create Canvas 
  TCanvas *myCanvas = new TCanvas("myCanvas","Fit ...", 1080,720);
  //myCanvas -> SetGrid();
  
  //Create TGraphErrors
  TGraphErrors *g1=new TGraphErrors(count, mu, sg, smu, ssg);
  g1->SetLineColor(kBlue);

  //create user defined function for fitting
  TF1 *f1 = new TF1("f1", "gaus", 0, 3);  //extremes?
  g1 -> Fit(f1);
  g1 -> Draw("ALP"); 
  
  TLegend *leg = new TLegend(0.11,0.75,0.5,0.89);
  leg -> AddEntry(f1, "Fit function:", "L");
  leg -> SetBorderSize(1);
  leg -> Draw();
  */
}
