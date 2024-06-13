// This macro simply holds any extra code I may have.


                                                                                                                                               
TProfile2D *S_SDC_lambda_pos = new TProfile2D("S_SDC_lambda_pos", "Positions of S Wavelengths in SDC as a Function of Position", 100, -14.9, 15.0, 100, -14.9, 15.0);             
TProfile2D *C_SDC_lambda_pos = new TProfile2D("C_SDC_lambda_pos", "Positions of C Wavelengths in SDC as a Function of Position", 100, -14.9, 15.0, 100, -14.9, 15.0);             
TProfile2D *S_SDS_lambda_pos = new TProfile2D("S_SDS_lambda_pos", "Positions of S Wavelengths in SDS as a Function of Position", 100, -14.9, 15.0, 100, -14.9, 15.0);             
TProfile2D *C_SDS_lambda_pos = new TProfile2D("C_SDS_lambda_pos", "Positions of C Wavelengths in SDS as a Function of Position", 100, -14.9, 15.0, 100, -14.9, 15.0);             
                                                                                                                                                                                  
                                                                                                                                                                                  
for (int i = 0; i < entries; i++) {                                                                                                                                               
  cout << SDC_S_lambdas->GetBinContent(i);                                                                                                                                        
  S_SDC_lambda_pos->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDC_S_lambdas->GetBinContent(i));                                                                
  C_SDC_lambda_pos->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDC_C_lambdas->GetBinContent(i));                                                                
  S_SDS_lambda_pos->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDS_S_lambdas->GetBinContent(i));                                                                
  C_SDS_lambda_pos->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDS_C_lambdas->GetBinContent(i));                                                                
 }                                                                                                                                                                                
                                                                                                                                                                                  
TCanvas *c7 = new TCanvas();                                                                                                                                                      
S_SDC_lambda_pos->Draw("colz");                                                                                                                                                   
                                                                                                                                                                                  
TCanvas *c8 = new TCanvas();                                                                                                                                                      
C_SDC_lambda_pos->Draw("colz");                                                                                                                                                   
                                                                                                                                                                                  
TCanvas *c9 = new TCanvas();                                                                                                                                                      
S_SDS_lambda_pos->Draw("colz");                                                                                                                                                   
                                                                                                                                                                                  
TCanvas *c10 = new TCanvas();      












THStack *S_generated_lambdas = new THStack("S_generated_lambdas", "Stacked Wavelength Distributions for Generated Scintillation Photons");
THStack *C_generated_lambdas = new THStack("C_generated_lambdas", "Stacked Wavelength Distributions for Generated Cherenkov Photons");

// This series of code sets the color and style of the histograms in the stacks.                                                                                                
front_S_generated->SetFillColor(kRed);
front_S_generated->SetMarkerStyle(21);
front_S_generated->SetMarkerColor(kRed);

rear_S_generated->SetFillColor(kBlue);
rear_S_generated->SetMarkerStyle(21);
rear_S_generated->SetMarkerColor(kBlue);

front_C_generated->SetFillColor(kRed);
front_C_generated->SetMarkerStyle(21);
front_C_generated->SetMarkerColor(kRed);

rear_C_generated->SetFillColor(kBlue);
rear_C_generated->SetMarkerStyle(21);
rear_C_generated->SetMarkerColor(kBlue);

S_generated_lambdas->Add(front_S_generated);
S_generated_lambdas->Add(rear_S_generated);
C_generated_lambdas->Add(front_C_generated);
C_generated_lambdas->Add(rear_C_generated);


// These canvases plot the stacks and create legends so that we know which distribution on the stack is which.                                                                 
TCanvas *b9 = new TCanvas();
b9->Divide(2, 2);
S_generated_lambdas->Draw("nostackp");
TLegend *S_lambdas = new TLegend(0.9, 0.7, 0.48, 0.9);
S_lambdas->SetHeader("KEY", "C");
S_lambdas->AddEntry(front_S_generated, "Front", "l")->SetTextColor(kRed);
S_lambdas->AddEntry(rear_S_generated, "Rear", "l")->SetTextColor(kBlue);
S_lambdas->SetBorderSize(5);
S_lambdas->Draw();
b9->Print("S_generated_lambdas.png");

