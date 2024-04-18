/////////////////////////////////////////////////////////////////
// Program validating the data after processed thru the code Ntuple
// 
//
/////////////////////////////////////////////////////////////////
#include <iostream>
#include <chrono>
#include <algorithm>
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <vector>
#include <utility> 
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TChain.h>
#include <sys/types.h>
#include <dirent.h>
using namespace std;


  // Declare variables to hold branch values
  int Tentries;
  vector<int>*    Tcublet_idx;
  vector<int>*    Tcell_idx;
  vector<double>* Tglob_t;

  TH2F* histogram = new TH2F("histogram", "Energy Deposition in Time", 50, 0, 10, 60, -600, 600);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
void processRootFile(const char* filename) {

  // Get the tree "Edep" and set branch addresses     
  TFile* inputFile = TFile::Open(filename); 
  TTree* Tree = dynamic_cast<TTree*>(inputFile->Get("outputTree"));

  // Set branch addresses
  Tree->SetBranchAddress("Tcells_in_event", &Tentries);
  Tree->SetBranchAddress("Tcublet_idx",     &Tcublet_idx);
  Tree->SetBranchAddress("Tcell_idx",       &Tcell_idx);
  Tree->SetBranchAddress("Tglob_t",         &Tglob_t);
  
  // Loop over entries in the tree
  Long64_t nEntries = Tree->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++) {
    Tree->GetEntry(i);
    
    for (size_t j = 0; j < Tentries; j++) {

      double t    = (*Tglob_t)[j]; 
      int cub_idx = (*Tcublet_idx)[j];
      int cel_idx = (*Tcell_idx)[j];

      double z = (cub_idx*10+cel_idx)/100*12 - 600;

      histogram->Fill(t, z);
    }
  }
  
  inputFile->Close();

  TCanvas* canvas = new TCanvas("canvas", "Energy Deposition in Time", 1200, 800);
  histogram->SetStats(0);
  histogram->Draw("COLZ");
  histogram->SetMarkerStyle(21);
  histogram->SetMarkerSize(2);
  histogram->SetMarkerColor(kRed);
  histogram->GetXaxis()->SetTitle("Time [ns]");
  histogram->GetYaxis()->SetTitle("Z Coordinate [mm]");
  canvas->SetLogz();
  canvas->Update();
  canvas->Draw();
  canvas->SaveAs("outputTree_time.png");

}// end function


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
  // Check if the file path is provided as a command-line argument
  if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <string filePath>" << std::endl;
        return 1; // Return a non-zero error code
    }

    // Use the file path provided as a command-line argument
    std::string filePath = argv[1];
    processRootFile(filePath.c_str());
    return 0;
}  


