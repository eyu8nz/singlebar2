#include <vector>
using std::vector;

int REAL_proton_test() { 



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
  float depositedEnergyECAL_f; // Deposited energy in front SiPM
  float depositedIonEnergyECAL_r; // Deposited energy of ions from proton beam. This one is VERY similar to detectedEnergyECAL_r
  float depositedIonEnergyECAL_f; // Deposited ionization energy in front SiPM
  vector<float> *inputInitialPosition = new vector<float>(3, 0.); // Positions of protons from beam  on xy-plane
  int SDCdetected_r_S; // Number of scintillation photons detected in SDC
  int SDCdetected_r_C; // Number of Cherenkov photons detected in SDC
  int SDSdetected_r_S; // Number of scintillation photons detected in SDS
  int SDSdetected_r_C; // Number of Cherenkov photons detected in SDS
  int SDFdetected_f_S; // Number of scintillation photons detected in front SiPM
  int SDFdetected_f_C; // Number of Cherenkov photons detected in front SiPM
  float SDCtime_r_S;
  float SDCtime_r_C;
  float SDStime_r_S;
  float SDStime_r_C;
  
  // Setting branch addresses of each variable. These are used for the histograms.
  chain->SetBranchAddress("depositedEnergyECAL_r", &depositedEnergyECAL_r);
  chain->SetBranchAddress("depositedEnergyECAL_f", &depositedEnergyECAL_f);
  chain->SetBranchAddress("depositedIonEnergyECAL_r", &depositedIonEnergyECAL_r);
  chain->SetBranchAddress("depositedIonEnergyECAL_f", &depositedIonEnergyECAL_f);
  chain->SetBranchAddress("inputInitialPosition", &inputInitialPosition);
  chain->SetBranchAddress("SDCdetected_r_S", &SDCdetected_r_S);
  chain->SetBranchAddress("SDCdetected_r_C", &SDCdetected_r_C);
  chain->SetBranchAddress("SDSdetected_r_S", &SDSdetected_r_S);
  chain->SetBranchAddress("SDSdetected_r_C", &SDSdetected_r_C);
  chain->SetBranchAddress("SDFdetected_f_S", &SDFdetected_f_S);
  chain->SetBranchAddress("SDFdetected_f_C", &SDFdetected_f_C);
  chain->SetBranchAddress("SDCtime_r_S", &SDCtime_r_S);
  chain->SetBranchAddress("SDCtime_r_C", &SDCtime_r_C);
  chain->SetBranchAddress("SDStime_r_S", &SDStime_r_S);
  chain->SetBranchAddress("SDStime_r_C", &SDStime_r_C);

  /* These histograms plot the wavelength distributions of each type of photon in each SiPM.
     They take the data from the corresponding histograms defined in CreateTree.hh and CreateTree.cc. */
  TH1D *SDC_S_lambdas = new TH1D("SDC_S_lambdas", "Wavelengths of Scintillation Photons in SDC", 1250, 0., 1250.);
  SDC_S_lambdas->GetXaxis()->SetTitle("Wavelength (nm)");
  SDC_S_lambdas->GetYaxis()->SetTitle("Count");

  TH1D *SDC_C_lambdas = new TH1D("SDC_C_lambdas", "Wavelengths of Cherenkov Photons in SDC", 1250, 0., 1250.);
  SDC_C_lambdas->GetXaxis()->SetTitle("Wavelength (nm)");
  SDC_C_lambdas->GetYaxis()->SetTitle("Count");

  TH1D *SDS_S_lambdas = new TH1D("SDS_S_lambdas", "Wavelengths of Scintillation Photons in SDS", 1250, 0., 1250.);
  SDS_S_lambdas->GetXaxis()->SetTitle("Wavelength (nm)");
  SDS_S_lambdas->GetYaxis()->SetTitle("Count");

  TH1D *SDS_C_lambdas = new TH1D("SDS_C_lambdas", "Wavelengths of Cherenkov Photons in SDS", 1250, 0., 1250.);
  SDS_C_lambdas->GetXaxis()->SetTitle("Wavelength (nm)");
  SDS_C_lambdas->GetYaxis()->SetTitle("Count");

  /* These histograms plot the generated wavelength distirbutions of each type of photon in 
     each SiPM. We want to see a smooth distribution as opposed to the double-peaked ones we
     see for the detected distributions. */
  TH1D *front_S_generated = new TH1D("front_S_generated", "Wavelengths of Generated Scintillation Photons in Front", 1250, 0., 1250.);
  front_S_generated->GetXaxis()->SetTitle("Wavelength (nm)");
  front_S_generated->GetYaxis()->SetTitle("Count");

  TH1D *rear_S_generated = new TH1D("rear_S_generated", "Wavelengths of Generated Scintillation Photons in Rear", 1250, 0., 1250.);
  rear_S_generated->GetXaxis()->SetTitle("Wavelength (nm)");
  rear_S_generated->GetYaxis()->SetTitle("Count");

  TH1D *front_C_generated = new TH1D("front_C_generated", "Wavelengths of Generated Cherenkov Photons in Front", 1250, 0., 1250.);
  front_C_generated->GetXaxis()->SetTitle("Wavelength (nm)");
  front_C_generated->GetYaxis()->SetTitle("Count");

  TH1D *rear_C_generated = new TH1D("rear_C_generated", "Wavelengths of Gerenated Cherenkov Photons in Rear", 1250, 0., 1250.);
  rear_C_generated->GetXaxis()->SetTitle("Wavelength (nm)");
  rear_C_generated->GetYaxis()->SetTitle("Count");

  TH1D *front_S_detected = new TH1D("front_S_detected", "Wavelengths of Detected Scintillation Photons in Front", 1250, 0., 1250.);
  front_S_detected->GetXaxis()->SetTitle("Wavelength (nm)");
  front_S_detected->GetYaxis()->SetTitle("Count");

  TH1D *front_C_detected = new TH1D("front_C_detected","Wavelengths of Detected Cherenkov Photons in Front", 1250, 0., 1250.);
  front_C_detected->GetXaxis()->SetTitle("Wavelength (nm)");
  front_C_detected->GetYaxis()->SetTitle("Count");



  // ........................................................................................... 
  // .........................PART 2: PLOTTING WAVELENGTH DISTRIBUTIONS.........................
  // ........................................................................................... 



  /* The TStrings filename, histname(s), and hist(s) are meant to distinguish between the defined histograms                                                                     
     so I can add the data from all the ROOT files together and plot them in one historgam. */
  TString filename = "";
  TString histnameDet = "", histnameGen = ""; // histnameDet deals with the detected wavelenghts; histnameGen deals with the generated wavelengths
  TString histnameOther = ""; // histnameOther is for the detected wavelengths in the front SiPM. I just realized I may have been incorrect the whole time...

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

    // Not going to lie, the if statements look somewhat clanky, but my syntax is valid. At least, I believe...
    for (int j = 0; j < 4; j++) {
      TFile *file = new TFile(filename);
      if (j == 0) histnameDet = "h_phot_lambda_SiPMC_r_Scin", histnameGen = "h_phot_lambda_ECAL_f_produce_Scin";
      else if (j == 1) histnameDet = "h_phot_lambda_SiPMC_r_Ceren", histnameGen = "h_phot_lambda_ECAL_r_produce_Scin";
      else if (j == 2) histnameDet = "h_phot_lambda_SiPMS_r_Scin", histnameGen = "h_phot_lambda_ECAL_f_produce_Ceren";
      else if (j == 3) histnameDet = "h_phot_lambda_SiPMS_r_Ceren", histnameGen = "h_phot_lambda_ECAL_r_produce_Ceren";
      TH1F *histDet = (TH1F*)file->Get(histnameDet);
      TH1D *histGen = (TH1D*)file->Get(histnameGen);
      if (j == 0) SDC_S_lambdas->Add(histDet), front_S_generated->Add(histGen);
      else if (j == 1) SDC_C_lambdas->Add(histDet), rear_S_generated->Add(histGen);
      else if (j == 2) SDS_S_lambdas->Add(histDet), front_C_generated->Add(histGen);
      else if (j == 3) SDS_C_lambdas->Add(histDet), rear_C_generated->Add(histGen);
    }

    for (int k = 0; k < 2; k++) {
      TFile *file = new TFile(filename);
      if (k == 0) histnameOther = "h_phot_lambda_SiPMF_f_Scin";
      else if (k == 1) histnameOther = "h_phot_lambda_SiPMF_f_Ceren";
      TH1D *histOther = (TH1D*)file->Get(histnameOther);
      if (k == 0) front_S_detected->Add(histOther);
      else if (k == 1) front_C_detected->Add(histOther);
    }
  }

  /* This section of code finds the wavelengths of each type of photon in each SiPM that are most detected
     (for the detected wavelengths) and finds the wavelengths of each type of photon in the front and rear
     (for the generated wavelengths). I also find the most abundant wavelength in each distribution (aka the peak). */
  float maxDetCountSSDC = 0.0, maxDetLambdaSSDC = 0.0, maxGenCountFrontS = 0.0, maxGenLambdaFrontS = 0.0;
  float maxDetCountCSDC = 0.0, maxDetLambdaCSDC = 0.0, maxGenCountRearS = 0.0, maxGenLambdaRearS = 0.0;
  float maxDetCountSSDS = 0.0, maxDetLambdaSSDS = 0.0, maxGenCountFrontC = 0.0, maxGenLambdaFrontC = 0.0;
  float maxDetCountCSDS = 0.0, maxDetLambdaCSDS = 0.0, maxGenCountRearC = 0.0, maxGenLambdaRearC = 0.0;
  float maxDetCountSSDF = 0.0, maxDetLambdaSSDF = 0.0, maxDetCountCSDF = 0.0, maxDetLambdaCSDF = 0.0;
  
  // These numbers will calculate the number of S and C photons with wavelengths greater than 600 nm and 650 nm, separately.
  float noSSDC600 = 0.0, noCSDC600 = 0.0, noSSDC650 = 0.0, noCSDC650 = 0.0;
  float totalSSDC = 0.0, totalCSDC = 0.0, fracS600 = 0.0, fracC600 = 0.0, fracS650 = 0.0, fracC650 = 0.0;
  
  for (int i = 0; i <= 1250; i ++) {
    totalSSDC += SDC_S_lambdas->GetBinContent(i);
    totalCSDC += SDC_C_lambdas->GetBinContent(i);
 
    // These fill the histograms for the detected wavelengths in the rear SiPMs.
    if (SDC_S_lambdas->GetBinContent(i) > maxDetCountSSDC) maxDetCountSSDC = SDC_S_lambdas->GetBinContent(i), maxDetLambdaSSDC = float(i);
    if (SDC_C_lambdas->GetBinContent(i) > maxDetCountCSDC) maxDetCountCSDC = SDC_C_lambdas->GetBinContent(i), maxDetLambdaCSDC = float(i);
    if (SDS_S_lambdas->GetBinContent(i) > maxDetCountSSDS) maxDetCountSSDS = SDS_S_lambdas->GetBinContent(i), maxDetLambdaSSDS = float(i);
    if (SDS_C_lambdas->GetBinContent(i) > maxDetCountCSDS) maxDetCountCSDS = SDC_C_lambdas->GetBinContent(i), maxDetLambdaCSDS = float(i);

    // These fill the histograms for the generated wavelengths.
    if (front_S_generated->GetBinContent(i) > maxGenCountFrontS) maxGenCountFrontS = front_S_generated->GetBinContent(i), maxGenLambdaFrontS = float(i);
    if (rear_S_generated->GetBinContent(i) > maxGenCountRearS) maxGenCountRearS = rear_S_generated->GetBinContent(i), maxGenLambdaRearS = float(i);
    if (front_C_generated->GetBinContent(i) > maxGenCountFrontC) maxGenCountFrontC = front_C_generated->GetBinContent(i), maxGenLambdaFrontC = float(i);
    if (rear_C_generated->GetBinContent(i) > maxGenCountRearC) maxGenCountRearC = rear_C_generated->GetBinContent(i), maxGenLambdaRearC = float(i);

    // These fill the histograms for the detected wavelengths in the front SiPM.
    if (front_S_detected->GetBinContent(i) > maxDetCountSSDF) maxDetCountSSDF = front_S_detected->GetBinContent(i), maxDetLambdaSSDF = float(i);
    if (front_C_detected->GetBinContent(i) > maxDetCountCSDF) maxDetCountCSDF = front_C_detected->GetBinContent(i), maxDetLambdaCSDF = float(i);
  
    if (i > 600) noSSDC600 += SDC_S_lambdas->GetBinContent(i), noCSDC600 += SDC_C_lambdas->GetBinContent(i);
    if (i > 650) noSSDC650 += SDC_S_lambdas->GetBinContent(i), noCSDC650 += SDC_C_lambdas->GetBinContent(i);
  }

  // These cout statements display the positions of the peaks for each SiPM and photon type.
  cout << "MOST ABUNDANT WAVELENGTH IN EACH SIPM FOR EACH PHOTON" << endl;
  cout << "       Scintillation        Cherenkov        " << endl;
  cout << "SDC:        " << maxDetLambdaSSDC << "                " << maxDetLambdaCSDC << endl;
  cout << "SDS:        " << maxDetLambdaSSDS << "                " << maxDetLambdaCSDS << endl;
  
  cout << " " << endl;
  
  cout << "MOST ABUNDANT GENERATED WAVELENGTH FOR FRONT AND REAR LOCATIONS FOR EACH PHOTON" << endl;
  cout << "       Scintillation        Cherenkov        " << endl;
  cout << "Front:      " << maxGenLambdaFrontS << "                " << maxGenLambdaFrontC << endl;
  cout << "Rear:       " << maxGenLambdaRearS << "                " << maxGenLambdaRearC << endl;

  cout << " " << endl;

  fracS600 = noSSDC600/totalSSDC;
  fracC600 = noCSDC600/totalCSDC;
  fracS650 = noSSDC650/totalSSDC;
  fracC650 = noCSDC650/totalCSDC;
  cout << fracS600 << " " << fracC600 << " " << fracS650 << " " << fracC650 << endl;

  // These canvases plot the wavelength distributions. The first four are for the detected wavelengths; the second four are for the generated wavelengths.
  // TCanvas *c1 = new TCanvas();
  // SDC_S_lambdas->Draw();
  // c1->Print(sample + "SDC_S_lambdas.png");

  // TCanvas *c2 = new TCanvas();
  // SDC_C_lambdas->Draw();
  // c2->Print(sample + "SDC_C_lambdas.png");
  
  // TCanvas *c3 = new TCanvas();
  // SDS_S_lambdas->Draw();
  // c3->Print(sample + "SDS_S_lambdas.png");

  // TCanvas *c4 = new TCanvas();
  // SDS_C_lambdas->Draw();
  // c4->Print(sample + "SDS_C_lambdas.png");

  // TCanvas *c5 = new TCanvas();
  // front_S_generated->Draw();
  // c5->Print(sample + "front_S_generated.png");

  // TCanvas *c6 = new TCanvas();
  // rear_S_generated->Draw();
  // c6->Print(sample + "rear_S_generated.png");

  // TCanvas *c7 = new TCanvas();
  // front_C_generated->Draw();
  // c7->Print(sample + "front_C_generated.png");

  // TCanvas *c8 = new TCanvas();
  // rear_C_generated->Draw();
  // c8->Print(sample + "rear_C_generated.png");

  // TCanvas *c9 = new TCanvas();
  // front_S_detected->Draw();
  // c9->Print(sample + "front_S_detected.png");

  // TCanvas *c10 = new TCanvas();
  // front_C_detected->Draw();
  // c10->Print(sample + "front_C_detected.png");


  
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
     compresses everything (effectively) so that we see where the peak is.*/
  TH1F *h_energy = new TH1F("h_energy", "Deposited Energy of Proton Beam", 100, 0.0, 30.0);
  h_energy->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  h_energy->GetYaxis()->SetTitle("Count");

  TH1F *h_energy_Landau = new TH1F("h_energy_Landau", "Deposited Energy of Proton Beam (From 0 to 0.2)", 50, 0.0, 0.2);
  h_energy_Landau->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  h_energy_Landau->GetYaxis()->SetTitle("Count");

  TH1F *h_energy_ion = new TH1F("h_energy_ion", "Deposited Energy of Ions from Proton Beam", 50, 0.0, 25.0);
  h_energy_ion->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  h_energy_ion->GetYaxis()->SetTitle("Count");

  TH1F *h_energy_LOG = new TH1F("h_energy_LOG", "Peak Energy of Proton Beam", 100, -3.0, 1.5);
  h_energy_LOG->GetXaxis()->SetTitle("Log_10 of Deposited Energy)");
  h_energy_LOG->GetYaxis()->SetTitle("Count");                                                    

  // This one deals with the locations of the protons when they reach the rear face of the detector.
  TH2F *pos_protons = new TH2F("pos_protons", "x- and y-Positions of Proton Beam", 25, -12.5, 12.5, 25, -12.5, 12.5);
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
  // avg_detected_photons_C_SDS->SetMaximum(77);

  TProfile2D *avg_detected_photons_S_SDF = new TProfile2D("avg_detected_photons_S_SDF", "Average Number of Detected S Photons per GeV in SDF", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_S_SDF->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_S_SDF->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_detected_photons_C_SDF = new TProfile2D("avg_detected_photons_C_SDF", "Average Number of Detected C Photons per GeV in SDF", 100, -15, 15, 100, -15, 15);
  avg_detected_photons_C_SDF->GetXaxis()->SetTitle("x-Position (mm)");
  avg_detected_photons_C_SDF->GetYaxis()->SetTitle("y-Position (mm)");

  /* And THESE four are very similar to the previous four, but they show the average count of                                                                                    
     each type of photon (for ONLY showers) in each SiPM. */
  TProfile2D *avg_S_SDC_shower = new TProfile2D("avg_S_SDC_shower", "Shower Detection of S per GeV in SDC", 100, -15, 15, 100, -15, 15);
  avg_S_SDC_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_S_SDC_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_C_SDC_shower = new TProfile2D("avg_C_SDC_shower", "Shower Detection of C per GeV in SDC", 100, -15, 15, 100, -15, 15);
  avg_C_SDC_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_C_SDC_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_S_SDS_shower = new TProfile2D("avg_S_SDS_shower", "Shower Detection of S per GeV in SDS", 100, -15, 15, 100, -15, 15);
  avg_S_SDS_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_S_SDS_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_C_SDS_shower = new TProfile2D("avg_C_SDS_shower", "Shower Detection of C per GeV in SDS", 100, -15, 15, 100, -15, 15);
  avg_C_SDS_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_C_SDS_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_S_SDF_shower = new TProfile2D("avg_S_SDF_shower", "Shower Detection of S per GeV in SDF", 100, -15, 15, 100, -15, 15);
  avg_S_SDF_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_S_SDF_shower->GetYaxis()->SetTitle("y-Position (mm)");

  TProfile2D *avg_C_SDF_shower = new TProfile2D("avg_C_SDF_shower", "Shower Detection of C per GeV in SDF", 100, -15, 15, 100, -15, 15);
  avg_C_SDF_shower->GetXaxis()->SetTitle("x-Position (mm)");
  avg_C_SDF_shower->GetYaxis()->SetTitle("y-Position (mm)");

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
      avg_detected_photons_S_SDF->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDFdetected_f_S)/depositedIonEnergyECAL_f);
      avg_detected_photons_C_SDF->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDFdetected_f_C)/depositedIonEnergyECAL_f);
    }

    if (depositedEnergyECAL_r >= 0.1) {
      avg_S_SDC_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_S)/depositedIonEnergyECAL_r);
      avg_C_SDC_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDCdetected_r_C)/depositedIonEnergyECAL_r);
      avg_S_SDS_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_S)/depositedIonEnergyECAL_r);
      avg_C_SDS_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDSdetected_r_C)/depositedIonEnergyECAL_r);
      avg_S_SDF_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDFdetected_f_S)/depositedIonEnergyECAL_f);
      avg_C_SDF_shower->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], float(SDFdetected_f_C)/depositedIonEnergyECAL_f);
    }

    if (depositedEnergyECAL_r > 0.1) noEDepTenth++;
  }  
  
  fracEDepTenth = float(noEDepTenth)/float(entries);
  cout << fracEDepTenth << endl;

  // TCanvas *test1 = new TCanvas();
  // test1->Divide(2, 1);
  // test1->cd(1);
  // h_energy->Draw("HIST");
  // test1->cd(2);
  // h_energy_LOG->Draw("HIST");
  // test1->Print("energy_plots.png");



  // ...........................................................................................  
  // .....................PART 4: TASKS DOLED OUT MONDAY, OCTOBER 30, 2023......................  
  // ...........................................................................................  



  /* I was asked the morning of Monday, October 30, 2023 to generate profiles of the number of detected
     scintillation photons in the front SiPM vs. the deposited ionization energy in both 1D and 2D. That
     evening, I was asked to generate more data and generate more plots. This section is dedicated to
     these tasks, which I will explain as I do them. */
 
  /* These are the TProfiles I created to complete the first of my tasks. The first is a 1D profile on which
     I plotted the number of scintillation photons detected in the front SiPM vs. the deposited ionization energy.
     For this plot, I put the deposited ionization energy on the x-axis and weighted the profile with the log
     of the number of detected photons (the log made the plot more interesting/easier to look at). The second is 
     a 2D profile plotting the same thing, but with a color map. For this plot, I put the deposited energy on the
     x-axis, the number of detected photons on the y-axis (no log this time), and weighted the profile by the ratio
     of the number of photons and the deposited ionization energy. */
  TProfile *front_S_photons_1D = new TProfile("front_S_photons_1D", "Average Number of Scintillation Photons vs. Deposited Ionization Energy", 50, 0, 30.0);
  front_S_photons_1D->GetXaxis()->SetTitle("Deposited Ionization Energy (GeV)");
  front_S_photons_1D->GetYaxis()->SetTitle("Average Count");

  TProfile2D *front_S_photons_2D = new TProfile2D("front_S_photons_2D", "Average Number of Scintillation Photons vs. Deposited Ionization Energy", 50, 0, 0.5, 100, 0, 10000);
  front_S_photons_2D->GetXaxis()->SetTitle("Deposited Ionization Energy (GeV)");
  front_S_photons_2D->GetYaxis()->SetTitle("Average Count");

  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    front_S_photons_1D->Fill(depositedIonEnergyECAL_f, log(SDFdetected_f_S));
    front_S_photons_2D->Fill(depositedIonEnergyECAL_f, SDFdetected_f_S, SDFdetected_f_S/depositedIonEnergyECAL_f);
  }
 
  // TCanvas *b21 = new TCanvas();
  // front_S_photons_1D->Draw("HIST");
  // b21->Print("front_S_photons_1D.png");
  
  // TCanvas *b22 = new TCanvas();
  // front_S_photons_2D->Draw("colz");
  // b22->Print("front_2_photons_2D.png");

  /* This section of code deals with plotting the number of Cherenkov photons detected in the SDC vs. either the deposited energy 
     (for either E > 0.1 GeV or E > 0.05 GeV) or the number of scintillation photons detected in the front SiPM (for E > 0.1 GeV). 
     I also added a TProfile2D for the latter case, weighed by SDCdetected_r_C/depositedEnergyECAL_r, because I thought it'd be illuminating. */
  TProfile *C_rear_dep_en_range1 = new TProfile("C_rear_dep_en_range1", "Average Number of Cherenkov Photons Detected in SDC vs. Deposited Energy for E > 0.1 GeV", 50, 0, 30);
  C_rear_dep_en_range1->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  C_rear_dep_en_range1->GetYaxis()->SetTitle("Average Count");

  TProfile *C_rear_dep_en_range1_zoomed = new TProfile("C_rear_dep_en_range1_zoomed", "Average Number of Cherenkov Photons Detected in SDC vs. Deposited Energy for E > 0.1 GeV", 50, 0, 0.55);
  C_rear_dep_en_range1_zoomed->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  C_rear_dep_en_range1_zoomed->GetYaxis()->SetTitle("Average Count");

  TProfile *C_rear_dep_en_range2 = new TProfile("C_rear_dep_en_range2", "Average Number of Cherenkov Photons Detected in SDC vs. Deposited Energy for E > 0.05 GeV", 50, 0, 30);
  C_rear_dep_en_range2->GetXaxis()->SetTitle("Deposited Energy (GeV)");
  C_rear_dep_en_range2->GetYaxis()->SetTitle("Average Count");

  TProfile *C_rear_front_1D = new TProfile("C_rear_front_1D", "Number of Cherenkov Photons Detected in SDC vs. Number of Scintillation Photons Detected in Front SiPM", 50, 0, 10);
  C_rear_front_1D->GetXaxis()->SetTitle("Log of Count of Scintillation Photons in Front SiPM");
  C_rear_front_1D->GetYaxis()->SetTitle("Average Count of Cherenkov Photons in SDC");

  TProfile2D *C_rear_front_2D = new TProfile2D("C_rear_front_2D", "Number of Cherenkov Photons Detected in SDC vs. Number of Scintillation Photons Detected in Front SiPM", 50, 0, 10, 50, 0, 10);
  C_rear_front_2D->GetXaxis()->SetTitle("Log of Count of Scintillation Photons in Front SiPM");
  C_rear_front_2D->GetYaxis()->SetTitle("Log of Count of Chereknov Photons in SDC");

  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    if (depositedEnergyECAL_r > 0.1) {
      C_rear_dep_en_range1->Fill(depositedEnergyECAL_r, SDCdetected_r_C);
      C_rear_dep_en_range1_zoomed->Fill(depositedEnergyECAL_r, SDCdetected_r_C);
      C_rear_front_1D->Fill(log(SDFdetected_f_S), SDCdetected_r_C);
      C_rear_front_2D->Fill(log(SDFdetected_f_S), log(SDCdetected_r_C), SDCdetected_r_C/depositedEnergyECAL_r);
    }
    if (depositedEnergyECAL_r > 0.05) C_rear_dep_en_range2->Fill(depositedEnergyECAL_r, SDCdetected_r_C);
  }
  
  // TCanvas *b23 = new TCanvas();
  // C_rear_dep_en_range1->Draw("HIST");
  // b23->Print("C_rear_dep_en_range1.png");

  // TCanvas *b24 = new TCanvas();
  // C_rear_dep_en_range1_zoomed->Draw("HIST");
  // b24->Print("C_rear_dep_en_range1_zoomed.png");
  
  // TCanvas *b25 = new TCanvas();
  // C_rear_dep_en_range2->Draw("HIST");
  // b25->Print("C_rear_dep_en_range2.png");
  
  // TCanvas *b26 = new TCanvas();
  // C_rear_front_1D->Draw("HIST");
  // b26->Print("C_rear_front_1D.png");

  // TCanvas *b27 = new TCanvas();
  // C_rear_front_2D->Draw("colz");
  // b27->Print("C_rear_front_2D.png");

  TProfile *C_SDC_dep_en = new TProfile("C_SDC_dep_en", "C Photons in SDC vs. E_ion_dep", 100, 0, 30);
  C_SDC_dep_en->GetXaxis()->SetTitle("Deposited Ion Energy (GeV)");
  C_SDC_dep_en->GetYaxis()->SetTitle("Average Count");
  
  TProfile *S_SDC_dep_en = new TProfile("S_SDC_dep_en", "S Photons in SDC vs. E_ion_dep", 100, 0, 30);
  S_SDC_dep_en->GetXaxis()->SetTitle("Deposited Ion Energy (GeV)");
  S_SDC_dep_en->GetYaxis()->SetTitle("Average Count");

  // int totalC = 0.0, totalS = 0.0, totalSagain = 0.0;
  float maxfront = 0.0;
  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    if (depositedIonEnergyECAL_r > maxfront) maxfront = depositedIonEnergyECAL_r;
    if (SDCdetected_r_C < 6000) C_SDC_dep_en->Fill(depositedIonEnergyECAL_r, float(SDCdetected_r_C) * fracC650);
    if (SDCdetected_r_S < 140000) {
      if (((depositedIonEnergyECAL_r > 20) && (SDCdetected_r_S < 58000)) || ((depositedIonEnergyECAL_r > 25) && (SDCdetected_r_S < 80000))) {
	continue;
      } else {
	S_SDC_dep_en->Fill(depositedIonEnergyECAL_r, SDCdetected_r_S);
      }
    }
    // totalC += SDCdetected_r_C;
    // totalS += SDFdetected_f_S;
    // totalSagain += SDCdetected_r_S;
  }

  S_SDC_dep_en->Fit("pol1");
  TF1 *linear_fit1 = S_SDC_dep_en->GetFunction("pol1");

  C_SDC_dep_en->Fit("pol1");
  TF1 *linear_fit2 = C_SDC_dep_en->GetFunction("pol1");
  
  cout << maxfront << endl;
  // cout << totalC << " " << totalS << " " << totalSagain << endl;

  /*
  TCanvas *test2 = new TCanvas();
  test2->Divide(2, 1);
  test2->cd(1);
  S_SDC_dep_en->Draw();
  linear_fit1->Draw("SAME");
  test2->cd(2);
  C_SDC_dep_en->Draw();
  linear_fit2->Draw("SAME");
  test2->Print("count_vs_energy.png");
  */

  TH2D *C_vs_S100cut = new TH2D("C_vs_S100cut", "C Photons in SDC vs. S Photons in SDF for E_dep > 0.1 GeV", 25, 0, 18667, 25, 0, 800);
  C_vs_S100cut->GetXaxis()->SetTitle("Count of S Photons");
  C_vs_S100cut->GetYaxis()->SetTitle("Count of C Photons");

  TH2D *C_vs_S_LOG100cut = new TH2D("C_vs_S_LOG100cut", "LOG-LOG of C Photons in SDC vs. S Photons in SDF for E_dep > 0.1 GeV", 25, 0, 10.5, 25, 0, 10.5);
  C_vs_S_LOG100cut->GetXaxis()->SetTitle("LOG of Count of S Photons");
  C_vs_S_LOG100cut->GetYaxis()->SetTitle("LOG of Count of C Photons");

  TH2D *C_vs_Snocut = new TH2D("C_vs_Snocut", "C Photons in SDC vs. S Photons in SDF", 25, 0, 18667, 25, 0, 800);
  C_vs_Snocut->GetXaxis()->SetTitle("Count of S Photons");
  C_vs_Snocut->GetYaxis()->SetTitle("Count of C Photons");

  TH2D *C_vs_S_LOGnocut = new TH2D("C_vs_S_LOGnocut", "LOG-LOG of C Photons in SDC vs. S Photons in SDF", 25, 0, 10.5, 25, 0, 10.5);
  C_vs_S_LOGnocut->GetXaxis()->SetTitle("LOG of Count of S Photons");
  C_vs_S_LOGnocut->GetYaxis()->SetTitle("LOG of Count of C Photons");

  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    C_vs_Snocut->Fill(float(SDCdetected_r_S)/depositedIonEnergyECAL_r, (float(SDCdetected_r_C) * fracC650)/depositedIonEnergyECAL_r);
    C_vs_S_LOGnocut->Fill(log(float(SDCdetected_r_S)/depositedIonEnergyECAL_r), log((float(SDCdetected_r_C) * fracC650)/depositedIonEnergyECAL_r));
    if (depositedIonEnergyECAL_r > 0.1) { 
      C_vs_S100cut->Fill(float(SDCdetected_r_S)/depositedIonEnergyECAL_r, (float(SDCdetected_r_C) * fracC650)/depositedIonEnergyECAL_r);
      C_vs_S_LOG100cut->Fill(log(float(SDCdetected_r_S)/depositedIonEnergyECAL_r), log((float(SDCdetected_r_C) * fracC650)/depositedIonEnergyECAL_r));
    }
  }
  /* 
  TCanvas *test3 = new TCanvas();
  test3->Divide(2, 1);
  test3->cd(1);
  // C_vs_S100cut->SetMaximum(30000);
  C_vs_S100cut->Draw("colz");
  test3->cd(2);
  C_vs_S_LOG100cut->Draw("colz");
  test3->Print("count_plots_100cut.png");
 
  TCanvas *test4 = new TCanvas();
  test4->Divide(2, 1);
  test4->cd(1);
  // C_vs_Snocut->SetMaximum(30000);
  C_vs_Snocut->Draw("colz");
  test4->cd(2);
  C_vs_S_LOGnocut->Draw("colz");
  test4->Print("count_plots_nocut.png");
  */ 

  TH1F *S_cut = new TH1F("C_cut", "Most Probable Number of S Photons per GeV", 25, 0.1, 18667);
  S_cut->GetXaxis()->SetTitle("Count of S Photons per GeV");
  S_cut->GetYaxis()->SetTitle("Events");

  TH1F *C_cut = new TH1F("S_cut", "Most Probable Number of C Photons per GeV", 25, 0.1, 800);
  C_cut->GetXaxis()->SetTitle("Count of C Photons per Gev");
  C_cut->GetYaxis()->SetTitle("Events");

  float x0 = 0.0, y0 = 0.0;
  // This for loop fills the center-cut histograms like the for loop that fills the total-cut histograms.     
  for (int i = 0; i < 25; i++) {
    x0 = i * (18667.0/25.0);
    y0 = i * (800.0/25.0);
    C_cut->Fill(y0, C_vs_S100cut->GetBinContent(4, i));
    S_cut->Fill(x0, C_vs_S100cut->GetBinContent(i, 3));
  }

  cout << C_cut->GetMaximumBin() * (800.0/25.0) << " " << S_cut->GetMaximumBin() * (18667.0/25.0) << endl;
  cout << "TESTING" << endl;
  /*
  TCanvas *testing = new TCanvas();
  testing->Divide(2, 1);
  testing->cd(1);
  C_cut->Draw("HIST");
  testing->cd(2);
  S_cut->Draw("HIST");
  testing->Print("C_S_cuts.png");
  */


  // ...........................................................................................
  // .....................PART 5: CANVASES OF EVERYTHING FROM PARTS 3 AND 4.....................
  // ...........................................................................................   



  /* These are all the canvases on which the histograms will be placed.
     I also write each graph/histogram to a PNG file so I can access them
     and discuss them later. My convention is the following:

     First Number:
     1) Deals with the energies of the protons
     2) Deals with the positions of the protons
     4) Deals with the average counts of the photons with respect to position.
     7) Deals with the cuts along the centers of the SiPMs.
     8) Deals with the cuts for shower events, but modified.
     9) Deals with the efficiencies of the SiPMS.
     
     Second Number: Counts how many histograms there are dealing with each number. */  

  // These five deal with the energies of the protons. The log graph is meant to show a peak in the energies.
  // TCanvas *c11 = new TCanvas();
  // h_energy->Draw();
  // c11->Print("depositedEnergyECAL_40k.png");

  // TCanvas *c13 = new TCanvas();
  // h_energy_Landau->Fit("landau");
  // h_energy_Landau->Draw();                                                                                                                                             
  // TF1 *h_energy_fit = h_energy_Landau->GetFunction("landau");
  // h_energy_fit->Draw("SAME"); 
  // c13->Print("depositedEnergyECAL_Landau_40k.png");

  // TCanvas *c14 = new TCanvas();
  // h_energy_ion->Draw();
  // c14->Print("depositedIonEnergyECAL_40k.png");

  // TCanvas *c15 = new TCanvas();
  // h_energy_LOG->Draw();
  // c15->Print("LOGdepositedEnergyECAL_40k_[-3,3].png");

  /* These two deal with the positions of the protons. The first is a heat map; the second is a scatterplot. 
     These graphs are meant to show that the protons travel in random trajectories. */
  // TCanvas *c21 = new TCanvas();
  // pos_protons->Draw("colz");
  // c21->Print("pos_protons_40k.png");
  
  // TCanvas *c22 = new TCanvas();
  // pos_protons->Draw("p");
  // c22->Print("pos_protons_40k_check.png");

  /* As said in the histogram definitions, these eight are similar to the previous four, 
     but they count the average number of photons in each detector. The first four
     are for non-shower events, while the second four are for shower events only. */
  // TCanvas *c41 = new TCanvas();
  // avg_detected_photons_S_SDC->Draw("colz");
  // c41->Print("avg_S_photons_SDC.png");
 
  // TCanvas *c42 = new TCanvas();
  // avg_detected_photons_C_SDC->Draw("colz");
  // c42->Print("avg_C_photons_SDC.png");
  
  // TCanvas *c43 = new TCanvas();
  // avg_detected_photons_S_SDS->Draw("colz");
  // c43->Print("avg_S_photons_SDS.png");
  
  // TCanvas *c44 = new TCanvas();
  // avg_detected_photons_C_SDS->Draw("colz");
  // c44->Print("avg_C_photons_SDS.png");

  // TCanvas *c45 = new TCanvas();
  // avg_S_SDC_shower->Draw("colz");
  // c45->Print("avg_S_SDC_shower.png");
 
  // TCanvas *c46 = new TCanvas();
  // avg_C_SDC_shower->Draw("colz");
  // c46->Print("avg_C_SDC_shower.png");
  
  // TCanvas *c47 = new TCanvas();
  // avg_S_SDS_shower->Draw("colz");
  // c47->Print("avg_S_SDS_shower.png");

  // TCanvas *c48 = new TCanvas();
  // avg_C_SDS_shower->Draw("colz");
  // c48->Print("avg_C_SDS_shower.png");

  // TCanvas *c49 = new TCanvas();            
  // avg_detected_photons_S_SDF->Draw("colz");
  // c49->Print(sample + "avg_S_photons_SDF.png");   

  // TCanvas *c410 = new TCanvas(); 
  // avg_detected_photons_C_SDF->Draw("colz");
  // c410->Print(sample + "avg_C_photons_SDF.png");   

  // TCanvas *c411 = new TCanvas();        
  // avg_S_SDF_shower->Draw("colz");      
  // c411->Print(sample + "avg_S_SDF_shower.png");  

  // TCanvas *c412 = new TCanvas();        
  // avg_C_SDF_shower->Draw("colz");      
  // c412->Print(sample + "avg_C_SDF_shower.png");  
  


  // ...........................................................................................                                                                                 
  // ..................................PART 6: CENTER CUTS......................................                                                                                 
  // ...........................................................................................   



  /* When I originally did my cuts (which i left in this macro to show progress and for extra analysis),
     I summed all the photons in all the x- and y-positions, when really we only care about the average 
     number of photons at the centers of the SiPMs in both the x- and y-positions. These histograms sum 
     all the counts along the centers of each SiPM in both directions. */
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

  TH1F *x_center_S_SDF = new TH1F("x_center_S_SDF", "Center Cut of S Photons in SDF (x = 0)", 100, -14.9, 15.0);
  x_center_S_SDF->GetXaxis()->SetTitle("y-Positions (mm)");
  x_center_S_SDF->GetYaxis()->SetTitle("Count");

  TH1F *x_center_C_SDF = new TH1F("x_center_C_SDF", "Center Cut of C Photons in SDF (x = 0)", 100, -14.9, 15.0);
  x_center_C_SDF->GetXaxis()->SetTitle("y-Positions (mm)");
  x_center_C_SDF->GetYaxis()->SetTitle("Count");

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
  
  TH1F *y_center_S_SDF = new TH1F("y_center_S_SDF", "Center Cut of S Photons in SDF (y = 0)", 100, -14.9, 15.0);
  y_center_S_SDF->GetXaxis()->SetTitle("x-Positions (mm)");
  y_center_S_SDF->GetYaxis()->SetTitle("Count");

  TH1F *y_center_C_SDF = new TH1F("y_center_C_SDF", "Center Cut of C Photons in SDF (y = 0)", 100, -14.9, 15.0);
  y_center_C_SDF->GetXaxis()->SetTitle("x-Positions (mm)");
  y_center_C_SDF->GetYaxis()->SetTitle("Count");

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

  TH1F *shower_x_center_S_SDF = new TH1F("shower_x_center_S_SDF", "Center Cut of S Photons in SDF (x = 0) for Showers", 100, -14.9, 15.0);
  shower_x_center_S_SDF->GetXaxis()->SetTitle("y-Positions (mm)");
  shower_x_center_S_SDF->GetYaxis()->SetTitle("Count");

  TH1F *shower_x_center_C_SDF = new TH1F("shower_x_center_C_SDF", "Center Cut of C Photons in SDF (x = 0) for Showers", 100, -14.9, 15.0);
  shower_x_center_C_SDF->GetXaxis()->SetTitle("y-Positions (mm)");
  shower_x_center_C_SDF->GetYaxis()->SetTitle("Count");

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

  TH1F *shower_y_center_S_SDF = new TH1F("shower_y_center_S_SDF", "Center Cut of S Photons in SDF (y = 0) for Showers", 100, -14.9, 15.0);
  shower_y_center_S_SDF->GetXaxis()->SetTitle("x-Positions (mm)");
  shower_y_center_S_SDF->GetYaxis()->SetTitle("Count");

  TH1F *shower_y_center_C_SDF = new TH1F("shower_y_center_C_SDF", "Center Cut of C Photons in SDF (y = 0) for Showers", 100, -14.9, 15.0);
  shower_y_center_C_SDF->GetXaxis()->SetTitle("x-Positions (mm)");
  shower_y_center_C_SDF->GetYaxis()->SetTitle("Count");



  float x = 0.0, y = 0.0;
  float xSSDC = 0.0, xSSDS = 0.0, xCSDC = 0.0, xCSDS = 0.0, ySSDC = 0.0, ySSDS = 0.0, yCSDC = 0.0, yCSDS = 0.0;
  float shxSSDC = 0.0, shxSSDS = 0.0, shxCSDC = 0.0, shxCSDS = 0.0, shySSDC = 0.0, shySSDS = 0.0, shyCSDC = 0.0, shyCSDS = 0.0;
  // This for loop fills the center-cut histograms like the for loop that fills the total-cut histograms.
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
  /*
  TCanvas *c1000 = new TCanvas();
  x_center_S_SDC->Draw("HIST");
  fit1->Draw("SAME");
  c1000->Print("x_center_S_SDC.png");

  TCanvas *c1100 = new TCanvas();
  shower_x_center_S_SDC->Draw("HIST");
  shfit1->Draw("SAME");
  c1100->Print("shower_x_center_S_SDC.png");

  TCanvas *c1001 = new TCanvas();
  x_center_C_SDC->Draw("HIST");
  fit2->Draw("SAME");
  c1001->Print("x_center_C_SDC.png");

  TCanvas *c1101 = new TCanvas();
  shower_x_center_C_SDC->Draw("HIST");
  shfit2->Draw("SAME");
  c1101->Print("shower_x_center_C_SDC.png");

  TCanvas *c1002 = new TCanvas();
  x_center_S_SDS->Draw("HIST");
  fit3->Draw("SAME");
  c1002->Print("x_center_S_SDS.png");

  TCanvas *c1102 = new TCanvas();
  shower_x_center_S_SDS->Draw("HIST");
  shfit3->Draw("SAME");
  c1102->Print("shower_x_center_S_SDS.png");

  TCanvas *c1003 = new TCanvas();
  x_center_C_SDS->Draw("HIST");
  fit4->Draw("SAME");
  c1003->Print("x_center_C_SDS.png");

  TCanvas *c1103 = new TCanvas();
  shower_x_center_C_SDS->Draw("HIST");
  shfit4->Draw("SAME");
  c1103->Print("shower_x_center_C_SDS.png");

  TCanvas *c1004 = new TCanvas();
  y_center_S_SDC->Draw("HIST");
  fit5->Draw("SAME");
  c1004->Print("y_center_S_SDC.png");

  TCanvas *c1104 = new TCanvas();
  shower_y_center_S_SDC->Draw("HIST");
  shfit5->Draw("SAME");
  c1104->Print("shower_y_center_S_SDC.png");

  TCanvas *c1005 = new TCanvas();
  y_center_C_SDC->Draw("HIST");
  fit6->Draw("SAME");
  c1005->Print("y_center_C_SDC.png");

  TCanvas *c1105 = new TCanvas();
  shower_y_center_C_SDC->Draw("HIST");
  shfit6->Draw("SAME");
  c1105->Print("shower_y_center_C_SDC.png");

  TCanvas *c1006 = new TCanvas();
  y_center_S_SDS->Draw("HIST");
  fit7->Draw("SAME");
  c1006->Print("y_center_S_SDS.png");

  TCanvas *c1106 = new TCanvas();
  shower_y_center_S_SDS->Draw("HIST");
  shfit7->Draw("SAME");
  c1106->Print("shower_y_center_S_SDS.png");

  TCanvas *c1007 = new TCanvas();
  y_center_C_SDS->Draw("HIST");
  fit8->Draw("SAME");
  c1007->Print("y_center_C_SDS.png");

  TCanvas *c1107 = new TCanvas();
  shower_y_center_C_SDS->Draw("HIST");
  shfit8->Draw("SAME");
  c1107->Print("shower_y_center_C_SDS.png");
  */
  cout << fit1->Mean(3.5, 9.5) << endl;
  cout << fit1->Eval(fit1->Mean(3.5, 9.5));
  cout << "BIG REVEAL" << endl;
  cout << 0.5 * (fit1->Eval(fit1->Mean(3.5, 9.5)) + fit5->Eval(fit5->Mean(-3.5, 3.5))) << " " << 0.5 * (shfit1->Eval(shfit1->Mean(3.5, 9.5)) + shfit5->Eval(shfit1->Mean(-3.5, 3.5))) << endl;
  cout << 0.5 * (fit2->Eval(fit2->Mean(3.5, 9.5)) + fit6->Eval(fit6->Mean(-3.5, 3.5))) << " " << 0.5 * (shfit2->Eval(shfit2->Mean(3.5, 9.5)) + shfit6->Eval(shfit6->Mean(-3.5, 3.5))) << endl;													
  cout << 0.5 * (fit3->Eval(fit3->Mean(-9.5, -3.5)) + fit7->Eval(fit7->Mean(-3.5, 3.5))) << " " << 0.5 * (shfit3->Eval(shfit3->Mean(-9.5, -3.5)) + shfit7->Eval(shfit7->Mean(-3.5, 3.5))) << endl;
  cout << 0.5 * (fit4->Eval(fit4->Mean(-9.5, -3.5)) + fit8->Eval(fit8->Mean(-3.5, 3.5))) << " " << 0.5 * (shfit4->Eval(shfit4->Mean(-9.5, -3.5)) + shfit8->Eval(shfit8->Mean(-3.5, 3.5))) << endl;



  // ...........................................................................................                                                                                 
  // ...............................PART 7: NUMBER OF PHOTONS...................................                                                                                 
  // ...........................................................................................



  /* This part of the code finds the average count of S and C photons in the middle of each SiPM.
     I did this by averaging the counts of all the photons that appear in the SiPMs, which are located
     between bins 40 and 60 in the x-direction and 62 and 82 (SDC) or 18 and 38 (SDS) in the y-direction. */
  float avg_count_S_SDC = 0.0, avg_count_C_SDC = 0.0, avg_count_S_SDS = 0.0, avg_count_C_SDS = 0.0, avg_count_S_SDF = 0.0, avg_count_C_SDF = 0.0;
  float avg_shower_S_SDC = 0.0, avg_shower_C_SDC = 0.0, avg_shower_S_SDS = 0.0, avg_shower_C_SDS = 0.0, avg_shower_S_SDF = 0.0, avg_shower_C_SDF = 0.0;
  int totalBins = 0;
  for (int i = 40; i < 60; i++) {
    for (int j = 0; j <= 100; j++) {
      if (j >= 62 && j < 82) {
	totalBins++;
	avg_count_S_SDC += avg_detected_photons_S_SDC->GetBinContent(i, j);
	avg_count_C_SDC += avg_detected_photons_C_SDC->GetBinContent(i, j);
	avg_shower_S_SDC += avg_S_SDC_shower->GetBinContent(i, j);
	avg_shower_C_SDC += avg_C_SDC_shower->GetBinContent(i, j);
      } else if (j >= 18 && j < 38) {
	avg_count_S_SDS += avg_detected_photons_S_SDS->GetBinContent(i, j);
	avg_count_C_SDS += avg_detected_photons_C_SDS->GetBinContent(i, j);
	avg_shower_S_SDS += avg_S_SDS_shower->GetBinContent(i, j);
	avg_shower_C_SDS += avg_C_SDS_shower->GetBinContent(i, j);
      } else if (j >= 40 && j < 60) {
	avg_count_S_SDF += avg_detected_photons_S_SDF->GetBinContent(i, j);
	avg_count_C_SDF += avg_detected_photons_C_SDF->GetBinContent(i, j);
	avg_shower_S_SDF += avg_S_SDF_shower->GetBinContent(i, j);
	avg_shower_C_SDF += avg_C_SDF_shower->GetBinContent(i, j);
      }
    }
  }	
  avg_count_S_SDC /= totalBins;
  avg_count_C_SDC /= totalBins;
  avg_count_S_SDS /= totalBins;
  avg_count_C_SDS /= totalBins;
  avg_count_S_SDF /= totalBins;
  avg_count_C_SDF /= totalBins;
  avg_shower_S_SDC /= totalBins;
  avg_shower_C_SDC /= totalBins;
  avg_shower_S_SDS /= totalBins;
  avg_shower_C_SDS /= totalBins;
  avg_shower_S_SDF /= totalBins;
  avg_shower_C_SDF /= totalBins;
  
  cout << "AVERAGE NUMBER OF PHOTONS IN EACH SIPM FOR EACH PHOTON" << endl;
  cout << "       Scintillation        Cherenkov        " << endl;
  cout << "SDC:      " << avg_count_S_SDC << "            " << avg_count_C_SDC << endl;
  cout << "SDS:       " << avg_count_S_SDS << "             " << avg_count_C_SDS << endl;
  cout << "SDF:      " << avg_count_S_SDF << "            " << avg_count_C_SDF << endl;
  
  cout << "AVERAGE NUMBER OF PHOTONS IN EACH SIPM FOR EACH PHOTON FOR SHOWER EVENTS" << endl;
  cout << "       Scintillation        Cherenkov        " << endl;
  cout << "SDC:      " << avg_shower_S_SDC << "            " << avg_shower_C_SDC << endl;
  cout << "SDS:       " << avg_shower_S_SDS << "             " << avg_shower_C_SDS << endl;
  cout << "SDF:      " << avg_shower_S_SDF << "            " << avg_shower_C_SDF << endl;



  // ...........................................................................................                                                                                 
  // ..........................PART 8: ENERGY VS. NUMBER FOR SHOWERS............................
  // ...........................................................................................



  /* This section of code is similar to that of the cuts in the x- and y-directions for each SiPM and photon, but now I'm
     considering showering events as well. The key difference between MIPs and showers is that the showers have a different
     energy distribution, so in order to make my cuts, I need to find the energy distribution of the showers and essentially
     correct for the difference. To do so, I made 2D histograms (TProfiles, to be accurate) for each x- and y-cut for each
     photon in each SiPM (what a mouthful!). On the x-axis is the position (either x or y depending on the cut), on the
     y-axis is the total deposited energy in the crystal, and on the z-axis is the number of photons. It's important to note
     that I chose my cuts not to be a single slice (i.e. x = 0), but as a small range of x- or y-values so I could have more data. */

  // These four TProfiles plot the x-cuts. 
  TProfile2D *shower_cuts_x_S_SDC = new TProfile2D("shower_cuts_x_S_SDC", "Number of Scintillation Photons in SDC including Shower Events", 30, -15.0, 15.0, 30, 0, 30);
  shower_cuts_x_S_SDC->GetXaxis()->SetTitle("y-Position (mm)");
  shower_cuts_x_S_SDC->GetYaxis()->SetTitle("Deposited Energy (GeV)");

  TProfile2D *shower_cuts_x_C_SDC = new TProfile2D("shower_cuts_x_C_SDC", "Number of Cherenkov Photons in SDC including Shower Events", 30, -15.0, 15.0, 30, 0, 30);
  shower_cuts_x_C_SDC->GetXaxis()->SetTitle("y-Position (mm)");
  shower_cuts_x_C_SDC->GetYaxis()->SetTitle("Deposited Energy (GeV)");

  TProfile2D *shower_cuts_x_S_SDS = new TProfile2D("shower_cuts_x_S_SDS", "Number of Scintillation Photons in SDS including Shower Events", 30, -15.0, 15.0, 30, 0, 30);
  shower_cuts_x_S_SDS->GetXaxis()->SetTitle("y-Position (mm)");
  shower_cuts_x_S_SDS->GetYaxis()->SetTitle("Deposited Energy (GeV)");

  TProfile2D *shower_cuts_x_C_SDS = new TProfile2D("shower_cuts_x_C_SDS", "Number of Cherenkov Photons in SDS including Shower Events", 30, -15.0, 15.0, 30, 0, 30);
  shower_cuts_x_C_SDS->GetXaxis()->SetTitle("y-Position (mm)");
  shower_cuts_x_C_SDS->GetYaxis()->SetTitle("Deposited Energy (GeV)");

  // These four TProfiles plot the y-cuts.
  TProfile2D *shower_cuts_y_S_SDC = new TProfile2D("shower_cuts_y_S_SDC", "Number of Scintillation Photons in SDC including Shower Events", 30, -15.0, 15.0, 30, 0, 30);
  shower_cuts_y_S_SDC->GetXaxis()->SetTitle("x-Position (mm)");
  shower_cuts_y_S_SDC->GetYaxis()->SetTitle("Deposited Energy (GeV)");

  TProfile2D *shower_cuts_y_C_SDC = new TProfile2D("shower_cuts_y_C_SDC", "Number of Cherenkov Photons in SDC including Shower Events", 30, -15.0, 15.0, 30, 0, 30);
  shower_cuts_y_C_SDC->GetXaxis()->SetTitle("x-Position (mm)");
  shower_cuts_y_C_SDC->GetYaxis()->SetTitle("Deposited Energy (GeV)");

  TProfile2D *shower_cuts_y_S_SDS = new TProfile2D("shower_cuts_y_S_SDS", "Number of Scintillation Photons in SDS including Shower Events", 30, -15.0, 15.0, 30, 0, 30);
  shower_cuts_y_S_SDS->GetXaxis()->SetTitle("x-Position (mm)");
  shower_cuts_y_S_SDS->GetYaxis()->SetTitle("Deposited Energy (GeV)");

  TProfile2D *shower_cuts_y_C_SDS = new TProfile2D("shower_cuts_y_C_SDS", "Number of Cherenkov Photons in SDS including Shower Events", 30, -15.0, 15.0, 30, 0, 30);
  shower_cuts_y_C_SDS->GetXaxis()->SetTitle("x-Position (mm)");
  shower_cuts_y_C_SDS->GetYaxis()->SetTitle("Deposited Energy (GeV)");

  // This for loop fills the TProfiles.
  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);

    // For the x-cuts, if the x-position is in the range -2.0 <= x <= 2.0...
    if ((*inputInitialPosition)[0] >= -2.0 && (*inputInitialPosition)[0] <= 2.0) {
      shower_cuts_x_S_SDC->Fill((*inputInitialPosition)[1], depositedEnergyECAL_r, SDCdetected_r_S/depositedEnergyECAL_r);
      shower_cuts_x_C_SDC->Fill((*inputInitialPosition)[1], depositedEnergyECAL_r, SDCdetected_r_C/depositedEnergyECAL_r);
      shower_cuts_x_S_SDS->Fill((*inputInitialPosition)[1], depositedEnergyECAL_r, SDSdetected_r_S/depositedEnergyECAL_r);
      shower_cuts_x_C_SDS->Fill((*inputInitialPosition)[1], depositedEnergyECAL_r, SDSdetected_r_C/depositedEnergyECAL_r);
    }

    // For the y-cuts, if the y-position is in the range 4.5 <= y <= 8.5 (SDC) or -8.5 <= y <= -4.5 (SDS).
    if ((*inputInitialPosition)[1] >= 4.5 && (*inputInitialPosition)[1] <= 8.5) {
      shower_cuts_y_S_SDC->Fill((*inputInitialPosition)[0], depositedEnergyECAL_r, SDCdetected_r_S/depositedEnergyECAL_r);
      shower_cuts_y_C_SDC->Fill((*inputInitialPosition)[0], depositedEnergyECAL_r, SDCdetected_r_C/depositedEnergyECAL_r);
    } else if ((*inputInitialPosition)[1] >= -8.5 && (*inputInitialPosition)[1] <= -4.5) {
      shower_cuts_y_S_SDS->Fill((*inputInitialPosition)[0], depositedEnergyECAL_r, SDSdetected_r_S/depositedEnergyECAL_r);
      shower_cuts_y_C_SDS->Fill((*inputInitialPosition)[0], depositedEnergyECAL_r, SDSdetected_r_C/depositedEnergyECAL_r);
    }
  }

  // This set of canvases plots the TProfiles as color maps.
  // TCanvas *c81 = new TCanvas();
  // c81->SetLogz();
  // shower_cuts_x_S_SDC->Draw("colz");
  // c81->Print("shower_cuts_x_S_SDC.png");

  // TCanvas *c82 = new TCanvas();
  // c82->SetLogz();
  // shower_cuts_x_C_SDC->Draw("colz");
  // c82->Print("shower_cuts_x_C_SDC.png");

  // TCanvas *c83 = new TCanvas();
  // shower_cuts_x_S_SDS->Draw("colz");
  // c83->Print("shower_cuts_x_S_SDS.png");

  // TCanvas *c84 = new TCanvas();
  // shower_cuts_x_C_SDS->Draw("colz");
  // c84->Print("shower_cuts_x_C_SDS.png");

  // TCanvas *c85 = new TCanvas();
  // shower_cuts_y_S_SDC->Draw("colz");
  // c85->Print("shower_cuts_y_S_SDC.png");

  // TCanvas *c86 = new TCanvas();
  // shower_cuts_y_C_SDC->Draw("colz");
  // c86->Print("shower_cuts_y_C_SDC.png");

  // TCanvas *c87 = new TCanvas();
  // shower_cuts_y_S_SDS->Draw("colz");
  // c87->Print("shower_cuts_y_S_SDS.png");

  // TCanvas *c88 = new TCanvas();
  // shower_cuts_y_C_SDS->Draw("colz");
  // c88->Print("shower_cuts_y_C_SDS.png");



  // ...........................................................................................                                                                                 
  // .........................PART 9: DETERMINING EFFICIENCY OF SIPMS........................... 
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
  // TString normalized = "yes_";
  TString normalized = "no_";

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

  TH1D *combined_front_S = new TH1D("combined_front_S", "Normalized Distribution of Scintillation Photons in Front SiPM Multiplied by PDE", 1250, 0., 1250.);
  combined_front_S->GetXaxis()->SetTitle("Wavelength (nm)");
  combined_front_S->GetYaxis()->SetTitle("Photon Detection Efficiency (%)");

  TH1D *combined_front_C = new TH1D("combined_front_C","Normalized Distribution of Cherenkov Photons in Front SiPM Multiplied by PDE", 1250, 0., 1250.);
  combined_front_C->GetXaxis()->SetTitle("Wavelength (nm)");
  combined_front_C->GetYaxis()->SetTitle("Photon Detection Efficiency (%)");

  // This is a TGraph of the values given for the PDE.
  TH1D *PDE_hist = new TH1D("PDE_hist", "Photon Detection Efficiency", 1250, 0., 1250.);
  PDE_hist->GetXaxis()->SetTitle("Wavelength (nm)");
  PDE_hist->GetYaxis()->SetTitle("Photon Detection Efficiency");

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
    PDE_hist->SetBinContent(x, y);
    PDE_graph->SetPoint(counter, x, y);
    counter++;                    
  }  
  file.close();

  // TCanvas *test = new TCanvas();
  // PDE_graph->Draw("AL");
  // PDE_hist->Draw("HIST SAME");
  // test->Print("PDE.png");

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
  /*  
  TCanvas *c91 = new TCanvas();
  combined_SDC_S->SetLineColor(4);
  combined_SDC_S->SetFillStyle(3001);
  SDC_S_lambdas->SetLineColor(2);
  SDC_S_lambdas->SetFillStyle(3001);
  
  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  combined_SDC_S->Draw("HIST");
  SDC_S_lambdas->Draw("HIST SAME");
  // SDC_S_lambdas->Draw("HIST");
  // combined_SDC_S->Draw("HIST SAME");
  TLegend *combined_SDC_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_SDC_S_legend->SetHeader("KEY", "C");
  combined_SDC_S_legend->AddEntry(SDC_S_lambdas, "Original", "l")->SetTextColor(2);
  combined_SDC_S_legend->AddEntry(combined_SDC_S, "PDE", "l")->SetTextColor(4);
  combined_SDC_S_legend->SetBorderSize(5);                                           
  combined_SDC_S_legend->Draw();   
  c91->Print(sample + normalized + "combined_SDC_S.png");

  TCanvas *c92 = new TCanvas();
  combined_SDC_C->SetLineColor(4);
  combined_SDC_C->SetFillStyle(3001);
  SDC_C_lambdas->SetLineColor(2);
  SDC_C_lambdas->SetFillStyle(3001);

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  combined_SDC_C->Draw("HIST");
  SDC_C_lambdas->Draw("HIST SAME");
  // SDC_C_lambdas->Draw("HIST");      
  // combined_SDC_C->Draw("HIST SAME"); 
  TLegend *combined_SDC_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_SDC_C_legend->SetHeader("KEY", "C");
  combined_SDC_C_legend->AddEntry(SDC_C_lambdas, "Original", "l")->SetTextColor(2);
  combined_SDC_C_legend->AddEntry(combined_SDC_C, "PDE", "l")->SetTextColor(4);
  combined_SDC_C_legend->SetBorderSize(5);
  combined_SDC_C_legend->Draw();
  c92->Print(sample + normalized +  "combined_SDC_C.png");

  TCanvas *c93 = new TCanvas();
  combined_SDS_S->SetLineColor(4);
  combined_SDS_S->SetFillStyle(3001);
  SDS_S_lambdas->SetLineColor(2);
  SDS_S_lambdas->SetFillStyle(3001);

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  combined_SDS_S->Draw("HIST");
  SDS_S_lambdas->Draw("HIST SAME");
  // SDS_S_lambdas->Draw("HIST");       
  // combined_SDS_S->Draw("HIST SAME"); 
  TLegend *combined_SDS_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_SDS_S_legend->SetHeader("KEY", "C");
  combined_SDS_S_legend->AddEntry(SDS_S_lambdas, "Original", "l")->SetTextColor(2);
  combined_SDS_S_legend->AddEntry(combined_SDS_S, "PDE", "l")->SetTextColor(4);
  combined_SDS_S_legend->SetBorderSize(5);
  combined_SDS_S_legend->Draw();
  c93->Print(sample + normalized + "combined_SDS_S.png");

  TCanvas *c94 = new TCanvas();
  combined_SDS_C->SetLineColor(4);
  combined_SDS_C->SetFillStyle(3001);
  SDS_C_lambdas->SetLineColor(2);
  SDS_C_lambdas->SetFillStyle(3001);

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  combined_SDS_C->Draw("HIST");
  SDS_C_lambdas->Draw("HIST SAME");
  // SDS_C_lambdas->Draw("HIST");       
  // combined_SDS_C->Draw("HIST SAME"); 
  TLegend *combined_SDS_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_SDS_C_legend->SetHeader("KEY", "C");
  combined_SDS_C_legend->AddEntry(SDS_C_lambdas, "Original", "l")->SetTextColor(2);
  combined_SDS_C_legend->AddEntry(combined_SDS_C, "PDE", "l")->SetTextColor(4);
  combined_SDS_C_legend->SetBorderSize(5);
  combined_SDS_C_legend->Draw();
  c94->Print(sample + normalized + "combined_SDS_C.png");
  */

  /*
  TCanvas *c95 = new TCanvas();
  combined_front_S->SetLineColor(4);
  combined_front_S->SetFillStyle(3001);
  front_S_detected->SetLineColor(2);
  front_S_detected->SetFillStyle(3001);

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  // combined_front_S->Draw("HIST");
  // front_S_detected->Draw("HIST SAME");
  front_S_detected->Draw("HIST");
  combined_front_S->Draw("HIST SAME"); 
  TLegend *combined_front_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_front_S_legend->SetHeader("KEY", "C");
  combined_front_S_legend->AddEntry(front_S_detected, "Original", "l")->SetTextColor(2);
  combined_front_S_legend->AddEntry(combined_front_S, "PDE", "l")->SetTextColor(4);
  combined_front_S_legend->SetBorderSize(5);
  combined_front_S_legend->Draw();
  c95->Print(sample + normalized + "combined_front_S.png");
   
  TCanvas *c97 = new TCanvas();
  combined_front_C->SetLineColor(4);
  combined_front_C->SetFillStyle(3001);
  front_C_detected->SetLineColor(2);
  front_C_detected->SetFillStyle(3001);

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.  
  // combined_front_C->Draw("HIST");
  // front_C_detected->Draw("HIST SAME");
  front_C_detected->Draw("HIST");
  combined_front_C->Draw("HIST SAME");
  TLegend *combined_front_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_front_C_legend->SetHeader("KEY", "C");
  combined_front_C_legend->AddEntry(front_C_detected, "Original", "l")->SetTextColor(2);
  combined_front_C_legend->AddEntry(combined_front_C, "PDE", "l")->SetTextColor(4);
  combined_front_C_legend->SetBorderSize(5);
  combined_front_C_legend->Draw();
  c97->Print(sample + normalized +  "combined_front_C.png");
  */

  /*
  // These canvases overlay all the relevant comparisons onto the same canvas.
  TCanvas *c101 = new TCanvas();
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
  combined_SDC_S->Draw("HIST"); 
  SDC_S_lambdas->Draw("HIST SAME");
  // SDC_S_lambdas->Draw("HIST");
  // combined_SDC_S->Draw("HIST SAME");
  combined_SDS_S->Draw("HIST SAME");
  SDS_S_lambdas->Draw("HIST SAME");
  // SDS_S_lambdas->Draw("HIST SAME");
  // combined_SDS_S->Draw("HIST SAME");
  TLegend *combined_S_type_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_S_type_legend->SetHeader("KEY", "C");
  combined_S_type_legend->AddEntry(SDC_S_lambdas, "Original for SDC", "l")->SetTextColor(2);
  combined_S_type_legend->AddEntry(combined_SDC_S, "PDE for SDC", "l")->SetTextColor(1);
  combined_S_type_legend->AddEntry(SDS_S_lambdas, "Original for SDS", "l")->SetTextColor(4);
  combined_S_type_legend->AddEntry(combined_SDS_S, "PDE for SDS", "l")->SetTextColor(3);
  combined_S_type_legend->SetBorderSize(5);
  combined_S_type_legend->Draw();
  c101->Print(sample + normalized + "combined_S_SDC-SDS.png");

  TCanvas *c102 = new TCanvas();
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
  combined_SDC_C->Draw("HIST");
  SDC_C_lambdas->Draw("HIST SAME");
  // SDC_C_lambdas->Draw("HIST SAME");
  // combined_SDC_C->Draw("HIST SAME");
  combined_SDS_C->Draw("HIST SAME");
  SDS_C_lambdas->Draw("HIST SAME");
  // SDS_C_lambdas->Draw("HIST SAME");
  // combined_SDS_C->Draw("HIST SAME");
  TLegend *combined_C_type_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_C_type_legend->SetHeader("KEY", "C");
  combined_C_type_legend->AddEntry(SDC_C_lambdas, "Original for SDC", "l")->SetTextColor(2);
  combined_C_type_legend->AddEntry(combined_SDC_C, "PDE for SDC", "l")->SetTextColor(1);
  combined_C_type_legend->AddEntry(SDS_C_lambdas, "Original for SDS", "l")->SetTextColor(4);
  combined_C_type_legend->AddEntry(combined_SDS_C, "PDE for SDS", "l")->SetTextColor(3);
  combined_C_type_legend->SetBorderSize(5);
  combined_C_type_legend->Draw();
  c102->Print(sample + normalized + "combined_C_SDC-SDS.png");
  */

  /*
  TCanvas *c103 = new TCanvas();
  combined_front_S->SetLineColor(1);
  combined_front_S->SetFillStyle(3001);
  combined_front_S->SetTitle("Overlaid Distributions of Scintillation Photons in Front and Rear SiPMs");
  front_S_detected->SetLineColor(2);
  front_S_detected->SetFillStyle(3001);
  combined_SDC_S->SetLineColor(3);
  combined_SDC_S->SetFillStyle(3001);
  SDC_S_lambdas->SetLineColor(4);
  SDC_S_lambdas->SetFillStyle(3001);

  // If the first and third  two are commented out, then we have unnormalized data. If the second and fourth two are commented out, we have normalized data. 
  // combined_front_S->Draw("HIST"); 
  // front_S_detected->Draw("HIST SAME");
  SDC_S_lambdas->Draw("HIST");
  combined_SDC_S->Draw("HIST SAME");
  // combined_SDC_S->Draw("HIST SAME");
  // SDC_S_lambdas->Draw("HIST SAME");
  front_S_detected->Draw("HIST SAME");
  combined_front_S->Draw("HIST SAME");
  TLegend *combined_S_position_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_S_position_legend->SetHeader("KEY", "C");
  combined_S_position_legend->AddEntry(front_S_detected, "Original for Front", "l")->SetTextColor(2);
  combined_S_position_legend->AddEntry(combined_front_S, "PDE for Front", "l")->SetTextColor(1);
  combined_S_position_legend->AddEntry(SDC_S_lambdas, "Original for Rear", "l")->SetTextColor(4);
  combined_S_position_legend->AddEntry(combined_SDC_S, "PDE for Rear", "l")->SetTextColor(3);
  combined_S_position_legend->SetBorderSize(5);
  combined_S_position_legend->Draw();
  c103->Print(sample + normalized +  "combined_S_front-rear.png");

  TCanvas *c104 = new TCanvas();
  combined_front_C->SetLineColor(1);
  combined_front_C->SetFillStyle(3001);
  combined_front_C->SetTitle("Overlaid Distributions of Cherenkov Photons in Front and Rear SiPMs");
  front_C_detected->SetLineColor(2);
  front_C_detected->SetFillStyle(3001);
  combined_SDC_C->SetLineColor(3);
  combined_SDC_C->SetFillStyle(3001);
  SDC_C_lambdas->SetLineColor(4);
  SDC_C_lambdas->SetFillStyle(3001);

  // If the first and third  two are commented out, then we have unnormalized data. If the second and fourth two are commented out, we have normalized data. 
  // combined_front_C->Draw("HIST");
  // front_C_detected->Draw("HIST SAME");
  SDC_C_lambdas->Draw("HIST");
  combined_SDC_C->Draw("HIST SAME");
  // combined_SDC_C->Draw("HIST SAME");
  // SDC_C_lambdas->Draw("HIST SAME");
  // front_C_detected->Draw("HIST SAME");
  // combined_front_C->Draw("HIST SAME");
  TLegend *combined_C_position_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  combined_C_position_legend->SetHeader("KEY", "C");
  combined_C_position_legend->AddEntry(front_C_detected, "Original for Front", "l")->SetTextColor(2);
  combined_C_position_legend->AddEntry(combined_front_C, "PDE for Front", "l")->SetTextColor(1);
  combined_C_position_legend->AddEntry(SDC_C_lambdas, "Original for Rear", "l")->SetTextColor(4);
  combined_C_position_legend->AddEntry(combined_SDC_C, "PDE for Rear", "l")->SetTextColor(3);
  combined_C_position_legend->SetBorderSize(5);
  combined_C_position_legend->Draw();
  c104->Print(sample + normalized + "combined_C_front-rear.png");
  */



  // ...........................................................................................                                                                                 
  // ...........................PART 10: TIME OF ARRIVAL OF PHOTONS.............................  
  // ...........................................................................................  



  /* This purpose of this section is to determine when the photons arrive at the back of the SiPM. This is important
     because then we can understand when they're created, what wavelengths we can expect them to have when they
     arrive, and more. */

  // These histograms show the distributions of the times of arrival of each photon.
  TH1F *time_SDC_S_detected = new TH1F("time_SDC_S_detected","Time of Detected Scintillation Photons in SDC", 500, 0., 50.);
  time_SDC_S_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDC_S_detected->GetYaxis()->SetTitle("Count");

  TH1F *time_SDC_C_detected = new TH1F("time_SDC_C_detected","Time of Detected Cherenkov Photons in SDC", 500, 0., 50.);
  time_SDC_C_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDC_C_detected->GetYaxis()->SetTitle("Count");

  TH1F *time_SDS_S_detected = new TH1F("time_SDS_S_detected","Time of Detected Scintillation Photons in SDS", 500, 0., 50.);
  time_SDS_S_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDS_S_detected->GetYaxis()->SetTitle("Count");

  TH1F *time_SDS_C_detected = new TH1F("time_SDS_C_detected","Time of Detected Cherenkov Photons in SDS", 500, 0., 50.);
  time_SDC_S_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDC_S_detected->GetYaxis()->SetTitle("Count");

  TH1F *time_SDF_S_detected = new TH1F("time_SDF_S_detected","Time of Detected Scintillation Photons in SDF", 500, 0., 50.);
  time_SDF_S_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDF_S_detected->GetYaxis()->SetTitle("Count");

  TH1F *time_SDF_C_detected = new TH1F("time_SDF_C_detected","Time of Detected Cherenkov Photons in SDF", 500, 0., 50.);
  time_SDF_C_detected->GetXaxis()->SetTitle("Time (ns)");
  time_SDF_C_detected->GetYaxis()->SetTitle("Count");

  TString filename1 = "";
  TString histnameDet1 = "";
  TString histnameOther1 = ""; 

  // This for loop lets me add all the histograms from each file together at the same time.                                                                                      
  for (int i = 0; i < 8; i++) {
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

    for (int k = 0; k < 2; k++) {
      TFile *file = new TFile(filename1);
      if (k == 0) histnameOther1 = "h_phot_time_SiPMF_Scin";
      else if (k == 1) histnameOther1 = "h_phot_time_SiPMF_Ceren";
      TH1D *histOther = (TH1D*)file->Get(histnameOther1);
      if (k == 0) time_SDF_S_detected->Add(histOther);
      else if (k == 1) time_SDF_C_detected->Add(histOther);
    }
  }

  TH1F *time_number_S_SDC = new TH1F("time_number_S_SDC", "Timing of Scintillation Photons in SDC", 500, 0., 500.);     
  time_number_S_SDC->GetXaxis()->SetTitle("Time (ns)");                                                                        
  time_number_S_SDC->GetYaxis()->SetTitle("Count");

  TH1F *time_number_C_SDC = new TH1F("time_number_C_SDC", "Timing of Cherenkov Photons in SDC", 500, 0., 50.);         
  time_number_C_SDC->GetXaxis()->SetTitle("Time (ns)");                                                                
  time_number_C_SDC->GetYaxis()->SetTitle("Count");  

  TH1F *time_number_S_SDS = new TH1F("time_number_S_SDS", "Timing of Scintillation Photons in SDS", 500, 0., 500.);  
  time_number_S_SDS->GetXaxis()->SetTitle("Time (ns)");                                                       
  time_number_S_SDS->GetYaxis()->SetTitle("Count");                                                           
                                                                                                              
  TH1F *time_number_C_SDS = new TH1F("time_number_C_SDS", "Timing of Cherenkov Photons in SDS", 500, 0., 50.);
  time_number_C_SDS->GetXaxis()->SetTitle("Time (ns)");
  time_number_C_SDS->GetYaxis()->SetTitle("Count");   

  TH1F *shower_time_number_S_SDC = new TH1F("shower_time_number_S_SDC", "Timing of Scintillation Photons in SDC for Showers", 500, 0., 500.);
  shower_time_number_S_SDC->GetXaxis()->SetTitle("Time (ns)");
  shower_time_number_S_SDC->GetYaxis()->SetTitle("Count");

  TH1F *shower_time_number_C_SDC = new TH1F("shower_time_number_C_SDC", "Timing of Cherenkov Photons in SDC for Showers", 500, 0., 50.);
  shower_time_number_C_SDC->GetXaxis()->SetTitle("Time (ns)");
  shower_time_number_C_SDC->GetYaxis()->SetTitle("Count");

  TH1F *shower_time_number_S_SDS = new TH1F("shower_time_number_S_SDS", "Timing of Scintillation Photons in SDS for Showers", 500, 0., 500.);
  shower_time_number_S_SDS->GetXaxis()->SetTitle("Time (ns)");
  shower_time_number_S_SDS->GetYaxis()->SetTitle("Count");

  TH1F *shower_time_number_C_SDS = new TH1F("shower_time_number_C_SDS", "Timing of Cherenkov Photons in SDS for Showers", 500, 0., 50.);
  shower_time_number_C_SDS->GetXaxis()->SetTitle("Time (ns)");
  shower_time_number_C_SDS->GetYaxis()->SetTitle("Count");

  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    if (depositedEnergyECAL_r <= 0.1) {
      time_number_S_SDC->Fill(time_SDC_S_detected->GetBinContent(i));
      time_number_C_SDC->Fill(time_SDC_C_detected->GetBinContent(i));
      time_number_S_SDS->Fill(time_SDS_S_detected->GetBinContent(i));
      time_number_C_SDS->Fill(time_SDS_C_detected->GetBinContent(i));
    } else {
      shower_time_number_S_SDC->Fill(time_SDC_S_detected->GetBinContent(i));
      shower_time_number_C_SDC->Fill(time_SDC_C_detected->GetBinContent(i));
      shower_time_number_S_SDS->Fill(time_SDS_S_detected->GetBinContent(i));
      shower_time_number_C_SDS->Fill(time_SDS_C_detected->GetBinContent(i));
    }
  }

  
  // These canvases draw the timing histograms.
  // TCanvas *c101 = new TCanvas();             
  // time_SDC_S_detected->Draw();                   
  // c101->Print(sample + "time_SDC_S_detected.png");
  
  // TCanvas *c102 = new TCanvas(); 
  // time_SDC_C_detected->Draw();   
  // c102->Print(sample + "time_SDC_C_detected.png");
  
  // TCanvas *c105 = new TCanvas();                  
  // time_SDS_S_detected->Draw();                        
  // c105->Print(sample + "time_SDS_S_detected.png");      
                                                
  // TCanvas *c106 = new TCanvas();                  
  // time_SDS_C_detected->Draw();                        
  // c106->Print(sample + "time_SDS_C_detected.png");      
  
  // TCanvas *c109 = new TCanvas();
  // time_SDF_S_detected->Draw();
  // c109->Print(sample + "time_SDF_S_detected.png");
  
  // TCanvas *c110 = new TCanvas();
  // time_SDF_C_detected->Draw();
  // c110->Print(sample + "time_SDF_C_detected.png");

  /*
  // Honestly, I'm not sure what these variables do, but I made histograms of them anyway.
  TH1F *time_number_S_SDC = new TH1F("time_number_S_SDC", "Timing of Scintillation Photons in SDC", 500, 0., 500.);
  time_number_S_SDC->GetXaxis()->SetTitle("Time (ns)");
  time_number_S_SDC->GetYaxis()->SetTitle("Count");
  
  TH1F *time_number_C_SDC = new TH1F("time_number_C_SDC", "Timing of Cherenkov Photons in SDC", 500, 0., 50.);
  time_number_C_SDC->GetXaxis()->SetTitle("Time (ns)");
  time_number_C_SDC->GetYaxis()->SetTitle("Count");
  
  TH1F *time_number_S_SDS = new TH1F("time_number_S_SDS", "Timing of Scintillation Photons in SDS", 500, 0., 500.);
  time_number_S_SDS->GetXaxis()->SetTitle("Time (ns)");
  time_number_S_SDS->GetYaxis()->SetTitle("Count");
  
  TH1F *time_number_C_SDS = new TH1F("time_number_C_SDS", "Timing of Cherenkov Photons in SDS", 500, 0., 50.);
  time_number_C_SDS->GetXaxis()->SetTitle("Time (ns)");
  time_number_C_SDS->GetYaxis()->SetTitle("Count");
  
  TH1F *time_number_S_SDF = new TH1F("time_number_S_SDF", "Timing of Scintillation Photons in SDF", 500, 0., 50.);
  time_number_S_SDF->GetXaxis()->SetTitle("Time (ns)");
  time_number_S_SDF->GetYaxis()->SetTitle("Count");
  
  TH1F *time_number_C_SDF = new TH1F("time_number_C_SDF", "Timing of Cherenkov Photons in SDF", 500, 0., 50.);
  time_number_C_SDF->GetXaxis()->SetTitle("Time (ns)");
  time_number_C_SDF->GetYaxis()->SetTitle("Count");

  TH1F *shower_time_number_S_SDC = new TH1F("shower_time_number_S_SDC", "Timing of Scintillation Photons in SDC for Showers", 500, 0., 500.);
  shower_time_number_S_SDC->GetXaxis()->SetTitle("Time (ns)");
  shower_time_number_S_SDC->GetYaxis()->SetTitle("Count");

  TH1F *shower_time_number_C_SDC = new TH1F("shower_time_number_C_SDC", "Timing of Cherenkov Photons in SDC for Showers", 500, 0., 50.);
  shower_time_number_C_SDC->GetXaxis()->SetTitle("Time (ns)");
  shower_time_number_C_SDC->GetYaxis()->SetTitle("Count");

  TH1F *shower_time_number_S_SDS = new TH1F("shower_time_number_S_SDS", "Timing of Scintillation Photons in SDS for Showers", 500, 0., 500.);
  shower_time_number_S_SDS->GetXaxis()->SetTitle("Time (ns)");
  shower_time_number_S_SDS->GetYaxis()->SetTitle("Count");

  TH1F *shower_time_number_C_SDS = new TH1F("shower_time_number_C_SDS", "Timing of Cherenkov Photons in SDS for Showers", 500, 0., 50.);
  shower_time_number_C_SDS->GetXaxis()->SetTitle("Time (ns)");
  shower_time_number_C_SDS->GetYaxis()->SetTitle("Count");

  
  for (int i = 0; i < entries; i++) {
    chain->GetEntry(i);
    if (depositedEnergyECAL_r <= 0.1) {
      if (SDCtime_r_S <= 500) time_number_S_SDC->Fill(SDCtime_r_S, SDCdetected_r_S);
      time_number_C_SDC->Fill(SDCtime_r_C, SDCdetected_r_C);
      if (SDStime_r_S <= 500) time_number_S_SDS->Fill(SDStime_r_S, SDSdetected_r_S);
      time_number_C_SDS->Fill(SDStime_r_C, SDSdetected_r_C);
    } else {
      if (SDCtime_r_S <= 500) shower_time_number_S_SDC->Fill(SDCtime_r_S, SDCdetected_r_S);
      shower_time_number_C_SDC->Fill(SDCtime_r_C, SDCdetected_r_C);
      if (SDStime_r_S <= 500) shower_time_number_S_SDS->Fill(SDStime_r_S, SDSdetected_r_S);
      shower_time_number_C_SDS->Fill(SDStime_r_C, SDSdetected_r_C);
    }
  }

  */

  /*  
  TCanvas *c111 = new TCanvas();
  // c111->Divide(2, 1);
  // c111->cd(1);
  time_number_S_SDC->Draw("HIST");
  TCanvas *c1115 = new TCanvas();
  // c111->cd(2);
  shower_time_number_S_SDC->Draw("HIST");
  //c111->Print(sample + "time_number_S_SDC.png");
  
  TCanvas *c112 = new TCanvas();
  // c112->Divide(2, 1);
  // c112->cd(1);
  time_number_C_SDC->Draw("HIST");
  TCanvas *c1125 = new TCanvas();
  // c112->cd(2);
  shower_time_number_C_SDC->Draw("HIST");
  // c112->Print(sample + "time_number_C_SDC.png");
  
  TCanvas *c113 = new TCanvas();
  // c113->Divide(2, 1);
  // c113->cd(1);
  time_number_S_SDS->Draw("HIST");
  TCanvas *c1135 = new TCanvas();
  // c113->cd(2);
  shower_time_number_S_SDS->Draw("HIST");
  // c113->Print(sample + "time_number_S_SDS.png");
  
  TCanvas *c114 = new TCanvas();
  // c114->Divide(2, 1);
  // c114->cd(1);
  time_number_C_SDS->Draw("HIST");
  TCanvas *c1145 = new TCanvas();
  // c114->cd(2);
  shower_time_number_C_SDS->Draw("HIST");
  // c114->Print(sample + "time_number_C_SDS.png");
  */



  // ...........................................................................................                                                                                 
  // ..............................PART 11: FILTERING OF PHOTONS................................      
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

  TH1D *filter_front_S = new TH1D("filter_front_S", "Distribution of Scintillation Photons in Front SiPM Multiplied by Filter", 1250, 0., 1250.);
  filter_front_S->GetXaxis()->SetTitle("Wavelength (nm)");
  filter_front_S->GetYaxis()->SetTitle("Transmission (%)");

  TH1D *filter_front_C = new TH1D("filter_front_C","Distribution of Cherenkov Photons in Front SiPM Multiplied by Filter", 1250, 0., 1250.);
  filter_front_C->GetXaxis()->SetTitle("Wavelength (nm)");
  filter_front_C->GetYaxis()->SetTitle("Transmission (%)");

  TGraph *glass_graph = new TGraph("pyrex_glass.txt", "%lg %lg");
  glass_graph->SetTitle("Transmission of Pyrex Glass");
  glass_graph->GetXaxis()->SetTitle("Wavelength (nm)");
  glass_graph->GetYaxis()->SetTitle("Transmission (%)");

  fstream file1;
  
  file1.open("hoya_R60.txt", ios::in);
  // file1.open("hoya_U330.txt", ios::in);
  
  int counter1 = 0;
  while (counter1 < 96) {
    double x1, y1;
    file1 >> x1 >> y1;
    y1 /= 100.0;
    glass_graph->SetPoint(counter1, x1, y1);
    counter1++;
  }
  file1.close();

  // TCanvas *test5 = new TCanvas();
  // glass_graph->Draw("AL");
  // test5->Print(sample + "glass_graph.png");

  /* This for loop multiplies each bin of the existing wavelength distribution by the value of the PDE evaluated at each wavelength.
     It then fills the histogram with those values and normalizes the new histograms. Since the PDE is defined only for specific
     wavelengths, I multiplied each histogram bin by 0 if we were out of the range of the PDE. */
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

  /*
  // These TCanvases plot the normalized histograms with the original ones plotted on the same canvas.                                       
  TCanvas *c211 = new TCanvas();            
  combined_SDC_S->SetLineColor(4);        
  combined_SDC_S->SetFillStyle(3001);    
  filter_SDC_S->SetLineColor(2);
  filter_SDC_S->SetFillStyle(3001);
  combined_SDC_S->GetYaxis()->SetTitle("Count");
  combined_SDC_S->SetTitle("Wavelengths of S Photons from SDC Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  combined_SDC_S->Draw("HIST");                                                    
  filter_SDC_S->Draw("HIST SAME");                                                
  // filter_SDC_S->Draw("HIST");                                                  
  // combined_SDC_S->Draw("HIST SAME");                                            
  TLegend *filter_SDC_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);                
  filter_SDC_S_legend->SetHeader("KEY", "C");                                    
  filter_SDC_S_legend->AddEntry(filter_SDC_S, "Filter", "l")->SetTextColor(2);
  filter_SDC_S_legend->AddEntry(combined_SDC_S, "PDE", "l")->SetTextColor(4);
  filter_SDC_S_legend->SetBorderSize(5);
  filter_SDC_S_legend->Draw();
  c211->Print(sample + normalized + "filter_SDC_S.png");       

  TCanvas *c212 = new TCanvas();
  combined_SDC_C->SetLineColor(4);
  combined_SDC_C->SetFillStyle(3001);
  filter_SDC_C->SetLineColor(2);
  filter_SDC_C->SetFillStyle(3001);
  combined_SDC_C->GetYaxis()->SetTitle("Count");
  combined_SDC_C->SetTitle("Wavelengths of C Photons from SDC Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data. 
  combined_SDC_C->Draw("HIST");
  filter_SDC_C->Draw("HIST SAME");
  // filter_SDC_C->Draw("HIST"); 
  // combined_SDC_C->Draw("HIST SAME");
  TLegend *filter_SDC_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  filter_SDC_C_legend->SetHeader("KEY", "C");
  filter_SDC_C_legend->AddEntry(filter_SDC_C, "Filter", "l")->SetTextColor(2);
  filter_SDC_C_legend->AddEntry(combined_SDC_C, "PDE", "l")->SetTextColor(4);
  filter_SDC_C_legend->SetBorderSize(5);
  filter_SDC_C_legend->Draw();
  c212->Print(sample + normalized + "filter_SDC_C.png");

  TCanvas *c213 = new TCanvas();
  combined_SDS_S->SetLineColor(4);
  combined_SDS_S->SetFillStyle(3001);
  filter_SDS_S->SetLineColor(2);
  filter_SDS_S->SetFillStyle(3001);
  combined_SDS_S->GetYaxis()->SetTitle("Count");
  combined_SDS_S->SetTitle("Wavelengths of S Photons from SDS Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  combined_SDS_S->Draw("HIST");
  filter_SDS_S->Draw("HIST SAME");
  // filter_SDS_S->Draw("HIST");    
  // combined_SDS_S->Draw("HIST SAME");
  TLegend *filter_SDS_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  filter_SDS_S_legend->SetHeader("KEY", "C");
  filter_SDS_S_legend->AddEntry(filter_SDS_S, "Filter", "l")->SetTextColor(2);
  filter_SDS_S_legend->AddEntry(combined_SDS_S, "PDE", "l")->SetTextColor(4);
  filter_SDS_S_legend->SetBorderSize(5);
  filter_SDS_S_legend->Draw();
  c213->Print(sample + normalized + "filter_SDS_S.png");

  TCanvas *c214 = new TCanvas();
  combined_SDS_C->SetLineColor(4);
  combined_SDS_C->SetFillStyle(3001);
  filter_SDS_C->SetLineColor(2);
  filter_SDS_C->SetFillStyle(3001);
  combined_SDS_C->GetYaxis()->SetTitle("Count");
  combined_SDS_C->SetTitle("Wavelengths of C Photons from SDS Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  combined_SDS_C->Draw("HIST");
  filter_SDS_C->Draw("HIST SAME");
  // filter_SDS_C->Draw("HIST");       
  // combined_SDS_C->Draw("HIST SAME");
  TLegend *filter_SDS_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  filter_SDS_C_legend->SetHeader("KEY", "C");
  filter_SDS_C_legend->AddEntry(filter_SDS_C, "Filter", "l")->SetTextColor(2);
  filter_SDS_C_legend->AddEntry(combined_SDS_C, "PDE", "l")->SetTextColor(4);
  filter_SDS_C_legend->SetBorderSize(5);
  filter_SDS_C_legend->Draw();
  c214->Print(sample + normalized + "filter_SDS_C.png");

  TCanvas *c215 = new TCanvas();
  combined_front_S->SetLineColor(4);
  combined_front_S->SetFillStyle(3001);
  filter_front_S->SetLineColor(2);
  filter_front_S->SetFillStyle(3001);
  combined_front_S->GetYaxis()->SetTitle("Count");
  combined_front_S->SetTitle("Wavelengths of S Photons from SDF Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  combined_front_S->Draw("HIST");
  filter_front_S->Draw("HIST SAME");
  // filter_front_S->Draw("HIST");        
  // combined_front_S->Draw("HIST SAME");  
  TLegend *filter_front_S_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  filter_front_S_legend->SetHeader("KEY", "C");
  filter_front_S_legend->AddEntry(filter_front_S, "Filter", "l")->SetTextColor(2);
  filter_front_S_legend->AddEntry(combined_front_S, "PDE", "l")->SetTextColor(4);
  filter_front_S_legend->SetBorderSize(5);
  filter_front_S_legend->Draw();
  c215->Print(sample + normalized + "filter_front_S.png");

  TCanvas *c216 = new TCanvas();
  combined_front_C->SetLineColor(4);
  combined_front_C->SetFillStyle(3001);
  filter_front_C->SetLineColor(2);
  filter_front_C->SetFillStyle(3001);
  combined_front_C->GetYaxis()->SetTitle("Count");
  combined_front_C->SetTitle("Wavelengths of C Photons from SDF Multiplied by Filter");

  // If these two are commented out, then we have unnormalized data. If the other two are commented out, we have normalized data.
  combined_front_C->Draw("HIST");
  filter_front_C->Draw("HIST SAME");
  // filter_front_C->Draw("HIST");
  // combined_front_C->Draw("HIST SAME");
  TLegend *filter_front_C_legend = new TLegend(0.9, 0.8, 0.7, 0.9);
  filter_front_C_legend->SetHeader("KEY", "C");
  filter_front_C_legend->AddEntry(filter_front_C, "Filter", "l")->SetTextColor(2);
  filter_front_C_legend->AddEntry(combined_front_C, "PDE", "l")->SetTextColor(4);
  filter_front_C_legend->SetBorderSize(5);
  filter_front_C_legend->Draw();
  c216->Print(sample + normalized + "filter_front_C.png");
  */  

  return 0;  
}


//  Code written by Christian Guinto-Brody for Bob Hirosky's High-Energy-Physics group with his and his colleagues' help.
