//
//
// $Id: DetectorConstruction.hh,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

  public:
     
     void SetDetectorDistance (G4double);     

     void SetCrystalMaterial (G4String);     
     void SetWindowMaterial (G4String);     
     void SetWallMaterial (G4String);     

     void SetCrystalHalfLength(G4double);
     void SetCrystalRad(G4double);
     void SetCrystalEndRad(G4double);
     void SetHoleRad(G4double);
     void SetHoleDepth(G4double);
     void SetDeadLayerThick(G4double);

     void SetShellHalfLength(G4double);
     void SetEndGap(G4double);
     void SetWindowThickness(G4double);
     void SetWallThickness(G4double);

     
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
     
  public:
  
     void PrintHPGeParameters(); 
                    
     G4double GetWorldSizeX()           {return WorldSizeX;}; 
     G4double GetWorldSizeYZ()          {return WorldSizeYZ;};
     
     G4double GetDetectorDistance()	{return detectorDistance;};

     G4double GetCrystalHalfLength()	{return crystalHalfLength;};
     G4double GetCrystalRad()			{return crystalRad;}; 
     G4double GetCrystalEndRad()		{return crystalEndRad;};
     G4double GetHoleRad()				{return holeRad;};
     G4double GetHoleDepthRad()			{return holeDepth;};
     G4double GetDeadLayerThick()		{return deadLayerThick;};
      
     G4double GetShellHalfLength()		{return shellHalfLength;}; 
     G4double GetEndGap()				{return endGap;};
     G4double GetWindowThickness()		{return windowThickness;};
     G4double GetWallThickness()		{return wallThickness;};
      
     G4Material* GetCrystalMaterial()       {return crystalMaterial;};
     G4Material* GetWindowMaterial()       {return windowMaterial;};
     G4Material* GetWallMaterial()       {return wallMaterial;};
     
     const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};           
     const G4VPhysicalVolume* GetCrystal()   {return physiCrystal;};
                 
  private:
     
     G4Material*        crystalMaterial;
     G4Material*        windowMaterial;
     G4Material*        wallMaterial;
     
     G4double           detectorDistance;

     G4double           crystalHalfLength;
     G4double           crystalRad;
     G4double           crystalEndRad;
     G4double           holeRad;
     G4double           holeDepth;
     G4double           deadLayerThick;

     G4double           shellHalfLength;
     G4double           endGap;
     G4double           windowThickness;
     G4double           wallThickness;

	 G4double 			theta;
     
     G4Material*        defaultMaterial;
     G4double           WorldSizeYZ;
     G4double           WorldSizeX;
            
     G4Box*             solidWorld;    //pointer to the solid World 
     G4LogicalVolume*   logicWorld;    //pointer to the logical World
     G4VPhysicalVolume* physiWorld;    //pointer to the physical World

	 G4VSolid *activeCrystal;
	 G4VSolid *deadLayer;
	 G4VSolid *hole;

     G4LogicalVolume *logCrystal;    //pointer to the logical crystal 
     G4LogicalVolume *logDeadLayer;    //pointer to the logical dead layer
     G4VPhysicalVolume *physiCrystal;    //pointer to the physical crystal 
     G4VPhysicalVolume *physiDeadLayer;    //pointer to the physical dead layer

     G4LogicalVolume *logShell;    //pointer to the logical shell 
     G4VPhysicalVolume *physiShell;    //pointer to the physical shell 
     
     G4LogicalVolume *logWindow;    //pointer to the logical window 
     G4VPhysicalVolume *physiWindow;    //pointer to the physical window 
     
     G4LogicalVolume *logShield1;    //pointer to the logical shield1 
     G4VPhysicalVolume *physiShield1;    //pointer to the physical shield1 
     G4LogicalVolume *logShield2;    //pointer to the logical shield2 
     G4VPhysicalVolume *physiShield2;    //pointer to the physical shield2 

     DetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
  private:
    
     void DefineMaterials();
     void ComputeHPGeParameters();
     G4VPhysicalVolume* ConstructHPGe();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstruction::ComputeHPGeParameters()
{
	G4double detSize = (2*crystalHalfLength)+endGap+windowThickness;
	if (detectorDistance>detSize){
    	WorldSizeX = 2*detectorDistance*cm;
	} 
	else if (detectorDistance<=detSize){
    	WorldSizeX = 2*detSize*cm;
	} 

	WorldSizeYZ = 2.1*crystalRad;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

