/* Example of fitting the 241Am spectrum peaks with asymmetric functions
 * to model the tail due to energy loss in the source and in the dead 
 * layer of the detector. 
 * The fits can be used to obtain the number of events belonging to each peak.
 *
 * Run in ROOT with:
 * .x fitAm.C
 *
 */

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TAxis.h>
#include <TStyle.h>
#include <TVirtualPad.h>

using namespace std;


//Read spectrum data from MCA file 
TGraphErrors* fill(string filename) {
  ifstream file(filename.c_str());
  if (!file) {
    cerr << filename << "Not found" <<endl;
    return nullptr;
  }
  vector<double> x, y, sy;
  string line;
  int channel=-1;
  int count;
  while (getline(file, line)) {
    stringstream linestr;
    linestr << line;
    if (linestr >> count) {
      ++channel;
      x.push_back(channel);
      y.push_back(count);
      sy.push_back(sqrt(count));
    }
  }
  return new TGraphErrors(x.size(),x.data(),y.data(),0,sy.data());
}

void fitAm() {
  //Fit range: exclude additional tails which are not described by 
  //the functional form used in the fit.
  double fitmin = 1280;
  double fitmax = 1445;

  TGraphErrors* spectrum = fill("Am247.mca");
  
  //The crystalball function is a Gaussian with an exponential tail.
  //cf. https://en.wikipedia.org/wiki/Crystal_Ball_function
  //The parameters of ROOT::Math::crystalball_function are, in this order: alpha, N, sigma, mu.
  //Here we assume that the width (sigma) and the tail  parameters (alpha, N) are identical for all peaks.
  TF1* f = new TF1("am241",
		   "[p0]*ROOT::Math::crystalball_function(x,[p1],[p2],[p3],[p4])+"
		   "[p5]*ROOT::Math::crystalball_function(x,[p1],[p2],[p3],[p6])+"
		   "[p7]*ROOT::Math::crystalball_function(x,[p1],[p2],[p3],[p8])", 0, 1500);

  //Initialize parameters to meaningful valures
  //Main peak
  f->SetParName(0,"Norm_1");
  f->SetParameter(0,5e4); 
  f->SetParName(1,"Alpha");
  f->SetParameter(1,1.);
  f->SetParName(2,"N");
  f->SetParameter(2,2.);
  f->SetParName(3,"sigma");
  f->SetParameter(3,5.);
  f->SetParName(4,"Mean_1");
  f->SetParameter(4,1430.);

  // Second peak
  f->SetParName(5,"Norm_2");
  f->SetParameter(5,1e4);
  f->SetParName(6,"Mean_2");
  f->SetParameter(6,1385.);

  // Third peak
  f->SetParName(7,"Norm_3");
  f->SetParameter(7,1e3);
  f->SetParName(8,"Mean_3");
  f->SetParameter(8,1330.);

  spectrum->Fit(f, "","", fitmin, fitmax);

  spectrum->SetTitle("^{241}Am;Channel;Counts");
  spectrum->GetXaxis()->SetRangeUser(1280,1480);
  spectrum->Draw("AP");
  gPad->SetLogy();

  //Draw the three peaks separately and compute their integrals
  TF1* g[3];
  double* p = f->GetParameters();
  g[0] = new TF1("Peak0","[p0]*ROOT::Math::crystalball_function(x,[p1],[p2],[p3],[p4])", 0, 1500);
  g[0] ->SetParameters(p[0],p[1],p[2],p[3],p[4]);
  g[0]->SetLineColor(kMagenta);
  g[1] = new TF1("Peak1","[p0]*ROOT::Math::crystalball_function(x,[p1],[p2],[p3],[p4])", 0, 1500);
  g[1] ->SetParameters(p[5],p[1],p[2],p[3],p[6]);
  g[1]->SetLineColor(kBlue);
  g[2] = new TF1("Peak2","[p0]*ROOT::Math::crystalball_function(x,[p1],[p2],[p3],[p4])", 0, 1500);
  g[2] ->SetParameters(p[7],p[1],p[2],p[3],p[8]);
  g[2]->SetLineColor(kGreen);

  double gInt[3];
  for (int i=0; i<3; ++i) {
    g[i]->SetLineWidth(2);
    g[i]->Draw("same");
    gInt[i] = g[i]->Integral(0, 1500);
  }

  //We assume that the uncertainty on the integrals is domincated by the uncertainty on the 
  //normalization parameter
  double gIntErr[3] = {gInt[0]*f->GetParError(0)/f->GetParameter(0), 
		       gInt[1]*f->GetParError(5)/f->GetParameter(5), 
		       gInt[2]*f->GetParError(7)/f->GetParameter(7)};

  
  //Redraw the sum of the three peaks on top
  f->SetLineColor(kRed);
  f->SetLineWidth(3);
  f->Draw("same");

  // Print integrals of the three peaks 
  cout << endl << "Integrals:" << endl;
  for (int i=0; i<3; ++i) {
    cout << std::setprecision(0) << std::fixed;
    cout << "Peak " << i << ": "  << gInt[i] << "+-" << gIntErr[i] << endl;
  }
  cout << endl;

}
