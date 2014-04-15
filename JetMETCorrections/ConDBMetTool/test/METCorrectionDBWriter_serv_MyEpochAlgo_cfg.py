# Location /JetMETCorrections/Modules/test
import FWCore.ParameterSet.Config as cms 
process = cms.Process('jecdb') 
process.load('CondCore.DBCommon.CondDBCommon_cfi') 
process.CondDBCommon.connect = 'sqlite_file:MET111_V0.db'  
process.source = cms.Source('EmptySource')

##____________________________________________________________________________||
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring('rfio:corr_inputs_numEvent10.root')
    )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

##____________________________________________________________________________||
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('corrShiftData1.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p1') ),
    outputCommands = cms.untracked.vstring('keep *')
    )

##____________________________________________________________________________||
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

#process.vvv = selectedVerticesForMEtCorr = cms.EDFilter("VertexSelector",
#    src = cms.InputTag('offlinePrimaryVertices'),
#    cut = cms.string("isValid & ndof >= 4 & chi2 > 0 & tracksSize > 0 & abs(z) < 24 & abs(position.Rho) < 2."),
#    filter = cms.bool(False)                                          
#)

process.METCorrectionService = cms.ESSource(
    'ShiftxyMETCorrectionESSource', # was created as JetCorrectionService, renamed
    #'LXXXCorrectionESSource', # was created as JetCorrectionService, renamed
    #'L1OffsetCorrectionESSource',
    #'L1FastjetCorrectionESSource',
    level    = cms.string('L3Absolute'), # test standard jet corr
    #level    = cms.string('L5Flavor'),
    era = cms.string('myEpoch'),         # test standard jet corr
    #algorithm = cms.string('AK5PF'),     # test standard jet corr
    algorithm = cms.string('metShiftxy'),
    #algorithm = cms.string('AK5PF'),
    section   = cms.string(''),
    useCondDB = cms.untracked.bool(True),
    debug = cms.untracked.bool(True)
    )

#process.mmm = cms.EDProducer('PFJetCorrectionProducer',
process.mmm = cms.EDProducer('METxyShiftCorrectionProducer',
    #src         = cms.InputTag('ak5PFJets'),
    src         = cms.InputTag('pfMet'),
    srcVertices = cms.InputTag('offlinePrimaryVertices'),
    correctors  = cms.vstring('METCorrectionService'),
    #verbose = cms.untracked.bool(True)
    )


process.p1 = cms.Path( 
process.mmm
) 

process.e1 = cms.EndPath(
    process.out
    )

 # the above 3 elements are needed only when the service is initialized from local txt files
 # the 'algorithm' tag is also the name of the DB payload
