// location JetMETCorrections/Modules/interface

#ifndef METCorrectionESSource_h
#define METCorrectionESSource_h

//
// Original Author:  Alexandre Sakharov
// Created:  Nov. 15, 2013
//

#include "boost/shared_ptr.hpp"
#include <string>
#include <iostream>

#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/METCorrectorParameters.h"
#include "FWCore/Framework/interface/IOVSyncValue.h"
#include "FWCore/Framework/interface/ValidityInterval.h"
#include "FWCore/Framework/interface/SourceFactory.h"

class JetCorrector;
class METCorrector;
class JetCorrectionsRecord;
class METCorrectionsRecord;

namespace edm {
  namespace eventsetup {
    class EventSetupRecordKey;
  }
}

#define DEFINE_MET_CORRECTION_ESSOURCE(corrector_, name_ ) \
typedef METCorrectionESSource <corrector_>  name_; \
DEFINE_FWK_EVENTSETUP_SOURCE(name_)

template <class Corrector>
class METCorrectionESSource : public edm::ESProducer,
			      public edm::EventSetupRecordIntervalFinder
{
private:
  edm::ParameterSet mParameterSet;
  std::string mLevel;
  std::string mEra;
  std::string mAlgo;
  std::string mSection;
  bool mDebug;

public:
  METCorrectionESSource(edm::ParameterSet const& fConfig) : mParameterSet(fConfig) 
  {
    std::string label = fConfig.getParameter<std::string>("@module_label");
    mLevel            = fConfig.getParameter<std::string>("level");
    mEra              = fConfig.getParameter<std::string>("era");
    mAlgo             = fConfig.getParameter<std::string>("algorithm");
    mSection          = fConfig.getParameter<std::string>("section");
    mDebug            = fConfig.getUntrackedParameter<bool>("debug",false);

    setWhatProduced(this, label);
    //findingRecord<JetCorrectionsRecord>(); 
    findingRecord<METCorrectionsRecord>();
  }

  ~METCorrectionESSource() {}

  //boost::shared_ptr<JetCorrector> produce(JetCorrectionsRecord const& iRecord)
  //boost::shared_ptr<METCorrector> produce(JetCorrectionsRecord const& iRecord) 
  boost::shared_ptr<METCorrector> produce(METCorrectionsRecord const& iRecord) 
  {
    std::string fileName("CondFormats/JetMETObjects/data/");
    if (!mEra.empty())
      fileName += mEra;
    if (!mLevel.empty())
      fileName += "_"+mLevel;
    if (!mAlgo.empty())
      fileName += "_"+mAlgo;
    fileName += ".txt";
    if (mDebug)
      std::cout << " METCorrectionESSource Parameter File: " << fileName << std::endl;
    edm::FileInPath fip(fileName);
    //JetCorrectorParameters *tmpJetCorPar = new JetCorrectorParameters(fip.fullPath(), mSection);
    METCorrectorParameters *tmpMETCorPar = new METCorrectorParameters(fip.fullPath(),mSection);
    if (mDebug){
       tmpMETCorPar->printScreen();
       //tmpJetCorPar->printScreen();
       std::cout << "mParameterSet(fConfig) from METCorrectionESSource " << std::endl;
       std::cout << mParameterSet << std::endl;
   }
    //boost::shared_ptr<JetCorrector> mCorrector(new Corrector(*tmpJetCorPar, mParameterSet));
    //boost::shared_ptr<METCorrector> mCorrector(new Corrector(*tmpJetCorPar, mParameterSet));
    boost::shared_ptr<METCorrector> mCorrector(new Corrector(*tmpMETCorPar, mParameterSet));
    return mCorrector;
  }

  void setIntervalFor(edm::eventsetup::EventSetupRecordKey const&, edm::IOVSyncValue const&, edm::ValidityInterval& fIOV)
  {
    fIOV = edm::ValidityInterval(edm::IOVSyncValue::beginOfTime(), edm::IOVSyncValue::endOfTime()); // anytime
    // Added by Sasha (begin)
    std::cout << " MetCorrectionESSource fIOV" << std::endl;
    // Added by Sasha (end)
  }
};
#endif
