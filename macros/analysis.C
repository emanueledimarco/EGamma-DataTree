#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TROOT.h>        
#include <TSystem.h>      
#include <TFile.h>        
#include <TTree.h>        
#include <TClonesArray.h> 
#include <vector>         
#include <iostream>       
#include <iomanip>        
#include <fstream>        
#include <string>         
#include <sstream>        
#include <TH1F.h>                
#include <TCanvas.h>                

// define structures to read in ntuple
#include "EGamma/DataTree/interface/Types.h"
#include "EGamma/DataTree/interface/TEventInfo.hh"
#include "EGamma/DataTree/interface/TGenParticle.hh"
#include "EGamma/DataTree/interface/TElectron.hh"

#endif

using namespace std;

void analysis(const string inputfile) {

  //*****************************************************************************************
  // Set up input
  //*****************************************************************************************
  TFile *infile=0;
  TTree *eventTree=0;
  
  // Data structures to store info from TTrees
  egmana::TEventInfo *info  = new egmana::TEventInfo();
  TClonesArray *genparticleArr = new TClonesArray("egmana::TGenParticle");
  TClonesArray *electronArr = new TClonesArray("egmana::TElectron");

  // Read input file and get the TTrees
  cout << "Processing " << inputfile << "..." << endl;
  infile = TFile::Open(inputfile.c_str(),"read");
  assert(infile);
    
  eventTree = (TTree*)infile->Get("Events"); assert(eventTree);  
  eventTree->SetBranchAddress("Info",     &info);        TBranch *infoBr     = eventTree->GetBranch("Info");
  eventTree->SetBranchAddress("GenParticle", &genparticleArr); TBranch *genparticleBr = eventTree->GetBranch("GenParticle");
  eventTree->SetBranchAddress("Electron", &electronArr); TBranch *electronBr = eventTree->GetBranch("Electron");
  cout << "NEvents = " << eventTree->GetEntries() << endl;

  
  Double_t weight = 1;

  // loop over events
  for(UInt_t ientry=0; ientry<eventTree->GetEntries(); ientry++) {
    if (ientry % 1000 == 0) cout << "Processed Event " << ientry << endl;
    infoBr->GetEntry(ientry);

    genparticleArr->Clear(); 
    electronArr->Clear();

    genparticleBr->GetEntry(ientry);
    electronBr->GetEntry(ientry);

    //***********************************************************
    // Gen-Level particles
    //***********************************************************
    for(Int_t i=0; i<genparticleArr->GetEntriesFast(); i++) {
      const egmana::TGenParticle *p = (egmana::TGenParticle*)((*genparticleArr)[i]);

      cout << p->pdgid << " " << p->status << " " << p->pt << " " << p->eta << " " << p->phi 
           << " | " << p->motherPdgID << "\n";

    }


    //********************************************************
    //Loop Over Electrons
    //********************************************************
    for(Int_t i=0; i<electronArr->GetEntriesFast(); i++) {
      const egmana::TElectron *ele = (egmana::TElectron*)((*electronArr)[i]);
      
      if (!(ele->pt > 5)) continue;
      if (!(fabs(ele->scEta) < 2.5)) continue;
      
      //pass loose veto cuts
      //if (!PassEleSimpleCutsVeto( ele, pfcandidateArr, rho,
      //                            kDataEra_2012_MC, false)) continue;

      cout << "Electrons : " << ele->pt << " " << ele->eta << " " << ele->phi << "\n";

    }

  }
  delete infile;
  infile=0, eventTree=0;      
  delete info;
  delete genparticleArr;
  
  
  //--------------------------------------------------------------------------------------------------------------
  // Output
  //==============================================================================================================
  
}