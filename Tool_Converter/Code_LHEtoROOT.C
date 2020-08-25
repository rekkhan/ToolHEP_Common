#include <stdio.h>
#include <iostream>

void CodeAna_LHEtoROOT (int fileID)
{
	system ("mkdir -p  /afs/cern.ch/work/l/lcaophuc/Task_ZgVBS/Input/");
	
	
	std::ifstream file_input;
	TString name_filein[2] =
	{
		"/afs/cern.ch/work/l/lcaophuc/Task_GrPk/Gridpack/ZATo2NuAVBS_Private2016_LO/ZATo2NuAVBS_Private2016.lhe",
		"/afs/cern.ch/work/l/lcaophuc/Task_GrPk/Gridpack/ZATo2NuAVBS_Private2017_LO/ZATo2NuAVBS_Private2017.lhe"
	};
	
	TString name_fileout[2] =
	{
		"/afs/cern.ch/work/l/lcaophuc/Task_ZgVBS/Input/ZATo2NuAVBS_Private2016.root",
		"/afs/cern.ch/work/l/lcaophuc/Task_ZgVBS/Input/ZATo2NuAVBS_Private2017.root"
	};
	
	file_input . open (name_filein[fileID].Data());
	
	TFile *file_output = new TFile (name_fileout[fileID].Data(), "recreate");
	
	
	
	long count_event = 0;
	
	
	// * Variable
	// * - String to read the whole line
	string   my_line;
	// * - Line of event
	int   nLHE, processID;
	float eventWeight, eventScalePDF, eventQEDCoupling, eventQCDCoupling;
	// * - Line of particles
	int    lhePID_, lheStatus_, lheParent1_, lheParent2_, lheParentPID1_, lheParentPID2_, lheColour1_, lheColour2_;
	float  lhePx_ , lhePy_, lhePz_, lhePt_, lheEta_, lhePhi_, lheEnergy_, lheEt_, lheMass_, lheLife_, lheSpin_;
	vector<int>    lhePID, lheStatus, lheParent1, lheParent2, lheParentPID1, lheParentPID2;
	vector<float>  lhePx, lhePy, lhePz, lhePt, lheEta, lhePhi, lheEnergy, lheEt, lheMass, lheLife, lheSpin;
	
	
	// * Output tree
	TTree *tree_out = new TTree ("treeLHE", "Tree that stores some variable from lhe file");
	// * - Branches of event
	tree_out -> Branch("nLHE",         &nLHE);           // number of particle per event
	//tree_out -> Branch("processID",         &processID);           // Process ID (?)
	//tree_out -> Branch("eventWeight",       &eventWeight);         // Weight (?)
	//tree_out -> Branch("eventScalePDF",     &eventScalePDF);       // Scale of/for PDF (?)
	//tree_out -> Branch("eventQEDCoupling",  &eventQEDCoupling);    // Coupling constant of QED (seem to be normalized to 1)
	//tree_out -> Branch("eventQCDCoupling",  &eventQCDCoupling);    // Coupling constant of QCD
	// * - Branches of particles in an event
	tree_out -> Branch("lhePID",         &lhePID);         // Particle ID (as can be found in PGD)
	tree_out -> Branch("lheStatus",      &lheStatus);      // Status of particle, -1 is initial, +1 is final, 2 is intermediate
	tree_out -> Branch("lheParent1",     &lheParent1);     // Parent 1 of particle, the number mean the order of the lheent particle in the same event
	tree_out -> Branch("lheParent2",     &lheParent2);     // Parent 2 of particle, blah blah
	tree_out -> Branch("lheParentPID1",  &lheParentPID1);  // Particle ID of the first lheent
	tree_out -> Branch("lheParentPID2",  &lheParentPID2);  // Particle ID of the second lheent
	tree_out -> Branch("lhePx",          &lhePx);          // x component of momentum
	tree_out -> Branch("lhePy",          &lhePy);          // y component of momentum
	tree_out -> Branch("lhePz",          &lhePz);          // z component of momentum
	tree_out -> Branch("lhePt",          &lhePt);          // tranverse momentum
	tree_out -> Branch("lheEta",         &lheEta);         // Eta of momentum
	tree_out -> Branch("lhePhi",         &lhePhi);         // Phi of momentum
	tree_out -> Branch("lheEnergy",      &lheEnergy);      // Total energy
	tree_out -> Branch("lheEt",          &lheEt);          // Total tranverse energy
	tree_out -> Branch("lheMass",        &lheMass);        // Mass of particle, usually 0 for electron, muon and some other light particle
	
	
	// * Read lhe file
	while (!file_input . eof())
	{
		std :: getline (file_input, my_line);
		
		if (file_input . eof())   break;
		
		
		if (strncmp (my_line.data(), "<event", 6) == 0)
		{
			count_event ++;
			if (count_event%5000 == 0)   printf ("Event number %ld\n", count_event);
			
			file_input >> nLHE >> processID >> eventWeight >> eventScalePDF >> eventQEDCoupling >> eventQCDCoupling;
			
			lhePID        . clear ();
			lheStatus     . clear ();
			lheParent1    . clear ();
			lheParent2    . clear ();
			lheParentPID1 . clear ();
			lheParentPID2 . clear ();
			lhePx         . clear ();
			lhePy         . clear ();
			lhePz         . clear ();
			lhePt         . clear ();
			lheEta        . clear ();
			lhePhi        . clear ();
			lheEnergy     . clear ();
			lheEt         . clear ();
			lheMass       . clear ();
			//lheLife       . clear ();
			//lheSpin       . clear ();
			
			for (int i=0; i<nLHE; i++)
			{
				file_input >> lhePID_ >> lheStatus_ >> lheParent1_ >> lheParent2_ >> lheColour1_ >> lheColour2_ >> lhePx_ >> lhePy_ >> lhePz_ >> lheEnergy_ >> lheMass_ >> lheLife_ >> lheSpin_;
				
				TLorentzVector lheV4D;
				
				lheV4D . SetPxPyPzE (lhePx_, lhePy_, lhePz_, lheEnergy_);
				lhePt_   = lheV4D . Pt ();
				lheEta_  = lheV4D . Eta ();
				lhePhi_  = lheV4D . Phi ();
				lheMass_ = lheV4D . M ();
				lheEt_   = lheV4D . Et ();
				
				lhePID     . push_back (lhePID_);
				lheStatus  . push_back (lheStatus_);
				lheParent1 . push_back (lheParent1_-1);
				lheParent2 . push_back (lheParent2_-1);
				lhePx      . push_back (lhePx_);
				lhePy      . push_back (lhePy_);
				lhePz      . push_back (lhePz_);
				lhePt      . push_back (lhePt_);
				lheEta     . push_back (lheEta_);
				lhePhi     . push_back (lhePhi_);
				lheEnergy  . push_back (lheEnergy_);
				lheEt      . push_back (lheEt_);
				lheMass    . push_back (lheMass_);
			}
			
			for (int i=0; i<nLHE; i++)
			{
				lheParentPID1_ = lhePID[lheParent1[i]];
				lheParentPID2_ = lhePID[lheParent2[i]];
				
				lheParentPID1 . push_back (lheParentPID1_);
				lheParentPID2 . push_back (lheParentPID2_);
			}
			
			tree_out -> Fill ();
		}
	}
	
	printf ("There are %ld events in the file\n", count_event);
	
	file_output -> cd ();
	tree_out -> Write("", TObject::kOverwrite);
}
