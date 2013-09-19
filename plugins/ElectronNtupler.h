// system include files
#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TString.h"

using namespace std;
using namespace edm;


//*****************************************************************************
// Electron Ntuple Data Formats
//*****************************************************************************
#include "EGamma/DataTree/interface/DataTreeDefs.hh"
#include "EGamma/DataTree/interface/TEventInfo.hh"
#include "EGamma/DataTree/interface/TGenParticle.hh"
#include "EGamma/DataTree/interface/TElectron.hh"
#include "EGamma/DataTree/interface/TVertex.hh"
#include <TClonesArray.h>

class ElectronNtupler : public edm::EDAnalyzer {
   public:
      explicit ElectronNtupler(const edm::ParameterSet&);
      ~ElectronNtupler();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
                   
   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

      // ----------member data ---------------------------
      double DeltaPhi(double v1, double v2); 
      double GetDeltaR(double eta1, double eta2, double phi1, double phi2); 

  
    //*****************************************************************
    // Input Collection Tags
    //*****************************************************************
    string fGenParticlesSrcName;
    string fGenJetsSrcName;
    string fPrimaryVerticesSrcName;
    string fPrimaryVerticesBSSrcName;
    string fElectronsSrcName;

    //***************************************************************************
    //OUTPUT
    //***************************************************************************
    egmana::TEventInfo      fEventInfo;       // general event information
    TClonesArray           *fGenParticleArr;  // genparticle array
    TClonesArray           *fElectronArr;     // electron array
    TClonesArray           *fVertexArr;       // Vertex array
    
    TFile                  *fOutputFile;      // output file handle
    string                  fOutputName;      // output file name
    TTree*                  fEventTree;       // event tree
    int nEventsProcessed; 
    
    //***************************************************************************
    //OPTIONS
    //***************************************************************************
    bool                  fPrintDebug;
    bool                  fUseGen;          // flag whether to look at generator info
    bool                  fFillGenOnly;     // flag to skip reco objects       
    bool                  fFillEGRegressionVars; //flat to fill geometry dependent variables

    Double_t                fElePtMin;        // minimum supercluster ET
    Double_t                fElePtMax;        // maximum supercluster ET
    Double_t                fEleEtaMin;       // minimum supercluster eta
    Double_t                fEleEtaMax;       // maximum supercluster eta


  protected:
    
    template <typename TYPE>
    void                     GetProduct(const std::string name, edm::Handle<TYPE> &prod,
                                        const edm::Event &event) const;    

};



//--------------------------------------------------------------------------------------------------
template <typename TYPE>
inline void ElectronNtupler::GetProduct(const std::string edmname, edm::Handle<TYPE> &prod,
                                   const edm::Event &event) const
{
  // Try to access data collection from EDM file. We check if we really get just one
  // product with the given name. If not we print an error and exit.

  try {
    event.getByLabel(edm::InputTag(edmname),prod);
    if (!prod.isValid()) 
      throw edm::Exception(edm::errors::Configuration, "BaseFiller::GetProduct()\n")
        << "Cannot get collection with label " << edmname <<  std::endl;
  } catch (...) {
    edm::LogError("ElectronNtupler") << "Cannot get collection with label "
                                << edmname << std::endl;
//     PrintErrorAndExit(Form("Cannot get collection with label %s", 
//                            edmname.c_str()));
  }
}