TCanvas *b10 = new TCanvas();
b10->Divide(2, 2);
C_generated_lambdas->Draw("nostackp");
TLegend *C_lambdas = new TLegend(0.9, 0.7, 0.48, 0.9);
C_lambdas->SetHeader("KEY", "C");
C_lambdas->AddEntry(front_C_generated, "Front", "l")->SetTextColor(kRed);
C_lambdas->AddEntry(rear_C_generated, "Rear", "l")->SetTextColor(kBlue);
C_lambdas->SetBorderSize(5);
C_lambdas->Draw();
b10->Print("C_generated_lambdas.png");



/*                                                                                                                                                                              
  TProfile2D *total_S_SDC = new TProfile2D("total_S_SDC", "Total Scintillation Photons in SDC vs. Position", 100, -15.0, 15.0, 100, -15.0, 15.0);                                 
  total_S_SDC->Add(avg_detected_photons_S_SDC);                                                                                                                                   
  total_S_SDC->Add(avg_S_SDC_shower);                                                                                                                                             
  TCanvas *test = new TCanvas();                                                                                                                                                  
  total_S_SDC->Draw("colz");                                                                                                                                                      
                                                                                                                                                                                  
  TProfile2D *total_C_SDC = new TProfile2D("total_C_SDC", "Total Cherenkov Photons in SDC vs. Position", 100, -15.0, 15.0, 100, -15.0, 15.0);                                     
  total_C_SDC->Add(avg_detected_photons_C_SDC);                                                                                                                                   
  total_C_SDC->Add(avg_C_SDC_shower);                                                                                                                                             
                                                                                                                                                                                  
  TProfile2D *total_S_SDS = new TProfile2D("total_S_SDS", "Total Scintillation Photons in SDS vs. Position", 100, -15.0, 15.0, 100, -15.0, 15.0);                                 
  total_S_SDS->Add(avg_detected_photons_S_SDS);                                                                                                                                   
  total_S_SDS->Add(avg_S_SDS_shower);                                                                                                                                             
                                                                                                                                                                                  
  TProfile2D *total_C_SDS = new TProfile2D("total_C_SDS", "Total Cherenkov Photons in SDS vs. Position", 100, -15.0, 15.0, 100, -15.0, 15.0);                                     
total_C_SDS->Add(avg_detected_photons_C_SDS);                                                                                                                                   
total_C_SDS->Add(avg_C_SDS_shower);                                                                                                                                             
                                                                                                                                                                                  
  */
// TH1D *total_energy_S_SDC = new TH1D("total_energy_S_SDC", "Total Energy Distribution of Scintillation Photons in SDC", 100, 0.0, 25.0);                                      
// TH1D *total_energy_C_SDC = new TH1D("total_energy_C_SDC", "Total Energy Distribution of Cherenkov Photons in SDC", 100, 0.0, 25.0);                                          
// TH1D *total_energy_S_SDS = new TH1D("total_energy_S_SDS", "Total Energy Distribution of Scintillation Photons in SDS", 100, 0.0, 25.0);                                      
// TH1D *total_energy_C_SDS = new TH1D("total_energy_C_SDS", "Total Energy Distribution of Cherenkov Photons in SDS", 100, 0.0, 25.0);                                          

