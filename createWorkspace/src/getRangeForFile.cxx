#include "include/reBinHistogram.h"

using namespace std;

void getRangeForFile(std::string& rootFileName, int &firstBin, int &lastBin)    
{
    //=======================================================
    /*
      Get access into each directory 
      1. create a list of histograms
      2. make new histograms for each bin
      3. remember to extract the bin errors
    */

    //Make a vector of new root file names with different mass points
    TFile *fileOriginal = new TFile(rootFileName.c_str(),"READ");

    TKey *keyDir,*keyHist;
    TDirectoryFile *dir;
    string classDir, classHist;
    
    TIter next(fileOriginal->GetListOfKeys());
    double threshold = 0.1; //for SR2
    //double threshold = 0.00001; //for SR1

    //loop over directories to get firstBin, lastBin with > 0.1 yield

    while ( (keyDir = (TKey*) next() ) )
	{
	    classDir = keyDir->GetClassName();
	    if (classDir != "TDirectoryFile") continue;
	    dir = (TDirectoryFile*) keyDir->ReadObj();
	    string tdirectoryName = dir->GetName();

	    // if(tdirectoryName != "Nominal") continue;//Loan: this is for current low mass due to problematic input
	    
	    TIter nextHist(dir->GetListOfKeys());
	    while( (keyHist = (TKey*) nextHist() ) )
		{
		    classHist = keyHist->GetClassName();
		    if(classHist != "TH1D" && classHist != "TH1F") continue;
		    
		    TH1D* hist = (TH1D*) keyHist->ReadObj();
		    const char* histName = hist->GetName();

		    cout << "Histo " << histName << "\n";
		    getNonZeroBinRange(hist, threshold, firstBin, lastBin);
		    cout << firstBin << ", " << lastBin << endl;
		    hist->Delete();
		}
	}
    cout << "Final firstBin, lastBin for " << rootFileName << " are " << firstBin << ", " << lastBin << endl;
    
    fileOriginal->Close();
    fileOriginal->Delete();
    
}



