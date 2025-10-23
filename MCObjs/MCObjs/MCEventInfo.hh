#ifndef MCEventInfo_hh
#define MCEventInfo_hh

#include "TObject.h"

class MCEventInfo : public TObject {
public:
  MCEventInfo();
  MCEventInfo(const MCEventInfo & info);
  virtual ~MCEventInfo();

  void SetEventNumber(unsigned int n);
  unsigned int GetEventNumber() const;

  virtual void Print(const Option_t * opt = "");

  // 이벤트 간 시간 간격 추가
  void SetTimeSincePriorEvent(double t);
  double GetTimeSincePriorEvent() const;

  // 절대 시간도 추가 (옵션)
  void SetUniversalTime(double t);
  double GetUniversalTime() const;


private:
  unsigned int fEventNumber;
  double fTimeSincePriorEvent;  // 이전 이벤트로부터의 시간 간격 [ns]
  double fUniversalTime;        // 절대 시간 [ns]

  ClassDef(MCEventInfo, 1)
};

inline void MCEventInfo::SetEventNumber(unsigned int n) { fEventNumber = n; }
inline unsigned int MCEventInfo::GetEventNumber() const { return fEventNumber; }

inline void MCEventInfo::SetTimeSincePriorEvent(double t) { fTimeSincePriorEvent = t; }
inline double MCEventInfo::GetTimeSincePriorEvent() const { return fTimeSincePriorEvent; }

inline void MCEventInfo::SetUniversalTime(double t) { fUniversalTime = t; }
inline double MCEventInfo::GetUniversalTime() const { return fUniversalTime; }

#endif
