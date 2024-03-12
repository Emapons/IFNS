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

void fitCs137(string input = "cs137-gruppob11.txt") 
{
 
  ifstream parInput(input.c_str());

  // in ADC counts
  float xMin = 525.; float xMax = 30525.;  float binWidth = 50.;

  TCanvas c1("c1","c1",20,20,800,600);
  float x;  float y; string parName; 
  TH1F* theHisto = new TH1F("theHisto", "Electron energy", (int)((xMax-xMin)/binWidth), xMin, xMax);
  //theHisto->Sumw2();
  
  if (parInput.is_open()) {
    while ( parInput.good() ) {
      parInput >> x >> y;   
      cout << x << " " << y << endl;
      int theBin = theHisto->FindBin(x);
      theHisto->SetBinContent(theBin,float(y));
      // theHisto->SetBinError(theBin,sqrt(float(y)));
    }
    parInput.close();
  }

  // rebinning  
  theHisto->Rebin(2);    // constant

  // fit
  TF1* ffit = new TF1("ffit","gaus(0)+pol2(3)",0.,50000.);
  ffit->SetParLimits(0,0.,10000.);
  ffit->SetParLimits(1,11000.,13000.);
  ffit->SetParLimits(2,1000.,4000.);
  theHisto->Draw("e1");
  theHisto->Fit("ffit","","e1",8500.,16000.);

  //draw components
  TF1* gauscomp = (TF1*)ffit->Clone();
  gauscomp->SetRange(8500.,16000.);
  gauscomp->SetLineColor(kBlue);   gauscomp->SetLineStyle(kDashed);
  gauscomp->SetParameter(3,0.);
  gauscomp->SetParameter(4,0.);
  gauscomp->SetParameter(5,0.);
  TF1* parabcomp = (TF1*)ffit->Clone();
  parabcomp->SetRange(8500.,16000.);
  parabcomp->SetLineColor(kCyan);   parabcomp->SetLineStyle(kDashed);
  parabcomp->SetParameter(0,0.);
  gauscomp->Draw("same");    parabcomp->Draw("same");
  
  c1.SaveAs("fitCs137.gif");
}
