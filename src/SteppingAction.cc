//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"

// #include "DetectorConstruction.hh"
// #include "Run.hh"
// #include "EventAction.hh"
#include "HistoManager.hh"

#include "G4Positron.hh"
#include "G4RunManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    G4double RotationAngle = -35 * deg;
    
    if(aStep->IsLastStepInVolume() && aStep->GetTrack()->GetDefinition()->GetPDGCharge()>0)
    {
//         G4cout << "Last step in volume" << G4endl;
        G4double KineticEnergy = aStep->GetTrack()->GetKineticEnergy();
        G4ThreeVector position = aStep->GetTrack()->GetPosition();
        
        G4double Zedd0 = 1*m;
        
        analysisManager->FillNtupleDColumn(1,0,KineticEnergy / MeV);
        analysisManager->FillNtupleDColumn(1,1,position.getZ() / mm);
        analysisManager->FillNtupleDColumn(1,2,(sqrt(pow(position.getX(),2.)+pow(position.getY(),2.))) / mm);
        analysisManager->FillNtupleDColumn(1,3,position.getX() / mm);
        analysisManager->FillNtupleDColumn(1,4,position.getY() / mm);
        analysisManager->FillNtupleDColumn(1,5,position.getZ() / mm);
        analysisManager->FillNtupleDColumn(1,6,(position.getX()*cos(RotationAngle) + (position.getZ() - Zedd0)*sin(RotationAngle)) / mm);//This is xprime
        analysisManager->FillNtupleDColumn(1,7,position.getY()/ mm); //No rotation in the Y direction so it doesn't change the Y coordinate
        analysisManager->FillNtupleDColumn(1,8,(-1*position.getX()*sin(RotationAngle) + (position.getZ() - Zedd0) * cos(RotationAngle)) / mm);
        analysisManager->FillNtupleSColumn(1,9,aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
        
        analysisManager->AddNtupleRow(1);  
    }
    
}