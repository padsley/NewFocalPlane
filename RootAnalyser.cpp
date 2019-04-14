#include <TFile.h>
#include <TTree.h>
#include <string>
#include <vector>

void RootAnalyser()
{
    std::vector<string> RunsToAnalyse;
    RunsToAnalyse.push_back("Proton50MeV");
    RunsToAnalyse.push_back("Alpha50MeV");
    RunsToAnalyse.push_back("He3_50MeV");
    RunsToAnalyse.push_back("Li750MeV);
    RunsToAnalyse.push_back("C12_50MeV");
    RunsToAnalyse.push_back("O16_50MeV");
    
    TH2F **EnergyLoss = new TH2F*[RunsToAnalyse.size()];
    
    for(unsigned int i=0;i<RunsToAnalyse.size();i++)
    {
        char buffer[256];
        sprintf(buffer,"exgps%s.root",RunsToAnalyse.at(i).c_str());
        std::cout << buffer << std::endl;
        
        TFile *fin = TFile::Open(buffer);
        TTree *trin = (TTree*)fin->FindObjectAny("TrackInfo");
        
        sprintf(buffer,"EnergyLoss_%s",RunsToAnalyse.at(i).c_str());
        std::cout << buffer << std::endl;
        EnergyLoss[i] = new TH2F(buffer,buffer,2000,900,1100,5100,0,51);
        
        sprintf(buffer,"KineticEnergy:Depth>>EnergyLoss_%s",RunsToAnalyse.at(i).c_str());
        std::cout << buffer << std::endl;
        
        trin->Draw(buffer);
        EnergyLoss[i]->SetMarkerColor(i+1);
    }
    
    for(unsigned int i=0;i<RunsToAnalyse.size();i++)
    {
        if(i==0)EnergyLoss[i]->Draw();
        else EnergyLoss[i]->Draw("same");
    }
    
    TFile *foutput = new TFile("OneAtmosphere.root","RECREATE");
}