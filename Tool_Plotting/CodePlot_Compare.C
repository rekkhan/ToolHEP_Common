#include <stdio.h>
#include <iostream>

#include "Setup_Style1Dim.h"
#include "Setup_Draw.h"


// + Define some colours
//======================
int myColour[3] = {kOrange+7, kPink-3, kAzure+2};










// + Fill histograms
//==================
// * Set the argument of TRandom3 to 0 to have unique random value
TRandom *myrandLan = new TRandom3 (0);
TRandom *myrandGau = new TRandom3 (0);

void FillHistogram (TH1F *hist,   TString name_distribution,   long nEntries)
{
	float valRandom;
	float gauRandom = 0;
	float lanRandom = 0;
	
	// * Fill the histogram with a Landau distribution
	for (long i=0; i<nEntries; i++)
	{
		if (name_distribution . Contains("Truth"))
		{
			if (i==0)   printf ("Use  on %s\n", hist->GetName());
			lanRandom = myrandLan -> Landau (35, 6);
			gauRandom = myrandGau -> Gaus (0, 6);
		}
		else if (name_distribution . Contains("Gauss"))
		{
			if (i==0)   printf ("Use larger Gaussian width on %s\n", hist->GetName());
			lanRandom = myrandLan -> Landau (35, 5.5);
			gauRandom = myrandGau -> Gaus (0, 6.5);
		}
		else if (name_distribution . Contains("Landau"))
		{
			if (i==0)   printf ("Use larger Landau width on %s\n", hist->GetName());
			lanRandom = myrandLan -> Landau (35, 6.5);
			gauRandom = myrandGau -> Gaus (0, 5.5);
		}
		else
		{
			if (i==0)   printf ("Use equal width on %s\n", hist->GetName());
			lanRandom = myrandLan -> Landau (35, 6.0);
			gauRandom = myrandGau -> Gaus (0, 6.0);
		}
		
		valRandom = gauRandom + lanRandom;
		
		hist -> Fill (valRandom);
	}
	
	hist -> Sumw2 ();
}










// + Main function
//================
void CodePlot_Compare ()
{
	system ("mkdir -p  /home/longhoa/ROOT_Work/Task_Test/Output/");
	gStyle -> SetOptStat (0);
	
	
	TH1F *histDT  = new TH1F ("histDT",  "",  20, 0, 100);
	TH1F *histMC1 = new TH1F ("histMC1", "",  20, 0, 100);
	TH1F *histMC2 = new TH1F ("histMC2", "",  20, 0, 100);
	TH1F *histMC3 = new TH1F ("histMC3", "",  20, 0, 100);
	
	TH1F *histR1;
	TH1F *histR2;
	TH1F *histR3;
	
	long nEntries = 200000;
	
	FillHistogram (histDT,  "Truth",   nEntries);
	FillHistogram (histMC1, "Gauss",   nEntries);
	FillHistogram (histMC2, "Landau",  nEntries);
	FillHistogram (histMC3, "LanGau",  nEntries);
	
	histR1 = (TH1F*)histDT -> Clone();
	histR1 -> Divide (histMC1);
	histR2 = (TH1F*)histDT -> Clone();
	histR2 -> Divide (histMC2);
	histR3 = (TH1F*)histDT -> Clone();
	histR3 -> Divide (histMC3);
	
	
	// + Characterize the object
	//--------------------------
	// * Name of the histograms
	TString name_Hist[4] =
	{
		"Truth",
		"Model1",
		"Model2",
		"Model3"
	};
	
	// * Modify the style of the histograms
	float ratioPanel = 7.0 / 3.0;
	
	CharacterizeHist (histDT,  1, 1, ratioPanel, int(kBlack), "Arbitrary variable (A.U)", "Events");
	CharacterizeHist (histMC1, 1, 1, ratioPanel, myColour[0], "Arbitrary variable (A.U)", "Events");
	CharacterizeHist (histMC2, 1, 1, ratioPanel, myColour[1], "Arbitrary variable (A.U)", "Events");
	CharacterizeHist (histMC3, 1, 1, ratioPanel, myColour[2], "Arbitrary variable (A.U)", "Events");
	CharacterizeHist (histR1,  1, 0, ratioPanel, myColour[0], "Arbitrary variable (A.U)", "Truth/Model");
	CharacterizeHist (histR2,  1, 0, ratioPanel, myColour[1], "Arbitrary variable (A.U)", "Truth/Model");
	CharacterizeHist (histR3,  1, 0, ratioPanel, myColour[2], "Arbitrary variable (A.U)", "Truth/Model");
	
	
	// + Draw the histograms
	//----------------------
	TCanvas *canvas = new TCanvas ("canvas", "", 1000, 1000);
	
	vector<TH1F*>    vecHistDistribution;
	vector<TH1F*>    vecHistRatio;
	vector<TString>  vecNameHist;
	
	vecHistDistribution . clear();
	vecHistRatio        . clear();
	vecNameHist         . clear();
	
	vecHistDistribution . push_back (histDT);
	vecHistDistribution . push_back (histMC1);
	vecHistDistribution . push_back (histMC2);
	vecHistDistribution . push_back (histMC3);
	
	vecNameHist . push_back (name_Hist[0]);
	vecNameHist . push_back (name_Hist[1]);
	vecNameHist . push_back (name_Hist[2]);
	vecNameHist . push_back (name_Hist[3]);
	
	vecHistRatio . push_back (histR1);
	vecHistRatio . push_back (histR2);
	vecHistRatio . push_back (histR3);
	
	float luminosity = 100;
	
	DrawHist_wRatio_LegendTop (canvas, ratioPanel, vecHistDistribution, vecHistRatio, vecNameHist, luminosity, (TString)"Plot_Comparing_LegendTop.png");
	DrawHist_wRatio_LegendLeft (canvas, ratioPanel, vecHistDistribution, vecHistRatio, vecNameHist, luminosity, (TString)"Plot_Comparing_LegendLeft.png");
}
