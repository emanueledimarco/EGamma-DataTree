// $Id: CMSNtupler.cc,v 1.14 2013/06/30 13:27:51 sixie Exp $
//
 
#include "EGamma/DataTree/plugins/ElectronNtupler.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/plugins/TransientTrackBuilderESProducer.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "RecoEgamma/EgammaTools/interface/ConversionTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "RecoEgamma/EgammaTools/interface/EcalClusterLocal.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h" 
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"


using namespace std;
using namespace edm;
using namespace reco;


//
// constructors and destructor
//
ElectronNtupler::ElectronNtupler(const edm::ParameterSet& iConfig)

{
  
  cout << "ElectronNtupler initialized... " << endl;
  
  fGenParticlesSrcName = iConfig.getParameter<string> ("GenParticlesSrcName");
  fPrimaryVerticesSrcName = iConfig.getParameter<string> ("PrimaryVerticesSrcName");
  fPrimaryVerticesBSSrcName = iConfig.getParameter<string> ("PrimaryVerticesBSSrcName");
  fElectronsSrcName = iConfig.getParameter<string> ("ElectronsSrcName");

  //   debug_ = iConfig.getParameter<int> ("debugLevel");
  fUseGen = iConfig.getParameter<bool> ("UseGen");
  fPrintDebug = iConfig.getParameter<bool> ("PrintDebug");
  fFillEGRegressionVars = iConfig.getParameter<bool> ("FillEGRegressionVars");

  fElePtMin = iConfig.getParameter<double> ("ElePtMin");
  fElePtMax = iConfig.getParameter<double> ("ElePtMax");
  fEleEtaMin = iConfig.getParameter<double> ("EleEtaMin");
  fEleEtaMax = iConfig.getParameter<double> ("EleEtaMax");

  //output filename
  fOutputName = iConfig.getParameter<std::string>("outputFile");

  // Don't write TObject part of the objects
  egmana::TEventInfo::Class()->IgnoreTObjectStreamer();
  egmana::TGenParticle::Class()->IgnoreTObjectStreamer();
  egmana::TElectron::Class()->IgnoreTObjectStreamer();
  egmana::TVertex::Class()->IgnoreTObjectStreamer();
  
  nEventsProcessed  =0;

}


