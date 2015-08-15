#include "NeroProducer/Bambu/interface/MetFiller.h"

#include "MitAna/DataTree/interface/PFMetCol.h"
#include "MitAna/DataTree/interface/MetCol.h"
#include "MitAna/DataTree/interface/ParticleCol.h"
#include "MitAna/DataTree/interface/PFCandidateCol.h"
#include "MitAna/DataTree/interface/EventHeader.h"
#include "MitAna/DataTree/interface/Names.h"

#include "TVector2.h"
#include "TMath.h"

ClassImp(mithep::nero::MetFiller)

void
mithep::nero::MetFiller::fill()
{
  auto* metCol = getSource<mithep::PFMetCol>(metName_, false);
  mithep::PFMetCol* jesUpMetCol(0);
  mithep::PFMetCol* jesDownMetCol(0);
  if (jesUpMetName_.Length() != 0) 
    jesUpMetCol = getSource<mithep::PFMetCol>(jesUpMetName_, false);
  if (jesDownMetName_.Length() != 0)
    jesDownMetCol = getSource<mithep::PFMetCol>(jesDownMetName_, false);

  auto& met(*metCol->At(0));

  newP4(out_, met);

  if (jesUpMetCol)
    out_.ptJESUP->push_back(jesUpMetCol->At(0)->Pt());
  if (jesDownMetCol)
    out_.ptJESDOWN->push_back(jesDownMetCol->At(0)->Pt());

  out_.metNoMu->SetXYZT(met.Px(), met.Py(), met.Pz(), met.E());
  auto* muons = getSource<mithep::ParticleCol>(muonsName_);
  for (unsigned iM(0); iM != muons->GetEntries(); ++iM) {
    auto& muon(*muons->At(iM));
    *out_.metNoMu += TLorentzVector(muon.Px(), muon.Py(), muon.Pz(), muon.E());
  }

  *out_.pfMet_e3p0 *= 0.;
  *out_.metChargedHadron *= 0.;
  *out_.metNeutralHadron *= 0.;
  *out_.metNeutralEM *= 0.;
  auto* pfCandidates = getSource<mithep::PFCandidateCol>(pfCandidatesName_);
  for (unsigned iP(0); iP != pfCandidates->GetEntries(); ++iP) {
    auto& cand(*pfCandidates->At(iP));

    if (cand.AbsEta() < 3.)
      *out_.pfMet_e3p0 -= TLorentzVector(cand.Px(), cand.Py(), cand.Pz(), cand.E());

    switch (cand.PFType()) {
    case mithep::PFCandidate::eHadron:
      *out_.metChargedHadron -= TLorentzVector(cand.Px(), cand.Py(), cand.Pz(), cand.E());
      break;
    case mithep::PFCandidate::eNeutralHadron:
    case mithep::PFCandidate::eHadronHF:
      *out_.metNeutralHadron -= TLorentzVector(cand.Px(), cand.Py(), cand.Pz(), cand.E());
      break;
    case mithep::PFCandidate::eGamma:
    case mithep::PFCandidate::eEGammaHF:
      *out_.metNeutralEM -= TLorentzVector(cand.Px(), cand.Py(), cand.Pz(), cand.E());
      break;
    default:
      break;
    }
  }

  if (genMetName_ != "" && getSource<mithep::EventHeader>(Names::gkEvtHeaderBrn)->IsMC()) {
    auto* genMetCol = getSource<mithep::MetCol>(genMetName_);
    auto& genMet(*genMetCol->At(0));

    newP4(*out_.genP4, genMet);
  }
}
