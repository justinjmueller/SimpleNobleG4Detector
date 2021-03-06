#include "NobleG4DetectorConstruction.hh"
#include "NobleG4SensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"


NobleG4DetectorConstruction::NobleG4DetectorConstruction()
: G4VUserDetectorConstruction(),
  fActiveVolume(0)
{ }


NobleG4DetectorConstruction::~NobleG4DetectorConstruction()
{ }


G4VPhysicalVolume* NobleG4DetectorConstruction::Construct()
{
  // Get NIST material manager.
  G4NistManager* NIST = G4NistManager::Instance();

  // Set volume parameters.
  G4double CubeX = 2000*cm;
  G4double CubeY = 2000*cm;
  G4double CubeZ = 2000*cm;
  G4Material* ActiveMat = NIST->FindOrBuildMaterial("G4_lAr");

  // World creation.
  G4Box* SolidWorld = new G4Box("World",                               // The volume name.
				0.5*CubeX,                             // (Half) x length.
				0.5*CubeY,                             // (Half) y length.
				0.5*CubeZ);                            // (Half) z length.

  G4LogicalVolume* LogicalWorld = new G4LogicalVolume(SolidWorld,      // The solid volume.
						      ActiveMat,       // The material.
						      "World");        // The volume name.
  
  G4VPhysicalVolume* PhysicalWorld = new G4PVPlacement(0,               // No rotation.
						      G4ThreeVector(), // Placed at (0,0,0).
						      LogicalWorld,    // The logical volume.
						      "World",         // The volume name.
						      0,               // The mother volume (none).
						      false,           // No boolean operation.
						      0,               // The copy number.
						      false);          // No overlap checking.

  // Set the sensitive volume.
  fActiveVolume = LogicalWorld;

  NobleG4SensitiveDetector* SD = new NobleG4SensitiveDetector("NobleG4SD");
  G4SDManager* SDManager = G4SDManager::GetSDMpointer();
  SDManager->AddNewDetector(SD);
  LogicalWorld->SetSensitiveDetector(SD);
  
  return PhysicalWorld;
}
