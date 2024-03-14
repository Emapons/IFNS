/* 
 * Examples of fits of distributions including several contributions
 * 
 * Run in ROOT with:
 *  .L fitExamples.C
 *  fitExample1();
 *  fitExample2();
 *
 * N. Amapane, 2018
 */

#include <TF1.h>
#include <TH1F.h>
#include <TStyle.h>
#include <iostream>

using namespace std;


void fitExamples() {
  cout << "\nThis macro shows some fit examples. Run in ROOT with:" << endl;
  cout << ".L fitExamples.C" << endl;
  cout << "fitExample1();" << endl;
  cout << "fitExample2();" << endl << endl;
}


// First example: a Gaussian signal over an exponential background.
void fitExample1() {
  gStyle->SetOptStat(10);
  gStyle->SetOptFit(111);


  // Generate a random distribution of a Gaussian signal on top of an 
  // exponential background
  TH1F* hDistr = new TH1F("hDistr","Gaussian over background",50,0,100);
  {
    TF1* genSig = new TF1("genSig","gaus",0,100);
    genSig->SetParameters(1,50,5);
    hDistr->FillRandom("genSig",1500);
    TF1* genBkg = new TF1("genBkg","expo",0,100);
    genBkg->SetParameters(1,-0.03);
    hDistr->FillRandom("genBkg",8500);
    delete genSig;
    delete genBkg;
  }



  hDistr->SetMarkerStyle(21);
  hDistr->SetMarkerSize(0.7);
  hDistr->GetXaxis()->SetTitle("E [A.U.]");
  hDistr->GetYaxis()->SetTitle("Counts");
  hDistr->Draw("e1");
 
  // Fit with the sum of a Gaussian and an exponential
  TF1* f = new TF1("func","[0]*exp(-[1]*x) + gaus(2)",0,100);
  f->SetParNames("Norm_exp","slope","Norm","#mu","#sigma");
  // Set mameningful initial parameters
  f->SetParameter (0,500);
  f->SetParameter (1,0.1);
  f->SetParameter (2,200);
  f->SetParameter (3,50);
  f->SetParameter (4,3);

  hDistr->Fit(f);

  // Draw signal and background separately, using the parameters from the fit
  TF1* fGaus = new TF1("Signal", "gaus", 0,100);
  for (int j=0; j<3; ++j) {
    fGaus->SetParameter(j,f->GetParameter(2+j));
  }
  fGaus->SetLineColor(kBlue); 
  fGaus->SetLineWidth(2);
  fGaus->Draw("same");

  TF1* fBkg = new TF1("Background", "[0]*exp(-[1]*x)", 0,100);
  fBkg->SetParameter(0,f->GetParameter(0));
  fBkg->SetParameter(1,f->GetParameter(1));
  fBkg->SetLineColor(kMagenta);
  fBkg->SetLineWidth(2);
  fBkg->Draw("same");   
  f->Draw("same");

}


// Second example: fitting a spectrum with multiple Gaussian peaks
void fitExample2() {
  gStyle->SetOptStat(10);
  gStyle->SetOptFit(111);


  // Generate a random distribution with three different Gaussian contributions
  TH1F* hDistr = new TH1F("hDistr","Sum of three Gaussians",50,0,100);
  double n[] = {7500, 1850, 650};
  {
    TF1* genSig1 = new TF1("genSig1","gaus",0,100);
    genSig1->SetParameters(1,58,5);
    hDistr->FillRandom("genSig1",n[0]);
    TF1* genSig2 = new TF1("genSig2","gaus",0,100);
    genSig2->SetParameters(1,42,4);
    hDistr->FillRandom("genSig2",n[1]);
    TF1* genSig3 = new TF1("genSig3","gaus",0,100);
    genSig3->SetParameters(1,28,4);
    hDistr->FillRandom("genSig3",n[2]);
    delete genSig1;
    delete genSig2;
    delete genSig3;
  }

 
  hDistr->SetMarkerStyle(21);
  hDistr->SetMarkerSize(0.7);
  hDistr->GetXaxis()->SetTitle("E [A.U.]");
  hDistr->GetYaxis()->SetTitle("Counts");
  hDistr->Draw("e1");


  // Fit with the sum of three Gaussians
  TF1* f = new TF1("func","gaus(0) + gaus(3) + gaus(6)",0,100);
  f->SetParNames("Norm_{1}","#mu_{1}","#sigma_{1}","Norm_{2}","#mu_{2}","#sigma_{2}","Norm_{3}","#mu_{3}","#sigma_{3}");
  // Set mameningful initial parameters
  f->SetParameter (0,1000);
  f->SetParameter (1,60);
  f->SetParameter (2,3);
  f->SetParameter (3,400);
  f->SetParameter (4,40);
  f->SetParameter (5,3);
  f->SetParameter (6,400);
  f->SetParameter (7,25);
  f->SetParameter (8,3);
  hDistr->Fit(f);

  // Draw individual peaks separately, using the parameters from the fit
  TF1* g[3];
  g[0] = new TF1("g0", "gaus", 0,100);
  g[0]->SetLineColor(kMagenta);
  g[1] = new TF1("g1", "gaus", 0,100);
  g[1]->SetLineColor(kBlue);
  g[2] = new TF1("g2", "gaus", 0,100);
  g[2]->SetLineColor(kGreen);
  double g_int[3];
  double int_total=0.;
  for (int i=0; i<3; ++i) {
    for (int j=0; j<3; ++j) {
      g[i]->SetParameter(j,f->GetParameter(3*i+j));
    }
    g_int[i] = g[i]->Integral(0,100);
    int_total += g_int[i];
    g[i]->SetLineWidth(2);
    g[i]->Draw("same");   
  }
  f->Draw("same");

  // Print the observed BRs 
  cout << endl << "BRs:" << endl;
  cout << "        Measured     | True" << endl;
  for (int i=0; i<3; ++i) {
    double fr = g_int[i]/int_total;
    cout << std::setprecision(3) << std::fixed;
    cout << "Peak " << i << ": "  << fr << "+-" << sqrt(fr*(1-fr)/int_total) 
	 << " | "  << n[i]/(n[0]+n[1]+n[2]) << endl;
  }
    cout << endl;
}
