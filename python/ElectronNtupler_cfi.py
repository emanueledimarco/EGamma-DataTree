import FWCore.ParameterSet.Config as cms

myElectronNtupler = cms.EDAnalyzer('ElectronNtupler',
                                   outputFile = cms.string('electron_tuple.root'),
                                   GenParticlesSrcName = cms.string('genParticles'),
                                   PrimaryVerticesSrcName = cms.string('offlinePrimaryVertices'),
                                   PrimaryVerticesBSSrcName = cms.string('offlinePrimaryVerticesWithBS'),
                                   ElectronsSrcName = cms.string('gsfElectrons'),
                                   debugLevel = cms.int32(0),
                                   UseGen = cms.bool(True),
                                   PrintDebug = cms.bool(True),
                                   FillEGRegressionVars = cms.bool(False),
                                   
                                   ElePtMin = cms.double(5.0),
                                   ElePtMax =  cms.double(99999999),
                                   EleEtaMin = cms.double(-3.0),
                                   EleEtaMax = cms.double(3.0)
                                   )
