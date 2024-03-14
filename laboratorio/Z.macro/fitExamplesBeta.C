#include <iostream>
#include <fstream>
#include <math.h>   
#include <string>

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

Double_t fpoisson(Double_t *x,Double_t *par) {
  return par[0]*TMath::Poisson(x[0],par[1]);
}

void EsempioLetturaFileADueColonne(string input = "test.txt") {
  // file di test contiene:
  //    > more test.txt
  // 1    4
  // 2    5
  // 3    6
  static const int nBins = 4;
  float x[nBins];  int y[nBins]; 
  ifstream parInput(input.c_str());
  int i = 0;

  if (parInput.is_open()) {
    while ( parInput.good() ) {
      parInput >> x[i] >> y[i];   
      cout << i << " " << x[i] << " " << y[i] << endl;
      i++;   if (i == nBins) break;
    }
    parInput.close();
  }

  // come si riempie un TH1
  TH1F *myHisto = new TH1F("myHisto","myHisto",nBins,x[0],x[nBins-1]);  
  for (int j=0;j<nBins;j++) {
    myHisto->SetBinContent(j+1,y[j]);
  }

  // come si accorpano gli eventi in bin adiacenti
  //myHisto->Rebin(4);
  // ora myHisto ha nBins/4 classi e ognuna contiene gli eventi di 4 adiacenti
  myHisto->Draw();
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

