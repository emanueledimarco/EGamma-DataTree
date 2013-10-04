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

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource",
                            # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring(
    # my RECO with pfseeding V3
#     'file:/cmsrm/pc24_2/emanuele/data/egamma/pfseedingV3/electrons_10.root',
#     'file:/cmsrm/pc24_2/emanuele/data/egamma/pfseedingV3/electrons_2.root',
#     'file:/cmsrm/pc24_2/emanuele/data/egamma/pfseedingV3/electrons_3.root',
#     'file:/cmsrm/pc24_2/emanuele/data/egamma/pfseedingV3/electrons_5.root',
#     'file:/cmsrm/pc24_2/emanuele/data/egamma/pfseedingV3/electrons_6.root',
#     'file:/cmsrm/pc24_2/emanuele/data/egamma/pfseedingV3/electrons_7.root',
#     'file:/cmsrm/pc24_2/emanuele/data/egamma/pfseedingV3/electrons_9.root'
'/store/relval/CMSSW_7_0_0_pre3/RelValZEE/GEN-SIM-RECO/PU_PRE_ST62_V8-v1/00000/145F25B8-1613-E311-8A60-002481E0FE70.root',
'/store/relval/CMSSW_7_0_0_pre3/RelValZEE/GEN-SIM-RECO/PU_PRE_ST62_V8-v1/00000/88283733-1513-E311-9623-003048D2C1E0.root',
'/store/relval/CMSSW_7_0_0_pre3/RelValZEE/GEN-SIM-RECO/PU_PRE_ST62_V8-v1/00000/D063B956-E113-E311-948E-02163E008E5A.root',
'/store/relval/CMSSW_7_0_0_pre3/RelValZEE/GEN-SIM-RECO/PU_PRE_ST62_V8-v1/00000/E4F65B56-9C14-E311-97DC-02163E009430.root',
'/store/relval/CMSSW_7_0_0_pre3/RelValZEE/GEN-SIM-RECO/PU_PRE_ST62_V8-v1/00000/E65BE867-B014-E311-8BEF-02163E00A0D9.root',
'/store/relval/CMSSW_7_0_0_pre3/RelValZEE/GEN-SIM-RECO/PU_PRE_ST62_V8-v1/00000/FE0BBB08-AC14-E311-B2B2-02163E009DF6.root'
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
