#ifndef EGAMMA_NTUPLER_TEVENTINFO_HH
#define EGAMMA_NTUPLER_TEVENTINFO_HH

#include <TObject.h>

namespace egmana 
{
  class TEventInfo : public TObject
  {
    public:
      TEventInfo(){
        runNum = 0;
        evtNum = 0;
        lumiSec = 0;
        eventweight = -999;
        nPU = 0;
        nPUMinusOne= 0;                  
        nPUPlusOne= 0;
        nPUMean = -999;
        nPUMeanMinusOne = -999;
        nPUMeanPlusOne = -999;        
        RhoKt6PFJets = -999;
        RhoKt6PFJetsCentralChargedPileup = -999;
        RhoKt6PFJetsCentralNeutral = -999;
        RhoKt6PFJetsCentralNeutralTight = -999;        
        triggerBits = 0;
        l1triggerBits = 0;
        nGoodPV = 0;
        pvx = -999;
        pvy = -999;
        pvz = -999;
        bsx = -999;
        bsy = -999;
        bsz = -999;
        pfMEx = -999;
        pfMEy= -999;
        pfTrackMEx = -999;
        pfTrackMEy = -999; 
        genVertexX = -999;
        genVertexY = -999;
        genVertexZ = -999;
      }
      ~TEventInfo(){}

      UInt_t runNum; 			    // run number in data
      UInt_t evtNum; 			    // event number in data
      UInt_t lumiSec;			    // lumi section      
      Float_t eventweight;
      UInt_t nPU;                           // number of pileup events.
      UInt_t nPUMinusOne;                  
      UInt_t nPUPlusOne;
      Float_t nPUMean;
      Float_t nPUMeanMinusOne;
      Float_t nPUMeanPlusOne;

      Float_t RhoKt6PFJets;
      Float_t RhoKt6PFJetsCentralChargedPileup;
      Float_t RhoKt6PFJetsCentralNeutral;
      Float_t RhoKt6PFJetsCentralNeutralTight;

      ULong_t triggerBits;		    // HLT trigger bits 
      UInt_t l1triggerBits;		    // L1 trigger bits 

      UInt_t nGoodPV;                       // number of reconstructed primary vertices in event
      Float_t pvx, pvy, pvz;		    // primary vertex with the most associated tracks 
      Float_t bsx, bsy, bsz;		    // beamspot

      Float_t pfMEx, pfMEy;	            // particle flow MET
      Float_t pfTrackMEx, pfTrackMEy;       // particle flow track MET

      Float_t genVertexX, genVertexY, genVertexZ;

    ClassDef(TEventInfo,2)
  };
}
#endif
