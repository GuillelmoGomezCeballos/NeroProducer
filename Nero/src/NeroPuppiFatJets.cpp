#include "NeroProducer/Nero/interface/NeroPuppiFatJets.hpp"
#include "NeroProducer/Nero/interface/NeroPuppiJets.hpp" // JetId
#include "NeroProducer/Nero/interface/Nero.hpp"

//JES


NeroPuppiFatJets::NeroPuppiFatJets() : 
        NeroCollection(),
        BarePuppiFatJets()
{
}

NeroPuppiFatJets::~NeroPuppiFatJets(){
}


int NeroPuppiFatJets::analyze(const edm::Event& iEvent){

    if ( mOnlyMc  ) return 0;

    if ( mMinId == "none" ) return 0;

    // maybe handle should be taken before
    iEvent.getByToken(token, handle);

    edm::Handle<reco::JetTagCollection> pfBoostedDoubleSecondaryVertex;  //HBB 74X
    iEvent.getByLabel("pfBoostedDoubleSecondaryVertexAK8BJetTags",pfBoostedDoubleSecondaryVertex); //HBB 74X

    if ( not handle.isValid() ) cout<<"[NeroPuppiFatJets]::[analyze]::[ERROR] handle is not valid"<<endl;
    if ( not pfBoostedDoubleSecondaryVertex.isValid() )  cout<<"[NeroPuppiFatJets]::[analyze]::[ERROR] pfBoosted.. handle is not valid"<<endl;

    int ijetRef = -1;
    int nsubjet = 0;
    for (const pat::Jet& j : *handle)
    {
        ijetRef++;
        if (j.pt() < mMinPt ) continue;
	if (fabs(j.eta() ) > mMaxEta)  continue;

        // JET ID
        if ( !NeroPuppiJets::JetId(j,mMinId) ) continue;

        // GET  ValueMaps

        // Fill output object   
        new ( (*p4)[p4->GetEntriesFast()]) TLorentzVector(j.px(), j.py(), j.pz(), j.energy());

        rawPt -> push_back (j.pt()*j.jecFactor("Uncorrected"));
        flavour -> push_back( j.partonFlavour() );
        tau1 -> push_back(j.userFloat("NjettinessAK8Puppi:tau1"));
        tau2 -> push_back(j.userFloat("NjettinessAK8Puppi:tau2"));
        tau3 -> push_back(j.userFloat("NjettinessAK8Puppi:tau3"));

        trimmedMass ->push_back(j.userFloat("ak8PuppiJetsTrimmedMass"));
        prunedMass  ->push_back(j.userFloat("ak8PuppiJetsPrunedMass"));
        filteredMass->push_back(j.userFloat("ak8PuppiJetsFilteredMass"));
        softdropMass->push_back(j.userFloat("ak8PuppiJetsSoftDropMass"));
        ak8_hasSubjet->push_back(j.hasSubjets("SoftDrop"));

        // --float hbb= j.bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags"); // HBB 75X
        // --cout <<"Hbb tagger="<<hbb<<endl;
        // --if(hbb>10) cout<<endl;
        //float hbb =  (*pfBoostedDoubleSecondaryVertex).value(ijetRef) ;//HBB 74X
        hbb -> push_back( (*pfBoostedDoubleSecondaryVertex).value(ijetRef) ) ;

        auto Subjets = j.subjets("SoftDrop");
        for ( auto const & i : Subjets ) {
            new ( (*ak8_subjet)[nsubjet]) TLorentzVector(i->px(), i->py(), i->pz(), i->energy());
            ak8subjet_btag->push_back(i->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags"));
            nsubjet++;
        }

    }
    return 0;
}
