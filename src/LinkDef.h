#ifndef EGAMMA_NTUPLER_LINKDEF_H
#define EGAMMA_NTUPLER_LINKDEF_H
#include "EGamma/DataTree/interface/TEventInfo.hh"
#include "EGamma/DataTree/interface/TGenParticle.hh"
#include "EGamma/DataTree/interface/TElectron.hh"
#include "EGamma/DataTree/interface/TVertex.hh"
#include "EGamma/DataTree/interface/Types.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace egmana;

#pragma link C++ class egmana::TEventInfo+;
#pragma link C++ class egmana::TGenParticle+;
#pragma link C++ class egmana::TElectron+;
#pragma link C++ class egmana::TVertex+;
#pragma link C++ typedef egmana::FourVector;
#pragma link C++ typedef egmana::FourVectorM;
#pragma link C++ typedef egmana::FourVectorE;
#endif
