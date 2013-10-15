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
#include <TMath.h>

// define structures to read in ntuple
#include "EGamma/DataTree/interface/Types.h"
#include "EGamma/DataTree/interface/TEventInfo.hh"
#include "EGamma/DataTree/interface/TGenParticle.hh"
#include "EGamma/DataTree/interface/TElectron.hh"
#include "EGamma/DataTree/interface/TTrack.hh"
#include "EGamma/DataTree/interface/TCluster.hh"

#include "AnalysisTools/Common/interface/EfficiencyEvaluator.hh"

#endif

using namespace std;
using namespace vecbos;

void analysis(const string inputfile, const string outputfile) {

  //*****************************************************************************************
  // Set up input
  //*****************************************************************************************
  TFile *infile=0;
  TTree *eventTree=0;

  TFile *histosfile = TFile::Open(outputfile.c_str(),"recreate");

  // histograms to be filled
  TH1F *GenEta = new TH1F("GenEta", "", 25, -2.5, 2.5);
  TH1F *RecoEta = (TH1F*)GenEta->Clone("RecoEta");
  TH1F *RecoSCEta = (TH1F*)GenEta->Clone("RecoSCEta");

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
  TH1F *RecoSCPt = (TH1F*)GenPt->Clone("RecoSCPt");
  TH1F *RecoSCPtEB = (TH1F*)GenPt->Clone("RecoSCPtEB");
  TH1F *RecoSCPtEE = (TH1F*)GenPt->Clone("RecoSCPtEE");


  // ID vars
  vector<TH1F*> ebvars, eevars;
  ebvars.push_back(new TH1F("reso_eb","",50,-0.2,0.2));
  ebvars.push_back(new TH1F("resolo_eb","",50,-0.2,0.2));
  ebvars.push_back(new TH1F("resohi_eb","",50,-0.2,0.2));
  ebvars.push_back(new TH1F("hoe_eb","",50,0,0.15));
  ebvars.push_back(new TH1F("deta_eb","",50,-0.02,0.02));
  ebvars.push_back(new TH1F("dphi_eb","",50,-0.15,0.15));
  ebvars.push_back(new TH1F("etareso_eb","",50,-0.02,0.02));
  ebvars.push_back(new TH1F("phireso_eb","",50,-0.2,0.2));
  ebvars.push_back(new TH1F("drreso_eb","",50,0.,0.1));
  ebvars.push_back(new TH1F("etaresop_eb","",50,-0.02,0.02));
  ebvars.push_back(new TH1F("phiresop_eb","",50,-0.2,0.2));
  ebvars.push_back(new TH1F("drresop_eb","",50,0.,0.1));
  ebvars.push_back(new TH1F("etareson_eb","",50,-0.02,0.02));
  ebvars.push_back(new TH1F("phireson_eb","",50,-0.2,0.2));
  ebvars.push_back(new TH1F("drreson_eb","",50,0.,0.1));

  ebvars.push_back(new TH1F("seedsubdet2_eb","",11,0,11));
  ebvars.push_back(new TH1F("seedhitsmask_eb","",13,0,13));
  ebvars.push_back(new TH1F("seedhitsmaskBPIX2_eb","",13,0,13));
  ebvars.push_back(new TH1F("seedhitsmaskFPIX2_eb","",13,0,13));
  ebvars.push_back(new TH1F("seedhitsmaskTEC2_eb","",13,0,13));
  ebvars.push_back(new TH1F("seeddphi2_eb","",50,-0.01,0.01));
  ebvars.push_back(new TH1F("seedrz2_eb","",50,-0.03,0.03));
  
  for(int i=0;i<(int)ebvars.size();++i) {
    TString nameeb(ebvars[i]->GetName());
    TString nameee = nameeb.ReplaceAll("_eb","_ee");
    eevars.push_back((TH1F*)ebvars[i]->Clone(nameee));
  }

  // Data structures to store info from TTrees
  egmana::TEventInfo *info  = new egmana::TEventInfo();
  TClonesArray *genparticleArr = new TClonesArray("egmana::TGenParticle");
  TClonesArray *electronArr = new TClonesArray("egmana::TElectron");
  TClonesArray *gsftrackArr = new TClonesArray("egmana::TTrack");
  TClonesArray *sclusterArr = new TClonesArray("egmana::TCluster");

  // Read input file and get the TTrees
  cout << "Processing " << inputfile << "..." << endl;
  infile = TFile::Open(inputfile.c_str(),"read");
  assert(infile);
    
  eventTree = (TTree*)infile->Get("Events"); assert(eventTree);  
  eventTree->SetBranchAddress("Info",     &info);        TBranch *infoBr     = eventTree->GetBranch("Info");
  eventTree->SetBranchAddress("GenParticle", &genparticleArr); TBranch *genparticleBr = eventTree->GetBranch("GenParticle");
  eventTree->SetBranchAddress("Electron", &electronArr); TBranch *electronBr = eventTree->GetBranch("Electron");
  eventTree->SetBranchAddress("GsfTrack", &gsftrackArr); TBranch *gsftrackBr = eventTree->GetBranch("GsfTrack");
  eventTree->SetBranchAddress("SuperCluster", &sclusterArr); TBranch *sclusterBr = eventTree->GetBranch("SuperCluster");
  cout << "NEvents = " << eventTree->GetEntries() << endl;

  
  vector<TVector3> geneles;
  vector<int> genpdgid;

  // loop over events
  for(UInt_t ientry=0; ientry<eventTree->GetEntries(); ientry++) {
    if (ientry % 1000 == 0) cout << "Processed Event " << ientry << endl;
    infoBr->GetEntry(ientry);

    genparticleArr->Clear(); 
    electronArr->Clear();

    genparticleBr->GetEntry(ientry);
    electronBr->GetEntry(ientry);
    gsftrackBr->GetEntry(ientry);
    sclusterBr->GetEntry(ientry);

    //***********************************************************
    // Event information
    //***********************************************************
    TVector3 genVtxPos(info->pvx,info->pvy,info->pvz);
    // TVector3 genVtxPos(0,0,info->pvz);  
    //    cout << info->pvz << " cm" << endl;

    //***********************************************************
    // Gen-Level particles
    //***********************************************************
    geneles.clear();
    genpdgid.clear();
    for(Int_t i=0; i<genparticleArr->GetEntriesFast(); i++) {
      const egmana::TGenParticle *p = (egmana::TGenParticle*)((*genparticleArr)[i]);

//       cout << p->pdgid << " " << p->status << " " << p->pt << " " << p->eta << " " << p->phi 
//            << " | " << p->motherPdgID << "\n";

      if(fabs(p->pdgid)!=11 || p->pt<5 || fabs(p->eta)>2.5 || p->status!=1) continue;
      TVector3 thegenele;
      thegenele.SetPtEtaPhi(p->pt,p->eta,p->phi);
      geneles.push_back(thegenele);
      genpdgid.push_back(p->pdgid);

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
      
      int truee=-1;
      for(int e=0;e<(int)geneles.size();++e) 
        if(fabs(geneles[e].DeltaR(theele))<0.01) truee=e;

      if(truee>0) {
        TVector3 scPos(ele->scX,ele->scY,ele->scZ);
        
        TVector3 corrScPos=scPos-genVtxPos;

        // RecoEta->Fill(geneles[truee].Eta());
        // RecoPt->Fill(geneles[truee].Pt());
        if(ele->isEB) {
          // RecoPtEB->Fill(geneles[truee].Pt());
          ebvars[0]->Fill((ele->scEt-geneles[truee].Pt())/geneles[truee].Pt());
          if(geneles[truee].Pt()<20) ebvars[1]->Fill((ele->scEt-geneles[truee].Pt())/geneles[truee].Pt());
          else ebvars[2]->Fill((ele->scEt-geneles[truee].Pt())/geneles[truee].Pt());
          ebvars[3]->Fill(ele->HoverE);
          ebvars[4]->Fill(ele->deltaEtaIn);
          ebvars[5]->Fill(ele->deltaPhiIn);
          ebvars[6]->Fill(corrScPos.Eta()-geneles[truee].Eta());
          ebvars[7]->Fill(corrScPos.Phi()-geneles[truee].Phi());
          ebvars[8]->Fill(corrScPos.DeltaR(geneles[truee]));
          int c = (genpdgid[truee]>0) ? 0 : 3;
          ebvars[9+c]->Fill(corrScPos.Eta()-geneles[truee].Eta());
          ebvars[10+c]->Fill(corrScPos.Phi()-geneles[truee].Phi());
          ebvars[11+c]->Fill(corrScPos.DeltaR(geneles[truee]));            
          ebvars[15]->Fill(ele->TkSeedSubDet2);
          ebvars[16]->Fill(ele->TkHitsMask);
          if(ele->TkSeedSubDet2==1) ebvars[17]->Fill(ele->TkHitsMask);
          if(ele->TkSeedSubDet2==2) ebvars[18]->Fill(ele->TkHitsMask);
          if(ele->TkSeedSubDet2==6) ebvars[19]->Fill(ele->TkHitsMask);
          ebvars[20]->Fill(ele->TkSeedDPhi2);
          ebvars[21]->Fill(ele->TkSeedDRZ2);
        }
        else {
          // RecoPtEE->Fill(geneles[truee].Pt());
          eevars[0]->Fill((ele->scEt-geneles[truee].Pt())/geneles[truee].Pt());
          if(geneles[truee].Pt()<20) eevars[1]->Fill((ele->scEt-geneles[truee].Pt())/geneles[truee].Pt());
          else eevars[2]->Fill((ele->scEt-geneles[truee].Pt())/geneles[truee].Pt());
          eevars[3]->Fill(ele->HoverE);
          eevars[4]->Fill(ele->deltaEtaIn);
          eevars[5]->Fill(ele->deltaPhiIn);
          eevars[6]->Fill(corrScPos.Eta()-geneles[truee].Eta());
          eevars[7]->Fill(corrScPos.Phi()-geneles[truee].Phi());
          eevars[8]->Fill(corrScPos.DeltaR(geneles[truee]));
          int c = (genpdgid[truee]>0) ? 0 : 3;
          eevars[9+c]->Fill(corrScPos.Eta()-geneles[truee].Eta());
          eevars[10+c]->Fill(corrScPos.Phi()-geneles[truee].Phi());
          eevars[11+c]->Fill(corrScPos.DeltaR(geneles[truee]));            
          eevars[15]->Fill(ele->TkSeedSubDet2);
          eevars[16]->Fill(ele->TkHitsMask);
          if(ele->TkSeedSubDet2==1) eevars[17]->Fill(ele->TkHitsMask);
          if(ele->TkSeedSubDet2==2) eevars[18]->Fill(ele->TkHitsMask);
          if(ele->TkSeedSubDet2==6) eevars[19]->Fill(ele->TkHitsMask);
          eevars[20]->Fill(ele->TkSeedDPhi2);
          eevars[21]->Fill(ele->TkSeedDRZ2);
        }          
      }

    }

    for(int e=0;e<(int)geneles.size();++e) {

      //********************************************************
      //Loop Over GSF Tracks
      //********************************************************
      for(Int_t i=0; i<gsftrackArr->GetEntriesFast(); i++) {
        const egmana::TTrack *gsf = (egmana::TTrack*)((*gsftrackArr)[i]);
        TVector3 thegsf;
        thegsf.SetPtEtaPhi(gsf->pt,gsf->eta,gsf->phi);
        
        bool matchtrk=false;
        if(fabs(geneles[e].DeltaR(thegsf))<0.01) matchtrk=true;

        if(matchtrk) {
          RecoEta->Fill(geneles[e].Eta());
          RecoPt->Fill(geneles[e].Pt());
          if(fabs(geneles[e].Eta())<1.479) {
            RecoPtEB->Fill(geneles[e].Pt());
          } else {
            RecoPtEE->Fill(geneles[e].Pt());
          }
          break;
        }
      } // end loop over tracks

      //********************************************************
      //Loop Over Super Clusters
      //********************************************************
      for(Int_t i=0; i<sclusterArr->GetEntriesFast(); i++) {
        const egmana::TCluster *clu = (egmana::TCluster*)((*sclusterArr)[i]);

        float theta=2*atan(TMath::Exp(-clu->eta));
        float pt=clu->energy * fabs(sin(theta));

        TVector3 theclu;
        theclu.SetPtEtaPhi(pt,clu->eta,clu->phi);
        TVector3 corrScPos=theclu-genVtxPos;

        bool matchclu=false;
        if(fabs(geneles[e].DeltaR(corrScPos))<0.4) matchclu=true;

        if(matchclu) {
          RecoSCEta->Fill(geneles[e].Eta());
          RecoSCPt->Fill(geneles[e].Pt());
          if(fabs(geneles[e].Eta())<1.479) {
            RecoSCPtEB->Fill(geneles[e].Pt());
          } else {
            RecoSCPtEE->Fill(geneles[e].Pt());
          }
          break;
        }
      } // end loop over tracks

    } // loop over true ele
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
  ElectronEffEta.AddNumerator(RecoSCEta);
  ElectronEffEta.SetDenominator(GenEta);
  ElectronEffEta.ComputeEfficiencies();
  ElectronEffEta.Write();
  
  EfficiencyEvaluator ElectronEffPt("eff_pt.root");
  ElectronEffPt.AddNumerator(GenPt);
  ElectronEffPt.AddNumerator(RecoPt);
  ElectronEffPt.AddNumerator(RecoSCPt);
  ElectronEffPt.SetDenominator(GenPt);
  ElectronEffPt.ComputeEfficiencies();
  ElectronEffPt.Write();

  EfficiencyEvaluator ElectronEffPtEB("eff_pt_EB.root");
  ElectronEffPtEB.AddNumerator(GenPtEB);
  ElectronEffPtEB.AddNumerator(RecoPtEB);
  ElectronEffPtEB.AddNumerator(RecoSCPtEB);
  ElectronEffPtEB.SetDenominator(GenPtEB);
  ElectronEffPtEB.ComputeEfficiencies();
  ElectronEffPtEB.Write();

  EfficiencyEvaluator ElectronEffPtEE("eff_pt_EE.root");
  ElectronEffPtEE.AddNumerator(GenPtEE);
  ElectronEffPtEE.AddNumerator(RecoPtEE);
  ElectronEffPtEE.AddNumerator(RecoSCPtEE);
  ElectronEffPtEE.SetDenominator(GenPtEE);
  ElectronEffPtEE.ComputeEfficiencies();
  ElectronEffPtEE.Write();

  histosfile->cd();
  for(int i=0;i<(int)eevars.size();++i) {
    ebvars[i]->Write();
    eevars[i]->Write();
  }
  histosfile->Close();
  
}
