#include <vector>
using std::vector;

int proton_test() { 

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1, 0);

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

  /* The following lines of code create the histograms for each variable.
     For each variable, I define a histogram that will plot the distribution
     of that variable over a range specific to that variable. */

  /* These five deal with the energy distribution of the protons. 
     The second shows a slightly more interesting shape, the third 
     one shows a VERY MUCH more interesting shape, and the third 
     compresses everything (effectively) so that we see where the peak is.*/
  TH1D *h_energy = new TH1D("h_energy", "Deposited Energy of Proton Beam", 50, 0, 25);
  h_energy->GetXaxis()->SetTitle("Deposited Energy (MeV)");
  h_energy->GetYaxis()->SetTitle("Count");

  TH1D *h_energy_zoomed = new TH1D("h_energy_zoomed", "Deposited Energy of Proton Beam (From 0.1 to 50)", 50, 0.1, 25);
  h_energy_zoomed->GetXaxis()->SetTitle("Deposited Energy (MeV)");
  h_energy_zoomed->GetYaxis()->SetTitle("Count");

  TH1D *h_energy_Landau = new TH1D("h_energy_Landau", "Deposited Energy of Proton Beam (From 0 to 0.2)", 50, 0, 0.2);
  h_energy->GetXaxis()->SetTitle("Deposited Energy (MeV)");
  h_energy->GetYaxis()->SetTitle("Count");

  TH1D *h_energy_ion = new TH1D("h_energy_ion", "Deposited Energy of Ions from Proton Beam", 50, 0, 25);
  h_energy_ion->GetXaxis()->SetTitle("Deposited Energy (MeV)");
  h_energy_ion->GetYaxis()->SetTitle("Count");

  TH1D *h_energy_LOG = new TH1D("h_energy_LOG", "Peak Energy of Proton Beam", 50, -7, 3);                                                                                      
  h_energy->GetXaxis()->SetTitle("Log_10 of Deposited Energy)");                                                                                                                 
  h_energy->GetYaxis()->SetTitle("Count");                                                                                                                                          
  // This one deals with the locations of the protons when they reach the rear face of the detector.
  TH2D *pos_protons = new TH2D("pos_protons", "x- and y-Positions of Proton Beam", 25, -12.5, 12.5, 25, -12.5, 12.5);
  pos_protons->GetXaxis()->SetTitle("x-Position (mm)");
  pos_protons->GetYaxis()->SetTitle("y-Position (mm)");

  /* These four deal with the positions of the scintillation and Cherenkov photons in each
     SiPM relative to where the protons landed. This is the data we're after right now. */
  TH2D *pos_S_SDC = new TH2D("pos_S_SDC", "Number of Scintillation Photons in SDC vs. x- and y-Position of Proton Beam", 25, -12.5, 12.5, 25, -12.5, 12.5);
  pos_S_SDC->GetXaxis()->SetTitle("x-Position (mm)");
  pos_S_SDC->GetYaxis()->SetTitle("y-Position (mm)");

  TH2D *pos_C_SDC = new TH2D("pos_C_SDC", "Number of Cherenkov Photons in SDC vs. x- and y-Position of Proton Beam", 25, -12.5, 12.5, 25, -12.5, 12.5);
  pos_C_SDC->GetXaxis()->SetTitle("x-Position (mm)");
  pos_C_SDC->GetYaxis()->SetTitle("y-Position (mm)");

  TH2D *pos_S_SDS = new TH2D("pos_S_SDS", "Number of Scintillation Photons in SDS vs. x- and y-Position of Proton Beam", 25, -12.5, 12.5, 25, -12.5, 12.5);
  pos_S_SDS->GetXaxis()->SetTitle("x-Position (mm)");
  pos_S_SDS->GetYaxis()->SetTitle("y-Position (mm)");

  TH2D *pos_C_SDS = new TH2D("pos_C_SDS", "Number of Cherenkov Photons in SDS vs. x- and y-Position of Proton Beam", 25, -12.5, 12.5, 25, -12.5, 12.5);
  pos_C_SDS->GetXaxis()->SetTitle("x-Position (mm)");
  pos_C_SDS->GetYaxis()->SetTitle("y-Position (mm)");

  /* These four are very similar to the previous four, but they show the average count of 
     each type of photon (excluding showers) in each SiPM. That's what TProfile does--it 
     plots the average number of the given variable. */
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

  float maxSDC = 0.0, maxSDS = 0.0;
  // This for loop fills the histograms.
  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);    
    h_energy->Fill(depositedEnergyECAL_r);
    h_energy_zoomed->Fill(depositedEnergyECAL_r);
    h_energy_Landau->Fill(depositedEnergyECAL_r);
    h_energy_ion->Fill(depositedIonEnergyECAL_r);
    h_energy_LOG->Fill(log10(depositedEnergyECAL_r));
    pos_protons->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1]);
    pos_S_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_S);
    pos_C_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_C);
    pos_S_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDSdetected_r_S);
    pos_C_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDSdetected_r_C);

    if (depositedEnergyECAL_r < 0.15) {
      avg_detected_photons_S_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_S);
      avg_detected_photons_C_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_C);
      avg_detected_photons_S_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDSdetected_r_S);
      avg_detected_photons_C_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDSdetected_r_C);
    }
  }  
  for (int i = 1; i <= avg_detected_photons_C_SDC->GetNbinsX(); i++) {
    for (int j = 1; j <= avg_detected_photons_C_SDC->GetNbinsY(); j++) {
      if (avg_detected_photons_C_SDC->GetBinContent(i, j) >= 25) {
	if (avg_detected_photons_C_SDC->GetBinContent(i, j) > maxSDC) {
	  maxSDC = avg_detected_photons_C_SDC->GetBinContent(i, j);
	}
	// std::cout << avg_detected_photons_C_SDC->GetBinContent(i, j) << std::endl;
	// cout << i << "" << j << endl;
      }
    }
  }
  for (int i = 1; i <= avg_detected_photons_C_SDS->GetNbinsX(); i++) {
    for (int j = 1; j <= avg_detected_photons_C_SDS->GetNbinsY(); j++) {
      if (avg_detected_photons_C_SDS->GetBinContent(i, j) >= 25) {
	if (avg_detected_photons_C_SDS->GetBinContent(i, j) > maxSDS) {
          maxSDS = avg_detected_photons_C_SDS->GetBinContent(i,j);
	}
	// std::cout << avg_detected_photons_C_SDS->GetBinContent(i, j) << std::endl;
        // cout << i << "" << j << endl;                                                                                                                                         
      }
    }
  }
  cout <<  maxSDC << " " << maxSDS << endl;
  
  // for (int i = 1; i <= avg_detected_photons_S_SDC->GetNbinsX(); i++) std::cout << avg_detected_photons_S_SDC->GetBinContent(i) << std::endl;

  /* These are all the canvases on which the histograms will be placed.
     I also write each graph/histogram to a PNG file so I can access them
     and discuss them later. My convention is the following:

     First Number:
     1) Deals with the energies of the protons
     2) Deals with the positions of the protons
     3) Deals with the positions of the photons
     
     Second Number: Counts how many histograms there are dealing with 1), 2), or 3). */  

  // These five  deal with the energies of the protons. The log graph is meant to show a peak in the energies.
  TCanvas *c11 = new TCanvas();
  h_energy->Draw();
  // c11->Print("depositedEnergyECAL_40k.png");

  TCanvas *c12 = new TCanvas();
  h_energy_zoomed->Draw();
  // c12->Print("depositedEnergyECAL_zoomed_40k.png");  

  TCanvas *c13 = new TCanvas();
  h_energy_Landau->Draw();
  // c13->Print("depositedEnergyECAL_Landau_40k.png");

  TCanvas *c14 = new TCanvas();
  h_energy_ion->Draw();
  // c14->Print("depositedIonEnergyECAL_40k.png");

  TCanvas *c15 = new TCanvas();
  h_energy_LOG->Draw();
  // c15->Print("LOGdepositedEnergyECAL_40k_[-7,3].png");

  /* These two deal with the positions of the protons. The first is a heat map; the second is a scatterplot. 
     These graphs are meant to show that the protons travel in random trajectories. */
  TCanvas *c21 = new TCanvas();
  pos_protons->Draw("colz");
  // c21->Print("pos_protons_40k.png");
  
  TCanvas *c22 = new TCanvas();
  pos_protons->Draw("p");
  // c22->Print("pos_protons_40k_check.png");

  // These four deal with the positions of the photons. We particularly want to see what's up with these ones. 
  TCanvas *c31 = new TCanvas();
  pos_S_SDC->Draw("colz");
  // c31->Print("pos_S_SDC_40k.png");

  TCanvas *c32 = new TCanvas();
  pos_C_SDC->Draw("colz");
  // c32->Print("pos_C_SDC_40k.png");
 
  TCanvas *c33 = new TCanvas();
  pos_S_SDS->Draw("colz");
  // c33->Print("pos_S_SDS_40k.png");                                           

  TCanvas *c34 = new TCanvas();
  pos_C_SDS->Draw("colz");
  // c34->Print("pos_C_SDS_40k.png");

  /* As said in the histogram definitions, these four are similar to the previous four, 
     but they count the average number of photons in each detector, excluding showers. */
  TCanvas *c41 = new TCanvas();
  avg_detected_photons_S_SDC->Draw("colz");
  // c41->Print("avg_S_photons_SDC.png");

  TCanvas *c42 = new TCanvas();
  avg_detected_photons_C_SDC->Draw("colz");
  // c42->Print("avg_C_photons_SDC.png");

  TCanvas *c43 = new TCanvas();
  avg_detected_photons_S_SDS->Draw("colz");
  // c43->Print("avg_S_photons_SDS.png");
  
  TCanvas *c44 = new TCanvas();
  avg_detected_photons_C_SDS->Draw("colz");
  // c44->Print("avg_C_photons_SDS.png");

  return 0;
}
 

//  Code written by Christian Guinto-Brody for Bob Hirosky's High-Energy-Physics group with his and his colleagues' help.




/* The following lines of code were used for testing.

// This code can be mimicked to get the bin content of a histogram.
for (int i = 1; i <= avg_detected_photons_C_SDS->GetNbinsX(); i++) {
  for (int j = 1; j <= avg_detected_photons_C_SDS->GetNbinsY(); j++) {
    if (avg_detected_photons_C_SDS->GetBinContent(i, j) >= 25) {
      if (avg_detected_photons_C_SDS->GetBinContent(i, j) > maxSDS) {
	maxSDS = avg_detected_photons_C_SDS->GetBinContent(i,j);
      }
      // std::cout << avg_detected_photons_C_SDS->GetBinContent(i, j) << std::endl;                                                                                             
      // cout << i << "" << j << endl;                                                                                                                                         
    }
  }
 }
cout <<  maxSDC << " " << maxSDS << endl;
*/
