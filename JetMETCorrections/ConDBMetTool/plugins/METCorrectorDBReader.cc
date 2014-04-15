// location JetMETCorrections/Modules/plugins

// -*- C++ -*-
//
// Package:    METCorrectorDBReader
// Class:      
// 
/**\class METCorrectorDBReader

 Description: Reads out *.db format for MET corrections 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author: Alexandre Sakharov 
//         Created: December 8th 2013
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/METCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/METCorrectionsRecord.h"


//
// class declaration
//

class METCorrectorDBReader : public edm::EDAnalyzer {
public:
  explicit METCorrectorDBReader(const edm::ParameterSet&);
  ~METCorrectorDBReader();
  
  
private:
  virtual void beginJob() override ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override ;
 
  std::string mPayloadName,mGlobalTag;
  bool mCreateTextFile,mPrintScreen;
};


METCorrectorDBReader::METCorrectorDBReader(const edm::ParameterSet& iConfig)
{
  mPayloadName    = iConfig.getUntrackedParameter<std::string>("payloadName");
  mGlobalTag      = iConfig.getUntrackedParameter<std::string>("globalTag");  
  mPrintScreen    = iConfig.getUntrackedParameter<bool>("printScreen");
  mCreateTextFile = iConfig.getUntrackedParameter<bool>("createTextFile");
}


METCorrectorDBReader::~METCorrectorDBReader()
{
 
}

void METCorrectorDBReader::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::ESHandle<METCorrectorParameters> METCorParams;
  std::cout <<"Inspecting JEC payload with label: "<< mPayloadName <<std::endl;
  iSetup.get<METCorrectionsRecord>().get(mPayloadName,METCorParams);
  /*
  std::vector<JetCorrectorParametersCollection::key_type> keys;
  JetCorParamsColl->validKeys( keys );
  for ( std::vector<JetCorrectorParametersCollection::key_type>::const_iterator ibegin = keys.begin(),
	  iend = keys.end(), ikey = ibegin; ikey != iend; ++ikey ) {
    std::cout<<"-------------------------------------------------" << std::endl;
    std::cout<<"Processing key = " << *ikey << std::endl;
    std::cout<<"object label: "<<JetCorParamsColl->findLabel(*ikey)<<std::endl;
    JetCorrectorParameters const & JetCorParams = (*JetCorParamsColl)[*ikey];

    if (mCreateTextFile)
      {
	std::cout<<"Creating txt file: "<<mGlobalTag+"_"+mPayloadName+"_"+JetCorParamsColl->findLabel(*ikey)+".txt"<<std::endl;
	JetCorParams.printFile(mGlobalTag+"_"+JetCorParamsColl->findLabel(*ikey)+"_"+mPayloadName+".txt");
      }
    
    if (mPrintScreen)
      JetCorParams.printScreen();
  }
  */
  METCorrectorParameters const & METCorParams_extracted = (*METCorParams);
  METCorParams_extracted.printScreen();
}

void 
METCorrectorDBReader::beginJob()
{
}

void 
METCorrectorDBReader::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(METCorrectorDBReader);
