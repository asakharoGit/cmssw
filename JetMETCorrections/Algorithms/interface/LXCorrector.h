// Location /JetMETCorrections/Algorithms/interface
// Generic LX (to be turned in LXMET) jet corrector class. Inherits from JetCorrector.h
#ifndef LXCorrector_h
#define LXCorrector_h

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/METCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/METCorrector.h"
#include "CondFormats/JetMETObjects/interface/SimpleMETCorrector.h"

//----- classes declaration -----------------------------------
namespace edm 
{
  class ParameterSet;
}
class FactorizedJetCorrector;
class SimpleMETCorrector;
//----- LXCorrector interface -------------------------------
class LXCorrector : public METCorrector 
{
  public:
    //----- constructors---------------------------------------
    //LXCorrector(const JetCorrectorParameters& fConfig, const edm::ParameterSet& fParameters); 
    LXCorrector(const METCorrectorParameters& fConfig, const edm::ParameterSet& fParameters);   

    //----- destructor ----------------------------------------
    virtual ~LXCorrector();

    //----- apply correction using Jet information only -------
    virtual double correction(const LorentzVector& fJet) const;

    //----- apply correction using Jet information only -------
    virtual double correction(const reco::Jet& fJet) const;

    //----- apply corrections transmitting parameters
    virtual std::vector<double> correction() const;

    //----- if correction needs event information -------------
    virtual bool eventRequired() const {return false;} 

    //----- if correction needs a jet reference -------------
    virtual bool refRequired() const { return false; }

  private:
    //----- member data ---------------------------------------
    unsigned mLevel;
    //FactorizedJetCorrector* mCorrector;
    SimpleMETCorrector* mCorrector;
};

#endif
