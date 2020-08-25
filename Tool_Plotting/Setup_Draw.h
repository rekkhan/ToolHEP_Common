#ifndef SETUP_DRAW_H
#define SETUP_DRAW_H


// + Pad set up
//=============
void Characterize_Pad (TPad *pad, float left, float bottom, float right, float top)
{
	pad -> SetGrid (1, 1);
	pad -> SetLeftMargin   (left);
	pad -> SetRightMargin  (right);
	pad -> SetTopMargin    (top);
	pad -> SetBottomMargin (bottom);
}










// + Decide which histogram to draw first
//=======================================
// * Simple case: 2 histograms
//----------------------------
void DrawFirst (TH1F *hist1, TH1F *hist2)
{
	bool order = 1;
	
	float bin1 = hist1 -> GetBinContent (hist1 -> GetMaximumBin());
	float bin2 = hist2 -> GetBinContent (hist2 -> GetMaximumBin());
	
	
	if (bin1 < bin2)  order = 0;
	
	
	if (order == 1)
	{
		hist1 -> Draw ("ep");
		hist2 -> Draw ("hist same");
	}
	else
	{
		hist2 -> Draw ("hist");
		hist1 -> Draw ("ep same");
	}
}



// * More complicated case: >2 histograms
//---------------------------------------
void DrawFirst (vector<TH1F*> hist,   bool isTopPanel,   bool drawLine,   bool legOnTop)
{
	// + Declare some variables for sorting
	//-------------------------------------
	float tmpHeight = 0;
	
	vector<float> binHeight;
	binHeight . clear();
	
	vector<int> orderHist;
	orderHist . clear();
	
	vector<int> orderDraw;
	orderDraw . clear();
	
	vector<int>::iterator it;
	
	printf ("hist . size() = %zu\n", hist.size());
	
	// + Get the original ordering of the histograms
	//----------------------------------------------
	for (unsigned int i=0; i<hist.size(); i++)
	{
		// * Find the maximum height of each histogram, we sort by this variable
		tmpHeight = hist[i] -> GetBinContent (hist[i] -> GetMaximumBin());
		
		// * If we draw legend on top, increase the upper range of the Y axis
		float height = (legOnTop) ? tmpHeight*1.3 : tmpHeight*1.15;
		hist[i] -> SetMaximum (height);
		
		if (!isTopPanel)   hist[i] -> GetYaxis() -> SetRangeUser (0.75, 1.25);
		
		binHeight . push_back (tmpHeight);
		orderHist . push_back (i);
	}
	
	printf ("binHeight . size() = %zu\n", binHeight.size());
	printf ("orderHist . size() = %zu\n", orderHist.size());
	
	
	// + Decice the order for drawing
	//-------------------------------
	orderDraw . push_back (orderHist[0]);
	
	for (unsigned int i=1; i<hist.size(); i++)
	{
		bool trigInsert = false;
		
		for (it=orderDraw.begin(); it<orderDraw.end(); it++)
		{
			if (binHeight[i] > binHeight[*it])
			{
				orderDraw . insert (it, orderHist[i]);
				trigInsert = true;
				break;
			}
		}
		
		if (trigInsert == false)
		{
			orderDraw . push_back (orderHist[i]);
		}
	}
	
	
	printf ("orderDraw . size() = %zu\n", orderDraw.size());
	
	
	
	for (unsigned int i=0; i<orderDraw.size(); i++)
	{
		printf ("component %d\n", orderDraw[i]);
	}
	
	
	hist[orderDraw[0]] -> Draw ("ep");
	if (drawLine)   hist[orderDraw[0]] -> Draw ("hist same");
	
	for (int i=1; i<orderDraw.size(); i++)
	{
		printf ("Drawing %d histograms\n", orderDraw[i]);
		hist[orderDraw[i]] -> Draw ("ep same");
		if (drawLine)   hist[orderDraw[i]] -> Draw ("hist same");
	}
}










