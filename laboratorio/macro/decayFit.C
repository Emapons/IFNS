/*
 * Simulation of the measurement of the decay constant (lambda) 
 * of a short-lived isotope, obtained by fitting the observed number 
 * of decays with an exponential.
 * Repeated experiments show the distribution of the measured values 
 * of lambda and of the chi2 of the fit.
 * 
 * Run in ROOT with: 
 * .x decayFit.C
 *
 * N. Amapane, 2018
 */


#include <TRandom3.h>
#include <TH1F.h>
#include <TF1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <iostream>
#include <string>


using namespace std;

TRandom3 rndGen;


// Simulate the number of decays of of a source of nParents nuclei with 
// decay constant lambda, in nIntervals consecutive time intervals of
// duration deltat. The simulation is performed in time iterations of 
// duration dt, which should be chosen so that lambda*dt << 1.
TH1F* decayExperiment(int nParents, float lambda, float dt, float deltat, int nIntervals) {
  TH1F* result = new TH1F("decay","Decay experiment",nIntervals,0,nIntervals*deltat);
  result->SetMarkerStyle(21);
  result->SetMarkerSize(0.7);
  result->GetXaxis()->SetTitle("t (s)");
  result->GetYaxis()->SetTitle("Counts");

  // probability for a given nucleus to decay in the interval dt
  float prob = lambda*dt;   

  // Time iterations
  for (float t=0; t<nIntervals*deltat; t+=dt) {
    int nDecayed = 0;
    // loop over parent nuclei
    for (int j=0; j<nParents; ++j) {
      if (rndGen.Rndm()<prob) { // the j-th parent decayed
	result->Fill(t);
	++nDecayed;        
      }
    }    
    nParents -= nDecayed; // surviving parents for the next iteration
  }

  return result;
}


// Run the simulation of an experiment to measure labmda, N times. 
void decayFit(int N=200) { 

  gStyle->SetOptStat(10);
  gStyle->SetOptFit(111);

  TCanvas* c1 = new TCanvas("c1","Single Experiment",650,500);
  TCanvas* c2 = new TCanvas("c2", "All Experiments", 650,650);
  c2->Divide(1,2);

  c1->cd(3);
  TH1F* hLambda = new TH1F("hLambda","Distribution of measured #lambda in all experiments",50,0.03,0.07);
  TH1F* hChi2 = new TH1F("hChi2","Distribution of #chi^{2} in all experiments",30,0,30);
  hLambda->SetMarkerStyle(21);
  hLambda->SetMarkerSize(0.7);
  hLambda->GetXaxis()->SetTitle("#lambda (s^{-1})");
  hLambda->GetYaxis()->SetTitle("Experiments");
  hChi2->SetMarkerStyle(21);
  hChi2->SetMarkerSize(0.7);
  hChi2->GetXaxis()->SetTitle("#chi^{2}");
  hChi2->GetYaxis()->SetTitle("Experiments");

  
  TF1* f = new TF1("decay","[0]*exp(-[1]*x)",0,20);
  f->SetParName(0,"Constant");
  f->SetParName(1,"Lambda");

  TH1F* h = 0;
  bool pause = true;
  
  string input; 
  for (int i=0; i<N; ++i) {    
    c1->cd();
    delete h; // Delete the histogram from the previous iteration (if any)
    h = decayExperiment(10000,0.05, 0.02, 2, 10);
    f->SetParameters(10000,0.1); // set physical initial values
    h->Fit(f);
    h->SetMaximum(1050); // Fix the Y scale to be the same for all experiments
    h->SetMinimum(300);
    h->Draw("e1");
    c1->Modified();
    c1->Update();

    c2->cd(1);
    hLambda->Fill(f->GetParameter("Lambda"));
    hChi2->Fill(f->GetChisquare());
    hLambda->Draw("e1");
    c2->cd(2);
    hChi2->Draw("e1");
    c2->Modified();
    c2->Update();

    if (pause) {
      cout << endl << "Press c to loop on all experiments; r to restart; q to exit; any other key for next experiment" << endl;
      getline (cin, input);
      if (input=="q") {
	break;
      } else if (input=="c") {
	pause = false;
      } else if (input=="r") { // Restart
	hLambda->Reset();
	hChi2->Reset();
	i = -1;
      }
    }
  }

  // Pause before showing the fit of lambda and the theoretical chi2 
  cout << endl << "Press any key to continue" << endl;
  getline (cin, input);

  // Fit the distribution of Lambda with a Gaussian
  c2->cd(1);
  hLambda->Fit("gaus");
  hLambda->Draw("e1");
  TPaveStats *ps = (TPaveStats*)hLambda->GetListOfFunctions()->FindObject("stats");
  ps->SetX1NDC(0.61);
  ps->SetY1NDC(0.53);
  ps->SetY2NDC(0.9);


  c2->cd(2);
  hChi2->SetStats(false);
  hChi2->Draw("e1");

  // Show the theoretical chi2 distribution on top of the 
  // observed chi2 distribition
  TF1* chi2Distr =  new TF1("Chi2","[0]*ROOT::Math::chisquared_pdf(x,8)",0,30);
  chi2Distr->SetParameter(0,hChi2->Integral());
  chi2Distr->SetLineColor(kBlue);
  chi2Distr->Draw("same");

  TLegend* legend = new TLegend(0.49,0.71,0.89,0.85);
  legend->AddEntry(hChi2,"Experiments","p");
  legend->AddEntry(chi2Distr,"Theoretical #chi^{2} distribition for k=8","l");
  legend->Draw();
  
}
