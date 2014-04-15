// location JetMETCorrections/Modules/interface

#ifndef METCorrectionProducer_h
#define METCorrectionProducer_h

#include <sstream>
#include <string>
#include <vector>
#include <TFormula.h>
#include <TString.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "CommonTools/Utils/interface/PtComparator.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/METCorrector.h"

#include "DataFormats/METReco/interface/CorrMETData.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/METReco/interface/MET.h"

namespace edm 
{
  class ParameterSet;
}

//class JetCorrector;
class METCorrector;

namespace cms 
{
  template<class T>
  class METCorrectionProducer : public edm::EDProducer
  {
  public:
    //typedef std::vector<T> JetCollection;
    typedef std::vector<T> METCollection;
    explicit METCorrectionProducer (const edm::ParameterSet& fParameters);
    virtual ~METCorrectionProducer () {}
    virtual void produce(edm::Event&, const edm::EventSetup&);
  private:
    edm::InputTag mInput;
    std::vector <std::string> mCorrectorNames;
    // cache
    std::vector <const METCorrector*> mCorrectors;
    unsigned long long mCacheId;
    bool mVerbose;

    std::string moduleLabel_;

    edm::InputTag src_; // PFCandidate input collection
    edm::InputTag srcVertices_; // Vertex input collection

    TFormula* corrPx_;
    TFormula* corrPy_;
  };
}

// ---------- implementation ----------

namespace cms {
  
  template<class T>
  METCorrectionProducer<T>::METCorrectionProducer(const edm::ParameterSet& fConfig)
    : mInput(fConfig.getParameter <edm::InputTag> ("src"))
    , mCorrectorNames(fConfig.getParameter<std::vector<std::string> >("correctors"))
    , mCorrectors(mCorrectorNames.size(), 0)
    , mCacheId (0)
    , mVerbose (fConfig.getUntrackedParameter <bool> ("verbose", false)) 
    , srcVertices_(fConfig.getParameter<edm::InputTag>("srcVertices"))
  {
    /*
    std::string alias = fConfig.getUntrackedParameter <std::string> ("alias", "");
    if (alias.empty ()){
      //produces <JetCollection>();
      produces <METCollection>();
      // Added by sasha (begin)
      std::cout << " Hello from METCorrectionProducer<T> " << std::endl;
      // Added by saasha (end)
      } 
    else 
      produces <JetCollection>().setBranchAlias(alias);
     */
      //produces<METCollection>();
      produces<CorrMETData>();
  }
  
  template<class T>
  void METCorrectionProducer<T>::produce(edm::Event& fEvent,
					 const edm::EventSetup& fSetup) 
  {
   
    // look for correctors
    //const JetCorrectionsRecord& record = fSetup.get <JetCorrectionsRecord> ();
    const METCorrectionsRecord& record = fSetup.get <METCorrectionsRecord> ();
    // Added by Sasha (begin) 
    std::cout << " METCorrectionProducer::Hello from the producer " << std:: endl;
    // Added by Sasha (end)
    if (record.cacheIdentifier() != mCacheId) 
      { // need to renew cache
        // Added by Sasha (begin)
        std::cout << "METCorrectionProducer mCorrectorNames Size= " << mCorrectorNames.size() << std::endl; 
        // Added by Sasha (end)
        
	for (unsigned i = 0; i < mCorrectorNames.size(); i++) 
	  {
	    edm::ESHandle <METCorrector> handle;
            //edm::ESHandle <JetCorrector> handle;
	    record.get (mCorrectorNames [i], handle);
	    mCorrectors [i] = &*handle;
	  }
        
	mCacheId = record.cacheIdentifier();
      }
    std::vector<double> xyShiftTransmitted = mCorrectors [0]->correction();
    
    std::stringstream formulaForPx;
    std::stringstream a_x;
    std::stringstream b_x;
    if(xyShiftTransmitted[0]>=0){a_x << "+" << xyShiftTransmitted[0];}
    else { a_x << xyShiftTransmitted[0]; }
    if(xyShiftTransmitted[1]>=0){b_x << " + " << xyShiftTransmitted[1];}
    else { b_x << " " << xyShiftTransmitted[1]; }
    formulaForPx << a_x.str().c_str() << " " <<  b_x.str().c_str() << "*Nvtx";
    TString corrPxFormula(formulaForPx.str().c_str()); 

    std::stringstream formulaForPy;
    std::stringstream a_y;
    std::stringstream b_y;
    if(xyShiftTransmitted[2]>=0){a_y << "+" << xyShiftTransmitted[2];}
    else { a_y << xyShiftTransmitted[2]; }
    if(xyShiftTransmitted[3]>=0){b_y << " + " << xyShiftTransmitted[3];}
    else { b_y << " " << xyShiftTransmitted[1]; }
    formulaForPy << a_y.str().c_str() << " " <<  b_y.str().c_str() << "*Nvtx";
    TString corrPyFormula(formulaForPy.str().c_str()); 

    std::cout << corrPxFormula << std::endl;
    std::cout << corrPyFormula << std::endl;

    corrPxFormula.ReplaceAll("sumEt", "x");
     corrPxFormula.ReplaceAll("Nvtx", "y");
     std::string corrPxName = std::string(moduleLabel_).append("_corrPx");
     corrPx_ = new TFormula(corrPxName.data(), corrPxFormula.Data());
     //corrPx_->Print();
     
     corrPyFormula.ReplaceAll("sumEt", "x");
     corrPyFormula.ReplaceAll("Nvtx", "y");
     std::string corrPyName = std::string(moduleLabel_).append("_corrPy");
     corrPy_ = new TFormula(corrPyName.data(), corrPyFormula.Data());     

    typedef edm::View<T> METView;
    edm::Handle<METView> met;
    fEvent.getByLabel(mInput, met);
  
  if ( met->size() != 1 ) 
    throw cms::Exception("SysShiftMETcorrInputProducer::produce") 
      << "Failed to find unique MET object !!\n";

  double sumEt = met->front().sumEt();
  std::cout << " sumEt = " << sumEt << std::endl;

  size_t Nvtx = 0;
  if ( srcVertices_.label() != "" ) {
    edm::Handle<reco::VertexCollection> vertices;
    fEvent.getByLabel(srcVertices_, vertices);
    Nvtx = vertices->size();
  }
  std::cout << " Nvtx = " << Nvtx << std::endl;

  std::auto_ptr<CorrMETData> metCorr(new CorrMETData());
  metCorr->mex = -corrPx_->Eval(sumEt, Nvtx);
  metCorr->mey = -corrPy_->Eval(sumEt, Nvtx);
  std::cout << "--> metCorr: Px = " << metCorr->mex << ", Py = " << metCorr->mey << std::endl;
  
  fEvent.put(metCorr);
  }
  
}

#endif