// + Draw plot with ratio: Legend on top of the plot
//==================================================
void DrawHist_wRatio_LegendTop (TCanvas *canvas,   float ratio,   vector<TH1F*> histDistribution,   vector<TH1F*> histRatio,   vector<TString> nameHist,   float luminosity,   TString name_output)
{
	// + Compute the vertical separation
	//----------------------------------
	float verticalSep = 1 - ratio/(1+ratio);
	
	
	// + Draw stuff on the upper panel
	//--------------------------------
	canvas -> cd();
	bool isTopPanel;
	
	// * Common alignment for top and bottom panel
	float alignLeft  = 0.11;
	float alignRight = 0.05;
	
	// * Make the pad to draw on
	TPad *padDist = new TPad ("padDist", "", 0.0, verticalSep, 1.0, 1.0);
	Characterize_Pad (padDist, alignLeft, 0.013, alignRight, 0.08);
	padDist -> Draw();
	padDist -> cd();
	
	// * Draw the histogram
	isTopPanel = true;
	DrawFirst (histDistribution, isTopPanel, true, true);
	
	// * Draw the legend
	TLegend *legend = new TLegend (0.135, 0.80, 0.945, 0.915);
	legend -> SetTextSize (0.045);
	legend -> SetNColumns (2);
	legend -> SetFillColorAlpha (0, 0.8);
	legend -> SetLineColorAlpha (1, 0.2);
	legend -> SetLineWidth      (3);
	
	for (unsigned int i=0; i<histDistribution.size(); i++)
	{
		legend -> AddEntry (histDistribution[i], nameHist[i].Data(), "lep");
	}
	
	legend -> Draw ("same");
	
	// * Write texts
	TLatex *texSymbol = new TLatex ();
	texSymbol -> SetNDC ();
	texSymbol -> SetTextFont (42);
	texSymbol -> SetTextSize (0.045);
	texSymbol -> DrawLatex (0.110, 0.935, "#bf{NCUHEP} #it{Preliminary}");
	
	TLatex *texLumi = new TLatex ();
	texLumi -> SetNDC ();
	texLumi -> SetTextFont (42);
	texLumi -> SetTextSize  (0.045);
	texLumi -> SetTextAlign (31);
	texLumi -> DrawLatex (0.95, 0.935, Form("#sqrt{S}=100 TeV, %.0f fb^{-1} (2050)", luminosity));
	
	
	
	
	
	// + Draw stuff on the lower panel
	//--------------------------------
	canvas -> cd();
	TPad *padRate = new TPad ("padRate", "", 0.0, 0.0, 1.0, verticalSep);
	Characterize_Pad (padRate, alignLeft, 0.25, alignRight, 0.005);
	padRate -> Draw();
	padRate -> cd();
	
	// * Draw the histogram
	isTopPanel = false;
	DrawFirst (histRatio, isTopPanel, false, false);
	
	
	
	
	
	// + Draw canvas to file
	//----------------------
	canvas -> SaveAs (name_output.Data());
}










// + Draw plot with ratio: Legend on the left of the plot
//=======================================================
void DrawHist_wRatio_LegendLeft (TCanvas *canvas,   float ratio,   vector<TH1F*> histDistribution,   vector<TH1F*> histRatio,   vector<TString> nameHist,   float luminosity,   TString name_output)
{
	// + Compute the vertical separation
	//----------------------------------
	float verticalSep = 1 - ratio/(1+ratio);
	
	
	// + Draw stuff on the upper panel
	//--------------------------------
	canvas -> cd();
	bool isTopPanel;
	
	// * Common alignment for top and bottom panel
	float alignLeft  = 0.11;
	float alignRight = 0.05;
	
	// * Make the pad to draw on
	TPad *padDist = new TPad ("padDist", "", 0.0, verticalSep, 1.0, 1.0);
	Characterize_Pad (padDist, alignLeft, 0.013, alignRight, 0.08);
	padDist -> Draw();
	padDist -> cd();
	
	// * Draw the histogram
	isTopPanel = true;
	DrawFirst (histDistribution, isTopPanel, true, false);
	
	// * Draw the legend
	TLegend *legend = new TLegend (0.75, 0.60, 0.94, 0.91);
	legend -> SetTextSize (0.045);
	legend -> SetFillColorAlpha (0, 0.8);
	legend -> SetLineColorAlpha (1, 0.2);
	legend -> SetLineWidth      (3);
	
	for (unsigned int i=0; i<histDistribution.size(); i++)
	{
		legend -> AddEntry (histDistribution[i], nameHist[i].Data(), "lep");
	}
	
	legend -> Draw ("same");
	
	// * Write texts
	TLatex *texSymbol = new TLatex ();
	texSymbol -> SetNDC ();
	texSymbol -> SetTextFont (42);
	texSymbol -> SetTextSize (0.045);
	texSymbol -> DrawLatex (0.110, 0.935, "#bf{NCUHEP} #it{Preliminary}");
	
	TLatex *texLumi = new TLatex ();
	texLumi -> SetNDC ();
	texLumi -> SetTextFont (42);
	texLumi -> SetTextSize  (0.045);
	texLumi -> SetTextAlign (31);
	texLumi -> DrawLatex (0.95, 0.935, Form("#sqrt{S}=100 TeV, %.0f fb^{-1} (2050)", luminosity));
	
	
	
	
	
	// + Draw stuff on the lower panel
	//--------------------------------
	canvas -> cd();
	TPad *padRate = new TPad ("padRate", "", 0.0, 0.0, 1.0, verticalSep);
	Characterize_Pad (padRate, alignLeft, 0.25, alignRight, 0.005);
	padRate -> Draw();
	padRate -> cd();
	
	// * Draw the histogram
	isTopPanel = false;
	DrawFirst (histRatio, isTopPanel, false, false);
	
	
	
	
	
	// + Save the canvas to file
	//--------------------------
	canvas -> SaveAs (name_output.Data());
}



#endif
