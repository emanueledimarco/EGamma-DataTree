#ifndef EGAMMA_NTUPLER_TELECTRON_HH
#define EGAMMA_NTUPLER_TELECTRON_HH

#include <TObject.h>

namespace egmana
{
  class TElectron : public TObject
  {
    public:
      TElectron() {

        pt = 0;
        eta = -999;
        phi = -999;
        p   = 0;        
        scEt = 0;
        scEta = -999;
        scPhi = -999;
        q = -99;  
        isEcalDriven = false;
        isTrackerDriven = false;
        isEB = false;           
        isEE = false;           
        Classification = -1;

        isMCReal = 0;    
        hltMatchBits = 0;   
        l1TriggerMatchBits = 0;

        TrackMomentumError = -1;
        nBrem = -1;
        fBrem = 0;
        EOverP = 0;
        pIn = 0;
        ESeedClusterOverPIn = 0;
        ESeedClusterOverPout = 0;
        EEleClusterOverPout = 0; 
        EcalEnergy = 0;
        EcalEnergyError = 0;
        deltaEtaIn = -999;
        deltaPhiIn = -999;
        dEtaCalo = -999;
        dPhiCalo = -999;
        sigiEtaiEta = -1;
        sigiPhiiPhi = -1;
        sigiEtaiPhi = -999;
        SCEtaWidth = -1;
        SCPhiWidth = -1;
        R9 = -1;
        PreShowerOverRaw = -1;
        HoverE = -1;
        HoverESingleTower = -1;   
        GsfTrackChi2OverNdof = 0;
        KFTrackChi2OverNdof = 0;
        KFTrackNHits = -1;
        KFTrackNLayersWithMeasurement = -1;
        SeedE1x5OverE5x5 = -1;

        isConv = -1;         
        nExpHitsInner = -1;

        d0 = -998;
        dz = -999;
        ip3d = -999;
        ip3dSig = -999;
        ip3dBS = -999;
        ip3dSigBS = -999;
        
        trkIso03 = -999;        
        emIso03 = -999;         
        hadIso03 = -999;        
        trkIso04 = -999;        
        emIso04 = -999;         
        hadIso04 = -999;        
        pfIso04ChargedHadron = -999;  
        pfIso04ChargedParticle = -999; 
        pfIso04NeutralHadron = -999;   
        pfIso04Photon = -999;          
        pfIso04NeutralHadronHighThreshold = -999; 
        pfIso04PhotonHighThreshold = -999;        
        pfIso04PU = -999;        

        SCRawEnergy = -999;
        E5x5 = -999;
        EtaSeed = -999;
        PhiSeed = -999;
        ESeed = -999;
        E3x3Seed = -999;
        E5x5Seed = -999;
        EMaxSeed = -999;
        E2ndSeed = -999;
        ETopSeed = -999;
        EBottomSeed = -999;
        ELeftSeed = -999;
        ERightSeed = -999;
        E2x5MaxSeed = -999;
        E2x5TopSeed = -999;
        E2x5BottomSeed = -999;
        E2x5LeftSeed = -999;
        E2x5RightSeed = -999;
        IEtaSeed = -999;
        IPhiSeed = -999;
        EtaCrySeed = -999;
        PhiCrySeed = -999;

        NSCMatchedPFCandidates = 0;
        for(int k=0; k<10;++k) SCMatchedPFCandidateIndex[k] = 0;
        NGsfTrackMatchedPFCandidates = 0;
        for(int k=0; k<10;++k) GsfTrackMatchedPFCandidateIndex[k] = 0;
      }


      ~TElectron(){}
    
      Float_t pt, eta, phi, p;     // kinematics, p is bestTrack momentum
      Float_t scEt, scEta, scPhi;  // supercluster
      Int_t q;                     // charge

      Bool_t isEcalDriven;         // is ECAL seeded electron?
      Bool_t isTrackerDriven;      // is TrackerDriven electron
      Bool_t  isEB;                // is in barrel
      Bool_t  isEE;                // is in endcap
      Int_t   Classification;

