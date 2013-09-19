//--------------------------------------------------------------------------------------------------

#ifndef EGAMMA_DATATREE_TYPES_H
#define EGAMMA_DATATREE_TYPES_H
 
#include <Rtypes.h>
#include <Math/GenVector/LorentzVector.h>
#include <Math/Vector4D.h>
#include <Math/Vector4Dfwd.h>

namespace egmana
{
  typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<Double_t> >   FourVector;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<Double_t> > FourVectorM;
  typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double_t> > FourVectorE;

}
#endif
