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
/// \file eventgenerator/exgps/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// $Id: HistoManager.cc 83882 2014-09-22 11:09:30Z maire $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("exgps")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  //
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();
  
  analysis->SetFileName(fFileName);
  analysis->SetVerboseLevel(1);
  analysis->SetActivation(true);     //enable inactivation of histos, nTuples
    
  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  // as we have not yet set nbins, vmin, vmax
  //
  analysis->SetHistoDirectoryName("histo");  
  analysis->SetFirstHistoId(1);
    
  G4int id = analysis->CreateH1("h1.1","kinetic energy", nbins, vmin, vmax);
  analysis->SetH1Activation(id, false);
    
  id = analysis->CreateH1("h1.2","vertex dist dN/dv = f(r)", nbins, vmin, vmax);
  analysis->SetH1Activation(id, false);

  id = analysis->CreateH1("h1.3","direction: cos(theta)", nbins, vmin, vmax);
  analysis->SetH1Activation(id, false);

  id = analysis->CreateH1("h1.4","direction: phi", nbins, vmin, vmax);
  analysis->SetH1Activation(id, false);  

  // histos 2D
  //
  id = analysis->CreateH2("h2.1","vertex: XY",nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  id = analysis->CreateH2("h2.2","vertex: YZ",nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  id = analysis->CreateH2("h2.3","vertex: ZX",nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  id = analysis->CreateH2("h2.4","direction: phi-cos(theta)",
                                          nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  id = analysis->CreateH2("h2.5","direction: phi-theta",
                                          nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  // nTuples
  //
  analysis->SetNtupleDirectoryName("ntuple");
  analysis->SetFirstNtupleId(0);       
  analysis->CreateNtuple("ParticleInfo", "Primary Particle Tuple");
  analysis->CreateNtupleIColumn(0,"particleID");    //column 0
  analysis->CreateNtupleDColumn(0,"Ekin");          //column 1
  analysis->CreateNtupleDColumn(0,"posX");          //column 2
  analysis->CreateNtupleDColumn(0,"posY");          //column 3
  analysis->CreateNtupleDColumn(0,"posZ");          //column 4
  analysis->CreateNtupleDColumn(0,"dirTheta");      //column 5
  analysis->CreateNtupleDColumn(0,"dirPhi");        //column 6
  analysis->CreateNtupleDColumn(0,"weight");        //column 7
  analysis->CreateNtupleSColumn(0,"name");          //column 8
  analysis->FinishNtuple(0);
  
  //nTuple for tracking
  analysis->CreateNtuple("TrackInfo", "Track of particle behaviour");
  analysis->CreateNtupleDColumn(1,"KineticEnergy"); //0
  analysis->CreateNtupleDColumn(1,"Depth"); //1
  analysis->CreateNtupleDColumn(1,"PosStrag"); //2
  analysis->CreateNtupleDColumn(1,"X"); //3
  analysis->CreateNtupleDColumn(1,"Y"); //4 
  analysis->CreateNtupleDColumn(1,"Z"); //5 
  analysis->CreateNtupleDColumn(1,"Xprime"); //6 
  analysis->CreateNtupleDColumn(1,"Yprime"); //7
  analysis->CreateNtupleDColumn(1,"Zprime"); //8
  analysis->CreateNtupleSColumn(1,"ParticleName"); //9
  analysis->FinishNtuple(1);
  
  analysis->SetNtupleActivation(true);             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
