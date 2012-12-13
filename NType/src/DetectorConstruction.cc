//
// $id: detectorconstruction.cc,v 1.1 2010-10-18 15:56:17 maire exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UImanager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Orb.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:crystalMaterial(0),defaultMaterial(0),
 solidWorld(0),logicWorld(0),physiWorld(0),
 logCrystal(0),physiCrystal(0)
{
  detectorDistance = 10.*cm;
  // default parameter values of the crystal 
  crystalHalfLength = (79.4/2)*mm;
  crystalRad = (64.5/2)*mm;
  crystalEndRad = (8)*mm;
  holeRad = (11.2/2)*mm;
  holeDepth = (72.7)*mm;
  deadLayerThick = 1.3*mm;
  // default parameter values of the HPGe 
  shellHalfLength = (105./2)*mm;
  endGap = 4.0*mm;
  windowThickness = 0.50*mm;
  wallThickness = 1.0*mm;
  

  ComputeHPGeParameters();
  
  // materials
  DefineMaterials();
  SetCrystalMaterial("Ge");
  SetWindowMaterial("Be");
  SetWallMaterial("Al");
  
  // create commands for interactive definition of the HPGe 
  detectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){ 
	delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct(){
  return ConstructHPGe();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials(){ 
 //This function illustrates the possible ways to define materials
 
G4String symbol;             //a=mass of a mole;
G4double a, z, density;      //z=mean number of protons;  

G4int ncomponents;
G4double fractionmass;

//
// define Elements
//

G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);

//
// define a material from elements.   case 2: mixture by fractional mass
//

G4Material* Air = 
new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
Air->AddElement(N, fractionmass=0.7);
Air->AddElement(O, fractionmass=0.3);

//
// examples of vacuum
//

G4Material* Vacuum =
new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 2.73*kelvin, 3.e-18*pascal);

G4Material* beam = 
new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
                       kStateGas, STP_Temperature, 2.e-2*bar);
beam->AddMaterial(Air, fractionmass=1.);

// print table
//
G4cout << *(G4Material::GetMaterialTable()) << G4endl;

