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

void drawOne(TString var, TString title, const char* file1, const char* file2) {
  
  cout << "plotting " << var.Data() << endl;
  
  TFile *f1 = TFile::Open(file1);
  TH1F *h1 = (TH1F*)f1->Get(var.Data());

  TFile *f2 = TFile::Open(file2);
  TH1F *h2 = (TH1F*)f2->Get(var.Data());

  h1->Sumw2();
  h2->Sumw2();

  h1->Rebin(2);
  h2->Rebin(2);
  
  float norm1 = h1->Integral();
  float norm2 = h2->Integral();
  h1->Scale(1./norm1);
  h2->Scale(1./norm2);

  float max = TMath::Max(h1->GetMaximum(),h2->GetMaximum());
  max = max + 0.2 * max;
  h2->SetMaximum(max);
  h1->SetMaximum(max);

  h1->SetLineColor(kAzure+3);
  h1->SetLineWidth(2);
  h1->SetFillColor(kAzure+5);
  h1->SetFillStyle(1001);

  h2->SetLineColor(kOrange+2);
  h2->SetLineWidth(2);
  h2->SetFillColor(kOrange+1);
  h2->SetFillStyle(3002);

  // draw the legend
  TLegend* legend = new TLegend(0.65, 0.75, 0.85, 0.85);
  legend->SetBorderSize(     0);
  legend->SetFillColor (     0);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->SetTextSize  (0.031);
  legend->AddEntry(h1, "ECAL clu.", "f");
  legend->AddEntry(h2, "PF clu.", "f");

  // cosmetics

  TCanvas c1("c1","",600,600);
  c1.cd();

  if(var.Contains("HoE")) {
    h1->SetMinimum(1);
    c1.SetLogy();
  }

  h1->GetXaxis()->SetTitle(title);
  h1->GetYaxis()->SetTitle("a.u.");
  h1->GetYaxis()->SetTitleOffset(1.8);
  h1->GetXaxis()->SetTitleOffset(1.0);
  h1->GetXaxis()->SetTitleSize(0.04);
  h1->GetYaxis()->SetTitleSize(0.04);
  h1->Draw("hist");
  h2->Draw("same hist");

  legend->Draw();
  
  TString pdf = TString(var)+TString(".pdf");
  TString png = TString(var)+TString(".png");
  TString macro = TString(var)+TString(".C");

  c1.SaveAs(pdf);
  c1.SaveAs(png);
  //  c1.SaveAs(macro);

}

