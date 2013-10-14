#include "RecoMuon/TrackerSeedGenerator/plugins/CollectionCombiner.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

typedef CollectionCombiner< reco::SuperClusterCollection > SuperClusterCombiner;
DEFINE_FWK_MODULE(SuperClusterCombiner);
