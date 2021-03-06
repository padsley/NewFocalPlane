//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: GeometryConstruction.cc 67272 2013-02-13 12:05:47Z ihrivnac $
//
/// \file eventgenerator/exgps/src/GeometryConstruction.cc
/// \brief Implementation of the GeometryConstruction class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GeometryConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GeometryConstruction::GeometryConstruction()
: G4VUserDetectorConstruction(),
fUniverse_phys(0),
//   fAl_phys(0),
fFPBox_phys(0)
//   fSphere_phys(0)
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GeometryConstruction::~GeometryConstruction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* GeometryConstruction::Construct()
{
    //
    //
    // Define materials.
    //
    G4double a, z, density,pressure, temperature;
    G4String name, symbol;
    
    
    density     = universe_mean_density;    //from PhysicalConstants.h
    pressure    = 3.e-18*pascal;
    temperature = 2.73*kelvin;
    G4Material* Vacuum   = new G4Material("Vacuum",
                                          1., 1.01*g/mole, density,
                                          kStateGas,temperature,pressure);    
    
    // Mylar
    
    a = 1.01*g/mole;
      G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);
   
   a = 12.01*g/mole;
      G4Element* elC = new G4Element(name="Carbon", symbol="C", z=6., a);
    
      a = 16.00*g/mole;
      G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
      
     density = 1.39*g/cm3;
     G4Material* Mylar = new G4Material(name="Mylar", density, 3);
     Mylar->AddElement(elO,2);
     Mylar->AddElement(elC,5);
   Mylar->AddElement(elH,4);
  
   
    //Molar mass of argon gas
    a = 39.948 * g/mole;    
    //Change this line if you want a different pressure
    pressure = 1.e-2*bar;
    
    density = a * pressure / (Avogadro * k_Boltzmann * STP_Temperature);
    G4cout << "density of argon gas = " << density / (g/cm3) << G4endl;
    G4Material* ArgonGas = new G4Material(name="Argon", z=18., a, density);
    
    //
    // Define size of world and volumes in it.
    //
    G4double world_r = 2000*cm;
    
    G4double box_x = 0.5*220*mm;
    G4double box_y = 0.5*100*mm;
    G4double box_z = 0.5*10*mm; //Make 10-mm slices for the detectors
    G4int NumberOfFPSlices = 10;
    G4double rotationAngle = -35*deg;
    
    // Define bodies, logical volumes and physical volumes.
    // First define the experimental hall.
    //
    G4Sphere * universe_s 
    = new G4Sphere("universe_s", 0, world_r, 0, twopi, 0, pi);
    G4LogicalVolume * universe_log
    = new G4LogicalVolume(universe_s,Vacuum,"universe_L",0,0,0);
    //
    fUniverse_phys
    = new G4PVPlacement(0,G4ThreeVector(),"universe_P",
                        universe_log,0,false,0);
    
    //Define the assembly volume which contains all of the parts of the focal-plane detector
//     G4LogicalVolume *CompleteFocalPlane_log = new G4LogicalVolume();
//     G4AssemblyVolume *CompleteFocalPlane = new G4AssemblyVolume();
    
    //define the focal plane box
    //
    G4Box * FP_box
    = new G4Box("FP_box", box_x, box_y, box_z);
    G4LogicalVolume * FP_log
    = new G4LogicalVolume(FP_box,ArgonGas,"Box_log",0,0,0);
    //
    
    //mylar entrance foil definition
    G4Box * EntranceFoil = new G4Box("EntranceFoil", box_x, box_y, 1.5/1000. *mm);
    G4LogicalVolume *EntranceFoil_log = new G4LogicalVolume(EntranceFoil,Mylar,"EntranceFoil_log",0,0,0);
    
    G4RotationMatrix *rotation = new G4RotationMatrix();
    //If you want to change the rotation for the central ray, this is the place to do it.
    rotation->rotateY(rotationAngle);
    
    for(G4int i=0;i<NumberOfFPSlices;i++)
    {
    
        char buffer[256];
        sprintf(buffer,"Box_phys_%d",i);
        
    fFPBox_phys
    = new G4PVPlacement(G4Transform3D(*rotation,G4ThreeVector(0.,0.,1.*m + ((double)i-(double)NumberOfFPSlices/2.)*2.*box_z)),
                        buffer,
                        FP_log,fUniverse_phys,
                        false,
                        0);
    }
    
    
    fEntranceFoil_phys = new G4PVPlacement(G4Transform3D(*rotation,G4ThreeVector(0.,0.,1.*m - NumberOfFPSlices*box_z)),
                                           "EntranceFoil_phys",
                                           EntranceFoil_log,
                                           fUniverse_phys,
                                           false,
                                           0);
    
    
    //--------- Visualization attributes -------------------------------
    universe_log->SetVisAttributes(G4VisAttributes::Invisible);
    G4VisAttributes* aVisAtt= new G4VisAttributes(G4Colour(0,1.0,1.0));
    //   Al_log->SetVisAttributes(aVisAtt);
    FP_log->SetVisAttributes(aVisAtt);
    EntranceFoil_log->SetVisAttributes(G4VisAttributes::solid);
    //   G4VisAttributes* bVisAtt= new G4VisAttributes(G4Colour(1.0,2.0,.0));
    //   aSphere_log->SetVisAttributes(bVisAtt);
    
    return fUniverse_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
