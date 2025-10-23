#include <iostream>
#include "TString.h"
#include "MCObjs/MCEventInfo.hh"

using namespace std;

ClassImp(MCEventInfo)

MCEventInfo::MCEventInfo()
  : TObject()
{
  fEventNumber = 0;
  fTimeSincePriorEvent = 0.0;
  fUniversalTime = 0.0;
}

MCEventInfo::MCEventInfo(const MCEventInfo & info)
  : TObject()
{
  fEventNumber = info.GetEventNumber();
  fTimeSincePriorEvent = info.GetTimeSincePriorEvent();
  fUniversalTime = info.GetUniversalTime();
}

MCEventInfo::~MCEventInfo() {}

void MCEventInfo::Print(const Option_t * opt)
{

  cout << Form("++++++++++++++++ Event No: %d ++++++++++++++++++", fEventNumber) << endl;
  cout << Form("  Universal Time: %.6e ns", fUniversalTime) << endl;
  cout << Form("  Time Since Prior Event: %.6e ns (%.6e s)",
               fTimeSincePriorEvent, fTimeSincePriorEvent/1e9) << endl;
}