/* for (int i = 0; i < entries; i++) {                                                                                                                                          
    chain->GetEntry(i);                                                                                                                                                           
    total_energy_S_SDC->Fill(depositedEnergyECAL_r, SDCdetected_r_S);                                                                                                             
    total_energy_C_SDC->Fill(depositedEnergyECAL_r, SDCdetected_r_C);                                                                                                             
    total_energy_S_SDS->Fill(depositedEnergyECAL_r, SDSdetected_r_S);                                                                                                             
    total_energy_C_SDS->Fill(depositedEnergyECAL_r, SDSdetected_r_C);                                                                                                             
  }                                                                                                                                                                               
*/
// TProfile2D *test_total = new TProfile2D("test_total", "Test Total", 100, -15.0, 15.0, 100, -15.0, 15.0);                                                                     
// test_total->GetXaxis()->SetTitle("x-Position");                                                                                                                              
// test_total->GetYaxis()->SetTitle("y-Position");     
/* if (log10(depositedEnergyECAL_r) >= 1 && log10(depositedEnergyECAL_r) < 3.5) {                                                                                                 
      test_total->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_S);                                                                                  
    }                                                                                                                                                                             
*/
/*                                                                                                                                                                                
																						  TCanvas *test2 = new TCanvas();                                                                                                                                                 
test_total->Draw("colz");                                                                                                                                                       
                                                                                                                                                                                  
for (int i = 0; i <= 100; i++) {                                                                                                                                                
x = i * (3.0/10.0) - 15.0;                                                                                                                                                    
// cout << x << endl;                                                                                                                                                         
y = i * (3.0/10.0) - 15.0;                                                                                                                                                    
if (x >= -2.1 && x < 2.1) {                                                                                                                                                   
for (int j = 0; j <= 100; j++) {                                                                                                                                            
number_vs_energy_S_SDC->Fill(j * (3.0/10.0) - 15.0, h_energy_LOG->GetBinContent(j), total_S_SDC->GetBinContent(x, j));                                                    
// cout << total_S_SDC->GetBinContent(x, j) << " " << test_total->GetBinContent(50, j) << endl;                                                                           
// cout << number_vs_energy_S_SDC->GetBinContent(x, j) << endl;                                                                                                           
number_vs_energy_C_SDC->Fill(j * (3.0/10.0) - 15.0, h_energy_LOG->GetBinContent(j), total_C_SDC->GetBinContent(x, j));                                                    
number_vs_energy_S_SDS->Fill(j * (3.0/10.0) - 15.0, h_energy_LOG->GetBinContent(j), total_S_SDS->GetBinContent(x, j));                                                    
number_vs_energy_C_SDS->Fill(j * (3.0/10.0) - 15.0, h_energy_LOG->GetBinContent(j), total_C_SDS->GetBinContent(x, j));                                                    
}                                                                                                                                                                           
}                                                                                                                                                                             
}                                                                                                                                                                               
*/
/* if (log10(depositedEnergyECAL_r) >= 1 && log10(depositedEnergyECAL_r) < 3.5) {                                                                                                 
      test_total->Fill((*inputInitialPosition)[0], (*inputInitialPosition)[1], SDCdetected_r_S);                                                                                  
    }                                                                                                                                                                             
*/
/*                                                                                                                                                                                
  TCanvas *test2 = new TCanvas();                                                                                                                                                 
  test_total->Draw("colz");                                                                                                                                                       
                                                                                                                                                                                  
  for (int i = 0; i <= 100; i++) {                                                                                                                                                
    x = i * (3.0/10.0) - 15.0;                                                                                                                                                    
    // cout << x << endl;                                                                                                                                                         
    y = i * (3.0/10.0) - 15.0;                                                                                                                                                    
    if (x >= -2.1 && x < 2.1) {                                                                                                                                                   
      for (int j = 0; j <= 100; j++) {                                                                                                                                            
        number_vs_energy_S_SDC->Fill(j * (3.0/10.0) - 15.0, h_energy_LOG->GetBinContent(j), total_S_SDC->GetBinContent(x, j));                                                    
        // cout << total_S_SDC->GetBinContent(x, j) << " " << test_total->GetBinContent(50, j) << endl;                                                                           
        // cout << number_vs_energy_S_SDC->GetBinContent(x, j) << endl;                                                                                                           
        number_vs_energy_C_SDC->Fill(j * (3.0/10.0) - 15.0, h_energy_LOG->GetBinContent(j), total_C_SDC->GetBinContent(x, j));                                                    
number_vs_energy_S_SDS->Fill(j * (3.0/10.0) - 15.0, h_energy_LOG->GetBinContent(j), total_S_SDS->GetBinContent(x, j));                                                    
number_vs_energy_C_SDS->Fill(j * (3.0/10.0) - 15.0, h_energy_LOG->GetBinContent(j), total_C_SDS->GetBinContent(x, j));                                                    
}                                                                                                                                                                           
}                                                                                                                                                                             
}                                                                                                                                                                               
*/