ElectronNtupler::~ElectronNtupler()
{
 
  // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ElectronNtupler::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
    
  
  //*****************************
  //Clear Arrays
  //*****************************
  nEventsProcessed++; 
  fGenParticleArr->Clear();
  fElectronArr->Clear();
  fVertexArr->Clear();

  //***************************************************************
  //Fill Gen Particles
  //***************************************************************
  double genVertexX = -999;
  double genVertexY = -999;
  double genVertexZ = -999;
  
  if (fUseGen) {

    Handle<reco::GenParticleCollection> hGenPProduct;
    GetProduct(fGenParticlesSrcName, hGenPProduct, event);  
    const reco::GenParticleCollection genParticles = *(hGenPProduct.product());  
  
    bool foundGenVertex = false;
    // loop over all genparticles
    for (reco::GenParticleCollection::const_iterator pgen = genParticles.begin();
         pgen != genParticles.end(); ++pgen) {

      //find genVertex
      if (!foundGenVertex) {
        for (unsigned int i=0; i<pgen->numberOfDaughters(); ++i) {
          const reco::Candidate *dau = pgen->daughter(i);
          if (dau) {
            genVertexX = dau->vx();
            genVertexY = dau->vy();
            genVertexZ = dau->vz();
            foundGenVertex = true;
            break;
          }
        }
      }

      //only fill leptons, neutrinos, photons, bosons
      bool fillThisParticle = false;
      if (abs(pgen->pdgId()) >= 11 && abs(pgen->pdgId()) <= 16) 
        fillThisParticle = true;
      if (abs(pgen->pdgId()) == 23 || abs(pgen->pdgId()) == 24 || abs(pgen->pdgId()) == 25
          || abs(pgen->pdgId()) == 32 || abs(pgen->pdgId()) == 33 
          || abs(pgen->pdgId()) == 35 || abs(pgen->pdgId()) == 36 
          || abs(pgen->pdgId()) == 37 || abs(pgen->pdgId()) == 443
          || abs(pgen->pdgId()) == 553) 
        fillThisParticle = true;
      if (abs(pgen->pdgId()) == 22 && pgen->pt() > 0.0 && fabs(pgen->eta()) < 5.0)
        fillThisParticle = true;

      if (abs(pgen->pdgId()) == 1 || abs(pgen->pdgId()) == 2 || abs(pgen->pdgId()) == 3
          || abs(pgen->pdgId()) == 4 || abs(pgen->pdgId()) == 5 
          || abs(pgen->pdgId()) == 6)
        fillThisParticle = true;


      //debug Gen-Level information
      //cout << pgen->pdgId() << " " << pgen->status() << " " << pgen->mass() << " "
      //     << pgen->pt() << " " << pgen->eta() << " " << pgen->phi() << " ";
      //if (pgen->mother()) cout << pgen->mother()->pdgId() << " ";
      //cout << " | " << fillThisParticle << " \n";

      if (fillThisParticle) {
        TClonesArray &rGenParticleArr = *fGenParticleArr;
        assert(rGenParticleArr.GetEntries() < rGenParticleArr.GetSize());
        const Int_t index = rGenParticleArr.GetEntries();  
        new(rGenParticleArr[index]) egmana::TGenParticle();
        egmana::TGenParticle *pGenParticle = (egmana::TGenParticle*)rGenParticleArr[index];
      
        pGenParticle->pt	          = pgen->pt();
        pGenParticle->eta  	  = pgen->eta();
        pGenParticle->phi  	  = pgen->phi();
        pGenParticle->mass 	  = pgen->mass();
        pGenParticle->status        = pgen->status();
        pGenParticle->pdgid	  = pgen->pdgId();
        pGenParticle->motherPdgID   = 0;    
        if (pgen->mother()) {
          pGenParticle->motherPdgID = pgen->mother()->pdgId();
          if (pgen->mother()->pdgId() == pgen->pdgId()) {
            if (pgen->mother()->mother()) {
              pGenParticle->motherPdgID = pgen->mother()->mother()->pdgId();
            }
          }
        }
      }
    } //loop over gen particles
  } //if useGen == true


  //****************************************************************************************************
  //Pileup information
  //****************************************************************************************************
  
  std::vector<PileupSummaryInfo> inInfos;
  Handle<std::vector< PileupSummaryInfo > >  hPileupInfoProduct;
  event.getByLabel("addPileupInfo", hPileupInfoProduct);
  inInfos = *hPileupInfoProduct.product();

  for (std::vector<PileupSummaryInfo>::const_iterator edmPUInfo = inInfos.begin(); edmPUInfo != inInfos.end(); ++edmPUInfo) {    
//     printf("filling puinfo for bx %i with %i interactions with mean %.2f\n",edmPUInfo->getBunchCrossing(), 
//            edmPUInfo->getPU_NumInteractions(), edmPUInfo->getTrueNumInteractions());
    if (edmPUInfo->getBunchCrossing() == 0) {
      fEventInfo.nPU     = edmPUInfo->getPU_NumInteractions();
      fEventInfo.nPUMean = edmPUInfo->getTrueNumInteractions();
    }
    else if (edmPUInfo->getBunchCrossing() == -1) {
      fEventInfo.nPUMinusOne = edmPUInfo->getPU_NumInteractions();
      fEventInfo.nPUMeanMinusOne = edmPUInfo->getTrueNumInteractions();
    }
    else if (edmPUInfo->getBunchCrossing() == 1) {
      fEventInfo.nPUPlusOne  = edmPUInfo->getPU_NumInteractions();
      fEventInfo.nPUMeanPlusOne = edmPUInfo->getTrueNumInteractions();
    }
  }




  //****************************************************************************************************
  //Transient Track Builder Service
  //****************************************************************************************************
  edm::ESHandle<TransientTrackBuilder> hTransientTrackBuilder;
  setup.get<TransientTrackRecord>().get("TransientTrackBuilder",hTransientTrackBuilder);
  const TransientTrackBuilder *transientTrackBuilder = hTransientTrackBuilder.product();

  //****************************************************************************************************
  //Vertices
  //****************************************************************************************************
  const reco::Vertex *thevtx     = 0;
  const reco::Vertex *thevtxbs   = 0;

  edm::Handle<reco::VertexCollection> hVertex;
  event.getByLabel(fPrimaryVerticesSrcName, hVertex);
  const reco::VertexCollection *pvCol = hVertex.product();
  
  edm::Handle<reco::VertexCollection> hVertexBS;
  event.getByLabel(fPrimaryVerticesBSSrcName, hVertexBS);
  const reco::VertexCollection *pvBSCol = hVertexBS.product();

  UInt_t NGoodPV = 0;
  for (reco::VertexCollection::const_iterator inV = pvCol->begin(); 
       inV != pvCol->end(); ++inV) {

    TClonesArray &rVertexArr = *fVertexArr;
    assert(rVertexArr.GetEntries() < rVertexArr.GetSize());
    const Int_t index = rVertexArr.GetEntries();  
    new(rVertexArr[index]) egmana::TVertex();
    egmana::TVertex *pVertex = (egmana::TVertex*)rVertexArr[index];

    pVertex->x = inV->x();
    pVertex->y = inV->y();
    pVertex->z = inV->z();
    pVertex->isGoodVertex = false;

    if (inV->ndof() >= 4 &&
        fabs(inV->z()) <= 24.0 &&
        inV->position().Rho() <= 2.0
        ) {
      pVertex->isGoodVertex = true;
      if (!thevtx) thevtx = &(*inV);
      NGoodPV++;
    }

    //sumPt
    double sumpt = 0;
    for (reco::Vertex::trackRef_iterator iTrack = inV->tracks_begin(); iTrack!=inV->tracks_end(); ++iTrack) {
      const reco::Track* tmp = dynamic_cast<const reco::Track*>(iTrack->get());
      if (tmp) {
        sumpt += tmp->pt();
      } else {
        cout << "Error: track from vertex cannot be resolved.\n";
      }
    }
    //cout << "Vertex: " << inV->z() << " " << inV->tracksSize() << " " << sumpt << " | " << (inV->z() - genVertexX) << "\n";
  }
  //cout << "\n\n";

  for (reco::VertexCollection::const_iterator inV = pvBSCol->begin(); 
       inV != pvBSCol->end(); ++inV) {

    if (inV->ndof() >= 4 &&
        fabs(inV->z()) <= 24.0 &&
        inV->position().Rho() <= 2.0
        ) {
      if (!thevtxbs) thevtxbs = &(*inV);
      NGoodPV++;
    }
  }

  //****************************************************************************************************
  //Beamspot
  //****************************************************************************************************
  Handle<reco::BeamSpot> hBeamSpotProduct;
  GetProduct("offlineBeamSpot", hBeamSpotProduct, event);
  const reco::BeamSpot *inBeamSpot = hBeamSpotProduct.product();  


  //****************************************************************************************************
  //Handle to Electrons
  //****************************************************************************************************
  Handle<reco::GsfElectronCollection> hElectronProduct;
  GetProduct(fElectronsSrcName, hElectronProduct, event);
  const reco::GsfElectronCollection inElectrons = *(hElectronProduct.product());
 
  EcalClusterLazyTools *lazyTools = 0;
  EcalClusterLocal *local = 0;  
  if (fFillEGRegressionVars) {
    lazyTools = new EcalClusterLazyTools(event, setup, edm::InputTag("reducedEcalRecHitsEB"), 
                                         edm::InputTag("reducedEcalRecHitsEE"));
    local = new EcalClusterLocal;
  }

  //****************************************************************************************************
  //Electrons
  //****************************************************************************************************
  edm::Handle<reco::ConversionCollection> hConversions;
  event.getByLabel("allConversions", hConversions);

  for (reco::GsfElectronCollection::const_iterator iE = inElectrons.begin(); 
       iE != inElectrons.end(); ++iE) {

    TClonesArray &rElectronArr = *fElectronArr;
    assert(rElectronArr.GetEntries() < rElectronArr.GetSize());
    const Int_t index = rElectronArr.GetEntries();  
    new(rElectronArr[index]) egmana::TElectron();
    egmana::TElectron *pElectron = (egmana::TElectron*)rElectronArr[index];

    pElectron->pt              = iE->pt();
    pElectron->eta             = iE->eta();
    pElectron->phi             = iE->phi();

    if(iE->gsfTrack().isNonnull()) {
      pElectron->p = iE->gsfTrack()->p();
    } else if (iE->closestCtfTrackRef().isNonnull()) {
      pElectron->p = iE->closestCtfTrackRef()->p();
    }

    if (iE->superCluster().isNonnull()) {
      pElectron->scEt        = iE->superCluster()->energy() / cosh( iE->superCluster()->eta() ) ;
      pElectron->scEta           = iE->superCluster()->eta();
      pElectron->scPhi           = iE->superCluster()->phi();

      pElectron->scX         = iE->superCluster()->x();
      pElectron->scY         = iE->superCluster()->y();
      pElectron->scZ         = iE->superCluster()->z();
    }

    pElectron->q               = iE->charge();


    pElectron->isEcalDriven    = iE->ecalDrivenSeed();
    pElectron->isTrackerDriven = iE->trackerDrivenSeed();
    pElectron->isEB            = iE->isEB();
    pElectron->isEE            = iE->isEE();
    pElectron->Classification  = iE->classification();

//     pElectron->isMCReal        = isMCMatched;
//     pElectron->hltMatchBits    = MatchHLT(ele->SCluster()->Eta(),ele->SCluster()->Phi(), GetEventHeader()->RunNum(), GetEventHeader()->EvtNum());  
//     pElectron->l1TriggerMatchBits = MatchL1(ele->SCluster()->Eta(),ele->SCluster()->Phi());
    
    pElectron->TrackMomentumError = iE->trackMomentumError();
    pElectron->nBrem           = iE->basicClustersSize() - 1;
    pElectron->fBrem           = iE->fbrem();
    pElectron->EOverP          = iE->eSuperClusterOverP();
    pElectron->pIn             = iE->trackMomentumAtVtx().R();
    pElectron->ESeedClusterOverPIn  = iE->superCluster()->seed()->energy() / iE->trackMomentumAtVtx().R();
    pElectron->ESeedClusterOverPout = iE->eSeedClusterOverPout();
    pElectron->EEleClusterOverPout  = iE->eEleClusterOverPout();
    pElectron->EcalEnergy      = iE->correctedEcalEnergy();
    pElectron->EcalEnergyError = iE->correctedEcalEnergyError();

    pElectron->deltaEtaIn      = iE->deltaEtaSuperClusterTrackAtVtx();
    pElectron->deltaPhiIn      = iE->deltaPhiSuperClusterTrackAtVtx();
    pElectron->dEtaCalo        = iE->deltaEtaSeedClusterTrackAtCalo();
    pElectron->dPhiCalo        = iE->deltaPhiSeedClusterTrackAtCalo();
    pElectron->sigiEtaiEta     = iE->sigmaIetaIeta();

    
    if (fFillEGRegressionVars) {
      pElectron->sigiPhiiPhi     = sqrt(lazyTools->covariances(*(iE->superCluster()->seed()))[2]);
      if (isnan(pElectron->sigiPhiiPhi))  pElectron->sigiPhiiPhi = 0.0;
      
      
      if (pElectron->sigiEtaiEta*pElectron->sigiPhiiPhi > 0) {
        pElectron->sigiEtaiPhi = lazyTools->covariances(*(iE->superCluster()->seed()))[1]/(pElectron->sigiEtaiEta*pElectron->sigiPhiiPhi);
      } else if (lazyTools->covariances(*(iE->superCluster()->seed()))[1]>0) {
        pElectron->sigiEtaiPhi = 1.0; 
      } else {
        pElectron->sigiEtaiPhi = -1.0; 
      }
      pElectron->R9 = lazyTools->e3x3(*(iE->superCluster()->seed())) / iE->superCluster()->rawEnergy();
      if (lazyTools->e5x5(*(iE->superCluster()->seed())) > 0) {
        pElectron->SeedE1x5OverE5x5 = lazyTools->e1x5(*(iE->superCluster()->seed())) / lazyTools->e5x5(*(iE->superCluster()->seed()));
      } 
    }
    
    pElectron->SCEtaWidth = iE->superCluster()->etaWidth();
    pElectron->SCPhiWidth = iE->superCluster()->phiWidth();
    pElectron->PreShowerOverRaw   = iE->superCluster()->preshowerEnergy() / iE->superCluster()->rawEnergy();
    pElectron->HoverE             = iE->hcalOverEcal();
    pElectron->HoverESingleTower  = iE->hcalOverEcalBc();

    if(iE->gsfTrack().isNonnull()) {
      pElectron->GsfTrackChi2OverNdof = iE->gsfTrack()->normalizedChi2();
    }
    if (iE->closestCtfTrackRef().isNonnull()) {
      pElectron->KFTrackChi2OverNdof           = iE->closestCtfTrackRef()->normalizedChi2();
      pElectron->KFTrackNHits                  = iE->closestCtfTrackRef()->numberOfValidHits();
      pElectron->KFTrackNLayersWithMeasurement = iE->closestCtfTrackRef()->hitPattern().trackerLayersWithMeasurement();
    }


    
    if(thevtx && iE->gsfTrack().isNonnull()) {

      const reco::TransientTrack &tt = transientTrackBuilder->build(iE->gsfTrack());

      pElectron->d0              = iE->gsfTrack()->dxy(thevtx->position());
      pElectron->dz              = iE->gsfTrack()->dz(thevtx->position());

      const double gsfsign   = ( (-iE->gsfTrack()->dxy(thevtx->position()))   >=0 ) ? 1. : -1.;
      double gsfsignbs;
      if (thevtxbs) gsfsignbs = ( (-iE->gsfTrack()->dxy(thevtxbs->position())) >=0 ) ? 1. : -1.;
      else gsfsignbs = gsfsign;
      
      const std::pair<bool,Measurement1D> &ip3dpv =  IPTools::absoluteImpactParameter3D(tt,*thevtx);
      if (ip3dpv.first) {
        pElectron->ip3d = gsfsign*ip3dpv.second.value();
        pElectron->ip3dSig = gsfsign*ip3dpv.second.value() / ip3dpv.second.error();
      } 
      
      if (thevtxbs) {
	const std::pair<bool,Measurement1D> &ip3dpvbs =  IPTools::absoluteImpactParameter3D(tt,*thevtxbs);
	if (ip3dpvbs.first) {
	  pElectron->ip3dBS = gsfsignbs*ip3dpvbs.second.value();
	  pElectron->ip3dSigBS = gsfsignbs*ip3dpvbs.second.value() / ip3dpvbs.second.error();
	}
      }
      pElectron->nExpHitsInner   = iE->gsfTrack()->trackerExpectedHitsInner().numberOfHits();     
    }

    //Do this later
    if (inBeamSpot) {
      pElectron->isConv = ConversionTools::hasMatchedConversion(*iE,hConversions,inBeamSpot->position());
    }


    pElectron->trkIso03        = iE->dr03TkSumPt();
    pElectron->emIso03         = iE->dr03EcalRecHitSumEt();
    pElectron->hadIso03        = iE->dr03HcalDepth1TowerSumEt() + iE->dr03HcalDepth2TowerSumEt();
    pElectron->trkIso04        = iE->dr04TkSumPt();
    pElectron->emIso04         = iE->dr04EcalRecHitSumEt();
    pElectron->hadIso04        = iE->dr04HcalDepth1TowerSumEt() + iE->dr04HcalDepth2TowerSumEt();
    //PFIsolation needs to be implemented later
  

    pElectron->SCRawEnergy = iE->superCluster()->rawEnergy();
    pElectron->E5x5 = iE->e5x5();
    pElectron->EtaSeed = iE->superCluster()->seed()->eta();
    pElectron->PhiSeed = iE->superCluster()->seed()->phi();
    pElectron->ESeed   = iE->superCluster()->seed()->energy();

    if (fFillEGRegressionVars) {
      pElectron->E3x3Seed = lazyTools->e3x3(*(iE->superCluster()->seed()));
      pElectron->E5x5Seed = lazyTools->e5x5(*(iE->superCluster()->seed()));
      pElectron->EMaxSeed = lazyTools->eMax(*(iE->superCluster()->seed()));
      pElectron->E2ndSeed = lazyTools->e2nd(*(iE->superCluster()->seed()));
      pElectron->ETopSeed= lazyTools->e2x5Top(*(iE->superCluster()->seed()));
      pElectron->EBottomSeed = lazyTools->eBottom(*(iE->superCluster()->seed()));
      pElectron->ELeftSeed = lazyTools->eLeft(*(iE->superCluster()->seed()));
      pElectron->ERightSeed = lazyTools->eRight(*(iE->superCluster()->seed()));
      pElectron->E2x5MaxSeed = lazyTools->e2x5Max(*(iE->superCluster()->seed()));
      pElectron->E2x5TopSeed = lazyTools->e2x5Top(*(iE->superCluster()->seed()));
      pElectron->E2x5BottomSeed = lazyTools->e2x5Bottom(*(iE->superCluster()->seed()));
      pElectron->E2x5LeftSeed = lazyTools->e2x5Left(*(iE->superCluster()->seed()));
      pElectron->E2x5RightSeed = lazyTools->e2x5Right(*(iE->superCluster()->seed()));


      //Turn this off for now because it depends on detector geometry
      //local coordinates
      if (iE->superCluster()->seed()->hitsAndFractions().at(0).first.subdetId()==EcalBarrel) {
        float etacry, phicry, thetatilt, phitilt;
        int ieta, iphi;
        local->localCoordsEB(*(iE->superCluster()->seed()),setup,etacry,phicry,ieta,iphi,thetatilt,phitilt);

        pElectron->IEtaSeed = ieta;
        pElectron->IPhiSeed = iphi;
        pElectron->EtaCrySeed = etacry;
        pElectron->PhiCrySeed = phicry;        
      }
      else {
        pElectron->IEtaSeed = -999;
        pElectron->IPhiSeed = -999;
        pElectron->EtaCrySeed = -999;
        pElectron->PhiCrySeed = -999;
      }
    }

  } //loop over electrons


  //
  // Fill event info tree
  //
  fEventInfo.runNum       = event.id().run();
  fEventInfo.evtNum       = event.id().event();
  fEventInfo.lumiSec      = event.luminosityBlock();
  fEventInfo.eventweight  = 1.0;

  fEventInfo.nGoodPV      = NGoodPV;
  if (thevtx) {
    fEventInfo.pvx          = thevtx->x();
    fEventInfo.pvy          = thevtx->y();
    fEventInfo.pvz          = thevtx->z();
  }
  if (inBeamSpot) {
    fEventInfo.bsx          = inBeamSpot->x0();
    fEventInfo.bsy          = inBeamSpot->y0();
    fEventInfo.bsz          = inBeamSpot->z0();
  }

  fEventInfo.genVertexX     = genVertexX;
  fEventInfo.genVertexY     = genVertexY;
  fEventInfo.genVertexZ     = genVertexZ;

  ///Fill the branches
  fEventTree->Fill();

  //cleanup
  if (lazyTools) delete lazyTools;
  if (local) delete local;  
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
ElectronNtupler::beginJob()
{


  cout<<" ElectronNtupler::beginJob( " <<endl; 
  
  //************************************************************************************************
  // Set up output arrays
  //************************************************************************************************
  fGenParticleArr = new TClonesArray("egmana::TGenParticle"); assert(fGenParticleArr);
  fElectronArr    = new TClonesArray("egmana::TElectron");    assert(fElectronArr);
  fVertexArr      = new TClonesArray("egmana::TVertex");      assert(fVertexArr);

  //************************************************************************************************
  // Create output file
  //************************************************************************************************
  fOutputFile = new TFile(fOutputName.c_str(),"RECREATE"); 

  //************************************************************************************************
  // Initialize data trees and structs
  //************************************************************************************************
  fEventTree = new TTree("Events","Events");

  fEventTree->Branch("Info",&fEventInfo);
  if(fUseGen) {
    fEventTree->Branch("GenParticle",&fGenParticleArr);  
  }

  fEventTree->Branch("Electron",   &fElectronArr);
  fEventTree->Branch("Vertex",     &fVertexArr);

  cout<<" ElectronNtupler tree branches defined.. " <<endl; 
  

}







double ElectronNtupler::DeltaPhi(double v1, double v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
  double diff = v1 - v2;
  
  while (diff >acos(-1)) diff -= 2*acos(-1);
  while (diff <= -acos(-1)) diff += 2*acos(-1);
  
  return diff; 
}


double ElectronNtupler::GetDeltaR(double eta1, double eta2, double phi1, double phi2){ 
  // Computes the DeltaR of two objects from their eta and phi values
  
  return sqrt( (eta1-eta2)*(eta1-eta2) + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );
  
}


// ------------ method called once each job just after ending the event loop  ------------
void 
ElectronNtupler::endJob() 
{


  //fEventTree->Print();
  //fEventTree->Write();

  fOutputFile->cd();
  fOutputFile->Write();
  fOutputFile->Close();
  
  delete fGenParticleArr;
  delete fElectronArr;
  delete fVertexArr;

}

// ------------ method called when starting to processes a run  ------------
void 
ElectronNtupler::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
ElectronNtupler::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
ElectronNtupler::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
ElectronNtupler::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ElectronNtupler::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ElectronNtupler);
