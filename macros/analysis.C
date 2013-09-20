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
#include <TVector3.h>

// define structures to read in ntuple
#include "EGamma/DataTree/interface/Types.h"
#include "EGamma/DataTree/interface/TEventInfo.hh"
#include "EGamma/DataTree/interface/TGenParticle.hh"
#include "EGamma/DataTree/interface/TElectron.hh"

#include "AnalysisTools/Common/interface/EfficiencyEvaluator.hh"

#endif

using namespace std;
using namespace vecbos;

void analysis(const string inputfile) {

  //*****************************************************************************************
  // Set up input
  //*****************************************************************************************
  TFile *infile=0;
  TTree *eventTree=0;
  
  // histograms to be filled
  TH1F *GenEta = new TH1F("GenEta", "", 25, -2.5, 2.5);
  TH1F *RecoEta = (TH1F*)GenEta->Clone("RecoEta");

//   Float_t LowerPt[10];
//   int ipt=0;
//   for(float pt=5;pt<=25;pt+=5) {
//     LowerPt[ipt]=pt;
//     ++ipt;
//   }
//   for(float pt=35;pt<=65;pt+=10) {
//     LowerPt[ipt]=pt;
//     ++ipt;
//   }
//   LowerPt[9]=100;

  TH1F *GenPt = new TH1F("GenPt","", 19, 5, 100);
  TH1F *GenPtEB = (TH1F*)GenPt->Clone("GenPtEB");
  TH1F *GenPtEE = (TH1F*)GenPt->Clone("GenPtEE");
  TH1F *RecoPt = (TH1F*)GenPt->Clone("RecoPt");
  TH1F *RecoPtEB = (TH1F*)GenPt->Clone("RecoPtEB");
  TH1F *RecoPtEE = (TH1F*)GenPt->Clone("RecoPtEE");

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

  
  vector<TVector3> geneles;

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

//       cout << p->pdgid << " " << p->status << " " << p->pt << " " << p->eta << " " << p->phi 
//            << " | " << p->motherPdgID << "\n";

      if(fabs(p->pdgid)!=11 || p->pt<5 || fabs(p->eta)>2.5 || p->status!=1) continue;
      TVector3 thegenele;
      thegenele.SetPtEtaPhi(p->pt,p->eta,p->phi);
      geneles.push_back(thegenele);

      GenEta->Fill(p->eta);
      GenPt->Fill(p->pt);
      if(fabs(p->eta)<1.479) GenPtEB->Fill(p->pt);
      else GenPtEE->Fill(p->pt);
    }


    //********************************************************
    //Loop Over Electrons
    //********************************************************
    for(Int_t i=0; i<electronArr->GetEntriesFast(); i++) {
      const egmana::TElectron *ele = (egmana::TElectron*)((*electronArr)[i]);
      

      //if (!PassEleSimpleCutsVeto( ele, pfcandidateArr, rho,
      //                            kDataEra_2012_MC, false)) continue;

      //      cout << "Electrons : " << ele->pt << " " << ele->eta << " " << ele->phi << "\n";

      TVector3 theele;
      theele.SetPtEtaPhi(ele->pt,ele->eta,ele->phi);
      
      bool matches=false;
      float genpt=0;
      float geneta=-10.;
      for(int e=0;e<(int)geneles.size();++e) 
        if(fabs(geneles[e].DeltaR(theele))<0.01) {
          matches=true;
          genpt=geneles[e].Pt();
          geneta=geneles[e].Eta();
        }

      if(matches) {
        RecoEta->Fill(geneta);
        RecoPt->Fill(genpt);
        if(fabs(ele->eta)<1.479) RecoPtEB->Fill(genpt);
        else RecoPtEE->Fill(genpt);
      }

    }

  }
  delete infile;
  infile=0, eventTree=0;      
  delete info;
  delete genparticleArr;
  
  
  //--------------------------------------------------------------------------------------------------------------
  // Output
  //==============================================================================================================

  EfficiencyEvaluator ElectronEffEta("eff_eta.root");
  ElectronEffEta.AddNumerator(GenEta);
  ElectronEffEta.AddNumerator(RecoEta);
  ElectronEffEta.SetDenominator(GenEta);
  ElectronEffEta.ComputeEfficiencies();
  ElectronEffEta.Write();
  
  EfficiencyEvaluator ElectronEffPt("eff_pt.root");
  ElectronEffPt.AddNumerator(GenPt);
  ElectronEffPt.AddNumerator(RecoPt);
  ElectronEffPt.SetDenominator(GenPt);
  ElectronEffPt.ComputeEfficiencies();
  ElectronEffPt.Write();

  EfficiencyEvaluator ElectronEffPtEB("eff_pt_EB.root");
  ElectronEffPtEB.AddNumerator(GenPtEB);
  ElectronEffPtEB.AddNumerator(RecoPtEB);
  ElectronEffPtEB.SetDenominator(GenPtEB);
  ElectronEffPtEB.ComputeEfficiencies();
  ElectronEffPtEB.Write();

  EfficiencyEvaluator ElectronEffPtEE("eff_pt_EE.root");
  ElectronEffPtEE.AddNumerator(GenPtEE);
  ElectronEffPtEE.AddNumerator(RecoPtEE);
  ElectronEffPtEE.SetDenominator(GenPtEE);
  ElectronEffPtEE.ComputeEfficiencies();
  ElectronEffPtEE.Write();

}
