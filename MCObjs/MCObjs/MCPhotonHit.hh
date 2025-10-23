#ifndef MCPhotonHit_hh
#define MCPhotonHit_hh

#include "TObject.h"

class MCPhotonHit : public TObject {
public:
  MCPhotonHit();
  MCPhotonHit(const MCPhotonHit & photon);
  ~MCPhotonHit();

  void SetTime(float t);
  void SetKineticEnergy(float KE);

  float GetTime() const;
  float GetKineticEnergy() const;
  float GetWavelength() const;

  virtual bool IsSortable() const { return true; }
  virtual int Compare(const TObject * object) const;

  void SetTrackId(int id);
  void SetParentTrackId(int id);

  int GetTrackId() const;
  int GetParentTrackId() const;

private:
  float fTime;
  float fKE;
  int fTrackId;        // 추가: optical photon의 track ID
  int fParentTrackId;  // 추가: optical photon을 생성한 부모 track ID

  ClassDef(MCPhotonHit, 1)
};

inline void MCPhotonHit::SetTime(float t) { fTime = t; }
inline void MCPhotonHit::SetKineticEnergy(float KE) { fKE = KE; }
inline float MCPhotonHit::GetTime() const { return fTime; }
inline float MCPhotonHit::GetKineticEnergy() const { return fKE; }
inline float MCPhotonHit::GetWavelength() const { return 1239.83968E-6 / fKE; }

inline void MCPhotonHit::SetTrackId(int id) { fTrackId = id; }
inline void MCPhotonHit::SetParentTrackId(int id) { fParentTrackId = id; }
inline int MCPhotonHit::GetTrackId() const { return fTrackId; }
inline int MCPhotonHit::GetParentTrackId() const { return fParentTrackId; }

#endif
