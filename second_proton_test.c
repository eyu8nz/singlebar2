#include <vector>
using std::vector;

int second_proton_test() { 

  gStyle->SetOptStat(0);

  int fileType = 1;
 
  // This adds each ROOT file to a chain of trees.
  TChain *chain = new TChain("tree");
  chain->Add("first_40k_proton_120.root");
  chain->Add("second_40k_proton_test.root"); // Accidentally misnamed this one...
  chain->Add("third_40k_proton_120.root");
  chain->Add("fourth_40k_proton_120.root");
  chain->Add("fifth_40k_proton_120.root");

  float depositedEnergyECAL_r; // Deposited energy of proton beam
  float depositedIonEnergyECAL_r; // Deposited energy of ions from proton beam. This one is VERY similar to detectedEnergyECAL_r
  vector<float> *inputInitialPosition = new vector<float>(3, 0.); // Positions of protons from beam  on xy-plane
  int SDCdetected_r_S; // Number of scintillation photons on SDC
  int SDCdetected_r_C; // Number of Cherenkov photons on SDC
  int SDSdetected_r_S; // Number of scintillation photons on SDS
  int SDSdetected_r_C; // Number of Cherenkov photons on SDS

  // Setting branch addresses of each variable. These are used for the histograms.
  chain->SetBranchAddress("depositedEnergyECAL_r", &depositedEnergyECAL_r);
  chain->SetBranchAddress("depositedIonEnergyECAL_r", &depositedIonEnergyECAL_r);
  chain->SetBranchAddress("inputInitialPosition", &inputInitialPosition);
  chain->SetBranchAddress("SDCdetected_r_S", &SDCdetected_r_S);
  chain->SetBranchAddress("SDCdetected_r_C", &SDCdetected_r_C);
  chain->SetBranchAddress("SDSdetected_r_S", &SDSdetected_r_S);
  chain->SetBranchAddress("SDSdetected_r_C", &SDSdetected_r_C);
 
  // This gets the number of entries (in this case, events).
  int entries = chain->GetEntries();

  // These are the TProfiles from proton_test.c. I need them here so I can make new histograms.
  TProfile2D *avg_detected_photons_S_SDC = new TProfile2D("avg_detected_photons_S_SDC", "Average Number of Detected Scintillation Photons in SDC", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_S_SDC->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_S_SDC->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_detected_photons_C_SDC = new TProfile2D("avg_detected_photons_C_SDC", "Average Number of Detected Cherenkov Photons in SDC", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_C_SDC->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_C_SDC->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_detected_photons_S_SDS = new TProfile2D("avg_detected_photons_S_SDS", "Average Number of Detected Scintillation Photons in SDS", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_S_SDS->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_S_SDS->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_detected_photons_C_SDS = new TProfile2D("avg_detected_photons_C_SDS", "Average Number of Detected Cherenkov Photons in SDS", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_C_SDS->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_C_SDS->GetYaxis()->SetTitle("y-Position (mm)");
  avg_detected_photons_C_SDS->SetMaximum(77);

  TProfile2D *avg_S_SDC_shower = new TProfile2D("avg_S_SDC_shower", "Shower Detection of S in SDC", 100, -15, 15, 100, -15, 15);
  avg_S_SDC_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_S_SDC_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_C_SDC_shower = new TProfile2D("avg_C_SDC_shower", "Shower Detection of C in SDC", 100, -15, 15, 100, -15, 15);
  avg_C_SDC_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_C_SDC_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_S_SDS_shower = new TProfile2D("avg_S_SDS_shower", "Shower Detection of S in SDS", 100, -15, 15, 100, -15, 15);
  avg_S_SDS_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_S_SDS_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_C_SDS_shower = new TProfile2D("avg_C_SDS_shower", "Shower Detection of C in SDS", 100, -15, 15, 100, -15, 15);
  avg_C_SDS_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_C_SDS_shower->GetYaxis()->SetTitle("y-Position (mm)");

  // This for loop fills the histograms.
  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    if (depositedEnergyECAL_r >= 0.15) {
      avg_S_SDC_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_S);
      avg_C_SDC_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_C);
      avg_S_SDS_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDSdetected_r_S);
      avg_C_SDS_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDSdetected_r_C);
    }    
    if (depositedEnergyECAL_r < 0.15) {
      avg_detected_photons_S_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_S);
      avg_detected_photons_C_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_C);
      avg_detected_photons_S_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDSdetected_r_S);
      avg_detected_photons_C_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDSdetected_r_C);
    }
  }  

  // These are new histograms that create the cuts we need to analyze the data.
  TH1D *x_cuts_S_SDC = new TH1D("x_cuts_S_SDC", "Cuts of x-Positions for S Photons in SDC", 100, -14.9, 15.0);
  x_cuts_S_SDC->GetXaxis()->SetTitle("x-Position");
  x_cuts_S_SDC->GetYaxis()->SetTitle("Count");

  TH1D *x_cuts_C_SDC = new TH1D("x_cuts_C_SDC", "Cuts of x-Positions for C Photons in SDC", 100, -14.9, 15.0);
  x_cuts_C_SDC->GetXaxis()->SetTitle("x-Position");
  x_cuts_C_SDC->GetYaxis()->SetTitle("Count");

  TH1D *x_cuts_S_SDS = new TH1D("x_cuts_S_SDS", "Cuts of x-Positions for S Photons in SDS", 100, -14.9, 15.0);
  x_cuts_S_SDS->GetXaxis()->SetTitle("x-Position");
  x_cuts_S_SDS->GetYaxis()->SetTitle("Count");

  TH1D *x_cuts_C_SDS = new TH1D("x_cuts_c_SDS", "Cuts of x-Positions for C Photons in SDS", 100, -14.9, 15.0);
  x_cuts_C_SDS->GetXaxis()->SetTitle("x-Position");
  x_cuts_C_SDS->GetYaxis()->SetTitle("Count");

  TH1D *y_cuts_S_SDC = new TH1D("y_cuts_S_SDC", "Cuts of y-Positions for S Photons in SDC", 100, -14.9, 15.0);
  y_cuts_S_SDC->GetXaxis()->SetTitle("y-Position");
  y_cuts_S_SDC->GetYaxis()->SetTitle("Count");

  TH1D *y_cuts_C_SDC = new TH1D("y_cuts_C_SDC", "Cuts of y-Positions for C Photons in SDC", 100, -14.9, 15.0);
  y_cuts_C_SDC->GetXaxis()->SetTitle("y-Position");
  y_cuts_C_SDC->GetYaxis()->SetTitle("Count");

  TH1D *y_cuts_S_SDS = new TH1D("y_cuts_S_SDS", "Cuts of y-Positions for S Photons in SDS", 100, -14.9, 15.0);
  y_cuts_S_SDS->GetXaxis()->SetTitle("y-Position");
  y_cuts_S_SDS->GetYaxis()->SetTitle("Count");

  TH1D *y_cuts_C_SDS = new TH1D("y_cuts_c_SDS", "Cuts of y-Positions for C Photons in SDS", 100, -14.9, 15.0);
  y_cuts_C_SDS->GetXaxis()->SetTitle("y-Position");
  y_cuts_C_SDS->GetYaxis()->SetTitle("Count");
  y_cuts_C_SDS->SetMaximum(1350);

  TH1D *x_shower_S_SDC = new TH1D("x_shower_S_SDC", "Average Amplitude of Pulses of S Photons in SDC for Energy >= 15 GeV", 100, -14.9, 15.0);
  x_shower_S_SDC->GetXaxis()->SetTitle("x-Position");
  x_shower_S_SDC->GetYaxis()->SetTitle("Count");
 
  TH1D *x_shower_C_SDC = new TH1D("x_shower_C_SDC", "Average Amplitude of Pulses of C Photons in SDC for Energy >= 15 GeV", 100, -14.9, 15.0);
  x_shower_C_SDC->GetXaxis()->SetTitle("x-Position");
  x_shower_C_SDC->GetYaxis()->SetTitle("Count");
  x_shower_C_SDC->SetMaximum(150000);

  TH1D *x_shower_S_SDS = new TH1D("x_shower_S_SDS", "Average Amplitude of Pulses of S Photons in SDS for Energy >= 15 GeV", 100, -14.9, 15.0);
  x_shower_S_SDS->GetXaxis()->SetTitle("x-Position");
  x_shower_S_SDS->GetYaxis()->SetTitle("Count");

  TH1D *x_shower_C_SDS = new TH1D("x_shower_C_SDS", "Average Amplitude of Pulses of C Photons in SDS for Energy >= 15 GeV", 100, -14.9, 15.0);
  x_shower_C_SDS->GetXaxis()->SetTitle("x-Position");
  x_shower_C_SDS->GetYaxis()->SetTitle("Count");

  TH1D *y_shower_S_SDC = new TH1D("y_shower_S_SDC", "Average Amplitude of Pulses of S Photons in SDC for Energy >= 15 GeV", 100, -14.9,15.0);
  y_shower_S_SDC->GetXaxis()->SetTitle("y-Position");
  y_shower_S_SDC->GetYaxis()->SetTitle("Count");

  TH1D *y_shower_C_SDC = new TH1D("y_shower_C_SDC", "Average Amplitude of Pulses of C Photons in SDC for Energy >= 15 GeV", 100, -14.9, 15.0);
  y_shower_C_SDC->GetXaxis()->SetTitle("y-Position");
  y_shower_C_SDC->GetYaxis()->SetTitle("Count");
  y_shower_C_SDC->SetMaximum(175000);

  TH1D *y_shower_S_SDS = new TH1D("y_shower_S_SDS", "Average Amplitude of Pulses of S Photons in SDS for Energy >= 15 GeV", 100, -14.9, 15.0);
  y_shower_S_SDS->GetXaxis()->SetTitle("y-Position");
  y_shower_S_SDS->GetYaxis()->SetTitle("Count");

  TH1D *y_shower_C_SDS = new TH1D("y_shower_C_SDS", "Average Amplitude of Pulses of C Photons in SDS for Energy >= 15 GeV", 100, -14.9, 15.0);
  y_shower_C_SDS->GetXaxis()->SetTitle("y-Position");
  y_shower_C_SDS->GetYaxis()->SetTitle("Count");

  float x_counts_S_SDC = 0.0, x_counts_C_SDC = 0.0, x_counts_S_SDS = 0.0, x_counts_C_SDS = 0.0;
  float y_counts_S_SDC = 0.0, y_counts_C_SDC = 0.0, y_counts_S_SDS = 0.0, y_counts_C_SDS = 0.0;
  float x_counts_shower_S_SDC = 0.0, x_counts_shower_C_SDC = 0.0, x_counts_shower_S_SDS = 0.0, x_counts_shower_C_SDS = 0.0;
  float y_counts_shower_S_SDC = 0.0, y_counts_shower_C_SDC = 0.0, y_counts_shower_S_SDS = 0.0, y_counts_shower_C_SDS = 0.0;
  float x = 0.0, y = 0.0;
  
  // This loop fills the 1D histograms with the number of counts from each axis.
  for (int i = 0; i <= 100; i++) {                                    
    x = i * (3.0/10.0) - 15.0;
    y = i * (3.0/10.0) - 15.0;

    // This code calibrates the x-positions, fills the histograms with the data, and resets the variables.  
    x_counts_S_SDC = avg_detected_photons_S_SDC->ProjectionX()->GetBinContent(i);
    x_counts_C_SDC = avg_detected_photons_C_SDC->ProjectionX()->GetBinContent(i);
    x_counts_S_SDS = avg_detected_photons_S_SDS->ProjectionX()->GetBinContent(i);
    x_counts_C_SDS = avg_detected_photons_C_SDS->ProjectionX()->GetBinContent(i);
    x_cuts_S_SDC->Fill(x, x_counts_S_SDC);
    x_cuts_C_SDC->Fill(x, x_counts_C_SDC);
    x_cuts_S_SDS->Fill(x, x_counts_S_SDS);
    x_cuts_C_SDS->Fill(x, x_counts_C_SDS);
    x_counts_S_SDC = 0.0;
    x_counts_C_SDC = 0.0;
    x_counts_S_SDS = 0.0;
    x_counts_C_SDS = 0.0;
   
    // This code does the same as that for the x-positions, but with the y-positions.
    y_counts_S_SDC = avg_detected_photons_S_SDC->ProjectionY()->GetBinContent(i);
    y_counts_C_SDC = avg_detected_photons_C_SDC->ProjectionY()->GetBinContent(i);
    y_counts_S_SDS = avg_detected_photons_S_SDS->ProjectionY()->GetBinContent(i);
    y_counts_C_SDS = avg_detected_photons_C_SDS->ProjectionY()->GetBinContent(i);
    y_cuts_S_SDC->Fill(y, y_counts_S_SDC);
    y_cuts_C_SDC->Fill(y, y_counts_C_SDC);
    y_cuts_S_SDS->Fill(y, y_counts_S_SDS);
    y_cuts_C_SDS->Fill(y, y_counts_C_SDS);
    y_counts_S_SDC = 0.0;
    y_counts_C_SDC = 0.0;
    y_counts_S_SDS = 0.0;
    y_counts_C_SDS = 0.0;
   
    // This code plots the average shower data.
    x_counts_shower_S_SDC = avg_S_SDC_shower->ProjectionX()->GetBinContent(i);
    x_counts_shower_C_SDC = avg_C_SDC_shower->ProjectionX()->GetBinContent(i);
    x_counts_shower_S_SDS = avg_S_SDS_shower->ProjectionX()->GetBinContent(i);
    x_counts_shower_C_SDS = avg_C_SDS_shower->ProjectionX()->GetBinContent(i);
    x_shower_S_SDC->Fill(x, x_counts_shower_S_SDC);
    x_shower_C_SDC->Fill(x, x_counts_shower_C_SDC);
    x_shower_S_SDS->Fill(x, x_counts_shower_S_SDS);
    x_shower_C_SDS->Fill(x, x_counts_shower_C_SDS);
    x_counts_shower_S_SDC = 0.0;
    x_counts_shower_C_SDC = 0.0;
    x_counts_shower_S_SDS = 0.0;
    x_counts_shower_C_SDS = 0.0;

    // This code plots the average shower data.                                                                                                                                  
    y_counts_shower_S_SDC = avg_S_SDC_shower->ProjectionY()->GetBinContent(i);
    y_counts_shower_C_SDC = avg_C_SDC_shower->ProjectionY()->GetBinContent(i);
    y_counts_shower_S_SDS = avg_S_SDS_shower->ProjectionY()->GetBinContent(i);
    y_counts_shower_C_SDS = avg_C_SDS_shower->ProjectionY()->GetBinContent(i);
    y_shower_S_SDC->Fill(y, y_counts_shower_S_SDC);
    y_shower_C_SDC->Fill(y, y_counts_shower_C_SDC);
    y_shower_S_SDS->Fill(y, y_counts_shower_S_SDS);
    y_shower_C_SDS->Fill(y, y_counts_shower_C_SDS);
    y_counts_shower_S_SDC = 0.0;
    y_counts_shower_C_SDC = 0.0;
    y_counts_shower_S_SDS = 0.0;
    y_counts_shower_C_SDS = 0.0;
   
    x = 0.0;
    y = 0.0;
  }                                                             

  // These draw the histograms as, well, histograms (i.e. without error bars).
  TCanvas *c11 = new TCanvas();
  x_cuts_S_SDC->Draw("HIST");
  c11->Print("x_cuts_S_SDC.png");
  
  TCanvas *c12 = new TCanvas();
  x_cuts_C_SDC->Draw("HIST");
  c12->Print("x_cuts_C_SDC.png");

  TCanvas *c13 = new TCanvas();
  x_cuts_S_SDS->Draw("HIST");
  c13->Print("x_cuts_S_SDS.png");

  TCanvas *c14 = new TCanvas();
  x_cuts_C_SDS->Draw("HIST");
  c14->Print("x_cuts_C_SDS.png");

  TCanvas *c21 = new TCanvas();
  y_cuts_S_SDC->Draw("HIST");
  c21->Print("y_cuts_S_SDC.png");

  TCanvas *c22 = new TCanvas();
  y_cuts_C_SDC->Draw("HIST");
  c22->Print("y_cuts_C_SDC.png");

  TCanvas *c23 = new TCanvas();
  y_cuts_S_SDS->Draw("HIST");
  c23->Print("y_cuts_S_SDS.png");

  TCanvas *c24 = new TCanvas();
  y_cuts_C_SDS->Draw("HIST");
  c24->Print("y_cuts_C_SDS.png");

  TCanvas *c31 = new TCanvas();
  x_shower_S_SDC->Draw("HIST");
  c31->Print("x_shower_S_SDC.png");

  TCanvas *c32 = new TCanvas();
  x_shower_C_SDC->Draw("HIST");
  c32->Print("x_shower_C_SDC.png");

  TCanvas *c33 = new TCanvas();
  x_shower_S_SDS->Draw("HIST");
  c33->Print("x_shower_S_SDS.png");
  
  TCanvas *c34 = new TCanvas();
  x_shower_C_SDS->Draw("HIST");
  c34->Print("x_shower_C_SDS.png");

  TCanvas *c41 =new TCanvas();
  y_shower_S_SDC->Draw("HIST");
  c41->Print("y_shower_S_SDC.png");

  TCanvas *c42 = new TCanvas();
  y_shower_C_SDC->Draw("HIST");
  c42->Print("y_shower_C_SDC.png");

  TCanvas *c43 = new TCanvas();
  y_shower_S_SDS->Draw("HIST");
  c43->Print("y_shower_S_SDS.png");

  TCanvas *c44 = new TCanvas();
  y_shower_C_SDS->Draw("HIST");
  c44->Print("y_shower_C_SDS.png");

  TCanvas *c51 = new TCanvas();
  avg_detected_photons_S_SDC->Draw("COLZ");
  c51->Print("avg_detected_photons_S_SDC.png");

  TCanvas *c52 = new TCanvas();
  avg_detected_photons_C_SDC->Draw("COLZ");
  c52->Print("avg_detected_photons_C_SDC.png"); 

  TCanvas *c53 = new TCanvas();
  avg_detected_photons_S_SDS->Draw("COLZ");
  c53->Print("avg_detected_photons_S_SDS.png"); 

  TCanvas *c54 = new TCanvas();
  avg_detected_photons_C_SDS->Draw("COLZ");
  c54->Print("avg_detected_photons_C_SDS.png"); 

  TCanvas *c55 = new TCanvas();
  avg_S_SDC_shower->Draw("COLZ");
  c55->Print("avg_S_SDC_shower.png");

  TCanvas *c56 = new TCanvas();
  avg_C_SDC_shower->Draw("COLZ");
  c56->Print("avg_C_SDC_shower.png");
 
  TCanvas *c57 = new TCanvas();
  avg_S_SDS_shower->Draw("COLZ");
  c57->Print("avg_S_SDS_shower.png");

  TCanvas *c58 = new TCanvas();
  avg_C_SDS_shower->Draw("COLZ");
  c58->Print("avg_C_SDS_shower.png");

  return 0;
}
 

//  Code written by Christian Guinto-Brody for Bob Hirosky's High-Energy-Physics group with his and his colleagues' help.
