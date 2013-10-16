#ifndef EGAMMA_NTUPLER_TTRACK_HH
#define EGAMMA_NTUPLER_TTRACK_HH

#include <TObject.h>

namespace egmana
{
  class TTrack : public TObject
  {
    public:
      TTrack() {

        pt = 0;
        eta = -999;
        phi = -999;
        vx = -999;
        vy = -999;
        vz = -999;
        q = -99;  
        isEcalDriven = false;
        isTrackerDriven = false;

      }

    ~TTrack() {}

    Float_t pt, eta, phi;     // kinematics
    Float_t vx, vy, vz;       // vertex  position
    Int_t q;                  // charge
    Bool_t isEcalDriven;         // is ECAL seeded electron track?
    Bool_t isTrackerDriven;      // is TrackerDriven electron track?

    ClassDef(TTrack,2)
  };
}
#endif