// combined->Fit("landau");                                                                                                                                                     
// combined->Draw();                                                                                                                                                            
// TF1 *combined_fit = combined->GetFunction("landau");                                                                                                                         
// combined_fit->Draw();                                                                                                                                                        

// TH1D *PDE_SDC_S = new TH1D("PDE_SDC", "Probability Distribution of SDC S Photons", 1250, 0., 1250.);                                                                         
// PDE_SDC_S->Fill(SDC_S_lambdas->Fit("landau") * PDE->Fit("landau"));                                                                                                          
// PDE_SDC_S->Draw();                                                                                                                                                           

/*                                                                                                                                                                              
  TH1D *PDE = new TH1D("PDE", "Photon Detection Efficiency", 1250, 0., 1250.);                                                                                                    
  // PDE->SetMarkerStyle(kFullCircle);                                                                                                                                            
  fstream file;                                                                                                                                                                   
  file.open("s13360_series_50u.txt", ios::in);                                                                                                                                    
  while (1) {                                                                                                                                                                     
    double x, y;                                                                                                                                                                  
    file >> x >> y;                                                                                                                                                               
    PDE->SetBinContent(x, y);                                                                                                                                                     
    if (file.eof()) break;                                                                                                                                                        
}                                                                                                                                                                              
file.close();                                                                                                                                                                   
PDE->Draw("HIST");     // combined->Fit("landau");                                                                                                                                                     
// combined->Draw();                                                                                                                                                            
// TF1 *combined_fit = combined->GetFunction("landau");                                                                                                                         
// combined_fit->Draw();                                                                                                                                                        

// TH1D *PDE_SDC_S = new TH1D("PDE_SDC", "Probability Distribution of SDC S Photons", 1250, 0., 1250.);                                                                         
// PDE_SDC_S->Fill(SDC_S_lambdas->Fit("landau") * PDE->Fit("landau"));                                                                                                          
// PDE_SDC_S->Draw();                                                                                                                                                           

/*                                                                                                                                                                              
  TH1D *PDE = new TH1D("PDE", "Photon Detection Efficiency", 1250, 0., 1250.);                                                                                                    
  // PDE->SetMarkerStyle(kFullCircle);                                                                                                                                            
  fstream file;                                                                                                                                                                   
  file.open("s13360_series_50u.txt", ios::in);                                                                                                                                    
  while (1) {                                                                                                                                                                     
    double x, y;                                                                                                                                                                  
    file >> x >> y;                                                                                                                                                               
    PDE->SetBinContent(x, y);                                                                                                                                                     
    if (file.eof()) break;                                                                                                                                                        
}                                                                                                                                                                              
file.close();                                                                                                                                                                   
PDE->Draw("HIST");    

TCanvas *b = new TCanvas();                                                                                                                                                       
TGraph *PDE = new TGraph("s13360_series_50u.txt");                                                                                                                                
PDE->Draw("AL");                                                                                                                                                                  
// b->Print("plotNewIso.pdf");                                                                                                                                                    
                                                                                                                                                                                  
                                                                                                                                                                                  
// TGraph *PDE = new TGraph("s13360_series_50u.txt");                                                                                                                             
// PDE->Draw("AC");                                                                                                                                                               
                                                                                                                                                                                  
TGraph *PDE = new TGraph();                                                                                                                                                    \
                                                                                                                                                                                  
                                                                                                                                                                                 \
                                                                                                                                                                                  
PDE->SetMarkerStyle(kFullCircle);                                                                                                                                              \
                                                                                                                                                                                  
file.open("s13360_series_50u.txt", ios::in);                                                                                                                                   \
                                                                                                                                                                                  
while (1) {                                                                                                                                                                       
double x, y;                                                                                                                                                                      
file >> x >> y;                                                                                                                                                                   
PDE->SetPoint(PDE->GetN(), x, y);                                                                                                                                                 
if (file.eof()) break;                                                                                                                                                            
}                                                                                                                                                                                 
                                                                                                                                                                                 \
                                                                                                                                                                                  
file.close();   


// TGraph *PDE_graph = new TGraph(PDE->GetN(), PDE->GetX(), PDE->GetY());                                                                                                       
// delete PDE;                                                                                                                                                                  
// TCanvas *d1 = new TCanvas();                                                                                                                                                 
// PDE_graph->Draw("AL");                                                                                                                                                       

/* Double_t scale_bin = 0;                                                                                                                                                      
  for(int i = 0; i <= SDC_S-lambdas->GetNbinsx(); i++) {                                                                                                                          
    scale_bin = eff->Eval(hgamma->GetBinCenter(i));                                                                                                                               
    hgamma->SetBinContent(i,scale_bin*hgamma->GetBinContent(i));                                                                                                                  
  }                                                                                                                                                                               
*/
// gStyle->SetOptFit();                                                                                                                                                         
// TH1D *PDE_hist = new TH1D("PDE_hist", "Photon Detection Efficiency", 1250, 0., 1250.);                                                                                       
// auto nPoints = PDE_graph->GetN(); // number of points in your TGraph                                                                                                         
// cout << PDE->GetN() << endl;                                                                                                                                                 
// for(int i = 0; i < PDE->GetN(); ++i) {                                                                                                                                       
// double x, y;                                                                                                                                                                 
// double y_100;                                                                                                                                                              
// PDE->GetPoint(i, x, y);                                                                                                                                                    
// cout << y << endl;                                                                                                                                                         
// y_100 = y / 100.0;                                                                                                                                                         
// cout << y << endl;                                                                                                                                                         
// PDE_hist->Fill(x, y); // ?                                                                                                                                                 
// }                                                                                                                                                                            
// TCanvas *d1 = new TCanvas();                                                                                                                                                 
// PDE->Draw("AL");                                                                                                                                                             
// TCanvas *d2 = new TCanvas();                                                                                                                                                 
// PDE_hist->Draw("HIST"); 


