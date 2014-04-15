# Location /JetMETCorrections/Modules/test
import FWCore.ParameterSet.Config as cms 
process = cms.Process('jecdb') 
process.load('CondCore.DBCommon.CondDBCommon_cfi') 
#process.CondDBCommon.connect = 'sqlite_file:MET111_V0.db'  

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
#process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:startup', '')

#process.vvv = selectedVerticesForMEtCorr = cms.EDFilter("VertexSelector",
#    src = cms.InputTag('offlinePrimaryVertices'),
#    cut = cms.string("isValid & ndof >= 4 & chi2 > 0 & tracksSize > 0 & abs(z) < 24 & abs(position.Rho) < 2."),
#    filter = cms.bool(False)                                          
#)

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
      DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
        ),
      timetype = cms.string('runnumber'),
      toGet = cms.VPSet(
      cms.PSet(
              #record = cms.string('metShiftxy'),
              record = cms.string('METCorrectionsRecord'), 
              #tag    = cms.string('metShiftxy'),
              tag    = cms.string('MET11_V0'), 
              label  = cms.untracked.string('metShiftxy')
              #label  = cms.untracked.string('AK5CaloLocal') 
            ),                                                                               
       ),
      connect = cms.string('sqlite:MET11_V0.db')
)

process.METCorrectionService = cms.ESProducer(
    'ShiftxyMETCorrectionESProducer', # was created as JetCorrectionService, renamed
    #'LXXXCorrectionESSource', # was created as JetCorrectionService, renamed
    #'L1OffsetCorrectionESSource',
    #'L1FastjetCorrectionESSource',
    level    = cms.string('L3Absolute'), # test standard jet corr
    #level    = cms.string('L5Flavor'),
    #era = cms.string('myEpoch'),         # test standard jet corr
    #algorithm = cms.string('AK5PF'),     # test standard jet corr
    algorithm = cms.string('metShiftxy'),
    #algorithm = cms.string('AK5PF'),
    section   = cms.string(''),
    #debug = cms.untracked.bool(True)
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

