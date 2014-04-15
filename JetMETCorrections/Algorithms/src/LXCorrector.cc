// Implementation of class LXXXCorrector.
// Generic LX jet corrector class.

#include "JetMETCorrections/Algorithms/interface/LXCorrector.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
#include "CondFormats/JetMETObjects/interface/SimpleMETCorrector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"


using namespace std;


//------------------------------------------------------------------------ 
//--- LXXXCorrector constructor ------------------------------------------
//------------------------------------------------------------------------
//LXCorrector::LXCorrector(const JetCorrectorParameters& fParam, const edm::ParameterSet& fConfig)
LXCorrector::LXCorrector(const METCorrectorParameters& fParam, const edm::ParameterSet& fConfig) 
{
  /*
  string level = fParam.definitions().level();
  if (level == "L2Relative")
    mLevel = 2;
  else if (level == "L3Absolute")
    mLevel = 3;  
  else if (level == "L4EMF")
    mLevel = 4;
  else if (level == "L5Flavor")
    mLevel = 5;
  else if (level == "L7Parton")
    mLevel = 7;
  else
    throw cms::Exception("LXCorrector")<<" unknown correction level "<<level; 
  */
  //vector<JetCorrectorParameters> vParam;
  //JetCorrectorParameters vParam;
  METCorrectorParameters vParam;
  //vector<METCorrectorParameters> vParam;
  //Added by Sasha (begin)
  //fParam.printScreen();
  //Added by Sasha (end)
  vParam = fParam;
  //mCorrector = new FactorizedJetCorrector(vParam);
  mCorrector = new SimpleMETCorrector(vParam);
  //Added by Sasha (begin)
  std::cout << " LXCorrector::LXCorrector Hello from LXCorrector " << std::endl;
  //Added by Sasha (end)
  //std::cout << "" mCorrector 
  std::vector<double> transmittedParameters = (mCorrector->parameters()).getParVec();
  for(size_t i=0; i<transmittedParameters.size(); ++i) std::cout << " " << 
     transmittedParameters[i];
  std::cout << std::endl;
  vParam.printScreen();
}
//------------------------------------------------------------------------ 
//--- LXXXCorrector destructor -------------------------------------------
//------------------------------------------------------------------------
LXCorrector::~LXCorrector() 
{
  delete mCorrector;
} 
//------------------------------------------------------------------------ 
//--- Returns correction for a given 4-vector ----------------------------
//------------------------------------------------------------------------

double LXCorrector::correction(const LorentzVector& fJet) const 
{
/*
  // Added by Sasha (begin) 
  //std::cout << " Hi from LXXXCorrector::correction(const LorentzVector& fJet) const " << std::endl;
  // Added by Sasha (end)
  // L4 correction requires more information than a simple 4-vector
  if (mLevel == 4) {
    throw cms::Exception("Invalid jet type") << "L4EMFCorrection is applicable to CaloJets only";
    return 1;
  }
  else {
      mCorrector->setJetEta(fJet.eta()); 
      // Added by Sasha (begin)
      //std::cout << " setJetE= " << fJet.eta() << std::endl; 
      // Added by Sasha (end)
      mCorrector->setJetE(fJet.energy());
      mCorrector->setJetPt(fJet.pt());
      mCorrector->setJetPhi(fJet.phi());
  } 
  return mCorrector->getCorrection();
  */
  return 60;
}

//------------------------------------------------------------------------ 
//--- Returns correction for a given jet ---------------------------------
//------------------------------------------------------------------------

double LXCorrector::correction(const reco::Jet& fJet) const 
{
  /*
  // Added by Sasha (begin) 
  //std::cout << " Hi from LXXXCorrector::correction const(const reco::Jet& fJet) " << std::endl;
  // Added by Sasha (end)
  double result = 1.;
  // L4 correction applies to Calojets only
  if (mLevel == 4) {
      const reco::CaloJet& caloJet = dynamic_cast <const reco::CaloJet&> (fJet);
      mCorrector->setJetEta(fJet.eta()); 
      mCorrector->setJetPt(fJet.pt());
      mCorrector->setJetEMF(caloJet.emEnergyFraction());
      result = mCorrector->getCorrection();
  }
  else
    result = correction(fJet.p4());
  return result;
  */
   return 60;
}

std::vector <double> LXCorrector::correction() const
{
   return (mCorrector->parameters()).getParVec();
}

