#include <iostream>
#include <fstream>
#include <math.h>   
#include <memory>
#include <string>
#include <sstream>
#include <algorithm>

#include "TH1F.h"
#include "TF1.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFile.h"

using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::ostringstream; 


//debugging 
//gdb --args root.exe -l -b -q yourRootMacro.C
//and then "run"

Double_t fpoisson(Double_t *x,Double_t *par) {
  return par[0]*TMath::Poisson(x[0],par[1]);
}

void dcr() {
  //  > Files named: 
  //      dcr0.txt
  //      dcr2.txt
  //      dcr3.txt
  //      ...
  //  > With the following structure:
  // (float) (int)
  //      1   4
  //      2   5
  //      3   6
  //      ..  ..
  
  //Number of file
  int count=2;
  
  //Loading file in a ifstream array
  //may occor "segmentation fault" if the file are not found
  ifstream myFile[count];
  for (int i=0; i<count; i++) {
    ostringstream filename;
    filename<<"dcr"<<i<<".txt";
    myFile[i].open(filename.str().c_str()); //.c_str() for old library string
  }
  
  //count number of bin for each set
  unsigned int nBins[count]; //Bins Numeration (need static??)
  for (int i=0; i<count; i++) {  //for each file count the line non empty
    string line;
    //while (getline(myFile[i], line)) nBins[i]++;
    nBins[i]=std::count(std::istreambuf_iterator<char>(myFile[i]), std::istreambuf_iterator<char>(), '\n');
    //this two line is needed to restart on the top of the file
    myFile[i].clear( );
    myFile[i].seekg(0, std::ios::beg );
  }
  
  //Creation of data array (this method may waste some space, but it's simple)
  //Found max bin size 
  int max = nBins[0]; 
  for (int i = 1; i < count; i++) if (nBins[i] > max) max = nBins[i];
  //Creation data array
  float x[count][max];
  int y[count][max];
  //Fill data array
  int n=0;
  for (int i=0; i<count; i++) {
    n=0; 
    cout<<"Set "<<i<<", con nBins "<<nBins[i]<<" :"<<endl;
    if (myFile[i].is_open()) {
      while ( myFile[i].good() ) {
        myFile[i] >> x[i][n] >> y[i][n];   
        cout<<"\t"<<" "<<x[i][n]<<" "<<y[i][n]<<endl;
        n++;   
        if (n == nBins[i]) break;
      }
      myFile[i].close();
    }
  }
  
  double mu[count], sg[count], smu[count], ssg[count];

  //Histogram creating and fitting
  for (int i=0; i<count; i++) {
    //Histogram creation
    TH1F *myHisto = new TH1F("myHisto","myHisto",nBins[i],x[i][0],x[i][nBins[i]-1]);  
    
    //Histogram filling
    for (int j=0;j<nBins[i];j++) {
      myHisto->SetBinContent(j+1,y[i][j]);
    }
    
    // come si accorpano gli eventi in bin adiacenti
    //myHisto->Rebin(4);
    // ora myHisto ha nBins/4 classi e ognuna contiene gli eventi di 4 adiacenti

    //Regression  
    TF1* f1=new TF1("f1", "gaus", 0., 10.);  //userdefined function     estremi?
    myHisto->Fit(f1, "NR+");    //"N" is fornot drawingthe fit
    //Get parameter
    mu[i]=f1->GetParameter(1);
    smu[i]=f1->GetParError(1);
    sg[i]=f1->GetParameter(2);
    ssg[i]=f1->GetParError(2);
    cout<<endl<<"Media: "<<mu[i]<<"+-"<<smu[i]<<"\tSigma: "<<sg[i]<<"+-"<<ssg[i]<<endl;
    
    //free heap memory
    delete myHisto;
    
    //Draw histogram
    //myHisto->Draw();
  }
  
}

void EsempioFitMultipliSuStessoGrafico() {

  TH1F* myh = new TH1F("myh","myh",100,0.,10.);
  Double_t par[6];

  // riempi con 2 distr Gaussiane - 10000 eventi ognuna
  for (unsigned int ientry = 0; ientry < 10000; ientry++) {
    myh->Fill(gRandom->Gaus(2.5,1.0));
    myh->Fill(gRandom->Gaus(7.5,1.5));
  }

  TF1 *g1    = new TF1("g1","gaus",0.,4.8);
  g1->SetParameters(1000.,3.,1.);
  TF1 *g2    = new TF1("g2","gaus",4.8,12.);
  g2->SetParameters(1000.,8.,1.);
  TF1 *total = new TF1("total","gaus(0)+gaus(3)",0.,12.);
  total->SetLineColor(3);

  // FIT CON GAUSSIANE SINGOLE IN RANGE DIVERSI
  myh->Fit(g1,"R");
  myh->Fit(g2,"R+");

  // FIT CON SOMMA DI GAUSSIANE 
  g1->GetParameters(&par[0]);
  g2->GetParameters(&par[3]);
  total->SetParameters(par);
  myh->Fit(total,"R+");

}
