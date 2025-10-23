#include "MCObjs/MCPhotonHit.hh"

ClassImp(MCPhotonHit)

MCPhotonHit::MCPhotonHit()
  : TObject()
{
  fTime = 0;
  fKE = 0;
  fTrackId = -1;        // 추가
  fParentTrackId = -1;  // 추가
}

MCPhotonHit::MCPhotonHit(const MCPhotonHit & photon)
  : TObject()
{
  fTime = photon.GetTime();
  fKE = photon.GetKineticEnergy();
  fTrackId = photon.GetTrackId();              // 추가
  fParentTrackId = photon.GetParentTrackId();  // 추가
}

MCPhotonHit::~MCPhotonHit() {}


int MCPhotonHit::Compare(const TObject * object) const
{
  auto comp = (MCPhotonHit*)object;
  if (this->GetTime() < comp->GetTime()) return 1;
  else if (this->GetTime() > comp->GetTime()) return -1;

  return 0;
}
