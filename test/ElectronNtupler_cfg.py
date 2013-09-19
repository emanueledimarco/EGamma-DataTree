import FWCore.ParameterSet.Config as cms

process = cms.Process("CMSNTUPLER")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')
process.load('RecoVertex/PrimaryVertexProducer/OfflinePrimaryVertices_cfi')

process.configurationMetadata = cms.untracked.PSet(
    version    = cms.untracked.string('EDM_V0'),
    annotation = cms.untracked.string('AODSIM'),
    name       = cms.untracked.string('ElectronNtuplerProduction')
    )

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
    'file:/cmsrm/pc25/emanuele/releases/egamma/CMSSW_7_0_0_pre3/src/electrons.root'
                )
                            )

# Global Tag
process.GlobalTag.globaltag = 'PRE_ST62_V8::All'

#For Transient Track Builder
process.load('TrackingTools/TransientTrack/TransientTrackBuilder_cfi')

#Load Ntupler Module
process.load("EGamma.DataTree.ElectronNtupler_cfi")
process.myElectronNtupler.debugLevel = cms.int32(0)
process.myElectronNtupler.FillEGRegressionVars = cms.bool(True)
process.myElectronNtupler.UseGen = cms.bool(True)

#Define ntupler sequence
process.ntupler_sequence = cms.Sequence( process.myElectronNtupler )

process.ntupler_step  = cms.Path(process.ntupler_sequence)

# schedule definition
process.schedule = cms.Schedule(process.ntupler_step)
