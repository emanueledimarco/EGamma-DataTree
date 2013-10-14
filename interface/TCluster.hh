#ifndef EGAMMA_NTUPLER_TCLUSTER_HH
#define EGAMMA_NTUPLER_TCLUSTER_HH

#include <TObject.h>

namespace egmana
{
  class TCluster : public TObject
  {
    public:
      TCluster() {

        energy = 0;
        eta = -999;
        phi = -999;
        seedenergy = 0;
        seedeta = 0;
        seedphi = 0;

      }

    ~TCluster() {}

    Float_t energy, eta, phi;     // kinematics
    Float_t seedenergy, seedeta, seedphi;     // seed kinematics

    ClassDef(TCluster,2)
  };
}
#endif

