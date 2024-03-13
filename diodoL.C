#include <iostream>           // ci serve per stampare a video il testo

#include <TGraphErrors.h>

// ci serve per istanziare grafici
#include <TAxis.h>            // ci serve per manipolare gli assi dei grafici
#include <TCanvas.h>          // ci serve per disegnare i grafici
#include <TF1.h>              // ci serve per scrivere le funzioni con cui fittare i grafici
#include <iomanip>            // ci serve per manipolare l'output a video 
#include <TMath.h>
using namespace std; 

void diodoL(){
  // numero misure prese
  const int nmisure = 16; 
  
  float V_d[]  = 
{1.012,1.512,1.770,1.818,1.842,1.860,1.911,1.924,1.936,1.946,1.956,1.964,1.973,1.989,2.004,2.017};
  
  float I_d[]  =
{0.00012,0.00074,0.1198,0.3394,0.5716,0.8073,1.980,2.433,2.897,3.353,3.817,4.272,4.737,5.659,6.591,7.516};
  // ---------------------- Quantità derivate ----------------------- //

  float sV_d[nmisure], sI_d[nmisure];

  //Errori
  for(int j=0; j<nmisure;++j){
  	sV_d[j]=0.01*V_d[j]+0.005;
  	sI_d[j]=0.05*I_d[j]+0.01;
  	
  	// Stampa a video dei valori. \t inserisce un tab nel print out. Mettendo \n si va a capo invece
    cout << "Measurement number " << j << ": \t V_d = (" << V_d[j] << " +- " << sV_d[j] << ") V, \t I_d = (" << I_d[j] << " +- " << sI_d[j] << ") V." << endl;    
  }
  // ----------------------------------------------------------------- //


  // --------------------- Grafico P(R) ------------------------------ //
  TCanvas *cIV = new TCanvas("cIV","V(I)",200,10,600,400);
  cIV->SetFillColor(0);
  //cGf->SetLogx(1);
  cIV->cd();
  TGraphErrors *gIV = new TGraphErrors(nmisure,I_d,V_d,sI_d,sV_d); //TGraphErrors(nmisure,f,G,sf,sG);
  //gIV->GetXaxis()->SetRangeUser(-0.5,9);
  gIV->SetMarkerSize(0.6);
  gIV->SetMarkerStyle(21);
  gIV->SetTitle("V_{d}(I_d)");
  gIV->GetXaxis()->SetTitle("I_d [mA]");
  gIV->GetYaxis()->SetTitle("V_{d} [V]");
  gIV->Draw("AP");
  
  auto legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->SetHeader("Legenda Fit:","C");
   
   
  // ----------------------------------------------------------------- //

  int m=0, M=8; //min e max

  cout << "\n\n --- Relazione tra corrente e tensione di un diodo LED ---" <<endl;

  cout << "\n\n --- 1) Ipotesi  [0]*log(1+x/[1])+[2]*x --- \n" <<endl;      //log si scrive così??
  TF1 *funz1 = new TF1("funz1","[0]*log(1+x/[1])+[2]*x",m,M);
  
  funz1->SetParameter(0,0.04);
  funz1->SetParameter(1,pow(10,-18));
  funz1->SetParameter(2,-0.007);
  
  //funz1->SetParLimits(0,-1,0);
  //funz1->SetParLimits(1,-2,0);
  //funz1->SetParLimits(2,-1,0);
  //funz1->SetParameter(,);
  
  funz1->SetLineColor(kBlue); 
  
  gIV->Fit(funz1,"RM+");
  legend->AddEntry(funz1,"[0]*log(1+x/[1])+[2]*x","l");
   
  cout << "Chi^2:" << funz1->GetChisquare() << ", number of DoF: " << funz1->GetNDF() << " (Probability: " << funz1->GetProb() << ")." << endl;
  cout << "--------------------------------------------------------------------------------------------------------" << endl;
  legend->Draw();
  
}
