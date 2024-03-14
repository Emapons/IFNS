/*
 * An (over)simplified model to illustrate the effect of detector 
 * resolution on NaI spectra.
 * 
 * Run in ROOT with: 
 * .x NaIresolution.C
 *
 * N. Amapane, 2018
 */

#include <TH1F.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TStyle.h>
#include <TLegend.h>


// Compton formula. Energies in MeV; theta in radians.
double compton(double E, double theta) {
  return E/(1+(1-cos(theta))*E/0.511); 
}


void NaIresolution(){
  // Gamma energy
  double E = 0.662; 

  // Resolution; we assume 10% regardless of energy, for simplicity.
  // In reality, resolution improves with increasing energy.
  double reso = 0.1;

  TH1F *htrue = new TH1F("htrue","True distribution",250,0,1);
  TH1F *hmeas = new TH1F("hmeas","Measured distribution",250,0,1);

  TRandom3 rnd;
  Double_t px, py, pz;
  

  // Events in the photopeak
  for (Int_t i = 0; i < 500000; i++) {
    double Etrue = 0.661;
    double Emeas = Etrue+rnd.Gaus(0,Etrue*reso/2.355);
    htrue->Fill(Etrue);
    hmeas->Fill(Emeas);
  }

  // Events in the Compton continuum: for simplicity, we assume
  // a flat distribution between 0 and the Compton edge.
  for (Int_t i = 0; i < 1000000; i++) {
    double comptonEdge = E-compton(E,TMath::Pi());
    double Etrue = rnd.Rndm(1)*comptonEdge;
    double Emeas = Etrue+rnd.Gaus(0,Etrue*reso/2.355);
    htrue->Fill(Etrue);
    hmeas->Fill(Emeas);
  }


// Backscatter events: for simplicity, we assume detected backscatter 
// occurs for a flat distribution of theta between 120 and 180 degrees.
  for (Int_t i = 0; i < 50000; i++) { 
    double theta = (120.+60.*rnd.Rndm(1))*TMath::Pi()/180.; 
    double Etrue = compton(E, theta); 
    double Emeas = Etrue+rnd.Gaus(0,Etrue*reso/2.355); 
    
    htrue->Fill(Etrue); 
    hmeas->Fill(Emeas); 
  } 

  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);

  htrue->GetXaxis()->SetTitle("E (MeV)");
  htrue->GetYaxis()->SetTitle("Counts / 0.004 MeV");
  htrue->SetMaximum(50e3);
  htrue->Draw();
  hmeas->SetLineColor(2);
  hmeas->Draw("same");

  TLegend* legend = new TLegend(0.17,0.74,0.57,0.87);
  legend->AddEntry(htrue,"True distribution","l");
  legend->AddEntry(hmeas,"Measured distribution","l");
  legend->Draw();
}
