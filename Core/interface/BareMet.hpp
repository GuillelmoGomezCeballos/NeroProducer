#ifndef BARE_MET_H
#define BARE_MET_H

#include "NeroProducer/Core/interface/BareCollection.hpp"
#include "NeroProducer/Core/interface/BareP4.hpp"


class BareMet : virtual public BareP4
{
    public:
        BareMet();
        ~BareMet();
        void clear();
        void defineBranches(TTree *t);
        virtual void setBranchAddresses(TTree*) ;
        virtual inline string name(){return "BareMet";};
        virtual void compress();

        // -- variables
        //TClonesArray *p4;
        vector<float> *ptJESUP;
        vector<float> *ptJESDOWN;

        // Should not stay here, but in MC -> we will esculed it if run on onlyMc
        TClonesArray *genP4;

        TLorentzVector *metNoMu;
        TLorentzVector *pfMet_e3p0;
        TLorentzVector *metChargedHadron;
        TLorentzVector *metNeutralHadron;
        TLorentzVector *metNeutralEM;
  
        float caloMet_Pt;
        float caloMet_Phi;
        float caloMet_SumEt;
};
#endif
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
