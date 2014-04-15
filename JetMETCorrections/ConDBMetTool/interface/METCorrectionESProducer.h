// location JetMETCorrections/Modules/interface

#ifndef METCorrectionESProducer_h
#define METCorrectionESProducer_h

//
// Original Author:  Alexandre Sakharov
//         Created:  Dec. 10, 2013
//

#include <string>
#include "boost/shared_ptr.hpp"

#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "JetMETCorrections/Objects/interface/METCorrectionsRecord.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/JetMETObjects/interface/METCorrectorParameters.h"

class METCorrector;

#define DEFINE_MET_CORRECTION_ESPRODUCER(corrector_, name_ ) \
typedef METCorrectionESProducer <corrector_>  name_; \
DEFINE_FWK_EVENTSETUP_MODULE(name_)

template <class Corrector>
class METCorrectionESProducer : public edm::ESProducer
{
private:
  edm::ParameterSet mParameterSet;
  std::string mLevel;
  std::string mAlgo;

public:
  METCorrectionESProducer(edm::ParameterSet const& fConfig) : mParameterSet(fConfig) 
  {
    std::string label = fConfig.getParameter<std::string>("@module_label"); 
    mLevel            = fConfig.getParameter<std::string>("level");
    mAlgo             = fConfig.getParameter<std::string>("algorithm");
        
    setWhatProduced(this, label);
  }

  ~METCorrectionESProducer() {}

  boost::shared_ptr<METCorrector> produce(METCorrectionsRecord const& iRecord) 
  {
    //edm::ESHandle<JetCorrectorParametersCollection> JetCorParColl;
    edm::ESHandle<METCorrectorParameters> METCorPar0;
    iRecord.get(mAlgo,METCorPar0); 
    METCorrectorParameters const& METCorPar = (*METCorPar0);
    boost::shared_ptr<METCorrector> mCorrector(new Corrector(METCorPar, mParameterSet));
    return mCorrector;
  }
};
#endif