      Int_t  isMCReal;        
      ULong_t hltMatchBits;         // bits for matching with HLT primitives
      UInt_t l1TriggerMatchBits;   // bits for matching with L1 Seeds

      Float_t TrackMomentumError;
      Int_t   nBrem;               // Number of Brems
      Float_t fBrem, EOverP;       // fBrem, EOverP
      Float_t pIn;                 // mode of the gsf track at vertex
      Float_t ESeedClusterOverPIn; // ESeedClusterOverPout     
      Float_t ESeedClusterOverPout;// ESeedClusterOverPout    
      Float_t EEleClusterOverPout; 
      Float_t EcalEnergy;
      Float_t EcalEnergyError;
      Float_t deltaEtaIn;          // eta difference between track (at vertex) and SC
      Float_t deltaPhiIn;          // phi difference between track (at vertex) and SC
      Float_t dEtaCalo;
      Float_t dPhiCalo;
      Float_t sigiEtaiEta;         // eta-width of shower in number of crystals
      Float_t sigiPhiiPhi;         // phi-width of shower in number of crystals
      Float_t sigiEtaiPhi;
      Float_t SCEtaWidth;
      Float_t SCPhiWidth;
      Float_t R9;
      Float_t PreShowerOverRaw;
      Float_t HoverE;              // H / E
      Float_t HoverESingleTower;   // H / E
      Float_t GsfTrackChi2OverNdof;
      Float_t KFTrackChi2OverNdof;
      Float_t KFTrackNHits;
      Float_t KFTrackNLayersWithMeasurement;
      Float_t SeedE1x5OverE5x5;

      Bool_t  isConv;              // is conversion? (vertexing method)
      Float_t nExpHitsInner;       // number of hits expected before first hit
      Float_t partnerDeltaCot;     // cot(theta) difference with conversion partner track       
      Float_t partnerDist;         // distance in x-y plane to nearest conversion partner track
      Float_t partnerRadius;       // radius of helix intersection with conversion partner track

      Float_t d0, d0Err, dz;         // impact parameter
      Float_t ip3d, ip3dSig;         //IP3D PV
      Float_t ip3dBS, ip3dSigBS;     //IP3D Beamspot constrained PV

      Float_t trkIso03;            // track isolation
      Float_t emIso03;             // ECAL-based isolation
      Float_t hadIso03;            // HCAL-based isolation
      Float_t trkIso04;            // track isolation
      Float_t emIso04;             // ECAL-based isolation
      Float_t hadIso04;            // HCAL-based isolation
      Float_t pfIso04ChargedHadron;   // PF Isolation
      Float_t pfIso04ChargedParticle; // PF Isolation
      Float_t pfIso04NeutralHadron;   // PF Isolation
      Float_t pfIso04Photon;          // PF Isolation
      Float_t pfIso04NeutralHadronHighThreshold; // PF Isolation
      Float_t pfIso04PhotonHighThreshold;        // PF Isolation
      Float_t pfIso04PU;

      //regression related variables
      Float_t SCRawEnergy;
      Float_t E5x5;
      Float_t EtaSeed;
      Float_t PhiSeed;
      Float_t ESeed;
      Float_t E3x3Seed;
      Float_t E5x5Seed;
      Float_t EMaxSeed;
      Float_t E2ndSeed;
      Float_t ETopSeed;
      Float_t EBottomSeed;
      Float_t ELeftSeed;
      Float_t ERightSeed;
      Float_t E2x5MaxSeed;
      Float_t E2x5TopSeed;
      Float_t E2x5BottomSeed;
      Float_t E2x5LeftSeed;
      Float_t E2x5RightSeed;
      Float_t IEtaSeed;
      Float_t IPhiSeed;
      Float_t EtaCrySeed;
      Float_t PhiCrySeed;

      //for match to pf candidates
      UInt_t NSCMatchedPFCandidates;
      UInt_t SCMatchedPFCandidateIndex[10];    
      UInt_t NGsfTrackMatchedPFCandidates;
      UInt_t GsfTrackMatchedPFCandidateIndex[10];

    ClassDef(TElectron,2)
  };
}
#endif
