#ifndef EGAMMA_NTUPLER_TSEED_HH
#define EGAMMA_NTUPLER_TSEED_HH

#include <TObject.h>

namespace egmana 
{
  class TSeed : public TObject  
  {
  public:

    TSeed() {
      x = -999.;
      y = -999.;
      z = -999.;
    }
    
    ~TSeed(){}
    
    Float_t x,y,z;
    
    ClassDef(TSeed,2)
  };
}
#endif

