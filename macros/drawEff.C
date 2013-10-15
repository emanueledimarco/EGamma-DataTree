#include <iostream>
#include <vector>

#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TString.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TROOT.h>

#include <mystyle.C>

using namespace std;

void drawOneComparison(vector<TH1F*> histos, vector<TString> descr, TString xaxislabel, const char *filename, TPaveText *t=0) {
  
  if(histos.size()!=descr.size()) {
    cout << "description not complete!" << endl;
    return;
  }

  vector<int> colors;
  colors.push_back(kRed+1);
  colors.push_back(kAzure-6);
  colors.push_back(kTeal+3);
  colors.push_back(kBlue+3);

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  c1->SetGridx();
  c1->SetGridy();  
  TLegend* legend = new TLegend(0.20, 0.20, 0.43, 0.36);
  legend->SetBorderSize(   0);
  legend->SetFillColor (   0);
  legend->SetTextAlign (  12);
  legend->SetTextFont  (  42);
  legend->SetTextSize  (0.05);

  for(int i=0;i<(int)histos.size();++i) {
    
    histos[i]->SetMinimum(0.5);
    histos[i]->SetMaximum(1.1);
    histos[i]->SetMarkerSize(2);
    histos[i]->SetMarkerStyle(20);
    histos[i]->SetMarkerColor(colors[i]);
    histos[i]->SetLineColor(colors[i]);
    histos[i]->SetTitle("");
    if(TString(histos[i]->GetName()).Contains("PU")) {
      histos[i]->SetMaximum(1.1);
      histos[i]->Fit("pol1","","same",1,35);
      histos[i]->GetFunction("pol1")->SetLineColor(colors[i]);
    }
    histos[i]->GetXaxis()->SetTitle(xaxislabel);
    histos[i]->GetYaxis()->SetTitle("efficiency");
    histos[i]->GetYaxis()->SetTitleOffset(1.5);

    legend->AddEntry(histos[i],descr[i]);

    if(i==0) { 
      histos[i]->Draw("pe1");
      for(int bin=1;bin<=histos[i]->GetNbinsX();bin++) {
	//cout << "bin i = " << bin << ": " <<  histos[i]->GetBinContent(bin) << " +/- " << histos[i]->GetBinError(bin) << endl;
	//	cout << "m35_fakeRate[" << bin << "] = " << histos[i]->GetBinContent(bin) << ";" << endl;
      }
      for(int bin=1;bin<=histos[i]->GetNbinsX();bin++) {
	//	cout << "m35_fakeRate_err[" << bin << "] = " << histos[i]->GetBinError(bin) << ";" << endl;
      }
    }
    else histos[i]->Draw("same pe1");
  }
  legend->Draw();
  if(t) t->Draw();

  TString basename(filename);
  c1->SaveAs(basename+TString(".png"));
  c1->SaveAs(basename+TString(".pdf"));

}

void drawOneToOne(vector<TH1F*> set1, vector<TH1F*> set2, const char* desc1, const char* desc2, const char* xaxislabel) {
  if(set1.size()!=set2.size()) {
    cout << "first set and second set of histos have different sizes! ERROR! " << endl;
    return;
  }
  for(int i=0;i<(int)set1.size();++i) {
    vector<TH1F*> histos;
    vector<TString> desc;
    histos.push_back(set1[i]);
    histos.push_back(set2[i]);
    desc.push_back(TString(desc1));
    desc.push_back(TString(desc2));
    if(set1[i]==0 || set2[i]==0) {
      cout << "histogram not found!" << endl;
      continue;
    }
    drawOneComparison(histos,desc,TString(xaxislabel),set1[i]->GetName());
  }

}


void drawSeedingEff() {
  
  gROOT->ProcessLine(".L mystyle.C+");
  setstyle();

  TFile *file1 = TFile::Open("efficiency_std.root");
  TFile *file2 = TFile::Open("efficiency_pfV5.root");
  TFile *file3 = TFile::Open("efficiency_pfV6.root");
  TFile *file4 = TFile::Open("efficiency_pfV7.root");

  // eta
  TH1F *RecoEta1 = (TH1F*)file1->Get("RecoEta_Eff");
  TH1F *RecoEta2 = (TH1F*)file2->Get("RecoEta_Eff");
  TH1F *RecoEta3 = (TH1F*)file3->Get("RecoEta_Eff");
  TH1F *RecoEta4 = (TH1F*)file4->Get("RecoEta_Eff");

  vector<TH1F*> etaSet;
  etaSet.push_back(RecoEta1);
  etaSet.push_back(RecoEta2);
  etaSet.push_back(RecoEta3);
  etaSet.push_back(RecoEta4);
  
  vector<TString> descriptions;
  descriptions.push_back("ECAL Seeding");
  descriptions.push_back("PF Seeding, std., regr.");
  descriptions.push_back("PF Seeding, 1 GeV, regr.");
  descriptions.push_back("PF Seeding, 1 GeV, no regr.");

  drawOneComparison(etaSet,descriptions,"#eta",etaSet[0]->GetName());

  // pt
  TH1F *RecoPt1 = (TH1F*)file1->Get("RecoPt_Eff");
  TH1F *RecoPtEB1 = (TH1F*)file1->Get("RecoPtEB_Eff");
  TH1F *RecoPtEE1 = (TH1F*)file1->Get("RecoPtEE_Eff");

  TH1F *RecoPt2 = (TH1F*)file2->Get("RecoPt_Eff");
  TH1F *RecoPtEB2 = (TH1F*)file2->Get("RecoPtEB_Eff");
  TH1F *RecoPtEE2 = (TH1F*)file2->Get("RecoPtEE_Eff");

  TH1F *RecoPt3 = (TH1F*)file3->Get("RecoPt_Eff");
  TH1F *RecoPtEB3 = (TH1F*)file3->Get("RecoPtEB_Eff");
  TH1F *RecoPtEE3 = (TH1F*)file3->Get("RecoPtEE_Eff");

  TH1F *RecoPt4 = (TH1F*)file4->Get("RecoPt_Eff");
  TH1F *RecoPtEB4 = (TH1F*)file4->Get("RecoPtEB_Eff");
  TH1F *RecoPtEE4 = (TH1F*)file4->Get("RecoPtEE_Eff");

  vector<TH1F*> ptSet, ptSetEB, ptSetEE;
  ptSet.push_back(RecoPt1);
  ptSet.push_back(RecoPt2);
  ptSet.push_back(RecoPt3);
  ptSet.push_back(RecoPt4);

  ptSetEB.push_back(RecoPtEB1);
  ptSetEB.push_back(RecoPtEB2);
  ptSetEB.push_back(RecoPtEB3);
  ptSetEB.push_back(RecoPtEB4);

  ptSetEE.push_back(RecoPtEE1);
  ptSetEE.push_back(RecoPtEE2);
  ptSetEE.push_back(RecoPtEE3);
  ptSetEE.push_back(RecoPtEE4);

  drawOneComparison(ptSet,descriptions,"p_{T} (GeV)",ptSet[0]->GetName());
  drawOneComparison(ptSetEB,descriptions,"p_{T} (GeV)",ptSetEB[0]->GetName());
  drawOneComparison(ptSetEE,descriptions,"p_{T} (GeV)",ptSetEE[0]->GetName());

}


