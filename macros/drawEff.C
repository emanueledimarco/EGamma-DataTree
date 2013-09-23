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

using namespace std;

void drawOneComparison(vector<TH1F*> histos, vector<TString> descr, TString xaxislabel, const char *filename, TPaveText *t=0) {

  if(histos.size()>3) {
    cout << "more than 3 histos not implemented." << endl;
    return;
  }

  if(histos.size()!=descr.size()) {
    cout << "description not complete!" << endl;
    return;
  }

  vector<int> colors;
  colors.push_back(kRed+1);
  colors.push_back(kAzure-6);
  colors.push_back(kTeal+3);

  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);

  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  c1->SetGridx();
  c1->SetGridy();  
  TLegend* legend = new TLegend(0.20, 0.50, 0.43, 0.66);
  legend->SetBorderSize(   0);
  legend->SetFillColor (   0);
  legend->SetTextAlign (  12);
  legend->SetTextFont  (  42);
  legend->SetTextSize  (0.05);

  for(int i=0;i<(int)histos.size();++i) {
    
    histos[i]->SetMinimum(0);
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
    histos[i]->GetYaxis()->SetTitle("Loose #rightarrow Tight efficiency");
    histos[i]->GetYaxis()->SetTitleOffset(1.5);

    legend->AddEntry(histos[i],descr[i]);

    if(i==0) { 
      histos[i]->Draw("pe1");
      for(int bin=1;bin<=histos[i]->GetNbinsX();bin++) {
	//cout << "bin i = " << bin << ": " <<  histos[i]->GetBinContent(bin) << " +/- " << histos[i]->GetBinError(bin) << endl;
	cout << "m35_fakeRate[" << bin << "] = " << histos[i]->GetBinContent(bin) << ";" << endl;
      }
      for(int bin=1;bin<=histos[i]->GetNbinsX();bin++) {
	cout << "m35_fakeRate_err[" << bin << "] = " << histos[i]->GetBinError(bin) << ";" << endl;
      }
    }
    else histos[i]->Draw("same pe1");
  }
  legend->Draw();
  if(t) t->Draw();

  TString basename(filename);
  basename.ReplaceAll("Eff","FR");
  c1->SaveAs(basename+TString(".png"));
  c1->SaveAs(basename+TString(".pdf"));

}

void drawOne(vector<TH1F*> set1, vector<TString> bin, const char* desc1, const char* xaxislabel) {

  for(int i=0;i<(int)set1.size();++i) {

    TPaveText* text  = new TPaveText(0.15, 0.9, 0.8, 0.7, "ndc");
    text->AddText("#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");
    text->AddText(bin[i]);
    text->SetBorderSize(0);
    text->SetFillStyle(0);
    text->SetTextAlign(12);
    text->SetTextFont(32);
    text->SetTextSize(0.05);
    text->Draw();

    vector<TH1F*> histos;
    vector<TString> desc;
    histos.push_back(set1[i]);
    desc.push_back(TString(desc1));
    if(set1[i]==0) {
      cout << "histogram not found!" << endl;
      continue;
    }
    drawOneComparison(histos,desc,TString(xaxislabel),set1[i]->GetName(),text);
  }

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

void drawOneToTwo(vector<TH1F*> set1, vector<TH1F*> set2, vector<TH1F*> set3, const char* desc1, const char* desc2, const char* desc3, const char* xaxislabel) {
  if(set1.size()!=set2.size() || set1.size()!=set3.size()) {
    cout << "first set and second/third set of histos have different sizes! ERROR! " << endl;
    return;
  }
  for(int i=0;i<(int)set1.size();++i) {
    vector<TH1F*> histos;
    vector<TString> desc;
    histos.push_back(set1[i]);
    histos.push_back(set2[i]);
    histos.push_back(set3[i]);
    desc.push_back(TString(desc1));
    desc.push_back(TString(desc2));
    desc.push_back(TString(desc3));
    if(set1[i]==0 || set2[i]==0 || set3[i]==0) {
      cout << "histogram not found!" << endl;
      continue;
    }
    drawOneComparison(histos,desc,TString(xaxislabel),set1[i]->GetName());
  }

}

void drawEff() {
  
  TFile *file1 = TFile::Open("results_std.root");
  TFile *file2 = TFile::Open("results_pfseedingV2.root");

  // eta
  TH1F *RecoEta1 = (TH1F*)file1->Get("RecoEta_Eff");
  TH1F *RecoEta2 = (TH1F*)file2->Get("RecoEta_Eff");

  vector<TH1F*> etaSet1, etaSet2;
  etaSet1.push_back(RecoEta1);
  etaSet2.push_back(RecoEta2);
  
  drawOneToOne(etaSet1,etaSet2,"ECAL Seeding","PFClu. Seeding","#eta");

  // pt
  TH1F *RecoPt1 = (TH1F*)file1->Get("RecoPt_Eff");
  TH1F *RecoPtEB1 = (TH1F*)file1->Get("RecoPtEB_Eff");
  TH1F *RecoPtEE1 = (TH1F*)file1->Get("RecoPtEE_Eff");

  TH1F *RecoPt2 = (TH1F*)file2->Get("RecoPt_Eff");
  TH1F *RecoPtEB2 = (TH1F*)file2->Get("RecoPtEB_Eff");
  TH1F *RecoPtEE2 = (TH1F*)file2->Get("RecoPtEE_Eff");

  vector<TH1F*> ptSet1, ptSet2;
  ptSet1.push_back(RecoPt1);
  ptSet1.push_back(RecoPtEB1);
  ptSet1.push_back(RecoPtEE1);

  ptSet2.push_back(RecoPt2);
  ptSet2.push_back(RecoPtEB2);
  ptSet2.push_back(RecoPtEE2);

  drawOneToOne(ptSet1,ptSet2,"ECAL Seeding","PFClu. Seeding","p_{T} [GeV]");

}

