#include <vector>
using std::vector;

int TRUE_FINAL_proton_test() { 



  // ........................................................................................... 
  // .............................PART 1: DEFINITIONS AND VARIABLES.............................
  // ........................................................................................... 



  gStyle->SetOptStat(0);
  int fileType = 1;

  /* This adds each ROOT file to a chain of trees. I tried making a series of if-else statements so I could automate switching
     between different ROOT files, but it didn't seem to work, so I'm back here having to manually comment out code... >:( */
  TChain *chain = new TChain("tree");
 
  // These are for 120-GeV protons through PWO.
  TString sample = "PWO_40k_proton_120_";
  chain->Add("first_40k_proton_120.root");
  chain->Add("second_40k_proton_test.root"); // Accidentally misnamed this one...
  chain->Add("third_40k_proton_120.root");
  chain->Add("fourth_40k_proton_120.root");
  chain->Add("fifth_40k_proton_120.root");
  chain->Add("sixth_40k_proton_120.root");
  chain->Add("seventh_40k_proton_120.root");
  chain->Add("eighth_40k_proton_120.root");
  chain->Add("ninth_40k_proton_120.root");
  chain->Add("tenth_40k_proton_120.root");
  chain->Add("eleventh_40k_proton_120.root");

  // These are for 120-GeV protons through BGO.
  // TString sample = "BGO_2000_proton_120_";  
  // chain->Add("first_BGO_2000_proton_120.root");
  // chain->Add("second_BGO_2000_proton_120.root");
  // chain->Add("third_BGO_2000_proton_120.root");
  // chain->Add("fourth_BGO_2000_proton_120.root");
  // chain->Add("fifth_BGO_2000_proton_120.root");
  // chain->Add("sixth_BGO_2000_proton_120.root");
  // chain->Add("seventh_BGO_2000_proton_120.root");
  // chain->Add("eighth_BGO_2000_proton_120.root");

  // This is for 12-GeV electrons through PWO.
  // TString sample = "PWO_1000_electron_12_";
  // chain->Add("first_PWO_electron_12.root");

  float depositedEnergyECAL_r; // Deposited energy of proton beam in rear SiPMs
  float depositedIonEnergyECAL_r; // Deposited energy of ions from proton beam. This one is VERY similar to detectedEnergyECAL_r
  vector<float> *inputInitialPosition = new vector<float>(3, 0.); // Positions of protons from beam  on xy-plane
  int SDCdetected_r_S; // Number of scintillation photons detected in SDC
  int SDCdetected_r_C; // Number of Cherenkov photons detected in SDC
  int SDSdetected_r_S; // Number of scintillation photons detected in SDS
  int SDSdetected_r_C; // Number of Cherenkov photons detected in SDS
  
  // Setting branch addresses of each variable. These are used for the histograms.
  chain->SetBranchAddress("depositedEnergyECAL_r", &depositedEnergyECAL_r);
  chain->SetBranchAddress("depositedIonEnergyECAL_r", &depositedIonEnergyECAL_r);
  chain->SetBranchAddress("inputInitialPosition", &inputInitialPosition);
  chain->SetBranchAddress("SDCdetected_r_S", &SDCdetected_r_S);
  chain->SetBranchAddress("SDCdetected_r_C", &SDCdetected_r_C);
  chain->SetBranchAddress("SDSdetected_r_S", &SDSdetected_r_S);
  chain->SetBranchAddress("SDSdetected_r_C", &SDSdetected_r_C);

  /* These histograms plot the wavelength distributions of each type of photon detected in each SiPM.
     They take the data from the corresponding histograms defined in CreateTree.hh and CreateTree.cc. */
  TH1D *SDC_S_lambdas = new TH1D("SDC_S_lambdas", "Wavelengths of Scintillation Photons Detected in SDC", 1250, 0., 1250.);
  SDC_S_lambdas->GetXaxis()->SetTitle("Wavelength (nm)");
  SDC_S_lambdas->GetYaxis()->SetTitle("Count");

  TH1D *SDC_C_lambdas = new TH1D("SDC_C_lambdas", "Wavelengths of Cherenkov Photons Detected in SDC", 1250, 0., 1250.);
  SDC_C_lambdas->GetXaxis()->SetTitle("Wavelength (nm)");
  SDC_C_lambdas->GetYaxis()->SetTitle("Count");

  TH1D *SDS_S_lambdas = new TH1D("SDS_S_lambdas", "Wavelengths of Scintillation Photons Detected in SDS", 1250, 0., 1250.);
  SDS_S_lambdas->GetXaxis()->SetTitle("Wavelength (nm)");
  SDS_S_lambdas->GetYaxis()->SetTitle("Count");

  TH1D *SDS_C_lambdas = new TH1D("SDS_C_lambdas", "Wavelengths of Cherenkov Photons Detected in SDS", 1250, 0., 1250.);
  SDS_C_lambdas->GetXaxis()->SetTitle("Wavelength (nm)");
  SDS_C_lambdas->GetYaxis()->SetTitle("Count");

  /* There are two things to consider when dealing with the wavelength distributions. The first is that we should
     expect to see a smooth, somewhat sharp distributions for the scintillation photons and a double-peaked, more 
     broad distribution for the Cherenkov photons. (These are, in fact, what we see, but I'll explain the results
     when I plot the distributions.) The second is that I'm ignoring the wavelength distributions of the generated
     photons, which are divided into either "front" or "rear," referring to generation in the front or rear crystals.
     Since we're treaing the detector as if it's one bar, we don't care about the distinction, so I don't plot these. */



  // ........................................................................................... 
  // .........................PART 2: PLOTTING WAVELENGTH DISTRIBUTIONS.........................
  // ........................................................................................... 



  /* The TStrings filename, histname(s), and hist(s) are meant to distinguish between the defined histograms                                                                     
     so I can add the data from all the ROOT files together and plot them in one historgam. */
  TString filename = "";
  TString histnameDet = ""; // histnameDet deals with the detected wavelenghts

  // This for loop lets me add all the histograms from each file together at the same time.
  for (int i = 0; i < 11; i++) {
    if (sample == "PWO_40k_proton_120_") {
      if (i == 0) filename = "first_40k_proton_120.root";
      else if (i == 1) filename = "second_40k_proton_test.root";
      else if (i == 2) filename = "third_40k_proton_120.root";
      else if (i == 3) filename = "fourth_40k_proton_120.root";
      else if (i == 4) filename = "fifth_40k_proton_120.root";
      else if (i == 5) filename = "sixth_40k_proton_120.root";
      else if (i == 6) filename = "seventh_40k_proton_120.root";
      else if (i == 7) filename = "eighth_40k_proton_120.root";
      else if (i == 8) filename = "ninth_40k_proton_120.root";
      else if (i == 9) filename = "tenth_40k_proton_120.root";
      else if (i == 10) filename = "eleventh_40k_proton_120.root";
    } else if (sample == "BGO_2000_proton_120_") {
      if (i == 0) filename = "first_BGO_2000_proton_120.root";
      else if (i == 1) filename = "second_BGO_2000_proton_120.root";
      else if (i == 2) filename = "third_BGO_2000_proton_120.root";
      else if (i == 3) filename = "fourth_BGO_2000_proton_120.root";
      else if (i == 4) filename = "fifth_BGO_2000_proton_120.root";
      else if (i == 5) filename = "sixth_BGO_2000_proton_120.root";
      else if (i == 6) filename = "seventh_BGO_2000_proton_120.root";
      else if (i == 7) filename = "eighth_BGO_2000_proton_120.root";
    } else if (sample == "PWO_1000_electron_12_") {
      if (i == 0) filename = "first_PWO_electron_12.root";
    }
  }

    // Not going to lie, the if statements look somewhat clanky, but my syntax is valid. At least, I believe...
    for (int j = 0; j < 4; j++) {
      TFile *file = new TFile(filename);
      if (j == 0) histnameDet = "h_phot_lambda_SiPMC_r_Scin";
      else if (j == 1) histnameDet = "h_phot_lambda_SiPMC_r_Ceren";
      else if (j == 2) histnameDet = "h_phot_lambda_SiPMS_r_Scin";
      else if (j == 3) histnameDet = "h_phot_lambda_SiPMS_r_Ceren";
      TH1F *histDet = (TH1F*)file->Get(histnameDet);
      if (j == 0) SDC_S_lambdas->Add(histDet);
      else if (j == 1) SDC_C_lambdas->Add(histDet);
      else if (j == 2) SDS_S_lambdas->Add(histDet);
      else if (j == 3) SDS_C_lambdas->Add(histDet);
    }

  // These canvases plot the wavelength distributions.
  TCanvas *c1 = new TCanvas();
  SDC_S_lambdas->Draw();
  c1->Print(sample + "SDC_S_lambdas.png");

  TCanvas *c2 = new TCanvas();
  SDC_C_lambdas->Draw();
  c2->Print(sample + "SDC_C_lambdas.png");
  
  TCanvas *c3 = new TCanvas();
  SDS_S_lambdas->Draw();
  c3->Print(sample + "SDS_S_lambdas.png");

  TCanvas *c4 = new TCanvas();
  SDS_C_lambdas->Draw();
  c4->Print(sample + "SDS_C_lambdas.png");

 
  
  // ........................................................................................... 
  // ..........................PART 3: ENERGY AND POSITION HISTOGRAMS...........................
  // ........................................................................................... 



  // This gets the number of entries (in this case, events).
  int entries = chain->GetEntries();
  /* The following lines of code create the histograms for each variable.
     For each variable, I define a histogram that will plot the distribution
     of that variable over a range specific to that variable. */

  /* These five deal with the energy distribution of the protons. 
     The second shows a slightly more interesting shape, the third 
     one shows a VERY MUCH more interesting shape, and the third 
     compresses everything (effectively) so that we see where the peak is. */
  TH1F *h_energy = new TH1F("h_energy", "Deposited Energy of Proton Beam", 100, 0.0, 30.0);
  h_energy->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  h_energy->GetYaxis()->SetTitle("Count");

  TH1F *h_energy_Landau = new TH1F("h_energy_Landau", "Deposited Energy of Proton Beam (From 0 GeV to 0.2 GeV)", 50, 0.0, 0.2);
  h_energy_Landau->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  h_energy_Landau->GetYaxis()->SetTitle("Count");

  TH1F *h_energy_ion = new TH1F("h_energy_ion", "Deposited Ion Energy from Proton Beam", 50, 0.0, 25.0);
  h_energy_ion->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  h_energy_ion->GetYaxis()->SetTitle("Count");

  TH1F *h_energy_LOG = new TH1F("h_energy_LOG", "Peak Energy of Proton Beam", 100, -3.0, 1.5);
  h_energy_LOG->GetXaxis()->SetTitle("Log_10 of Deposited Energy)");
  h_energy_LOG->GetYaxis()->SetTitle("Count");                                                    

  // This one deals with the locations of the protons when they reach the rear face of the detector.
  TH2F *pos_protons = new TH2F("pos_protons", "x- and y-Positions of Protons in Beam", 25, -12.5, 12.5, 25, -12.5, 12.5);
  pos_protons->GetXaxis()->SetTitle("x-Position (mm)");
  pos_protons->GetYaxis()->SetTitle("y-Position (mm)");

  /* These four show the average count of each type of photon (excluding showers) in each SiPM. 
     That's what TProfile does--it plots the average number of the given variable. */
  TProfile2D *avg_detected_photons_S_SDC = new TProfile2D("avg_detected_photons_S_SDC", "Average Number of Detected S Photons per GeV in SDC", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_S_SDC->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_S_SDC->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_detected_photons_C_SDC = new TProfile2D("avg_detected_photons_C_SDC", "Average Number of Detected C Photons per GeV in SDC", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_C_SDC->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_C_SDC->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_detected_photons_S_SDS = new TProfile2D("avg_detected_photons_S_SDS", "Average Number of Detected S Photons per GeV in SDS", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_S_SDS->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_S_SDS->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_detected_photons_C_SDS = new TProfile2D("avg_detected_photons_C_SDS", "Average Number of Detected C Photons per GeV in SDS", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_C_SDS->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_C_SDS->GetYaxis()->SetTitle("y-Position (mm)");

  /* And THESE four are very similar to the previous six, but they show the average count of                                                                                    
     each type of photon (for ONLY showers) in each SiPM. */
  TProfile2D *avg_S_SDC_shower = new TProfile2D("avg_S_SDC_shower", "Average Number of Detected S per GeV in SDC for Showers", 100, -15, 15, 100, -15, 15);
  avg_S_SDC_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_S_SDC_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_C_SDC_shower = new TProfile2D("avg_C_SDC_shower", "Average Number of Detected C per GeV in SDC for Showers", 100, -15, 15, 100, -15, 15);
  avg_C_SDC_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_C_SDC_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_S_SDS_shower = new TProfile2D("avg_S_SDS_shower", "Average Number of Detected S per GeV in SDS for Showers", 100, -15, 15, 100, -15, 15);
  avg_S_SDS_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_S_SDS_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_C_SDS_shower = new TProfile2D("avg_C_SDS_shower", "Average Number of Detected C per GeV in SDS for Showers", 100, -15, 15, 100, -15, 15);
  avg_C_SDS_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_C_SDS_shower->GetYaxis()->SetTitle("y-Position (mm)");

  // These variables will let me get the fraction of events with depositedEnergyECAL_r > 0.1.
  int noEDepTenth = 0.0;
  float fracEDepTenth = 0.0;

  // This for loop fills the histograms.
  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    h_energy->Fill(depositedEnergyECAL_r);
    h_energy_Landau->Fill(depositedEnergyECAL_r);
    h_energy_ion->Fill(depositedIonEnergyECAL_r);
    h_energy_LOG->Fill(log10(depositedEnergyECAL_r));
    pos_protons->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1]);
   
    if (depositedEnergyECAL_r < 0.1) {
      avg_detected_photons_S_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_S)/depositedIonEnergyECAL_r);
      avg_detected_photons_C_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_C)/depositedIonEnergyECAL_r);
      avg_detected_photons_S_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_S)/depositedIonEnergyECAL_r);
      avg_detected_photons_C_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_C)/depositedIonEnergyECAL_r);
    }

    if (depositedEnergyECAL_r >= 0.1) {
      avg_S_SDC_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_S)/depositedIonEnergyECAL_r);
      avg_C_SDC_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_C)/depositedIonEnergyECAL_r);
      avg_S_SDS_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_S)/depositedIonEnergyECAL_r);
      avg_C_SDS_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_C)/depositedIonEnergyECAL_r);
    }

    if (depositedEnergyECAL_r > 0.1) noEDepTenth++;
  }  
  
  fracEDepTenth = float(noEDepTenth)/float(entries);
  cout << "FRACTION OF EVENTS WITH E_DEP > 0.1 GeV: " << fracEDepTenth << endl;
 
  TCanvas *c7 = new TCanvas();                                                                                                                                               
  h_energy->Draw();                                                                                                                                                           
  c7->Print(sample + "h_energy.png");  

  TCanvas *c8 = new TCanvas();                                                                                                                                               
  h_energy_Landau->Fit("landau");                                                                                                                                             
  h_energy_Landau->Draw();                                                                                                                                                    
  TF1 *h_energy_fit = h_energy_Landau->GetFunction("landau");                                                                                                                 
  h_energy_fit->Draw("SAME");                                                                                                                                                 
  c8->Print(sample + "h_energy_Landau.png");  

  TCanvas *c9 = new TCanvas();
  h_energy_ion->Draw();
  c9->Print(sample + "h_energy_ion.png");

  TCanvas *c10 = new TCanvas();
  h_energy_LOG->Draw();
  c10->Print(sample + "h_energy_LOG.png");

  TCanvas *c11 = new TCanvas();
  pos_protons->Draw();
  c11->Print(sample + "pos_protons.png");
  
  TCanvas *c12 = new TCanvas();                 
  avg_detected_photons_S_SDC->Draw("colz");     
  c12->Print(sample + "avg_S_photons_SDC.png");          

  TCanvas *c13 = new TCanvas();                 
  avg_detected_photons_C_SDC->Draw("colz");     
  c13->Print(sample + "avg_C_photons_SDC.png");          

  TCanvas *c14 = new TCanvas();                 
  avg_detected_photons_S_SDS->Draw("colz");     
  c14->Print(sample + "avg_S_photons_SDS.png");          

  TCanvas *c15 = new TCanvas();                 
  avg_detected_photons_C_SDS->Draw("colz");     
  c15->Print(sample + "avg_C_photons_SDS.png");          

  TCanvas *c16 = new TCanvas();                 
  avg_S_SDC_shower->Draw("colz");               
  c16->Print(sample + "avg_S_SDC_shower.png");           

  TCanvas *c17 = new TCanvas();                 
  avg_C_SDC_shower->Draw("colz");               
  c17->Print(sample + "avg_C_SDC_shower.png");           

  TCanvas *c18 = new TCanvas();                 
  avg_S_SDS_shower->Draw("colz");               
  c18->Print(sample + "avg_S_SDS_shower.png");           

  TCanvas *c19 = new TCanvas();                 
  avg_C_SDS_shower->Draw("colz");               
  c19->Print(sample + "avg_C_SDS_shower.png");           

 

  // ...........................................................................................                                                                                 
  // ..................................PART 4: CENTER CUTS......................................                                                                                 
  // ...........................................................................................   



  /* These histograms plot the distribution of the average numbers of photons along the centers of each SiPM. I took the 
     TProfile2Ds, found specific slices along the histograms (which show the positions of the back of the detector), 
     and plotted the distribution that arose from it along the x- and y-axes. */
  TH1F *x_center_S_SDC = new TH1F("x_center_S_SDC", "Center Cut of S Photons in SDC (x = 0)", 100, -14.9, 15.0);
  x_center_S_SDC->GetXaxis()->SetTitle("y-Positions (mm)");
  x_center_S_SDC->GetYaxis()->SetTitle("Count");

  TH1F *x_center_C_SDC = new TH1F("x_center_C_SDC", "Center Cut of C Photons in SDC (x = 0)", 100, -14.9, 15.0);
  x_center_C_SDC->GetXaxis()->SetTitle("y-Positions (mm)");
  x_center_C_SDC->GetYaxis()->SetTitle("Count");

  TH1F *x_center_S_SDS = new TH1F("x_center_S_SDS", "Center Cut of S Photons in SDS (x = 0)", 100, -14.9, 15.0);
  x_center_S_SDS->GetXaxis()->SetTitle("y-Positions (mm)");
  x_center_S_SDS->GetYaxis()->SetTitle("Count");

  TH1F *x_center_C_SDS = new TH1F("x_center_S_SDS", "Center Cut of C Photons in SDS (x = 0)", 100, -14.9, 15.0);
  x_center_C_SDS->GetXaxis()->SetTitle("y-Positions (mm)");
  x_center_C_SDS->GetYaxis()->SetTitle("Count");

  TH1F *y_center_S_SDC = new TH1F("y_center_S_SDC", "Center Cut of S Photons in SDC (y = 6.5)", 100, -14.9, 15.0);
  y_center_S_SDC->GetXaxis()->SetTitle("x-Positions (mm)");
  y_center_S_SDC->GetYaxis()->SetTitle("Count");

  TH1F *y_center_C_SDC = new TH1F("y_center_C_SDC", "Center Cut of C Photons in SDC (y = 6.5)", 100, -14.9, 15.0);
  y_center_C_SDC->GetXaxis()->SetTitle("x-Positions (mm)");
  y_center_C_SDC->GetYaxis()->SetTitle("Count");

  TH1F *y_center_S_SDS = new TH1F("y_center_S_SDS", "Center Cut of S Photons in SDS (y = -6.5)", 100, -14.9, 15.0);
  y_center_S_SDS->GetXaxis()->SetTitle("x-Positions (mm)");
  y_center_S_SDS->GetYaxis()->SetTitle("Count");

  TH1F *y_center_C_SDS = new TH1F("y_center_C_SDS", "Center Cut of C Photons in SDS (y = -6.5)", 100, -14.9, 15.0);
  y_center_C_SDS->GetXaxis()->SetTitle("x-Positions (mm)");
  y_center_C_SDS->GetYaxis()->SetTitle("Count");

  TH1F *shower_x_center_S_SDC = new TH1F("shower_x_center_S_SDC", "Center Cut of S Photons in SDC (x = 0) for Showers", 100, -14.9, 15.0);
  shower_x_center_S_SDC->GetXaxis()->SetTitle("y-Positions (mm)");
  shower_x_center_S_SDC->GetYaxis()->SetTitle("Count");

  TH1F *shower_x_center_C_SDC = new TH1F("shower_x_center_C_SDC", "Center Cut of C Photons in SDC (x = 0) for Showers", 100, -14.9, 15.0);
  shower_x_center_C_SDC->GetXaxis()->SetTitle("y-Positions (mm)");
  shower_x_center_C_SDC->GetYaxis()->SetTitle("Count");

  TH1F *shower_x_center_S_SDS = new TH1F("shower_x_center_S_SDS", "Center Cut of S Photons in SDS (x = 0) for Showers", 100, -14.9, 15.0);
  shower_x_center_S_SDS->GetXaxis()->SetTitle("y-Positions (mm)");
  shower_x_center_S_SDS->GetYaxis()->SetTitle("Count");

  TH1F *shower_x_center_C_SDS = new TH1F("shower_x_center_S_SDS", "Center Cut of C Photons in SDS (x = 0) for Showers", 100, -14.9, 15.0);
  shower_x_center_C_SDS->GetXaxis()->SetTitle("y-Positions (mm)");
  shower_x_center_C_SDS->GetYaxis()->SetTitle("Count");

  TH1F *shower_y_center_S_SDC = new TH1F("shower_y_center_S_SDC", "Center Cut of S Photons in SDC (y = 6.5) for Showers", 100, -14.9, 15.0);
  shower_y_center_S_SDC->GetXaxis()->SetTitle("x-Positions (mm)");
  shower_y_center_S_SDC->GetYaxis()->SetTitle("Count");

  TH1F *shower_y_center_C_SDC = new TH1F("shower_y_center_C_SDC", "Center Cut of C Photons in SDC (y = 6.5) for Showers", 100, -14.9, 15.0);
  shower_y_center_C_SDC->GetXaxis()->SetTitle("x-Positions (mm)");
  shower_y_center_C_SDC->GetYaxis()->SetTitle("Count");

  TH1F *shower_y_center_S_SDS = new TH1F("shower_y_center_S_SDS", "Center Cut of S Photons in SDS (y = -6.5) for Showers", 100, -14.9, 15.0);
  shower_y_center_S_SDS->GetXaxis()->SetTitle("x-Positions (mm)");
  shower_y_center_S_SDS->GetYaxis()->SetTitle("Count");

  TH1F *shower_y_center_C_SDS = new TH1F("shower_y_center_C_SDS", "Center Cut of C Photons in SDS (y = -6.5) for Showers", 100, -14.9, 15.0);
  shower_y_center_C_SDS->GetXaxis()->SetTitle("x-Positions (mm)");
  shower_y_center_C_SDS->GetYaxis()->SetTitle("Count");
  
  float x = 0.0, y = 0.0;
  for (int i = 0; i < 100; i++) {
    x = i * (3.0/10.0) - 15.0;
    y = i * (3.0/10.0) - 15.0;    

    /* The reason I have the above formulas is so that I can convert from the index/number of bins
       to the x- and y-positions of the rear face of the crystal. This is a result of the "Fill"
       function taking the actual x- or y-ranges of the histogram and the "GetBinContent" function
       taking the bin number. */   
    x_center_S_SDC->Fill(y, avg_detected_photons_S_SDC->GetBinContent(50, i)); // Bin 50 corresponds to x = 0.      
    x_center_C_SDC->Fill(y, avg_detected_photons_C_SDC->GetBinContent(50, i));
    x_center_S_SDS->Fill(y, avg_detected_photons_S_SDS->GetBinContent(50, i));
    x_center_C_SDS->Fill(y, avg_detected_photons_C_SDS->GetBinContent(50, i));
    y_center_S_SDC->Fill(x, avg_detected_photons_S_SDC->GetBinContent(i, 72)); // Bin 72 corresponds to y = -6.5.   
    y_center_C_SDC->Fill(x, avg_detected_photons_C_SDC->GetBinContent(i, 72));
    y_center_S_SDS->Fill(x, avg_detected_photons_S_SDS->GetBinContent(i, 28)); // Bin 28 corresponds to y = 6.5.    
    y_center_C_SDS->Fill(x, avg_detected_photons_C_SDS->GetBinContent(i, 28));
    
    shower_x_center_S_SDC->Fill(y, avg_S_SDC_shower->GetBinContent(50, i));
    shower_x_center_C_SDC->Fill(y, avg_C_SDC_shower->GetBinContent(50, i));
    shower_x_center_S_SDS->Fill(y, avg_S_SDS_shower->GetBinContent(50, i));
    shower_x_center_C_SDS->Fill(y, avg_C_SDS_shower->GetBinContent(50, i));
    shower_y_center_S_SDC->Fill(y, avg_S_SDC_shower->GetBinContent(i, 72));
    shower_y_center_C_SDC->Fill(y, avg_C_SDC_shower->GetBinContent(i, 72));
    shower_y_center_S_SDS->Fill(y, avg_S_SDS_shower->GetBinContent(i, 28));
    shower_y_center_C_SDS->Fill(y, avg_C_SDS_shower->GetBinContent(i, 28));
  }
 
  /* To find the number of photons per GeV in the centers of the SiPMs, I fit the peak where the SiPMs are
     with a Gaussian distribution and got the mean value of the Gaussian. I then average the means for the
     x- and y-cuts of each respective photon type with each SiPM to get the number I was looking for. */
  TF1 *fit1 = new TF1("m1", "gaus", 3.5, 9.5);
  TF1 *fit2 = new TF1("m2", "gaus", 3.5, 9.5);
  TF1 *fit3 = new TF1("m3", "gaus", -9.5, -3.5);
  TF1 *fit4 = new TF1("m4", "gaus", -9.5, -3.5);
  TF1 *fit5 = new TF1("m5", "gaus", -3.5, 3.5);
  TF1 *fit6 = new TF1("m6", "gaus", -3.5, 3.5);
  TF1 *fit7 = new TF1("m7", "gaus", -3.5, 3.5);
  TF1 *fit8 = new TF1("m8", "gaus", -3.5, 3.5);
  TF1 *shfit1 = new TF1("m9", "gaus", 3.5, 9.5);
  TF1 *shfit2 = new TF1("m10", "gaus", 3.5, 9.5);
  TF1 *shfit3 = new TF1("m11", "gaus", -9.5, -3.5);
  TF1 *shfit4 = new TF1("m12", "gaus", -9.5, -3.5);
  TF1 *shfit5 = new TF1("m13", "gaus", -3.5, 3.5);
  TF1 *shfit6 = new TF1("m14", "gaus", -3.5, 3.5);
  TF1 *shfit7 = new TF1("m15", "gaus", -3.5, 3.5);
  TF1 *shfit8 = new TF1("m16", "gaus", -3.5, 3.5);
 
  Double_t par1[3];
  Double_t par2[3];
  Double_t par3[3];
  Double_t par4[3];
  Double_t par5[3];
  Double_t par6[3];
  Double_t par7[3];
  Double_t par8[3];
  Double_t shpar1[3];
  Double_t shpar2[3];
  Double_t shpar3[3];
  Double_t shpar4[3];
  Double_t shpar5[3];
  Double_t shpar6[3];
  Double_t shpar7[3];
  Double_t shpar8[3];
 
  fit1->GetParameters(&par1[0]);
  shfit1->GetParameters(&shpar1[0]);
  fit2->GetParameters(&par2[0]);
  shfit2->GetParameters(&shpar2[0]);
  fit3->GetParameters(&par3[0]);
  shfit3->GetParameters(&shpar3[0]);
  fit4->GetParameters(&par4[0]);
  shfit4->GetParameters(&shpar4[0]);
  fit5->GetParameters(&par5[0]);
  shfit5->GetParameters(&shpar5[0]);
  fit6->GetParameters(&par6[0]);
  shfit6->GetParameters(&shpar6[0]);
  fit7->GetParameters(&par7[0]);
  shfit7->GetParameters(&shpar7[0]);
  fit8->GetParameters(&par8[0]);
  shfit8->GetParameters(&shpar8[0]);

  x_center_S_SDC->Fit(fit1, "R");
  shower_x_center_S_SDC->Fit(shfit1,"R");
  x_center_C_SDC->Fit(fit2, "R");
  shower_x_center_C_SDC->Fit(shfit2, "R");
  x_center_S_SDS->Fit(fit3, "R");
  shower_x_center_S_SDS->Fit(shfit3, "R");
  x_center_C_SDS->Fit(fit4, "R");
  shower_x_center_C_SDS->Fit(shfit4, "R");
  y_center_S_SDC->Fit(fit5, "R");
  shower_y_center_S_SDC->Fit(shfit5,"R");
  y_center_C_SDC->Fit(fit6, "R");
  shower_y_center_C_SDC->Fit(shfit6, "R");
  y_center_S_SDS->Fit(fit7, "R");
  shower_y_center_S_SDS->Fit(shfit7, "R");
  y_center_C_SDS->Fit(fit8, "R");
  shower_y_center_C_SDS->Fit(shfit8, "R");
  
  TCanvas *c20 = new TCanvas();
  x_center_S_SDC->Draw("HIST");
  fit1->Draw("SAME");
  c20->Print(sample + "x_center_S_SDC.png");

  TCanvas *c21 = new TCanvas();
  shower_x_center_S_SDC->Draw("HIST");
  shfit1->Draw("SAME");
  c21->Print(sample + "shower_x_center_S_SDC.png");

  TCanvas *c22 = new TCanvas();
  x_center_C_SDC->Draw("HIST");
  fit2->Draw("SAME");
  c22->Print(sample + "x_center_C_SDC.png");

  TCanvas *c23 = new TCanvas();
  shower_x_center_C_SDC->Draw("HIST");
  shfit2->Draw("SAME");
  c23->Print(sample + "shower_x_center_C_SDC.png");

  TCanvas *c24 = new TCanvas();
  x_center_S_SDS->Draw("HIST");
  fit3->Draw("SAME");
  c24->Print(sample + "x_center_S_SDS.png");

  TCanvas *c25 = new TCanvas();
  shower_x_center_S_SDS->Draw("HIST");
  shfit3->Draw("SAME");
  c25->Print(sample + "shower_x_center_S_SDS.png");

  TCanvas *c26 = new TCanvas();
  x_center_C_SDS->Draw("HIST");
  fit4->Draw("SAME");
  c26->Print(sample + "x_center_C_SDS.png");

  TCanvas *c27 = new TCanvas();
  shower_x_center_C_SDS->Draw("HIST");
  shfit4->Draw("SAME");
  c27->Print(sample + "shower_x_center_C_SDS.png");

  TCanvas *c28 = new TCanvas();
  y_center_S_SDC->Draw("HIST");
  fit5->Draw("SAME");
  c28->Print(sample + "y_center_S_SDC.png");

  TCanvas *c29 = new TCanvas();
  shower_y_center_S_SDC->Draw("HIST");
  shfit5->Draw("SAME");
  c29->Print(sample + "shower_y_center_S_SDC.png");

  TCanvas *c30 = new TCanvas();
  y_center_C_SDC->Draw("HIST");
  fit6->Draw("SAME");
  c30->Print(sample + "y_center_C_SDC.png");

  TCanvas *c31 = new TCanvas();
  shower_y_center_C_SDC->Draw("HIST");
  shfit6->Draw("SAME");
  c31->Print(sample + "shower_y_center_C_SDC.png");

  TCanvas *c32 = new TCanvas();
  y_center_S_SDS->Draw("HIST");
  fit7->Draw("SAME");
  c32->Print(sample + "y_center_S_SDS.png");

  TCanvas *c33 = new TCanvas();
  shower_y_center_S_SDS->Draw("HIST");
  shfit7->Draw("SAME");
  c33->Print(sample + "shower_y_center_S_SDS.png");

  TCanvas *c34 = new TCanvas();
  y_center_C_SDS->Draw("HIST");
  fit8->Draw("SAME");
  c34->Print(sample + "y_center_C_SDS.png");

  TCanvas *c35 = new TCanvas();
  shower_y_center_C_SDS->Draw("HIST");
  shfit8->Draw("SAME");
  c35->Print(sample + "shower_y_center_C_SDS.png");
 
  cout << "COUNTS OF PHOTONS PER GEV IN CENTERS OF SIPMS" << endl;
  cout << 0.5 * (fit1->Eval(fit1->Mean(3.5, 9.5)) + fit5->Eval(fit5->Mean(-3.5, 3.5))) << " " << 0.5 * (shfit1->Eval(shfit1->Mean(3.5, 9.5)) + shfit5->Eval(shfit1->Mean(-3.5, 3.5))) << endl;
  cout << 0.5 * (fit2->Eval(fit2->Mean(3.5, 9.5)) + fit6->Eval(fit6->Mean(-3.5, 3.5))) << " " << 0.5 * (shfit2->Eval(shfit2->Mean(3.5, 9.5)) + shfit6->Eval(shfit6->Mean(-3.5, 3.5))) << endl;													
  cout << 0.5 * (fit3->Eval(fit3->Mean(-9.5, -3.5)) + fit7->Eval(fit7->Mean(-3.5, 3.5))) << " " << 0.5 * (shfit3->Eval(shfit3->Mean(-9.5, -3.5)) + shfit7->Eval(shfit7->Mean(-3.5, 3.5))) << endl;
  cout << 0.5 * (fit4->Eval(fit4->Mean(-9.5, -3.5)) + fit8->Eval(fit8->Mean(-3.5, 3.5))) << " " << 0.5 * (shfit4->Eval(shfit4->Mean(-9.5, -3.5)) + shfit8->Eval(shfit8->Mean(-3.5, 3.5))) << endl;
 


  // ...........................................................................................                                                                                 
  // .........................PART 5: DETERMINING EFFICIENCY OF SIPMS........................... 
  // ...........................................................................................   



  /* The purpose of this section is to determine the efficiency of the SiPMs. Right now, we have
     values that describe how many photons are hitting the SiPMS, but we don't have values for 
     how many are actually being detected. These are important because we want to compare the
     Monte-Carlo data with the actual data (since SiPMs aren't perfect). So, the idea here is to
     1) noramlize the histograms we have; 2) multiply by the PDE, which stands for "Photon
     Detection Efficiency" and is a curve that shows which wavelengths are detected more readily;
     and 3) noramlize the new histogram. */

  // This section of code normalizes the original wavelength distributions. 
  double_t factor = 1;
  TString normalized = "no_";
  // TString normalized = "yes_";
  // SDC_S_lambdas->Scale(factor/SDC_S_lambdas->Integral("width"));
  // SDC_C_lambdas->Scale(factor/SDC_C_lambdas->Integral("width"));
  // SDS_S_lambdas->Scale(factor/SDS_S_lambdas->Integral("width"));
  // SDS_C_lambdas->Scale(factor/SDS_C_lambdas->Integral("width"));
  // front_S_detected->Scale(factor/front_S_detected->Integral("width"));
  // front_C_detected->Scale(factor/front_C_detected->Integral("width"));
  
  // These histograms detail the normalized histograms of the distributions of the wavelengths multiplied by the PDE.
  TH1D *combined_SDC_S = new TH1D("combined_SDC_S", "Normalized Distribution of Scintillation Photons in SDC Multiplied by PDE", 1250, 0., 1250.);
  combined_SDC_S->GetXaxis()->SetTitle("Wavelength (nm)");
  combined_SDC_S->GetYaxis()->SetTitle("Photon Detection Efficiency (%)");

  TH1D *combined_SDC_C = new TH1D("combined_SDC_C", "Normalized Distribution of Cherenkov Photons in SDC Multiplied by PDE", 1250, 0., 1250.);
  combined_SDC_C->GetXaxis()->SetTitle("Wavelength (nm)");
  combined_SDC_C->GetYaxis()->SetTitle("Photon Detection Efficiency (%)");

  TH1D *combined_SDS_S = new TH1D("combined_SDS_S", "Normalized Distribution of Scintillation Photons in SDS Multiplied by PDE", 1250, 0., 1250.);
  combined_SDS_S->GetXaxis()->SetTitle("Wavelength (nm)");
  combined_SDS_S->GetYaxis()->SetTitle("Photon Detection Efficiency (%)");

  TH1D *combined_SDS_C = new TH1D("combined_SDS_C", "Normalized Distribution of Cherenkov Photons in SDS Multiplied by PDE", 1250, 0., 1250.);
  combined_SDS_C->GetXaxis()->SetTitle("Wavelength (nm)");
  combined_SDS_C->GetYaxis()->SetTitle("Photon Detection Efficiency (%)");

  // This is a TGraph of the values given for the PDE.
  TGraph *PDE_graph = new TGraph("s1416_series.txt", "%lg %lg");
  PDE_graph->SetTitle("Photon Detection Efficiency of s1416 Series SiPMs");
  PDE_graph->GetXaxis()->SetTitle("Wavelength (nm)");
  PDE_graph->GetYaxis()->SetTitle("Photon Detection Efficiency");

  fstream file; 
  file.open("s1416_series.txt", ios::in); 
  int counter = 0;
  while (counter < 52) {  
    double x, y;  
    file >> x >> y;
    y /= 100.0;
    PDE_graph->SetPoint(counter, x, y);
    counter++;                    
  }  
  file.close();
  
  TCanvas *c36 = new TCanvas();
  PDE_graph->Draw("AL");
  c36->Print("PDE.png");
  
  /* This for loop multiplies each bin of the existing wavelength distribution by the value of the PDE evaluated at each wavelength.
     It then fills the histogram with those values and normalizes the new histograms. Since the PDE is defined only for specific
     wavelengths, I multiplied each histogram bin by 0 if we were out of the range of the PDE. */
  double_t scale_bin_SDC_S = 0, scale_bin_SDS_S = 0, scale_bin_SDC_C = 0, scale_bin_SDS_C = 0;
  double_t scale_bin_front_S = 0, scale_bin_rear_S = 0, scale_bin_front_C = 0, scale_bin_rear_C = 0;
  for (int i = 0; i <= 1250; i++) {
    if (i > 279 && i < 897) {
      scale_bin_SDC_S = PDE_graph->Eval(SDC_S_lambdas->GetBinCenter(i));
      scale_bin_SDC_C = PDE_graph->Eval(SDC_C_lambdas->GetBinCenter(i));
      scale_bin_SDS_S = PDE_graph->Eval(SDS_S_lambdas->GetBinCenter(i));
      scale_bin_SDS_C = PDE_graph->Eval(SDS_C_lambdas->GetBinCenter(i));
      scale_bin_front_S = PDE_graph->Eval(front_S_detected->GetBinCenter(i));
      scale_bin_front_C = PDE_graph->Eval(front_C_detected->GetBinCenter(i));
    } else {
      scale_bin_SDC_S = 0;
      scale_bin_SDC_C = 0;
      scale_bin_SDS_S = 0;
      scale_bin_SDS_C = 0;
      scale_bin_front_S = 0; 
      scale_bin_rear_S = 0;   
      scale_bin_front_C = 0; 
      scale_bin_rear_C = 0;
    }
    combined_SDC_S->SetBinContent(i, scale_bin_SDC_S * SDC_S_lambdas->GetBinContent(i));
    combined_SDC_C->SetBinContent(i, scale_bin_SDC_C * SDC_C_lambdas->GetBinContent(i));
    combined_SDS_S->SetBinContent(i, scale_bin_SDS_S * SDS_S_lambdas->GetBinContent(i));
    combined_SDS_C->SetBinContent(i, scale_bin_SDS_C * SDS_C_lambdas->GetBinContent(i));
    combined_front_S->SetBinContent(i, scale_bin_front_S * front_S_detected->GetBinContent(i));
    combined_front_C->SetBinContent(i, scale_bin_front_C * front_C_detected->GetBinContent(i));
  }
  // combined_SDC_S->Scale(factor/combined_SDC_S->Integral("width"));
  // combined_SDC_C->Scale(factor/combined_SDC_C->Integral("width"));
  // combined_SDS_S->Scale(factor/combined_SDS_S->Integral("width"));
  // combined_SDS_C->Scale(factor/combined_SDS_C->Integral("width"));
  // combined_front_S->Scale(factor/combined_front_S->Integral("width"));
  // combined_front_C->Scale(factor/combined_front_C->Integral("width"));
  
  // These TCanvases plot the normalized histograms with the original ones plotted on the same canvas.
  TCanvas *c37 = new TCanvas();
  combined_SDC_S->SetLineColor(4);
  combined_SDC_S->SetFillStyle(3001);
  SDC_S_lambdas->SetLineColor(2);
  SDC_S_lambdas->SetFillStyle(3001);
  
  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  // combined_SDC_S->Draw("HIST");
  // SDC_S_lambdas->Draw("HIST SAME");
  SDC_S_lambdas->SetTitle("Wavelengths of S Photons in SDC with and without PDE Applied");
  SDC_S_lambdas->Draw("HIST");
  combined_SDC_S->Draw("HIST SAME");
  TLegend *combined_SDC_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_SDC_S_legend->SetHeader("KEY", "C");
  combined_SDC_S_legend->AddEntry(SDC_S_lambdas, "Original", "l")->SetTextColor(2);
  combined_SDC_S_legend->AddEntry(combined_SDC_S, "PDE", "l")->SetTextColor(4);
  combined_SDC_S_legend->SetBorderSize(5);                                           
  combined_SDC_S_legend->Draw();   
  c37->Print(sample + normalized + "combined_SDC_S.png");

  TCanvas *c38 = new TCanvas();
  combined_SDC_C->SetLineColor(4);
  combined_SDC_C->SetFillStyle(3001);
  SDC_C_lambdas->SetLineColor(2);
  SDC_C_lambdas->SetFillStyle(3001);

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  // combined_SDC_C->Draw("HIST");
  // SDC_C_lambdas->Draw("HIST SAME");
  SDC_C_lambdas->SetTitle("Wavelengths of C Photons in SDC with and without PDE Applied");
  SDC_C_lambdas->Draw("HIST");      
  combined_SDC_C->Draw("HIST SAME"); 
  TLegend *combined_SDC_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_SDC_C_legend->SetHeader("KEY", "C");
  combined_SDC_C_legend->AddEntry(SDC_C_lambdas, "Original", "l")->SetTextColor(2);
  combined_SDC_C_legend->AddEntry(combined_SDC_C, "PDE", "l")->SetTextColor(4);
  combined_SDC_C_legend->SetBorderSize(5);
  combined_SDC_C_legend->Draw();
  c38->Print(sample + normalized +  "combined_SDC_C.png");

  TCanvas *c39 = new TCanvas();
  combined_SDS_S->SetLineColor(4);
  combined_SDS_S->SetFillStyle(3001);
  SDS_S_lambdas->SetLineColor(2);
  SDS_S_lambdas->SetFillStyle(3001);

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  // combined_SDS_S->Draw("HIST");
  // SDS_S_lambdas->Draw("HIST SAME");
  SDS_S_lambdas->SetTitle("Wavelengths of S Photons in SDS with and without PDE Applied");
  SDS_S_lambdas->Draw("HIST");       
  combined_SDS_S->Draw("HIST SAME"); 
  TLegend *combined_SDS_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_SDS_S_legend->SetHeader("KEY", "C");
  combined_SDS_S_legend->AddEntry(SDS_S_lambdas, "Original", "l")->SetTextColor(2);
  combined_SDS_S_legend->AddEntry(combined_SDS_S, "PDE", "l")->SetTextColor(4);
  combined_SDS_S_legend->SetBorderSize(5);
  combined_SDS_S_legend->Draw();
  c39->Print(sample + normalized + "combined_SDS_S.png");

  TCanvas *c40 = new TCanvas();
  combined_SDS_C->SetLineColor(4);
  combined_SDS_C->SetFillStyle(3001);
  SDS_C_lambdas->SetLineColor(2);
  SDS_C_lambdas->SetFillStyle(3001);

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  // combined_SDS_C->Draw("HIST");
  // SDS_C_lambdas->Draw("HIST SAME");
  SDS_C_lambdas->SetTitle("Wavelengths of C Photons in SDS with and without PDE Applied");
  SDS_C_lambdas->Draw("HIST");       
  combined_SDS_C->Draw("HIST SAME"); 
  TLegend *combined_SDS_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_SDS_C_legend->SetHeader("KEY", "C");
  combined_SDS_C_legend->AddEntry(SDS_C_lambdas, "Original", "l")->SetTextColor(2);
  combined_SDS_C_legend->AddEntry(combined_SDS_C, "PDE", "l")->SetTextColor(4);
  combined_SDS_C_legend->SetBorderSize(5);
  combined_SDS_C_legend->Draw();
  c40->Print(sample + normalized + "combined_SDS_C.png");

  // These canvases overlay all the relevant comparisons onto the same canvas.
  TCanvas *c41 = new TCanvas();
  combined_SDC_S->SetLineColor(1);
  combined_SDC_S->SetFillStyle(3001);
  combined_SDC_S->SetTitle("Overlaid Distributions of Scintillation Photons in SDC and SDS");
  SDC_S_lambdas->SetLineColor(2);
  SDC_S_lambdas->SetFillStyle(3001);
  combined_SDS_S->SetLineColor(3);
  combined_SDS_S->SetFillStyle(3001);
  SDS_S_lambdas->SetLineColor(4);
  SDS_S_lambdas->SetFillStyle(3001);

  // If the first and third  two are commented out, then we have unnormalized data. If the second and fourth two are commented out, we have normalized data.  
  // combined_SDC_S->Draw("HIST"); 
  // SDC_S_lambdas->Draw("HIST SAME");
  SDC_S_lambdas->SetTitle("Wavelengths of S Photons in Rear SiPMs with and without PDE Applied");
  SDC_S_lambdas->Draw("HIST");
  combined_SDC_S->Draw("HIST SAME");
  // combined_SDS_S->Draw("HIST SAME");
  // SDS_S_lambdas->Draw("HIST SAME");
  SDS_S_lambdas->Draw("HIST SAME");
  combined_SDS_S->Draw("HIST SAME");
  TLegend *combined_S_type_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_S_type_legend->SetHeader("KEY", "C");
  combined_S_type_legend->AddEntry(SDC_S_lambdas, "Original for SDC", "l")->SetTextColor(2);
  combined_S_type_legend->AddEntry(combined_SDC_S, "PDE for SDC", "l")->SetTextColor(1);
  combined_S_type_legend->AddEntry(SDS_S_lambdas, "Original for SDS", "l")->SetTextColor(4);
  combined_S_type_legend->AddEntry(combined_SDS_S, "PDE for SDS", "l")->SetTextColor(3);
  combined_S_type_legend->SetBorderSize(5);
  combined_S_type_legend->Draw();
  c41->Print(sample + normalized + "combined_S_SDC-SDS.png");

  TCanvas *c42 = new TCanvas();
  combined_SDC_C->SetLineColor(1);
  combined_SDC_C->SetFillStyle(3001);
  combined_SDC_C->SetTitle("Overlaid Distributions of Cherenkov Photons in SDC and SDS");
  SDC_C_lambdas->SetLineColor(2);
  SDC_C_lambdas->SetFillStyle(3001);
  combined_SDS_C->SetLineColor(3);
  combined_SDS_C->SetFillStyle(3001);
  SDS_C_lambdas->SetLineColor(4);
  SDS_C_lambdas->SetFillStyle(3001);

  // If the first and third  two are commented out, then we have unnormalized data. If the second and fourth two are commented out, we have normalized data. 
  // combined_SDC_C->Draw("HIST");
  // SDC_C_lambdas->Draw("HIST SAME");
  SDC_C_lambdas->SetTitle("Wavelengths of C Photons in Rear SiPMs with and without PDE Applied");
  SDC_C_lambdas->Draw("HIST SAME");
  combined_SDC_C->Draw("HIST SAME");
  // combined_SDS_C->Draw("HIST SAME");
  // SDS_C_lambdas->Draw("HIST SAME");
  SDS_C_lambdas->Draw("HIST SAME");
  combined_SDS_C->Draw("HIST SAME");
  TLegend *combined_C_type_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_C_type_legend->SetHeader("KEY", "C");
  combined_C_type_legend->AddEntry(SDC_C_lambdas, "Original for SDC", "l")->SetTextColor(2);
  combined_C_type_legend->AddEntry(combined_SDC_C, "PDE for SDC", "l")->SetTextColor(1);
  combined_C_type_legend->AddEntry(SDS_C_lambdas, "Original for SDS", "l")->SetTextColor(4);
  combined_C_type_legend->AddEntry(combined_SDS_C, "PDE for SDS", "l")->SetTextColor(3);
  combined_C_type_legend->SetBorderSize(5);
  combined_C_type_legend->Draw();
  c42->Print(sample + normalized + "combined_C_SDC-SDS.png");



  // ...........................................................................................                                                                                 
  // ............................PART 6: TIME OF ARRIVAL OF PHOTONS.............................  
  // ...........................................................................................  



  /* This purpose of this section is to determine when the photons arrive at the back of the SiPM. This is important
     because then we can understand when they're created, what wavelengths we can expect them to have when they
     arrive, and more. They serve as a good test to see that the simulation is working properly. */

  // These histograms show the distributions of the times of arrival of each photon.
  TH1F *time_SDC_S_detected = new TH1F("time_SDC_S_detected","Time of Arrival of Detected Scintillation Photons in SDC", 500, 0., 50.);
  time_SDC_S_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDC_S_detected->GetYaxis()->SetTitle("Count");

  TH1F *time_SDC_C_detected = new TH1F("time_SDC_C_detected","Time of Arrival of Detected Cherenkov Photons in SDC", 500, 0., 50.);
  time_SDC_C_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDC_C_detected->GetYaxis()->SetTitle("Count");

  TH1F *time_SDS_S_detected = new TH1F("time_SDS_S_detected","Time of Arrival of Detected Scintillation Photons in SDS", 500, 0., 50.);
  time_SDS_S_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDS_S_detected->GetYaxis()->SetTitle("Count");

  TH1F *time_SDS_C_detected = new TH1F("time_SDS_C_detected","Time of Arrival of Detected Cherenkov Photons in SDS", 500, 0., 50.);
  time_SDC_S_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDC_S_detected->GetYaxis()->SetTitle("Count");

  TString filename1 = "";
  TString histnameDet1 = "";

  // This for loop lets me add all the histograms from each file together at the same time.                                                                                      
  for (int i = 0; i < 11; i++) {
    if (sample == "PWO_40k_proton_120_") {
      if (i == 0) filename1 = "first_40k_proton_120.root";
      else if (i == 1) filename1 = "second_40k_proton_test.root";
      else if (i == 2) filename1 = "third_40k_proton_120.root";
      else if (i == 3) filename1 = "fourth_40k_proton_120.root";
      else if (i == 4) filename1 = "fifth_40k_proton_120.root";
      else if (i == 5) filename1 = "sixth_40k_proton_120.root";
      else if (i == 6) filename1 = "seventh_40k_proton_120.root";
      else if (i == 7) filename1 = "eighth_40k_proton_120.root";
      else if (i == 8) filename1 = "ninth_40k_proton_120.root";
      else if (i == 9) filename1 = "tenth_40k_proton_120.root";
      else if (i == 10) filename1 = "eleventh_40k_proton_120.root";
    } else if (sample == "BGO_2000_proton_120_") {
      if (i == 0) filename1 = "first_BGO_2000_proton_120.root";
      else if (i == 1) filename1 = "second_BGO_2000_proton_120.root";
      else if (i == 2) filename1 = "third_BGO_2000_proton_120.root";
      else if (i == 3) filename1 = "fourth_BGO_2000_proton_120.root";
      else if (i == 4) filename1 = "fifth_BGO_2000_proton_120.root";
      else if (i == 5) filename1 = "sixth_BGO_2000_proton_120.root";
      else if (i == 6) filename1 = "seventh_BGO_2000_proton_120.root";
      else if (i == 7) filename1 = "eighth_BGO_2000_proton_120.root";
    } else if (sample == "PWO_1000_electron_12_") {
      if (i == 0) filename1 = "first_PWO_electron_12.root";
    }
    
    // Not going to lie, the if statements look somewhat clanky, but my syntax is valid. At least, I believe...                                                                  
    for (int j = 0; j < 4; j++) {
      TFile *file = new TFile(filename1);
      if (j == 0) histnameDet1 = "h_phot_time_SiPMC_Scin";
      else if (j == 1) histnameDet1 = "h_phot_time_SiPMC_Ceren";
      else if (j == 2) histnameDet1 = "h_phot_time_SiPMS_Scin";
      else if (j == 3) histnameDet1 = "h_phot_time_SiPMS_Ceren";
      TH1F *histDet = (TH1F*)file->Get(histnameDet1);
      if (j == 0) time_SDC_S_detected->Add(histDet);
      else if (j == 1) time_SDC_C_detected->Add(histDet);
      else if (j == 2) time_SDS_S_detected->Add(histDet);
      else if (j == 3) time_SDS_C_detected->Add(histDet);
    }
  }
  
  // These canvases draw the timing histograms.
  TCanvas *c43 = new TCanvas();
  time_SDC_S_detected->Draw();                   
  c43->Print(sample + "time_SDC_S_detected.png");
  
  TCanvas *c44 = new TCanvas(); 
  time_SDC_C_detected->GetXaxis()->SetRange(0, 50);
  time_SDC_C_detected->Draw();   
  c44->Print(sample + "time_SDC_C_detected.png");
  
  TCanvas *c45 = new TCanvas();                 
  time_SDS_S_detected->Draw();                        
  c45->Print(sample + "time_SDS_S_detected.png");      
                                                
  TCanvas *c46 = new TCanvas();                  
  time_SDS_C_detected->GetXaxis()->SetRange(0, 50);
  time_SDS_C_detected->Draw();                        
  c46->Print(sample + "time_SDS_C_detected.png");      
  


  // ...........................................................................................                                                                                 
  // ...............................PART 7: FILTERING OF PHOTONS................................      
  // ...........................................................................................      
  


  // These histograms detail the normalized histograms of the distributions of the wavelengths multiplied by the PDE.
  TH1D *filter_SDC_S = new TH1D("filter_SDC_S", "Distribution of Scintillation Photons in SDC Multiplied by Filter", 1250, 0., 1250.);
  filter_SDC_S->GetXaxis()->SetTitle("Wavelength (nm)");
  filter_SDC_S->GetYaxis()->SetTitle("Count");

  TH1D *filter_SDC_C = new TH1D("filter_SDC_C", "Distribution of Cherenkov Photons in SDC Multiplied by Filter", 1250, 0., 1250.);
  filter_SDC_C->GetXaxis()->SetTitle("Wavelength (nm)");
  filter_SDC_C->GetYaxis()->SetTitle("Count");

  TH1D *filter_SDS_S = new TH1D("filter_SDS_S", "Distribution of Scintillation Photons in SDS Multiplied by Filter", 1250, 0., 1250.);
  filter_SDS_S->GetXaxis()->SetTitle("Wavelength (nm)");
  filter_SDS_S->GetYaxis()->SetTitle("Transmission (%)");

  TH1D *filter_SDS_C = new TH1D("filter_SDS_C", "Distribution of Cherenkov Photons in SDS Multiplied by Filter", 1250, 0., 1250.);
  filter_SDS_C->GetXaxis()->SetTitle("Wavelength (nm)");
  filter_SDS_C->GetYaxis()->SetTitle("Transmission (%)");

  TGraph *glass_graph = new TGraph("pyrex_glass.txt", "%lg %lg");
  glass_graph->SetTitle("Transmission of HOYA R60 Filter");
  glass_graph->GetXaxis()->SetTitle("Wavelength (nm)");
  glass_graph->GetYaxis()->SetTitle("Transmission (%)");

  fstream file1;
  file1.open("hoya_R60.txt", ios::in); // Used for PWO
  // file1.open("hoya_U330.txt", ios::in); // Used for BGO
  
  int counter1 = 0;
  while (counter1 < 96) {
    double x1, y1;
    file1 >> x1 >> y1;
    y1 /= 100.0;
    glass_graph->SetPoint(counter1, x1, y1);
    counter1++;
  }
  file1.close();
  
  TCanvas *c47 = new TCanvas();
  glass_graph->Draw("AL");
  c47->Print(sample + "glass_graph.png");
  
  /* This for loop multiplies each bin of the existing wavelength distribution by the value of the PDE evaluated at each wavelength.
     It then fills the histogram with those values and normalizes the new histograms. Since the PDE is defined only for specific
     wavelengths, I multiplied each histogram bin by 0 if we were out of the range of the filter (like with the PDE). */
  double_t filter_scale_bin_SDC_S = 0, filter_scale_bin_SDS_S = 0, filter_scale_bin_SDC_C = 0, filter_scale_bin_SDS_C = 0;
  double_t filter_scale_bin_front_S = 0, filter_scale_bin_rear_S = 0, filter_scale_bin_front_C = 0, filter_scale_bin_rear_C = 0;
  for (int i = 0; i <= 1250; i++) {
    if (i > 200 && i < 1200) {
      filter_scale_bin_SDC_S = glass_graph->Eval(combined_SDC_S->GetBinCenter(i));
      filter_scale_bin_SDC_C = glass_graph->Eval(combined_SDC_C->GetBinCenter(i));
      filter_scale_bin_SDS_S = glass_graph->Eval(combined_SDS_S->GetBinCenter(i));
      filter_scale_bin_SDS_C = glass_graph->Eval(combined_SDS_C->GetBinCenter(i));
      filter_scale_bin_front_S = glass_graph->Eval(combined_front_S->GetBinCenter(i));
      filter_scale_bin_front_C = glass_graph->Eval(combined_front_C->GetBinCenter(i));
    } else {
      filter_scale_bin_SDC_S = 0;
      filter_scale_bin_SDC_C = 0;
      filter_scale_bin_SDS_S = 0;
      filter_scale_bin_SDS_C = 0;
      filter_scale_bin_front_S = 0;
      filter_scale_bin_rear_S = 0;
      filter_scale_bin_front_C = 0;
      filter_scale_bin_rear_C = 0;
    }
    filter_SDC_S->SetBinContent(i, filter_scale_bin_SDC_S * combined_SDC_S->GetBinContent(i));
    filter_SDC_C->SetBinContent(i, filter_scale_bin_SDC_C * combined_SDC_C->GetBinContent(i));
    filter_SDS_S->SetBinContent(i, filter_scale_bin_SDS_S * combined_SDS_S->GetBinContent(i));
    filter_SDS_C->SetBinContent(i, filter_scale_bin_SDS_C * combined_SDS_C->GetBinContent(i));
    filter_front_S->SetBinContent(i, filter_scale_bin_front_S * combined_front_S->GetBinContent(i));
    filter_front_C->SetBinContent(i, filter_scale_bin_front_C * combined_front_C->GetBinContent(i));
  }
  
  // These TCanvases plot the normalized histograms with the original ones plotted on the same canvas.                                       
  TCanvas *c48 = new TCanvas();            
  combined_SDC_S->SetLineColor(4);        
  combined_SDC_S->SetFillStyle(3001);    
  filter_SDC_S->SetLineColor(2);
  filter_SDC_S->SetFillStyle(3001);
  combined_SDC_S->GetYaxis()->SetTitle("Count");
  combined_SDC_S->SetTitle("Wavelengths of S Photons from SDC Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  // combined_SDC_S->Draw("HIST");                                                    
  // filter_SDC_S->Draw("HIST SAME");                                                
  filter_SDC_S->SetTitle("Wavelength Distribution of S Photons in SDC with and without Filter Applied");
  filter_SDC_S->Draw("HIST");                                                  
  combined_SDC_S->Draw("HIST SAME");                                            
  TLegend *filter_SDC_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);                
  filter_SDC_S_legend->SetHeader("KEY", "C");                                    
  filter_SDC_S_legend->AddEntry(filter_SDC_S, "Filter", "l")->SetTextColor(2);
  filter_SDC_S_legend->AddEntry(combined_SDC_S, "PDE", "l")->SetTextColor(4);
  filter_SDC_S_legend->SetBorderSize(5);
  filter_SDC_S_legend->Draw();
  c48->Print(sample + normalized + "filter_SDC_S.png");       

  TCanvas *c49 = new TCanvas();
  combined_SDC_C->SetLineColor(4);
  combined_SDC_C->SetFillStyle(3001);
  filter_SDC_C->SetLineColor(2);
  filter_SDC_C->SetFillStyle(3001);
  combined_SDC_C->GetYaxis()->SetTitle("Count");
  combined_SDC_C->SetTitle("Wavelengths of C Photons from SDC Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data. 
  // combined_SDC_C->Draw("HIST");
  // filter_SDC_C->Draw("HIST SAME");
  filter_SDC_C->SetTitle("Wavelength Distribution of C Photons in SDC with and without Filter Applied");
  filter_SDC_C->Draw("HIST"); 
  combined_SDC_C->Draw("HIST SAME");
  TLegend *filter_SDC_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  filter_SDC_C_legend->SetHeader("KEY", "C");
  filter_SDC_C_legend->AddEntry(filter_SDC_C, "Filter", "l")->SetTextColor(2);
  filter_SDC_C_legend->AddEntry(combined_SDC_C, "PDE", "l")->SetTextColor(4);
  filter_SDC_C_legend->SetBorderSize(5);
  filter_SDC_C_legend->Draw();
  c49->Print(sample + normalized + "filter_SDC_C.png");

  TCanvas *c50 = new TCanvas();
  combined_SDS_S->SetLineColor(4);
  combined_SDS_S->SetFillStyle(3001);
  filter_SDS_S->SetLineColor(2);
  filter_SDS_S->SetFillStyle(3001);
  combined_SDS_S->GetYaxis()->SetTitle("Count");
  combined_SDS_S->SetTitle("Wavelengths of S Photons from SDS Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  // combined_SDS_S->Draw("HIST");
  // filter_SDS_S->Draw("HIST SAME");
  filter_SDS_S->SetTitle("Wavelength Distribution of S Photons in SDS with and without Filter Applied");
  filter_SDS_S->Draw("HIST");    
  combined_SDS_S->Draw("HIST SAME");
  TLegend *filter_SDS_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  filter_SDS_S_legend->SetHeader("KEY", "C");
  filter_SDS_S_legend->AddEntry(filter_SDS_S, "Filter", "l")->SetTextColor(2);
  filter_SDS_S_legend->AddEntry(combined_SDS_S, "PDE", "l")->SetTextColor(4);
  filter_SDS_S_legend->SetBorderSize(5);
  filter_SDS_S_legend->Draw();
  c50->Print(sample + normalized + "filter_SDS_S.png");

  TCanvas *c51 = new TCanvas();
  combined_SDS_C->SetLineColor(4);
  combined_SDS_C->SetFillStyle(3001);
  filter_SDS_C->SetLineColor(2);
  filter_SDS_C->SetFillStyle(3001);
  combined_SDS_C->GetYaxis()->SetTitle("Count");
  combined_SDS_C->SetTitle("Wavelengths of C Photons from SDS Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  // combined_SDS_C->Draw("HIST");
  // filter_SDS_C->Draw("HIST SAME");
  filter_SDS_C->SetTitle("Wavelength Distribution of C Photons in SDS with and without Filter Applied");
  filter_SDS_C->Draw("HIST");       
  combined_SDS_C->Draw("HIST SAME");
  TLegend *filter_SDS_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  filter_SDS_C_legend->SetHeader("KEY", "C");
  filter_SDS_C_legend->AddEntry(filter_SDS_C, "Filter", "l")->SetTextColor(2);
  filter_SDS_C_legend->AddEntry(combined_SDS_C, "PDE", "l")->SetTextColor(4);
  filter_SDS_C_legend->SetBorderSize(5);
  filter_SDS_C_legend->Draw();
  c51->Print(sample + normalized + "filter_SDS_C.png");



  // ...........................................................................................                                                                                 
  // ............................PART 8: PDE AND FILTER CORRECTION.............................   
  // ...........................................................................................



  /* This section is a little bit all over the place, but the main goal is to do analysis and find the number of photons per GeV with the PDe
     and the filter data combined. This is done by taking the ratio of the integral of the wavelength distributions with both applied (i.e.
     multiplying each bin by the PDE and filter distributions) to the integral of the original wavenelength distributions. Then, I multiplied
     the number of photons per GeV by this ratio to get the more accurate counts. *.

  /* These first few "paragraphs" of code deal with the ratios of the correcitons for the PDE, the filter, and both the PDE and the filter. 
     They are just printed to the screen right now, but the combined correction is used throughout this section to get more accurate results 
     to real life. For the filter correction, I have to define new histograms since I accounted for the PDE in my original filter distributions. */
  
  // PDE only
  cout << "PDE ONLY" << endl;
  cout << combined_SDC_S->Integral("width") / SDC_S_lambdas->Integral("width") << endl;
  cout << combined_SDC_C->Integral("width") / SDC_C_lambdas->Integral("width") << endl;
  cout << combined_SDS_S->Integral("width") / SDS_S_lambdas->Integral("width") << endl;
  cout << combined_SDS_C->Integral("width") / SDS_C_lambdas->Integral("width") << endl;

  // Filter only
  TH1D *only_filter_SDC_S = new TH1D("only_filter_SDC_S", "Distribution of Scintillation Photons in SDC Multiplied by Filter", 1250, 0., 1250.);
  only_filter_SDC_S->GetXaxis()->SetTitle("Wavelength (nm)");
  only_filter_SDC_S->GetYaxis()->SetTitle("Count");

  TH1D *only_filter_SDC_C = new TH1D("only_filter_SDC_C", "Distribution of Cherenkov Photons in SDC Multiplied by Filter", 1250, 0., 1250.);
  only_filter_SDC_C->GetXaxis()->SetTitle("Wavelength (nm)");
  only_filter_SDC_C->GetYaxis()->SetTitle("Count");

  TH1D *only_filter_SDS_S = new TH1D("only_filter_SDS_S", "Distribution of Scintillation Photons in SDS Multiplied by Filter", 1250, 0., 1250.);
  only_filter_SDS_S->GetXaxis()->SetTitle("Wavelength (nm)");
  only_filter_SDS_S->GetYaxis()->SetTitle("Transmission (%)");

  TH1D *only_filter_SDS_C = new TH1D("only_filter_SDS_C", "Distribution of Cherenkov Photons in SDS Multiplied by Filter", 1250, 0., 1250.);
  only_filter_SDS_C->GetXaxis()->SetTitle("Wavelength (nm)");
  only_filter_SDS_C->GetYaxis()->SetTitle("Transmission (%)");

  double_t only_filter_scale_bin_SDC_S = 0, only_filter_scale_bin_SDS_S = 0, only_filter_scale_bin_SDC_C = 0, only_filter_scale_bin_SDS_C = 0;
  for (int i = 0; i <= 1250; i++) {
    if (i > 200 && i < 1200) {
      only_filter_scale_bin_SDC_S = glass_graph->Eval(SDC_S_lambdas->GetBinCenter(i));
      only_filter_scale_bin_SDC_C = glass_graph->Eval(SDC_C_lambdas->GetBinCenter(i));
      only_filter_scale_bin_SDS_S = glass_graph->Eval(SDS_S_lambdas->GetBinCenter(i));
      only_filter_scale_bin_SDS_C = glass_graph->Eval(SDS_C_lambdas->GetBinCenter(i));
    } else {
      only_filter_scale_bin_SDC_S = 0;
      only_filter_scale_bin_SDC_C = 0;
      only_filter_scale_bin_SDS_S = 0;
      only_filter_scale_bin_SDS_C = 0;
    }
    only_filter_SDC_S->SetBinContent(i, only_filter_scale_bin_SDC_S * SDC_S_lambdas->GetBinContent(i));
    only_filter_SDC_C->SetBinContent(i, only_filter_scale_bin_SDC_C * SDC_C_lambdas->GetBinContent(i));
    only_filter_SDS_S->SetBinContent(i, only_filter_scale_bin_SDS_S * SDS_S_lambdas->GetBinContent(i));
    only_filter_SDS_C->SetBinContent(i, only_filter_scale_bin_SDS_C * SDS_C_lambdas->GetBinContent(i));
  }

  cout << "FILTER ONLY" << endl;
  cout << only_filter_SDC_S->Integral("width") / SDC_S_lambdas->Integral("width") << endl;
  cout << only_filter_SDC_C->Integral("width") / SDC_C_lambdas->Integral("width") << endl;
  cout << only_filter_SDS_S->Integral("width") / SDS_S_lambdas->Integral("width") << endl;
  cout << only_filter_SDS_C->Integral("width") / SDS_C_lambdas->Integral("width") << endl;

  // Both
  double ratio_SDC_S = filter_SDC_S->Integral("width") / SDC_S_lambdas->Integral("width");
  double ratio_SDC_C = filter_SDC_C->Integral("width") / SDC_C_lambdas->Integral("width");
  double ratio_SDS_S = filter_SDS_S->Integral("width") / SDS_S_lambdas->Integral("width");
  double ratio_SDS_C = filter_SDS_C->Integral("width") / SDS_C_lambdas->Integral("width");

  cout << "BOTH" << endl;
  cout << ratio_SDC_S << " " << ratio_SDC_C << " " << ratio_SDS_S << " " << ratio_SDS_C << endl;

  /* We're interested in finding the number of photons per GeV that are detected in the SiPMs so that we can                                                                     
     determine whether to implement dual-readout calorimetry. This section deals with that. */

  /* These first two histograms simply plot total number of C and S photons, respectively, as a function of the deposited                                                        
     ion energy on the rear face of the crystal. The plots exhibited a somewhat linear relationship, so finding the                                                              
     slope of the best fit lines would give us a number for the number of photons per GeV. */
  TProfile *C_SDC_dep_en = new TProfile("C_SDC_dep_en", "C Photons in SDC vs. E_ion_dep", 100, 0, 30);
  C_SDC_dep_en->GetXaxis()->SetTitle("Deposited Ion Energy (GeV)");
  C_SDC_dep_en->GetYaxis()->SetTitle("Average Count");

  TProfile *S_SDC_dep_en = new TProfile("S_SDC_dep_en", "S Photons in SDC vs. E_ion_dep", 100, 0, 30);
  S_SDC_dep_en->GetXaxis()->SetTitle("Deposited Ion Energy (GeV)");
  S_SDC_dep_en->GetYaxis()->SetTitle("Average Count");

  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    if (SDCdetected_r_C < 6000) C_SDC_dep_en->Fill(depositedIonEnergyECAL_r, float(SDCdetected_r_C * ratio_SDC_C));
    if (SDCdetected_r_S < 140000) {
      if (((depositedIonEnergyECAL_r > 20) && (SDCdetected_r_S < 58000)) || ((depositedIonEnergyECAL_r > 25) && (SDCdetected_r_S < 80000))) {
        continue;
      } else {
        S_SDC_dep_en->Fill(depositedIonEnergyECAL_r, float(SDCdetected_r_S * ratio_SDC_S));
      }
    }
  }

  S_SDC_dep_en->Fit("pol1");
  TF1 *linear_fit1 = S_SDC_dep_en->GetFunction("pol1");

  C_SDC_dep_en->Fit("pol1");
  TF1 *linear_fit2 = C_SDC_dep_en->GetFunction("pol1");

  TCanvas *c52 = new TCanvas();;
  S_SDC_dep_en->Draw();
  linear_fit1->Draw("SAME");
  c52->Print(sample + "S_SDC_dep_en.png");

  TCanvas *c53 = new TCanvas();
  C_SDC_dep_en->Draw();
  linear_fit2->Draw("SAME");
  c753->Print(sample + "C_SDC_dep_en.png");

  /* These histograms plot the number of each photon per GeV on each axis for MIPs  and showers only.                                                                     
     The z-axis shows the number of events. I also made LOG-LOG plots of the data.  */
  TH2D *C_vs_S100cut = new TH2D("C_vs_S100cut", "C Photons per GeV vs. S Photons per GeV for Showers in SDC", 10, 0, 10, 120, 0, 120);
  C_vs_S100cut->GetXaxis()->SetTitle("Count of S Photons per GeV");
  C_vs_S100cut->GetYaxis()->SetTitle("Count of C Photons per GeV");

  TH2D *C_vs_S_LOG100cut = new TH2D("C_vs_S_LOG100cut", "LOG-LOG of C Photons per GeV vs. S Photons per GeV for Showers in SDC", 25, 0, 10.5, 25, 0, 10.5);
  C_vs_S_LOG100cut->GetXaxis()->SetTitle("LOG of Count of S Photons per GeV");
  C_vs_S_LOG100cut->GetYaxis()->SetTitle("LOG of Count of C Photons per Gev");

  TH2D *C_vs_Snocut = new TH2D("C_vs_Snocut", "C Photons per GeV vs. S Photons per GeV for MIPs in SDC", 10, 0, 10, 120, 0, 120);
  C_vs_Snocut->GetXaxis()->SetTitle("Count of S Photons per GeV");
  C_vs_Snocut->GetYaxis()->SetTitle("Count of C Photons per GeV");

  TH2D *C_vs_S_LOGnocut = new TH2D("C_vs_S_LOGnocut", "LOG-LOG of C Photons per GeV vs. S Photons per GeV for MIPs in SDC", 25, 0, 10.5, 25, 0, 10.5);
  C_vs_S_LOGnocut->GetXaxis()->SetTitle("LOG of Count of S Photons per GeV");
  C_vs_S_LOGnocut->GetYaxis()->SetTitle("LOG of Count of C Photons per GeV");

  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    if (depositedIonEnergyECAL_r <= 0.1) {
      C_vs_Snocut->Fill(float(SDCdetected_r_S * ratio_SDC_S)/depositedIonEnergyECAL_r, float(SDCdetected_r_C * ratio_SDC_C)/depositedIonEnergyECAL_r);
      C_vs_S_LOGnocut->Fill(log(float(SDCdetected_r_S * ratio_SDC_S)/depositedIonEnergyECAL_r), log(float(SDCdetected_r_C * ratio_SDC_C)/depositedIonEnergyECAL_r));
    }
    if (depositedIonEnergyECAL_r > 0.1) {
      C_vs_S100cut->Fill(float(SDCdetected_r_S * ratio_SDC_S)/depositedIonEnergyECAL_r, float(SDCdetected_r_C * ratio_SDC_C)/depositedIonEnergyECAL_r);
      C_vs_S_LOG100cut->Fill(log(float(SDCdetected_r_S * ratio_SDC_S)/depositedIonEnergyECAL_r), log(float(SDCdetected_r_C * ratio_SDC_C)/depositedIonEnergyECAL_r));
    }
  }

  TCanvas *c54 = new TCanvas();
  C_vs_Snocut->Draw("colz");
  c54->Print(sample + "linear_count_plots_nocut.png");

  TCanvas *c55 = new TCanvas();
  C_vs_S_LOGnocut->Draw("colz");
  c55->Print(sample + "log_count_plots_nocut.png");

  TCanvas *c56 = new TCanvas();
  C_vs_S100cut->Draw("colz");
  c56->Print(sample + "linear_count_plots_100cut.png");

  TCanvas *c57 = new TCanvas();    
  C_vs_S_LOG100cut->Draw("colz");
  c57->Print(sample + "log_count_plots_100cut.png");

  // These histograms take the x- and y-cuts of the previously made C vs. S plot for showers to find the most probable number of photons per GeV.                                
  TH1F *S_cut = new TH1F("S_cut", "Most Probable Number of S Photons per GeV for MIPs", 10, 0.1, 10);
  S_cut->GetXaxis()->SetTitle("Count of S Photons per GeV");
  S_cut->GetYaxis()->SetTitle("Events");

  TH1F *C_cut = new TH1F("C_cut", "Most Probable Number of C Photons per GeV for MIPs", 120, 0.1, 120);
  C_cut->GetXaxis()->SetTitle("Count of C Photons per GeV");
  C_cut->GetYaxis()->SetTitle("Events");

  TH1F *S_cut_shower = new TH1F("S_cut_shower", "Most Probable Number of S Photons per GeV for Showers", 10, 0.1, 10);
  S_cut_shower->GetXaxis()->SetTitle("Count of S Photons per GeV");
  S_cut_shower->GetYaxis()->SetTitle("Events");

  TH1F *C_cut_shower = new TH1F("C_cut_shower", "Most Probable Number of C Photons per GeV for Showers", 120, 0.1, 120);
  C_cut_shower->GetXaxis()->SetTitle("Count of C Photons per GeV");
  C_cut_shower->GetYaxis()->SetTitle("Events");

  for (int i = 0; i < 120; i++) C_cut->Fill(i, C_vs_Snocut->GetBinContent(2, i));
  for (int i = 0; i < 120; i++) C_cut_shower->Fill(i, C_vs_S100cut->GetBinContent(2, i));
  for (int i = 0; i < 10; i++) S_cut->Fill(i, C_vs_Snocut->GetBinContent(i, 3));
  for (int i = 0; i < 10; i++) S_cut_shower->Fill(i, C_vs_S100cut->GetBinContent(i, 10));
    
  cout << "MIPS AND SHOWERS" << endl;
  cout << "MOST PROBABLE C: " << C_cut->GetMaximumBin() << " " << "MOST PROBABLE S: " << S_cut->GetMaximumBin() << endl;
  cout << "MOST PROBABLE C: " << C_cut_shower->GetMaximumBin() << " " << "MOST PROBABLE S: " << S_cut_shower->GetMaximumBin() << endl;

  TCanvas *c58 = new TCanvas();
  C_cut->Draw("HIST");
  c58->Print(sample + "C_cuts_LastPart.png");
  
  TCanvas *c59 = new TCanvas();
  S_cut->Draw("HIST");
  c59->Print(sample + "S_cuts_LastPart.png");

  TCanvas *c60 = new TCanvas();                                                                                                                                                  
  C_cut_shower->Draw("HIST");
  c60->Print(sample + "C_cuts_LastPart_shower.png");

  TCanvas *c61 = new TCanvas();
  S_cut_shower->Draw("HIST");
  c61->Print(sample + "S_cuts_LastPart_shower.png");

  /* This section of code is exactly the same as in Part 4, but it includes the corrections due to the inaccuracies 
     in the SiPM detection as well as the filter data. I included it here because of how I organized my macro. */
  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    if (depositedEnergyECAL_r < 0.1) {
      avg_detected_photons_S_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_S * ratio_SDC_S)/depositedIonEnergyECAL_r);
      avg_detected_photons_C_SDC->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_C * ratio_SDC_C)/depositedIonEnergyECAL_r);
      avg_detected_photons_S_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_S * ratio_SDS_S)/depositedIonEnergyECAL_r);
      avg_detected_photons_C_SDS->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_C * ratio_SDS_C)/depositedIonEnergyECAL_r);
    }

    if (depositedEnergyECAL_r >= 0.1) {
      avg_S_SDC_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_S * ratio_SDC_S)/depositedIonEnergyECAL_r);
      avg_C_SDC_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_C * ratio_SDC_C)/depositedIonEnergyECAL_r);
      avg_S_SDS_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_S * ratio_SDS_S)/depositedIonEnergyECAL_r);
      avg_C_SDS_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_C * ratio_SDS_C)/depositedIonEnergyECAL_r);
    }
  }

  TCanvas *c62 = new TCanvas();
  avg_detected_photons_S_SDC->Draw("colz");
  c62->Print(sample + "avg_S_photons_SDC.png");

  TCanvas *c63 = new TCanvas();
  avg_detected_photons_C_SDC->Draw("colz");
  c63->Print(sample + "avg_C_photons_SDC.png");

  TCanvas *c64 = new TCanvas();
  avg_detected_photons_S_SDS->Draw("colz");
  c64->Print(sample + "avg_S_photons_SDS.png");

  TCanvas *c65 = new TCanvas();
  avg_detected_photons_C_SDS->Draw("colz");
  c65->Print(sample + "avg_C_photons_SDS.png");

  TCanvas *c66 = new TCanvas();
  avg_S_SDC_shower->Draw("colz");
  c66->Print(sample + "avg_S_SDC_shower.png");

  TCanvas *c67 = new TCanvas();
  avg_C_SDC_shower->Draw("colz");
  c67->Print(sample + "avg_C_SDC_shower.png");

  TCanvas *c68 = new TCanvas();
  avg_S_SDS_shower->Draw("colz");
  c68->Print(sample + "avg_S_SDS_shower.png");

  TCanvas *c69 = new TCanvas();
  avg_C_SDS_shower->Draw("colz");
  c69->Print(sample + "avg_C_SDS_shower.png");

  for (int i = 0; i < 100; i++) {
    x = i * (3.0/10.0) - 15.0;
    y = i * (3.0/10.0) - 15.0;
    x_center_S_SDC->Fill(y, avg_detected_photons_S_SDC->GetBinContent(50, i)); // Bin 50 corresponds to x = 0.  
    x_center_C_SDC->Fill(y, avg_detected_photons_C_SDC->GetBinContent(50, i));
    x_center_S_SDS->Fill(y, avg_detected_photons_S_SDS->GetBinContent(50, i));
    x_center_C_SDS->Fill(y, avg_detected_photons_C_SDS->GetBinContent(50, i));
    y_center_S_SDC->Fill(x, avg_detected_photons_S_SDC->GetBinContent(i, 72)); // Bin 72 corresponds to y = -6.5. 
    y_center_C_SDC->Fill(x, avg_detected_photons_C_SDC->GetBinContent(i, 72));
    y_center_S_SDS->Fill(x, avg_detected_photons_S_SDS->GetBinContent(i, 28)); // Bin 28 corresponds to y = 6.5.   
    y_center_C_SDS->Fill(x, avg_detected_photons_C_SDS->GetBinContent(i, 28));

    shower_x_center_S_SDC->Fill(y, avg_S_SDC_shower->GetBinContent(50, i));
    shower_x_center_C_SDC->Fill(y, avg_C_SDC_shower->GetBinContent(50, i));
    shower_x_center_S_SDS->Fill(y, avg_S_SDS_shower->GetBinContent(50, i));
    shower_x_center_C_SDS->Fill(y, avg_C_SDS_shower->GetBinContent(50, i));
    shower_y_center_S_SDC->Fill(y, avg_S_SDC_shower->GetBinContent(i, 72));
    shower_y_center_C_SDC->Fill(y, avg_C_SDC_shower->GetBinContent(i, 72));
    shower_y_center_S_SDS->Fill(y, avg_S_SDS_shower->GetBinContent(i, 28));
    shower_y_center_C_SDS->Fill(y, avg_C_SDS_shower->GetBinContent(i, 28));
  }

  fit1->GetParameters(&par1[0]);
  shfit1->GetParameters(&shpar1[0]);
  fit2->GetParameters(&par2[0]);
  shfit2->GetParameters(&shpar2[0]);
  fit3->GetParameters(&par3[0]);
  shfit3->GetParameters(&shpar3[0]);
  fit4->GetParameters(&par4[0]);
  shfit4->GetParameters(&shpar4[0]);
  fit5->GetParameters(&par5[0]);
  shfit5->GetParameters(&shpar5[0]);
  fit6->GetParameters(&par6[0]);
  shfit6->GetParameters(&shpar6[0]);
  fit7->GetParameters(&par7[0]);
  shfit7->GetParameters(&shpar7[0]);
  fit8->GetParameters(&par8[0]);
  shfit8->GetParameters(&shpar8[0]);

  x_center_S_SDC->Fit(fit1, "R");
  shower_x_center_S_SDC->Fit(shfit1,"R");
  x_center_C_SDC->Fit(fit2, "R");
  shower_x_center_C_SDC->Fit(shfit2, "R");
  x_center_S_SDS->Fit(fit3, "R");
  shower_x_center_S_SDS->Fit(shfit3, "R");
  x_center_C_SDS->Fit(fit4, "R");
  shower_x_center_C_SDS->Fit(shfit4, "R");
  y_center_S_SDC->Fit(fit5, "R");
  shower_y_center_S_SDC->Fit(shfit5,"R");
  y_center_C_SDC->Fit(fit6, "R");
  shower_y_center_C_SDC->Fit(shfit6, "R");
  y_center_S_SDS->Fit(fit7, "R");
  shower_y_center_S_SDS->Fit(shfit7, "R");
  y_center_C_SDS->Fit(fit8, "R");
  shower_y_center_C_SDS->Fit(shfit8, "R");

  TCanvas *c70 = new TCanvas();
  x_center_S_SDC->Draw("HIST");
  fit1->Draw("SAME");
  c70->Print(sample + "x_center_S_SDC.png");

  TCanvas *c71 = new TCanvas();
  shower_x_center_S_SDC->Draw("HIST");
  shfit1->Draw("SAME");
  c71->Print(sample + "shower_x_center_S_SDC.png");

  TCanvas *c72 = new TCanvas();
  x_center_C_SDC->Draw("HIST");
  fit2->Draw("SAME");
  c72->Print(sample + "x_center_C_SDC.png");

  TCanvas *c73 = new TCanvas();
  shower_x_center_C_SDC->Draw("HIST");
  shfit2->Draw("SAME");
  c73->Print(sample + "shower_x_center_C_SDC.png");

  TCanvas *c74 = new TCanvas();
  x_center_S_SDS->Draw("HIST");
  fit3->Draw("SAME");
  c74->Print(sample + "x_center_S_SDS.png");

  TCanvas *c75 = new TCanvas();
  shower_x_center_S_SDS->Draw("HIST");
  shfit3->Draw("SAME");
  c75->Print(sample + "shower_x_center_S_SDS.png");

  TCanvas *c76 = new TCanvas();
  x_center_C_SDS->Draw("HIST");
  fit4->Draw("SAME");
  c76->Print(sample + "x_center_C_SDS.png");

  TCanvas *c77 = new TCanvas();
  shower_x_center_C_SDS->Draw("HIST");
  shfit4->Draw("SAME");
  c77->Print(sample + "shower_x_center_C_SDS.png");

  TCanvas *c78 = new TCanvas();
  y_center_S_SDC->Draw("HIST");
  fit5->Draw("SAME");
  c78->Print(sample + "y_center_S_SDC.png");

  TCanvas *c79 = new TCanvas();
  shower_y_center_S_SDC->Draw("HIST");
  shfit5->Draw("SAME");
  c79->Print(sample + "shower_y_center_S_SDC.png");

  TCanvas *c80 = new TCanvas();
  y_center_C_SDC->Draw("HIST");
  fit6->Draw("SAME");
  c80->Print(sample + "y_center_C_SDC.png");

  TCanvas *c81 = new TCanvas();
  shower_y_center_C_SDC->Draw("HIST");
  shfit6->Draw("SAME");
  c81->Print(sample + "shower_y_center_C_SDC.png");

  TCanvas *c82 = new TCanvas();
  y_center_S_SDS->Draw("HIST");
  fit7->Draw("SAME");
  c82->Print(sample + "y_center_S_SDS.png");

  TCanvas *c83 = new TCanvas();
  shower_y_center_S_SDS->Draw("HIST");
  shfit7->Draw("SAME");
  c83->Print(sample + "shower_y_center_S_SDS.png");

  TCanvas *c84 = new TCanvas();
  y_center_C_SDS->Draw("HIST");
  fit8->Draw("SAME");
  c84->Print(sample + "y_center_C_SDS.png");

  TCanvas *c85 = new TCanvas();
  shower_y_center_C_SDS->Draw("HIST");
  shfit8->Draw("SAME");
  c85->Print(sample + "shower_y_center_C_SDS.png");

  cout << "COUNTS OF PHOTONS PER GEV IN CENTERS OF SIPMS" << endl;
  cout << 0.5 * (fit1->GetMaximum() + fit5->GetMaximum()) << " " << 0.5 * (shfit1->GetMaximum() + shfit5->GetMaximum()) << endl;
  cout << 0.5 * (fit2->GetMaximum() + fit6->GetMaximum()) << " " << 0.5 * (shfit2->GetMaximum() + shfit6->GetMaximum()) << endl;
  cout << 0.5 * (fit3->GetMaximum() + fit7->GetMaximum()) << " " << 0.5 * (shfit3->GetMaximum() + shfit7->GetMaximum()) << endl;
  cout << 0.5 * (fit4->GetMaximum() + fit8->GetMaximum()) << " " << 0.5 * (shfit4->GetMaximum() + shfit8->GetMaximum()) << endl;

  return 0;  
}

/*  Code written by Christian Guinto-Brody for Bob Hirosky's High-Energy-Physics group with his and his colleagues' help.
    Special thanks to Bob, Alexander "Sasha" Ledovskoy, Grace Cummings, Chris Martin, and Max Dubnowsky for their input
    and guidance, as well as to Fermilab, the NSF, and the University of Virginia for funding this research. */