void drawSuperClusterEff() {
  
  gROOT->ProcessLine(".L mystyle.C+");
  setstyle();

  TFile *file1 = TFile::Open("efficiency_std.root");
  TFile *file2 = TFile::Open("efficiency_pfV5.root");
  TFile *file3 = TFile::Open("efficiency_pfV6.root");
  TFile *file4 = TFile::Open("efficiency_pfV7.root");

  // eta
  TH1F *RecoEta1 = (TH1F*)file1->Get("RecoSCEta_Eff");
  TH1F *RecoEta2 = (TH1F*)file2->Get("RecoSCEta_Eff");
  TH1F *RecoEta3 = (TH1F*)file3->Get("RecoSCEta_Eff");
  TH1F *RecoEta4 = (TH1F*)file4->Get("RecoSCEta_Eff");

  vector<TH1F*> etaSet;
  etaSet.push_back(RecoEta1);
  etaSet.push_back(RecoEta2);
  etaSet.push_back(RecoEta3);
  etaSet.push_back(RecoEta4);

  vector<TString> descriptions;
  descriptions.push_back("ECAL Seeding");
  descriptions.push_back("PF Seeding, std., regr.");
  descriptions.push_back("PF Seeding, 1 GeV, regr.");
  descriptions.push_back("PF Seeding, 1 GeV, no regr.");

  drawOneComparison(etaSet,descriptions,"#eta",etaSet[0]->GetName());

  // pt
  TH1F *RecoPt1 = (TH1F*)file1->Get("RecoSCPt_Eff");
  TH1F *RecoPtEB1 = (TH1F*)file1->Get("RecoSCPtEB_Eff");
  TH1F *RecoPtEE1 = (TH1F*)file1->Get("RecoSCPtEE_Eff");

  TH1F *RecoPt2 = (TH1F*)file2->Get("RecoSCPt_Eff");
  TH1F *RecoPtEB2 = (TH1F*)file2->Get("RecoSCPtEB_Eff");
  TH1F *RecoPtEE2 = (TH1F*)file2->Get("RecoSCPtEE_Eff");

  TH1F *RecoPt3 = (TH1F*)file3->Get("RecoSCPt_Eff");
  TH1F *RecoPtEB3 = (TH1F*)file3->Get("RecoSCPtEB_Eff");
  TH1F *RecoPtEE3 = (TH1F*)file3->Get("RecoSCPtEE_Eff");

  TH1F *RecoPt4 = (TH1F*)file4->Get("RecoSCPt_Eff");
  TH1F *RecoPtEB4 = (TH1F*)file4->Get("RecoSCPtEB_Eff");
  TH1F *RecoPtEE4 = (TH1F*)file4->Get("RecoSCPtEE_Eff");

  vector<TH1F*> ptSet, ptSetEB, ptSetEE;
  ptSet.push_back(RecoPt1);
  ptSet.push_back(RecoPt2);
  ptSet.push_back(RecoPt3);
  ptSet.push_back(RecoPt4);

  ptSetEB.push_back(RecoPtEB1);
  ptSetEB.push_back(RecoPtEB2);
  ptSetEB.push_back(RecoPtEB3);
  ptSetEB.push_back(RecoPtEB4);

  ptSetEE.push_back(RecoPtEE1);
  ptSetEE.push_back(RecoPtEE2);
  ptSetEE.push_back(RecoPtEE3);
  ptSetEE.push_back(RecoPtEE4);

  drawOneComparison(ptSet,descriptions,"p_{T} (GeV)",ptSet[0]->GetName());
  drawOneComparison(ptSetEB,descriptions,"p_{T} (GeV)",ptSetEB[0]->GetName());
  drawOneComparison(ptSetEE,descriptions,"p_{T} (GeV)",ptSetEE[0]->GetName());

}

