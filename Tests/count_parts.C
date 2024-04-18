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
vector<int>*    Tpdg;
vector<double>* Tmax_mom;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
void processRootFile(const char* filename) {

  // Get the tree "Edep" and set branch addresses     
  TFile* inputFile = TFile::Open(filename); 
  TTree* Tree = dynamic_cast<TTree*>(inputFile->Get("outputTree"));

  // Set branch addresses
  Tree->SetBranchAddress("Tcells_in_event", &Tentries);
  Tree->SetBranchAddress("Tpdg",            &Tpdg);
  Tree->SetBranchAddress("Tmax_mom",        &Tmax_mom);

  int tot_evts = 0;
  int N_pizero = 0;
  int N_g = 0;
  int N_ph = 0;
  int N_zero_mom = 0;
  int N_ph_zero = 0;
  int N_g_zero = 0;
  
  // Loop over entries in the tree
  Long64_t nEntries = Tree->GetEntries();
  for (Long64_t i = 0; i < nEntries; i++) {
    Tree->GetEntry(i);
    tot_evts += Tentries;
    for (size_t j = 0; j < Tentries; j++) {

      double p = (*Tmax_mom)[j]; 
      int pdg  = (*Tpdg)[j];
      
      if(p==0) {
        N_zero_mom++;
      }
      if(pdg==22){
        N_ph++;
      }
      if(pdg==111){
        N_pizero++;
      }
      if(pdg==9){
        N_g++;
      }
      if(p==0&&pdg==22){
        N_ph_zero++;
      }
      if(p==0&&pdg==9){
        N_g_zero++;
      }
    }
  }
  
  inputFile->Close();

  cout << "Number of photons:                " << N_ph << "/" << tot_evts 
          << " (" << double(N_ph)/tot_evts*100 << "%)" << endl
       << "Number of gluons:                 " << N_g << "/" << tot_evts 
          << " (" << double(N_g)/tot_evts*100 << "%)" << endl
       << "Number of pi zero:                " << N_pizero << "/" << tot_evts 
          << " (" << double(N_pizero)/tot_evts*100 << "%)" << endl
       << "Number of zero momenta particles: " << N_zero_mom << "/" << tot_evts
          << " (" << double(N_zero_mom)/tot_evts*100 << "%)" << endl
       << "Number of zero momenta photons:   " << N_ph_zero << "/" << N_ph
          << " (" << double(N_ph_zero)/N_ph*100 << "%)" << endl
       << "Number of zero momenta gluons:   " << N_g_zero << "/" << N_g
          << " (" << double(N_g_zero)/N_g*100 << "%)" << endl;


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


