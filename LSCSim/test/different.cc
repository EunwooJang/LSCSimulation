#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <cmath>

#include "G4RunManager.hh"
#include "G4String.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include "TRandom3.h"

#include "CLHEP/Random/MTwistEngine.h"

#include "GLG4Sim/GLG4PrimaryGeneratorAction.hh"
#include "LSCSim/LSCDetectorConstruction.hh"
#include "LSCSim/LSCPhysicsList.hh"
#include "LSCSim/LSCRootManager.hh"
#include "LSCSim/LSCEventAction.hh"
#include "LSCSim/LSCRunAction.hh"
#include "LSCSim/LSCSteppingAction.hh"
#include "LSCSim/LSCTrackingAction.hh"

using namespace std;

// Cylindrical → Cartesian 변환
tuple<double,double,double> Cyl2Cart(double r, double H, double thetaDeg) {
    double theta = thetaDeg * M_PI / 180.0;
    double x = r * cos(theta);
    double y = r * sin(theta);
    double z = H;
    return {x,y,z};
}

void PrintHelp()
{
  cout << endl;
  cout << "Usage: LSCSim [-n # of event] [-o output] [-f macro]" << endl
       << "              [-g geometry] [-p pmt_position data] [-m material] "
          "[-r bin.dat] [-v macro]"
       << endl;
  cout << endl;
  exit(0);
}

int main(int argc, char ** argv)
{
  if (argc < 2) {
    PrintHelp();
    return 0;
  }

  int opt;
  int nevent = 1;
  int doVis = 0;
  G4String outputFileName;
  G4String macroFileName;
  G4String materialData;
  G4String geometryData;
  G4String pmtposData;
  G4String binFileName;
  G4String detectorType = "LSCS";

  while ((opt = getopt(argc, argv, "o:f:m:g:p:n:r:vhcst")) != -1) {
    switch (opt) {
      case 'o': outputFileName = G4String(optarg); break;
      case 'f': macroFileName = G4String(optarg); break;
      case 'm': materialData = G4String(optarg); break;
      case 'g': geometryData = G4String(optarg); break;
      case 'p': pmtposData = G4String(optarg); break;
      case 'n': nevent = atoi(G4String(optarg).data()); break;
      case 'r': binFileName = G4String(optarg); break;
      case 'v': doVis = 1; break;
      case 'c': detectorType = "LSCC"; break;
      case 's': detectorType = "LSCS"; break;
      case 't': detectorType = "PROTO"; break;
      case 'h': PrintHelp(); break;
      default: PrintHelp();
    }
  }

  if (!doVis && nevent == 0) {
    G4cout << "LSCSim main: Number of event is not set !" << G4endl;
    PrintHelp();
  }
  if (!doVis && macroFileName.empty()) {
    G4cout << "LSCSim main: No macro file !" << G4endl;
    PrintHelp();
  }
  if (!doVis && binFileName.empty()) {
    G4cout << "LSCSim main: No bin.dat file !" << G4endl;
    PrintHelp();
  }

  // random seed
  gRandom->SetSeed(0);
  int seed = gRandom->Integer(kMaxInt);

  CLHEP::MTwistEngine randomEngine;
  G4Random::setTheEngine(&randomEngine);
  G4Random::getTheEngine()->setSeed(seed, 0);

  // RunManager 및 Detector 초기화
  G4RunManager * runManager = new G4RunManager;

  LSCDetectorConstruction * LSCDetector = new LSCDetectorConstruction();
  LSCDetector->SetDetectorType(detectorType);
  if (!geometryData.empty()) LSCDetector->SetGeometryDataFile(geometryData);
  if (!pmtposData.empty()) LSCDetector->SetPMTPositionDataFile(pmtposData);
  if (!materialData.empty()) LSCDetector->SetMaterialDataFile(materialData);

  runManager->SetUserInitialization(LSCDetector);
  runManager->SetUserInitialization(new LSCPhysicsList);

  if (outputFileName.empty()) outputFileName = "simout.root";

  LSCRootManager * rootManager = new LSCRootManager();

  runManager->SetUserAction(new GLG4PrimaryGeneratorAction);
  runManager->SetUserAction(new LSCRunAction(rootManager));
  runManager->SetUserAction(new LSCEventAction(rootManager));
  runManager->SetUserAction(new LSCTrackingAction(rootManager));
  runManager->SetUserAction(new LSCSteppingAction(rootManager));

  G4UImanager * UImanager = G4UImanager::GetUIpointer();

 if (!doVis) {

    // 매크로 실행 (beamOn은 루프 안에서 실행)
    G4String command = Form("/control/execute %s", macroFileName.c_str());
    UImanager->ApplyCommand(command);
    
    // === 입력 파일명에서 경로/확장자 분리 ===
    std::string baseName = outputFileName.substr(outputFileName.find_last_of("/\\") + 1);
    std::string prefixDir = outputFileName.substr(0, outputFileName.find_last_of("/\\"));
    if (baseName.size() > 5 && baseName.substr(baseName.size()-5) == ".root") {
        baseName = baseName.substr(0, baseName.size()-5);
    }

    // === bin.dat 읽기 ===
    int r_start, r_end, r_bin;
    int H_start, H_end, H_bin;
    int theta_start, theta_end, theta_bin;

    ifstream fin(binFileName.c_str());
    if(!fin.is_open()) {
        cerr << "Error: cannot open " << binFileName << endl;
        return 1;
    }
    fin >> r_start >> r_end >> r_bin
        >> H_start >> H_end >> H_bin
        >> theta_start >> theta_end >> theta_bin;
    fin.close();

    // === 위치 루프 실행 ===
    int idx = 0;
    for (int r = r_start; r <= r_end; r += r_bin) {
        for (int H = H_start; H <= H_end; H += H_bin) {
            int thetaMax = (r==0 ? 0 : theta_end);
            for (int theta = theta_start; theta <= thetaMax; theta += theta_bin) {
                auto [x,y,z] = Cyl2Cart(r,H,theta);
                G4String posCmd = Form("/generator/pos/set 9 \"%f %f %f\"",x,y,z);
                UImanager->ApplyCommand(posCmd);

                // === 파일명 파싱 및 r,theta,H 교체 ===
                std::vector<std::string> tokens;
                {
                    std::stringstream ss(baseName);
                    std::string tmp;
                    while (std::getline(ss, tmp, '_')) {
                        tokens.push_back(tmp);
                    }
                }

                if (tokens.size() >= 6) {
                    tokens[3] = std::to_string(r);      // r
                    tokens[4] = std::to_string(theta);  // theta
                    tokens[5] = std::to_string(H);      // H
                }

                std::ostringstream oss;
                for (size_t i=0; i<tokens.size(); i++) {
                    if (i > 0) oss << "_";
                    oss << tokens[i];
                }
                oss << ".root";

                std::string outname = prefixDir + "/" + oss.str();
                rootManager->SetRootFile(outname.c_str());

                // 이벤트 실행
                G4String runCmd = Form("/run/beamOn %d", nevent);
                UImanager->ApplyCommand(runCmd);

                idx++;
            }
        }
    }
    G4cout << "Total runs: " << idx << G4endl;
}

}

