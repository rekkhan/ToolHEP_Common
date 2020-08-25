#ifndef SETUP_STYLE1DIM_H
#define SETUP_STYLE1DIM_H



// + Charaterize the histograms
//=============================
// * This function requires 7 arguments
//   1) The pointer to the histogram
//   2) A boolean indicate the type of plot (is it a comparison or not, like data/MC)
//   3) A boolean telling which panel the histogram is plotted on (only valid when (2) is true)
//   4) Ratio between the two panel (again, only apply when (2) is true), this value governs the text size and text alignment
//   5) Colour of the histogram
//   6) X asis name
//   7) Y axis name
//-----------------------------
void CharacterizeHist (TH1F *hist,   bool isComparePlot,   bool isTopPanel,   float ratioPanel,   int colour,   TString nameAxisX,   TString nameAxisY)
{
	// + Define some additional variables
	//-----------------------------------
	float scaleX;
	float scaleY;
	float sizeLabel;
	float sizeTitle;
	float offsetXlabel;
	float offsetXtitle;
	float offsetYlabel;
	float offsetYtitle;
	
	// * These value can be tweaked to fit your purpose
	sizeLabel    = 0.035;
	sizeTitle    = 0.045;
	offsetXlabel = 0.003;
	offsetXtitle = 0.350;
	offsetYlabel = 0.003;
	offsetYtitle = 1.300;
	
	if (isComparePlot)
	{
		scaleX = (isTopPanel) ? 0.0 : ratioPanel;
		scaleY = (isTopPanel) ? 1.0 : ratioPanel;
	}
	else
	{
		scaleX = 1.0;
		scaleY = 1.0;
	}
	
	
	
	// + The setting really starts here
	//---------------------------------
	if (colour > 0)
	{
		hist -> SetLineColor   (colour);
		hist -> SetMarkerColor (colour);
	}
	
	hist -> SetTitle       ("");
	hist -> SetLineWidth   (2);
	hist -> SetMarkerStyle (20);
	hist -> SetMarkerSize  (1);
	
	hist -> GetXaxis() -> SetTitle       (nameAxisX.Data());
	hist -> GetXaxis() -> SetTitleSize   (sizeTitle*scaleX);
	hist -> GetXaxis() -> SetTitleOffset (offsetXtitle*scaleX);
	hist -> GetXaxis() -> SetLabelSize   (sizeLabel*scaleX);
	hist -> GetXaxis() -> SetLabelOffset (offsetXlabel*scaleX);
	
	hist -> GetYaxis() -> SetTitle       (nameAxisY.Data());
	hist -> GetYaxis() -> SetTitleSize   (sizeTitle*scaleY);
	hist -> GetYaxis() -> SetTitleOffset (offsetYtitle/scaleY);
	hist -> GetYaxis() -> SetLabelSize   (sizeLabel*scaleY);
	hist -> GetYaxis() -> SetLabelOffset (offsetYlabel*scaleY);
	
	if (isComparePlot && isTopPanel)
	{
		// * Force to use exponent expression on Y axis
		hist -> GetYaxis() -> SetMaxDigits (4);
		
		// * Adjust the position of the exponent
		// * Use negative value for x-coordinate to avoid overlapping texts
		TGaxis::SetExponentOffset (-0.055, 0.01, "y");
	}
	
	if (isComparePlot && !isTopPanel)
	{
		// * If in ratio panel, choose the reasonable range for Y axis.
		hist -> GetYaxis() -> SetRangeUser (0.75, 1.25);
		hist -> GetYaxis() -> SetNdivisions (505);
	}
}


#endif