void drawOneReso(TString var, TString title, const char* file1, const char* file2) {
  
  cout << "plotting " << var.Data() << endl;
  
  TFile *f1 = TFile::Open(file1);
  TH1F *h1 = (TH1F*)f1->Get(var.Data());
  TString outvar=var;
  TString varp = var.ReplaceAll("reso","resop");
  TString varn = var.ReplaceAll("resop","reson");  
  TH1F *h1p = (TH1F*)f1->Get(varp.Data());
  TH1F *h1n = (TH1F*)f1->Get(varn.Data());

  TFile *f2 = TFile::Open(file2);
  TH1F *h2 = (TH1F*)f2->Get(var.Data());
  TH1F *h2p = (TH1F*)f2->Get(varp.Data());
  TH1F *h2n = (TH1F*)f2->Get(varn.Data());

  cout << h1p->GetEntries() << "  " << h1n->GetEntries()  << endl;
  cout << h2p->GetEntries() << "  " << h2n->GetEntries()  << endl;

  h1->Sumw2();
  h1p->Sumw2();
  h1n->Sumw2();
  h2->Sumw2();
  h2p->Sumw2();
  h2n->Sumw2();
  
  float norm1 = h1->Integral();
  float norm2 = h2->Integral();
  h1->Scale(1./norm1);
  h1p->Scale(1./h1p->Integral());
  h1n->Scale(1./h1n->Integral());
//   h1p->Scale(0.5/norm1);
//   h1p->Scale(0.5/norm1);
  h2->Scale(1./norm2);
//   h2p->Scale(0.5/norm2);
//   h2p->Scale(0.5/norm2);
  h2p->Scale(1./h2p->Integral());
  h2n->Scale(1./h2n->Integral());

  float max = TMath::Max(h1->GetMaximum(),h2->GetMaximum());
  max = max + 0.2 * max;
  h2->SetMaximum(max);

  h1->SetLineColor(kAzure+3);
  h1->SetLineWidth(2);
  h1->SetFillColor(kAzure+5);
  h1->SetFillStyle(1001);
  h1p->SetMarkerColor(kBlue+1);
  h1n->SetMarkerColor(kBlue+2);
  //  h1p->SetLineStyle(kDashed);
  //  h1n->SetLineStyle(kDashed);

  h2->SetLineColor(kOrange+2);
  h2->SetLineWidth(2);
  h2->SetFillColor(kOrange+1);
  h2->SetFillStyle(3002);
  h2p->SetMarkerColor(kRed+1);
  h2n->SetMarkerColor(kRed+2);
//   h2p->SetLineStyle(kDashed);
//   h2n->SetLineStyle(kDashed);

  // draw the legend
  TLegend* legend = new TLegend(0.65, 0.75, 0.85, 0.85);
  legend->SetBorderSize(     0);
  legend->SetFillColor (     0);
  legend->SetTextAlign (    12);
  legend->SetTextFont  (    42);
  legend->SetTextSize  (0.031);
  legend->AddEntry(h1, "ECAL clu.", "f");
  legend->AddEntry(h1p, "ECAL clu. (+)", "l");
  legend->AddEntry(h1n, "ECAL clu. (-)", "l");
  legend->AddEntry(h2, "PF clu.", "f");
  legend->AddEntry(h2p, "PF clu. (+)", "l");
  legend->AddEntry(h2n, "PF clu. (-)", "l");

  // cosmetics

  TCanvas c1("c1","",600,600);
  c1.cd();

  if(var.Contains("HoE")) {
    h1->SetMinimum(1);
    c1.SetLogy();
  }

  h1->GetXaxis()->SetTitle(title);
  h1->GetYaxis()->SetTitle("a.u.");
  h1->GetYaxis()->SetTitleOffset(1.8);
  h1->GetXaxis()->SetTitleOffset(1.0);
  h1->GetXaxis()->SetTitleSize(0.04);
  h1->GetYaxis()->SetTitleSize(0.04);
  h1->Draw("hist");
  h1p->Draw("same pe");
  h1n->Draw("same pe");
  h2->Draw("same hist");
  h2p->Draw("same pe");
  h2n->Draw("same pe");

  legend->Draw();
  
  TString pdf = TString(outvar)+TString(".pdf");
  TString png = TString(outvar)+TString(".png");
  TString macro = TString(outvar)+TString(".C");

  c1.SaveAs(pdf);
  c1.SaveAs(png);
  //  c1.SaveAs(macro);

}


void drawVars(const char *file1, const char* file2) {

  gROOT->ProcessLine(".L mystyle.C+");
  setstyle();

  vector<TString> vars;
  vars.push_back("etareso_eb"); // 0
  vars.push_back("phireso_eb");
  vars.push_back("drreso_eb");
  vars.push_back("reso_eb");
  vars.push_back("hoe_eb"); // 4
  vars.push_back("deta_eb");
  vars.push_back("dphi_eb");
  vars.push_back("seedsubdet2_eb");
  vars.push_back("seedhitsmask_eb");
  vars.push_back("seedhitsmaskBPIX2_eb");
  vars.push_back("seedhitsmaskFPIX2_eb");
  vars.push_back("seedhitsmaskTEC2_eb");
  vars.push_back("seeddphi2_eb");
  vars.push_back("seedrz2_eb");

  vector<TString> titles;
  titles.push_back("#Delta#eta (SC - true)");
  titles.push_back("#Delta#phi (SC - true)");
  titles.push_back("#DeltaR (SC - true)");
  titles.push_back("#DeltaE/E");
  titles.push_back("H/E");
  titles.push_back("#Delta#eta_{in}");
  titles.push_back("#Delta#phi_{in}");
  titles.push_back("seed subdet. 2^{nd} layer");
  titles.push_back("seed hits mask");
  titles.push_back("seed hits mask, 2^{nd} is BPIX");
  titles.push_back("seed hits mask, 2^{nd} is FPIX");
  titles.push_back("seed hits mask, 2^{nd} is TEC");
  titles.push_back("seed #phi_{hit}-#phi_{pred} (rad)");
  titles.push_back("seed r(z)_{hit}-r(z)_{pred} (cm)");

  for(int i=0;i<3;++i) {
    drawOneReso(vars[i],titles[i],file1,file2);
    TString nameee = vars[i].ReplaceAll("_eb","_ee");
    drawOneReso(nameee,titles[i],file1,file2);
  }

  for(int i=3;i<(int)vars.size();++i) {
    drawOne(vars[i],titles[i],file1,file2);
    TString nameee = vars[i].ReplaceAll("_eb","_ee");
    drawOne(nameee,titles[i],file1,file2);
  }

}
