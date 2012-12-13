//
//
// $Id: DetectorMessenger.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(
                                           DetectorConstruction* Det)
:Detector(Det)
{ 
  HPGeDir = new G4UIdirectory("/HPGe/");
  HPGeDir->SetGuidance("UI commands");
  
  detDir = new G4UIdirectory("/HPGe/det/");
  detDir->SetGuidance("detector control");
       
  //materials
  CrystalMaterCmd = new G4UIcmdWithAString("/HPGe/det/setCrystalMat",this);
  CrystalMaterCmd->SetGuidance("Select Material of the Crystal.");
  CrystalMaterCmd->SetParameterName("choice",false);
  CrystalMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  WindowMaterCmd = new G4UIcmdWithAString("/HPGe/det/setWindowMat",this);
  WindowMaterCmd->SetGuidance("Select Material of the Window.");
  WindowMaterCmd->SetParameterName("choice",false);
  WindowMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  WallMaterCmd = new G4UIcmdWithAString("/HPGe/det/setWallMat",this);
  WallMaterCmd->SetGuidance("Select Material of the side wall.");
  WallMaterCmd->SetParameterName("choice",false);
  WallMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //crystal dimensions 
  CrystalHalfLengthCmd= new G4UIcmdWithADoubleAndUnit("/HPGe/det/setCrystalHalfLength",this);
  CrystalHalfLengthCmd->SetGuidance("Set the half length of the crystal.");
  CrystalHalfLengthCmd->SetParameterName("Size",false);
  CrystalHalfLengthCmd->SetRange("Size>0.");
  CrystalHalfLengthCmd->SetUnitCategory("Length");    
  CrystalHalfLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  CrystalRadCmd= new G4UIcmdWithADoubleAndUnit("/HPGe/det/setCrystalRad",this);
  CrystalRadCmd->SetGuidance("Set the radius of the crystal.");
  CrystalRadCmd->SetParameterName("Size",false);
  CrystalRadCmd->SetRange("Size>0.");
  CrystalRadCmd->SetUnitCategory("Length");    
  CrystalRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  CrystalEndRadCmd= new G4UIcmdWithADoubleAndUnit("/HPGe/det/setCrystalEndRad",this);
  CrystalEndRadCmd->SetGuidance("Set the radius of rounding on the crystal's edge.");
  CrystalEndRadCmd->SetParameterName("Size",false);
  CrystalEndRadCmd->SetRange("Size>0.");
  CrystalEndRadCmd->SetUnitCategory("Length");    
  CrystalEndRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  HoleRadCmd= new G4UIcmdWithADoubleAndUnit("/HPGe/det/setHoleRad",this);
  HoleRadCmd->SetGuidance("Set the end radius of the hole in the crystal.");
  HoleRadCmd->SetParameterName("Size",false);
  HoleRadCmd->SetRange("Size>0.");
  HoleRadCmd->SetUnitCategory("Length");    
  HoleRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  HoleDepthCmd= new G4UIcmdWithADoubleAndUnit("/HPGe/det/setHoleDepth",this);
  HoleDepthCmd->SetGuidance("Set the depth of the hole in the crystal.");
  HoleDepthCmd->SetParameterName("Size",false);
  HoleDepthCmd->SetRange("Size>0.");
  HoleDepthCmd->SetUnitCategory("Length");    
  HoleDepthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  DeadLayerCmd= new G4UIcmdWithADoubleAndUnit("/HPGe/det/setDeadLayer",this);
  DeadLayerCmd->SetGuidance("Set the thickness of the crystal's  dead layer.");
  DeadLayerCmd->SetParameterName("Size",false);
  DeadLayerCmd->SetRange("Size>0.");
  DeadLayerCmd->SetUnitCategory("Length");    
  DeadLayerCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //Shell and window dimensions
  ShellHalfLengthCmd = new G4UIcmdWithADoubleAndUnit("/HPGe/det/setShellHalfLength",this);
  ShellHalfLengthCmd->SetGuidance("Set the half length of the ourter shell.");
  ShellHalfLengthCmd->SetParameterName("Size",false);
  ShellHalfLengthCmd->SetRange("Size>0.");
  ShellHalfLengthCmd->SetUnitCategory("Length");    
  ShellHalfLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EndGapCmd = new G4UIcmdWithADoubleAndUnit("/HPGe/det/setEndGap",this);
  EndGapCmd->SetGuidance("Set the end gap size between the crystal and window.");
  EndGapCmd->SetParameterName("Size",false);
  EndGapCmd->SetRange("Size>0.");
  EndGapCmd->SetUnitCategory("Length");    
  EndGapCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  WindowThickCmd = new G4UIcmdWithADoubleAndUnit("/HPGe/det/setWindowThick",this);
  WindowThickCmd->SetGuidance("Set Thickness of the window");
  WindowThickCmd->SetParameterName("Size",false);
  WindowThickCmd->SetRange("Size>=0.");
  WindowThickCmd->SetUnitCategory("Length");
  WindowThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  WallThickCmd = new G4UIcmdWithADoubleAndUnit("/HPGe/det/setWallThick",this);
  WallThickCmd->SetGuidance("Set Thickness of the side wall");
  WallThickCmd->SetParameterName("Size",false);
  WallThickCmd->SetRange("Size>=0.");
  WallThickCmd->SetUnitCategory("Length");  
  WallThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  DetectorDistanceCmd= new G4UIcmdWithADoubleAndUnit("/HPGe/det/setDetectorDistance",this);
  DetectorDistanceCmd->SetGuidance("Set the distance of the detector to the source.");
  DetectorDistanceCmd->SetParameterName("Size",false);
  DetectorDistanceCmd->SetRange("Size>0.");
  DetectorDistanceCmd->SetUnitCategory("Length");    
  DetectorDistanceCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //update
  UpdateCmd = new G4UIcmdWithoutParameter("/HPGe/det/update",this);
  UpdateCmd->SetGuidance("Update geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete CrystalMaterCmd; delete WindowMaterCmd; delete WallMaterCmd;
  delete CrystalHalfLengthCmd; delete CrystalRadCmd; delete CrystalEndRadCmd;
  delete HoleRadCmd; delete HoleDepthCmd;
  delete DeadLayerCmd;
  delete ShellHalfLengthCmd; delete EndGapCmd;
  delete WindowThickCmd; delete WallThickCmd;
  delete DetectorDistanceCmd; 

  delete UpdateCmd;
  delete detDir;
  delete HPGeDir;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  //materials
  if( command == CrystalMaterCmd )
   { Detector->SetCrystalMaterial(newValue);}
   
  if( command == WindowMaterCmd )
   { Detector->SetWindowMaterial(newValue);}
  
  if( command == WallMaterCmd )
   { Detector->SetWallMaterial(newValue);}
  
  //Crystal dimensions
  if( command == CrystalHalfLengthCmd )
   { Detector->SetCrystalHalfLength(CrystalHalfLengthCmd->GetNewDoubleValue(newValue));}
   
  if( command == CrystalRadCmd )
   { Detector->SetCrystalRad(CrystalRadCmd->GetNewDoubleValue(newValue));}
   
  if( command == CrystalEndRadCmd )
   { Detector->SetCrystalEndRad(CrystalEndRadCmd->GetNewDoubleValue(newValue));}

  if( command == HoleRadCmd )
   { Detector->SetHoleRad(HoleRadCmd->GetNewDoubleValue(newValue));}
   
  if( command == HoleDepthCmd )
   { Detector->SetHoleDepth(HoleDepthCmd->GetNewDoubleValue(newValue));}
   
  if( command == DeadLayerCmd )
   { Detector->SetDeadLayerThick(DeadLayerCmd->GetNewDoubleValue(newValue));}
   
   //Shell Dimensions
  if( command == ShellHalfLengthCmd )
   { Detector->SetShellHalfLength(ShellHalfLengthCmd->GetNewDoubleValue(newValue));}
   
  if( command == EndGapCmd )
   { Detector->SetEndGap(EndGapCmd->GetNewDoubleValue(newValue));}
   
  if( command == WindowThickCmd )
   { Detector->SetWindowThickness(WindowThickCmd->GetNewDoubleValue(newValue));}

  if( command == WallThickCmd )
   { Detector->SetWallThickness(WallThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == DetectorDistanceCmd )
   { Detector->SetDetectorDistance(DetectorDistanceCmd->GetNewDoubleValue(newValue));}

  //update
  if( command == UpdateCmd )
   { Detector->UpdateGeometry(); }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
