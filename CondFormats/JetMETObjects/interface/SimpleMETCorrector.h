// Location CondFormats/JetMETObjects/interface

#ifndef SimpleMETCorrector_h
#define SimpleMETCorrector_h

#include <string>
#include <vector>

#include <TFormula.h>


class JetCorrectorParameters;
class METCorrectorParameters;

class SimpleMETCorrector 
{
 public:
  //-------- Constructors --------------
  SimpleMETCorrector();
  //SimpleMETCorrector(const std::string& fDataFile, const std::string& fOption = "");
  //SimpleMETCorrector(const JetCorrectorParameters& fParameters);
  SimpleMETCorrector(const METCorrectorParameters& fParameters);
  //-------- Destructor -----------------
  ~SimpleMETCorrector();
  //-------- Member functions -----------
  void   setInterpolation(bool fInterpolation) {mDoInterpolation = fInterpolation;}
  //float  correction(const std::vector<float>& fX,const std::vector<float>& fY) const;
  //std::vector<double> correction() const;  
  //const  JetCorrectorParameters& parameters() const {return *mParameters;}
  const  METCorrectorParameters& parameters() const {return *mParameters;} 

 private:
  //-------- Member functions -----------
  SimpleMETCorrector(const SimpleMETCorrector&);
  SimpleMETCorrector& operator= (const SimpleMETCorrector&);
  float    invert(const std::vector<float>& fX) const;
  //float    correctionBin(unsigned fBin,const std::vector<float>& fY) const;
  //unsigned findInvertVar();
  //-------- Member variables -----------
  bool                    mDoInterpolation;
  unsigned                mInvertVar; 
  TFormula*               mFunc;
  //JetCorrectorParameters* mParameters;
  METCorrectorParameters* mParameters;
};

#endif


