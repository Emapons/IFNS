#include <iostream>
#include <fstream>
#include <math.h>   

#include "TH1F.h"
#include "TF1.h"
#include "TLine.h"
#include "TRandom3.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TFile.h"

Double_t EfficiencyFalling(Double_t *x,Double_t *par) {
  return par[0]/(1+exp((x[0] - par[1])/par[2]));
}

Double_t EfficiencyRising(Double_t *x,Double_t *par) {
  return par[0]/(1+exp((par[1] - x[0])/par[2]));
}

void EsempioUtilizzoFunzioneEsterna() {
  static const int nPunti = 6;
  float x   [nPunti] = {100.,200.,300.,400.,500.,600.};
  float xerr[nPunti] = {10.,10.,10.,10.,10.,10.};
  float y   [nPunti] = {0.02,0.05,0.3,0.7,0.94,0.97};
  float yerr[nPunti] = {0.005,0.01,0.01,0.01,0.01,0.01};

  TGraphErrors* effGr = new TGraphErrors(nPunti,x,y,xerr,yerr);
  TF1* effF = new TF1("effF",EfficiencyRising,0.,700.,3);
  effF->SetParameters(0.95,350.,150.);
  effF->SetParNames ("Eff_max","V_HM","V_TO");
  effF->SetParLimits(0,0.8,1.);
  effF->SetParLimits(2,0.01,1000.);

  effGr->Fit("effF","","",50.,650.);
  effGr->DrawClone("AP");
 }

void EsempioLetturaFileAUnaColonna(string input = "test.txt") {
  
  // file di test contiene:
  //    > more test.txt
  // 4
  // 5
  // 6
  static const int nBins = 200;
  int y[nBins]; 
  ifstream parInput(input.c_str());
  int i = 0;

  if (parInput.is_open()) {
    while ( parInput.good() ) {
      parInput >> y[i];   
      cout << i << " " << y[i] << endl;
      i++;   if (i == nBins) break;
    }
    parInput.close();
  }

  // come si riempie un TH1
  TH1F *myHisto = new TH1F("myHisto","myHisto",nBins,1,nBins);  
  for (int j=0;j<nBins;j++) {
    myHisto->SetBinContent(j+1,y[j]);
  }

  // come si accorpano gli eventi in bin adiacenti
  myHisto->Rebin(4);
  // ora myHisto ha nBins/4 classi e ognuna contiene gli eventi di 4 adiacenti
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

