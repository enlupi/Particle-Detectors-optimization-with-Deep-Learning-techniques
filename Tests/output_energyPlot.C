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

vector<int>* Tcublet_idx;
vector<double>* Tedep;

TH2F *histogram = new TH2F("histogram", "Scatter Plot", 10, 0, 10, 10, 0, 10);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
void processRootFile(const char* filename) {
  // Get the tree "Edep" and set branch addresses     
  TFile* inputFile = TFile::Open(filename); 
  TTree* Tree = dynamic_cast<TTree*>(inputFile->Get("outputTree"));

  // Set branch addresses
  Tree->SetBranchAddress("Tcells_in_event", &Tentries);
  Tree->SetBranchAddress("Tcublet_idx", &Tcublet_idx);
  Tree->SetBranchAddress("Tedep", &Tedep);

  // Loop over entries in the tree
  Long64_t nEntries = Tree->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++) {
    Tree->GetEntry(i);
    for (size_t j = 0; j < Tentries; j++) {

      double E = (*Tedep)[j]; 
      int cub_idx = (*Tcublet_idx)[j];

      int layer_cublet_idx = cub_idx%100;
      int x = layer_cublet_idx % 10;
      int y = 9-layer_cublet_idx/10;

      histogram->Fill(x, y, E);
    }
  }

  inputFile->Close();

  TCanvas* canvas = new TCanvas("canvas", "Scatter Plot", 800, 800);
  histogram->Draw("COLZ");
  histogram->SetMarkerStyle(21);
  histogram->SetMarkerSize(2);
  histogram->SetMarkerColor(kRed);
  canvas->SetLogz();
  canvas->Update();
  canvas->Draw();
  canvas->SaveAs("outputTree_Edep.png");

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


