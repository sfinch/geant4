//
//
// $Id: DetectorMessenger.hh,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConstruction* Detector;
    
    G4UIdirectory*             HPGeDir;
    G4UIdirectory*             detDir;
	//materials
    G4UIcmdWithAString*        CrystalMaterCmd;
    G4UIcmdWithAString*        WindowMaterCmd;
    G4UIcmdWithAString*        WallMaterCmd;
	//Crystal Dimensions
    G4UIcmdWithADoubleAndUnit* CrystalHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* CrystalRadCmd;
    G4UIcmdWithADoubleAndUnit* CrystalEndRadCmd;
    G4UIcmdWithADoubleAndUnit* HoleRadCmd;
    G4UIcmdWithADoubleAndUnit* HoleDepthCmd;
    G4UIcmdWithADoubleAndUnit* DeadLayerCmd;
	//Shell dimensions
    G4UIcmdWithADoubleAndUnit* ShellHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* EndGapCmd;
    G4UIcmdWithADoubleAndUnit* WindowThickCmd;
    G4UIcmdWithADoubleAndUnit* WallThickCmd;
    G4UIcmdWithADoubleAndUnit* DetectorDistanceCmd;
	//update
    G4UIcmdWithoutParameter*   UpdateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