// TH1D combined_test = multiply(SDC_S_lambdas, PDE_hist);                                                                                                                      

// combined_test = SDC_S_lambdas * PDE_hist;                                                                                                                                    
// combined_test->Draw("HIST");                                                                                                                                                 

// TH1D *PDE = new TH1D("PDE", "Photon Detection Efficiency", 1250, 0., 1250.);                                                                                                 
// fstream file;                                                                                                                                                                
// file.open("s13360_series_50u.txt", ios::in);                                                                                                                                 
// int i = 0;                                                                                                                                                                   
// while (i < 22) {                                                                                                                                                             
// double x, y;                                                                                                                                                               
// file >> x >> y;                                                                                                                                                            
//  PDE->SetBinContent(x, y);                                                                                                                                                 
// i++;                                                                                                                                                                       
// }                                                                                                                                                                          
// file.close();    


Double_t scale_bin = 0;                                                                                                                                                         
for (int i = 0; i <= SDC_->GetNbinsX(); i++) {                                                                                                                                  
  scale_bin = PDE_fit->Eval(SDC_S_lambdas->GetBinCenter(i));                                                                                                                    
  combined_SDC_S->SetBinContent(i, scale_bin * SDC_S_lambdas->GetBinContent(i));                                                                                                
 }*/


// cout << combined_SDC_S->Integral(0., 1250.) << endl;                                                                                                                         
/*                                                                                                                                                                              
  // This section of code fills the above histograms.                                                                                                                             
  float y_SDC_S = 0, y_SDC_C = 0, y_SDS_S = 0, y_SDS_C = 0;                                                                                                                       
  float y_front_S = 0, y_rear_S = 0, y_front_C = 0, y_rear_C = 0;                                                                                                                 
  for (int i = 0; i < 1250; i++) {                                                                                                                                                
    y_SDC_S = SDC_S_fit->Eval(i) * PDE_fit->Eval(i);                                                                                                                              
    y_SDC_C = SDC_C_fit->Eval(i) * PDE_fit->Eval(i);                                                                                                                              
    y_SDS_S = SDS_S_fit->Eval(i) * PDE_fit->Eval(i);                                                                                                                              
    y_SDS_C = SDS_C_fit->Eval(i) * PDE_fit->Eval(i);                                                                                                                              
    y_front_S = front_S_fit->Eval(i) * PDE_fit->Eval(i);                                                                                                                          
    y_rear_S = rear_S_fit->Eval(i) * PDE_fit->Eval(i);                                                                                                                            
    y_front_C = front_C_fit->Eval(i) * PDE_fit->Eval(i);                                                                                                                          
    y_rear_C = rear_C_fit->Eval(i) * PDE_fit->Eval(i);     


  combined_SDC_S->Fill(i, y_SDC_S);                                                                                                                                             
    combined_SDC_C->Fill(i, y_SDC_C);                                                                                                                                             
    combined_SDS_S->Fill(i, y_SDS_S);                                                                                                                                             
    combined_SDS_C->Fill(i, y_SDS_C);                                                                                                                                             
    combined_front_S->Fill(i, y_front_S);                                                                                                                                         
    combined_rear_S->Fill(i, y_rear_S);                                                                                                                                           
combined_front_C->Fill(i, y_front_C);                                                                                                                                         
combined_rear_C->Fill(i, y_rear_C);                                                                                                                                           
}                                                                                                                                                                               
                                                                                                                                                                                  
// This section noramlizes the new histograms.                                                                                                                                  
combined_SDC_S->Scale(factor/combined_SDC_S->Integral("width"));                                                                                                                
combined_SDC_C->Scale(factor/combined_SDC_C->Integral("width"));                                                                                                                
combined_SDS_S->Scale(factor/combined_SDS_S->Integral("width"));                                                                                                                
combined_SDS_C->Scale(factor/combined_SDS_C->Integral("width"));                                                                                                                
combined_front_S->Scale(factor/combined_front_S->Integral("width"));                                                                                                            
combined_rear_S->Scale(factor/combined_rear_S->Integral("width"));                                                                                                              
combined_front_C->Scale(factor/combined_front_C->Integral("width"));                                                                                                            
combined_rear_C->Scale(factor/combined_rear_C->Integral("width"));                                                                                                              
  */

 combined_SDC_S->Fill(i, y_SDC_S);                                                                                                                                             
