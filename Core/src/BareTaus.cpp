#include "NeroProducer/Core/interface/BareTaus.hpp"
#include "NeroProducer/Core/interface/BareFunctions.hpp"

BareTaus::BareTaus(): BareP4() {
}

BareTaus::~BareTaus(){
    BareFunctions::Delete(selBits);
    BareFunctions::Delete(Q);
    BareFunctions::Delete(M);
    BareFunctions::Delete(iso);
    BareFunctions::Delete(chargedIsoPtSum);
    BareFunctions::Delete(neutralIsoPtSum);
    BareFunctions::Delete(isoDeltaBetaCorr);
    BareFunctions::Delete(isoPileupWeightedRaw);
}

void BareTaus::init(){
    BareP4::init();

    BareFunctions::New(selBits);
    BareFunctions::New(Q);
    BareFunctions::New(M);
    BareFunctions::New(iso);


    if ( IsExtend() )
    {
        BareFunctions::New(chargedIsoPtSum);
        BareFunctions::New(neutralIsoPtSum);
        BareFunctions::New(isoDeltaBetaCorr);
        BareFunctions::New(isoPileupWeightedRaw);
    }
}

void BareTaus::clear(){
    BareP4::clear();
    selBits->clear();
    Q->clear();
    M->clear();
    iso -> clear();
    if ( extend_ ) 
    {
        chargedIsoPtSum->clear();
        neutralIsoPtSum->clear() ;
        isoDeltaBetaCorr->clear();
        isoPileupWeightedRaw->clear();
    }
}

void BareTaus::defineBranches(TTree *t){
    BareP4::defineBranches(t, "tau" );
    //
    t->Branch("tauSelBits","vector<unsigned>",&selBits);
    //
    t->Branch("tauQ","vector<int>",&Q);
    //
    t->Branch("tauM","vector<float>",&M);
    //
    t->Branch("tauIso","vector<float>",&iso);

    if ( IsExtend() )
    {
        t->Branch("tauChargedIsoPtSum","vector<float>",&chargedIsoPtSum);
        t->Branch("tauNeutralIsoPtSum","vector<float>",&neutralIsoPtSum);
        t->Branch("tauIsoDeltaBetaCorr","vector<float>",&isoDeltaBetaCorr);
        t->Branch("tauIsoPileupWeightedRaw","vector<float>",&isoPileupWeightedRaw);
    }

}

void BareTaus::setBranchAddresses(TTree *t){
    BareP4::setBranchAddresses(t,"tau");

    BareFunctions::SetBranchAddress(t,"tauSelBits"	,&selBits);
    BareFunctions::SetBranchAddress(t,"tauQ"	,&Q);
    BareFunctions::SetBranchAddress(t,"tauM"	,&M);
    BareFunctions::SetBranchAddress(t,"tauIso"	,&iso);

    // EXTENDED VARIBALES
    if ( IsExtend() )
    {
        BareFunctions::SetBranchAddress(t,"tauChargedIsoPtSum",&chargedIsoPtSum);
        BareFunctions::SetBranchAddress(t,"tauNeutralIsoPtSum",&neutralIsoPtSum);
        BareFunctions::SetBranchAddress(t,"tauIsoDeltaBetaCorr",&isoDeltaBetaCorr);
        BareFunctions::SetBranchAddress(t,"tauIsoPileupWeightedRaw",&isoPileupWeightedRaw);
    }
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4
