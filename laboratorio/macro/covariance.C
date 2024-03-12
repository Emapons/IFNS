/* 
 * Example of case where correlation in the parameters of a fit is not negligible.
 * In this case, the full covariance matrix has to be taken into account when computing 
 * uncertainties, including the non-diagonal element (covariance).
 * Covariance can be neglected in this case if the fit is performed in a translated r.f.,
 * so that no extrapolation is involved in the determinattion of the constant term.
 * 
 * Run in ROOT 6 with: 
 * .x covariance.C
 *
 */

#include <TGraph.h>

double sqr(double x) {return x*x;}


void covariance() {

  double vx[] = {41., 42., 43., 44., 45., 46., 47., 48., 49., 50.};
  double vy[] = {33.9, 34.8, 34.7, 35.6, 36.4, 37.7, 36.8, 38.9, 38.6, 40.2};
  double sy[] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5};

  TGraphErrors* graph = new TGraphErrors(10,vx,vy,0,sy);
  graph->SetMarkerStyle(4);
  graph->SetTitle("Effect of correlation on fit parameters;x;y");
  graph->Draw("AP");


  cout << "--- Fit with y=ax+b" << endl << endl;
  TF1* myFunc = new TF1("myFunc","[1]*x+[0]");
  TFitResultPtr r = graph->Fit(myFunc,"ES");

  // Retrieve parameters and their errors
  double a = myFunc->GetParameter(1);
  double sa = myFunc->GetParError(1);
  double b = myFunc->GetParameter(0);
  double sb = myFunc->GetParError(0);
  
  // Get full covariance matrix
  TMatrixDSym cov = r->GetCovarianceMatrix();

  cout << endl;
  cout << "Covariance: " << cov[0][1] << endl; // this is rho*sigma_a*sigma_b
  cout << "Correlation coefficient: " << cov[0][1]/sqrt(cov[0][0]*cov[1][1]) << endl;

  // Compute value and error at x=45.5
  double x=45.5;
  cout << "Value at x=45.5 : " << myFunc->Eval(x) << endl;
  cout << "Uncertainty neglecting correlation: " << sqrt(x*x*sa*sa + sb*sb) << endl; // This is clearly overestimated
  cout << "Uncertainty including correlation: " << sqrt(x*x*sa*sa + sb*sb + 2*x*cov[0][1]) << endl << endl;

  // Now do the same fit in a translated r.f.
  cout << "--- Fit with y=a(x-45.5)+b" << endl << endl;
  TF1* myFunc_t = new TF1("myFunc_t","[1]*(x-45.5)+[0]");
  TFitResultPtr r_t = graph->Fit(myFunc_t,"ES");
  a = myFunc_t->GetParameter(1);
  sa = myFunc_t->GetParError(1);
  b = myFunc_t->GetParameter(0);
  sb = myFunc_t->GetParError(0);
  cov = r_t->GetCovarianceMatrix();

  cout << "Covariance: " << cov[0][1] << endl; // this is rho*sigma_a*sigma_b
  cout << "Correlation coefficient: " << cov[0][1]/sqrt(cov[0][0]*cov[1][1]) << endl;
  
  cout << "Value at x=45.5 : " << myFunc->Eval(x) << endl;
  cout << "Uncertainty neglecting correlation: " << sqrt(sqr(x-45.5)*sqr(sa) + sqr(sb)) << endl; 
  // Note that the effect of correlation is exactly zero at the point we have chosen as the new origin,
  // but it is negligible also for x around that point (in the range covered by data).
  cout << "Uncertainty including correlation: " << sqrt(sqr(x-45.5)*sqr(sa) + sqr(sb) + 2*(x-45.5)*cov[0][1]) << endl << endl;
  

}