combined_SDC_C->Fill(i, y_SDC_C);                                                                                                                                             
combined_SDS_S->Fill(i, y_SDS_S);                                                                                                                                             
combined_SDS_C->Fill(i, y_SDS_C);                                                                                                                                             
combined_front_S->Fill(i, y_front_S);                                                                                                                                         
combined_rear_S->Fill(i, y_rear_S);                                                                                                                                           
combined_front_C->Fill(i, y_front_C);                                                                                                                                         
combined_rear_C->Fill(i, y_rear_C);                                                                                                                                           
}                                                                                                                                                                               
                                                                                                                                                                                  
// This section noramlizes the new histograms.                                                                                                                                  
combined_SDC_S->Scale(factor/combined_SDC_S->Integral("width"));                                                                                                                
combined_SDC_C->Scale(factor/combined_SDC_C->Integral("width"));                                                                                                                
combined_SDS_S->Scale(factor/combined_SDS_S->Integral("width"));                                                                                                                
combined_SDS_C->Scale(factor/combined_SDS_C->Integral("width"));                                                                                                                
combined_front_S->Scale(factor/combined_front_S->Integral("width"));                                                                                                            
combined_rear_S->Scale(factor/combined_rear_S->Integral("width"));                                                                                                              
combined_front_C->Scale(factor/combined_front_C->Integral("width"));                                                                                                            
combined_rear_C->Scale(factor/combined_rear_C->Integral("width"));                                                                                                              
  */