//default materials of the World
defaultMaterial  = Vacuum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructHPGe(){

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the HPGe parameters definition
  ComputeHPGeParameters();
   
  //     
  // World
  //
  solidWorld = new G4Box("World",				//its name
                   WorldSizeX/2,WorldSizeYZ/2,WorldSizeYZ/2);	//its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                   defaultMaterial,	//its material
                                   "World");		//its name
                                   
  physiWorld = new G4PVPlacement(0,			//no rotation
  				 G4ThreeVector(),	//at (0,0,0)
                                 logicWorld,		//its logical volume				 
                                 "World",		//its name
                                 0,			//its mother  volume
                                 false,			//no boolean operation
                                 0);			//copy number
  
  //                               
  // Crystal
  //  
  
  //Making the exterior crystal shape
  G4VSolid *cyl1 = new G4Tubs("cyl1", //name
  			0.*cm, 			//inner radius
			crystalRad-crystalEndRad, 	//outer radius
			crystalHalfLength, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *cyl2 = new G4Tubs("cyl2", //name
  			0.*cm, 			//innter radius
			crystalRad, 	//outer radius
			crystalHalfLength-.5*crystalEndRad, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *tor = new G4Torus("tor",	//name
  			0.*cm, 			//inner r
			crystalEndRad,	//outer r
 			crystalRad-crystalEndRad,//torus rad
			0.*deg, 		//start phi
			360.*deg);		//end phi
  G4VSolid *crystal1 = new G4UnionSolid("crystal1", //name
  			cyl1,cyl2,0,G4ThreeVector(0.,0.,-.5*crystalEndRad));
  G4VSolid *crystal2 = new G4UnionSolid("crystal2", //name
  			crystal1, tor,0,G4ThreeVector(0.,0.,crystalHalfLength-crystalEndRad));
  
  //making the hole
  G4VSolid *inCyl = new G4Tubs("inCyl", //name
			0.*cm, 			//inner radius
			holeRad, 	//outer radius
			0.5*(holeDepth-holeRad), //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *inSph = new G4Orb("inSph",holeRad);
  hole = new G4UnionSolid("hole",
  			inCyl,inSph,0,G4ThreeVector(0.,0.,.5*(holeDepth-holeRad)));

  //making the dead layer
  G4VSolid *dead1 = new G4Tubs("dead1", //name
			0.*cm, 			//inner radius
			holeRad+deadLayerThick, 	//outer radius
			0.5*(holeDepth-holeRad), //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *dead2 = new G4Orb("dead2",holeRad+deadLayerThick);
  G4VSolid *dead3 = new G4UnionSolid("dead3",
  			dead1,dead2,0,G4ThreeVector(0.,0.,.5*(holeDepth-holeRad)));
  deadLayer = new G4SubtractionSolid("deadLayer",
  			dead3,hole,0,G4ThreeVector(0.,0.,0.));

  
  //final detector shape
  activeCrystal = new G4SubtractionSolid("solidCrystal",
  			crystal2,dead3,0,G4ThreeVector(0.,0.,-crystalHalfLength+0.5*holeDepth-0.5*holeRad));

  logCrystal = new G4LogicalVolume(activeCrystal,
  			crystalMaterial,	//material
			"logCrystal");
  logDeadLayer = new G4LogicalVolume(deadLayer,
  			crystalMaterial,	//material
			"logDeadLayer");
  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);
  physiCrystal = new G4PVPlacement(rm,	//rotation
					G4ThreeVector(crystalHalfLength+windowThickness+endGap,0.,0.),//placement
					logCrystal,	//its logical volume
					"physiCrystal",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
  physiDeadLayer = new G4PVPlacement(rm,	//rotation
					G4ThreeVector(3.*crystalHalfLength+windowThickness+endGap-holeDepth,0.*mm,0.),	//placement
					logDeadLayer,	//its logical volume
					"physiDeadLayer",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
 
  //Make the outer shell
  G4VSolid *outerShell = new G4Tubs("outerShell",	//name
  			crystalRad+endGap,	//inner rad
  			crystalRad+endGap+wallThickness,	//outer rad
  			shellHalfLength,	//half length
			0.*deg,		//starting phi
			360.*deg);	//ending phi
  logShell = new G4LogicalVolume(outerShell,
  			wallMaterial,	//material
			"logShell");
  physiShell = new G4PVPlacement(rm,	//rotation
					G4ThreeVector(shellHalfLength,0.,0.),	//placement
					logShell,	//its logical volume
					"physiShell",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number


  //Make the window
  G4VSolid *window = new G4Tubs("window",	//name
  			0,	//inner rad
  			crystalRad+endGap,	//outer rad
  			0.5*windowThickness,	//thickness
			0.*deg,		//starting phi
			360.*deg);	//ending phi
  logWindow = new G4LogicalVolume(window,
  			windowMaterial,	//material
			"logWindow");
  physiWindow = new G4PVPlacement(rm,	//rotation
					G4ThreeVector(.5*windowThickness,0.,0.),	//placement
					logWindow,	//its logical volume
					"physiWindow",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number

  //Make the sheilding 
  G4NistManager* manager = G4NistManager::Instance();
  G4Material *copper = manager->FindOrBuildMaterial("G4_Cu");
  G4Material *lead = manager->FindOrBuildMaterial("G4_Pb");

  G4VSolid *shield1 = new G4Tubs("shield1",	//name
  			0,	//inner rad
  			crystalRad+endGap+wallThickness,	//outer rad
  			0.5*4.0*mm,	//thickness
			0.*deg,		//starting phi
			360.*deg);	//ending phi
  logShield1 = new G4LogicalVolume(shield1,
  			copper,	//material
			"logShield1");
  physiShield1= new G4PVPlacement(rm,	//rotation
					G4ThreeVector(-.5*4.0*mm,0.,0.),	//placement
					logShield1,	//its logical volume
					"physiShield1",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
	
  /*
  G4VSolid *shield2 = new G4Tubs("shield2",	//name
  			0,	//inner rad
  			crystalRad+endGap+wallThickness,	//outer rad
  			0.5*9.0*mm,	//thickness
			0.*deg,		//starting phi
			360.*deg);	//ending phi
  logShield2 = new G4LogicalVolume(shield2,
  			lead,	//material
			"logShield2");
  physiShield2= new G4PVPlacement(rm,	//rotation
					G4ThreeVector((-.5*9.0-4.0)*mm,0.,0.),	//placement
					logShield2,	//its logical volume
					"physiShield2",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
  */
	
  //Set solid angle
  theta = pi - acos(detectorDistance*1./
  	sqrt(crystalRad*crystalRad + detectorDistance*detectorDistance));

  G4UImanager* UI = G4UImanager::GetUIpointer();
  std::stringstream strbld;

  strbld << "/gps/ang/mintheta " << theta << " rad";
  UI->ApplyCommand(strbld.str());
  strbld.str("");
  strbld.clear();
  
  strbld << "/gps/ang/rot1 0 0 -1";
  UI->ApplyCommand(strbld.str());
  strbld.str("");
  strbld.clear();
  			
  PrintHPGeParameters();     
  
  //                                        
  // Visualization attributes
  //
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);

 
  // Below are vis attributes that permits someone to test / play 
  // with the interactive expansion / contraction geometry system of the
  // vis/OpenInventor driver :
 {G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
 simpleBoxVisAtt->SetVisibility(true);
// delete logCrystal->GetVisAttributes();
 logShell->SetVisAttributes(simpleBoxVisAtt);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.2));
  atb->SetForceSolid(true);
  logCrystal->SetVisAttributes(atb);}
  
// {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.2));
//  atb->SetForceSolid(true);
//  logDeadLayer->SetVisAttributes(atb);}
  
 //{G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.2));
 // atb->SetForceSolid(true);
 // logWindow->SetVisAttributes(atb);}

 //{G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.01));
// atb->SetForceSolid(true);
//  logShell->SetVisAttributes(atb);}

  
  return physiWorld;	//always return the physical World
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintHPGeParameters(){
	
  G4double activeVol = activeCrystal->GetCubicVolume();
  G4double activeSA = activeCrystal->GetSurfaceArea();
  G4double deadVol = deadLayer->GetCubicVolume();
  G4double deadSA = deadLayer->GetSurfaceArea();
  G4double holeVol = hole->GetCubicVolume();
  G4double holeSA = hole->GetSurfaceArea();
  G4double solidangle = 2*pi*(1-cos(pi-theta));

  G4cout << "\n------------------------------------------------------------"
         << "\n---> The crysyal is " << crystalMaterial->GetName() << "\n"
         << crystalRad/mm << "mm outer radius \n"
         << 2.*crystalHalfLength/mm << "mm length \n" 
         << crystalEndRad/mm << "mm end radius \n" 
         << holeRad/mm << "mm hole radius \n" 
         << holeDepth/mm << "mm hole depth \n" 
         << deadLayerThick/mm << "mm dead layer \n" 
		 << "---> Shielding properties \n"
         << 2.*shellHalfLength/mm << "mm shell length \n" 
         << wallThickness/mm << "mm wall of " << wallMaterial->GetName() << "\n" 
         << endGap/mm << "mm end gap \n" 
         << windowThickness/mm << "mm window of " << windowMaterial->GetName() << "\n"
		 << "--->Calculated quantities \n"
		 << holeVol/(cm*cm*cm) << " cm^3 hole volume \n"
		 << holeSA/(cm*cm) << " cm^2 hole surface area \n"
		 << activeVol/(cm*cm*cm) << " cm^3 Active volume \n"
		 << activeSA/(cm*cm) << " cm^2 Active surface area \n"
		 << deadVol/(cm*cm*cm) << " cm^3 Dead layer volume \n"
		 << deadSA/(cm*cm) << " cm^2  Dead layer surface area \n"
  		 << theta << " theta \n"
  		 << solidangle << " /4*pi solid angle\n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCrystalMaterial(G4String materialChoice){
  // search the material by its name   
	G4NistManager* manager = G4NistManager::Instance();
	crystalMaterial = manager->FindOrBuildMaterial("G4_"+materialChoice);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWindowMaterial(G4String materialChoice){
  // search the material by its name   
	G4NistManager* manager = G4NistManager::Instance();
	windowMaterial = manager->FindOrBuildMaterial("G4_"+materialChoice);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWallMaterial(G4String materialChoice){
  // search the material by its name   
	G4NistManager* manager = G4NistManager::Instance();
	wallMaterial = manager->FindOrBuildMaterial("G4_"+materialChoice);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::SetDetectorDistance(G4double val){
  detectorDistance = val;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::SetCrystalHalfLength(G4double val){
  crystalHalfLength = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCrystalRad(G4double val){
  crystalRad = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCrystalEndRad(G4double val){
  crystalEndRad = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetHoleRad(G4double val){
  holeRad = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetHoleDepth(G4double val){
  holeDepth = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetDeadLayerThick(G4double val){
  deadLayerThick = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetShellHalfLength(G4double val){
   shellHalfLength = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetEndGap(G4double val){
   endGap = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWindowThickness(G4double val){
   windowThickness = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWallThickness(G4double val){
   wallThickness = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructHPGe());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
