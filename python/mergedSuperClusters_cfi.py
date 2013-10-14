import FWCore.ParameterSet.Config as cms
mergedSuperClusters = cms.EDProducer("SuperClusterCombiner",
                                     labels = cms.VInputTag(cms.InputTag("hybridSuperClusters"),
                                                            cms.InputTag("multi5x5SuperClustersWithPreshower")
                                     ))

