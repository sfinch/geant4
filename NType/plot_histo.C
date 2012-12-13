void plot_histo(){
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	
	TH1F *histo = new TH1F("histo","histo",20000,0,10000);
	ifstream infile;
	infile.open("simulated.dat");
	//infile.open("sim2.dat");
	
	for (int i=0; i<20000; i++){
		int counts = 0;
		infile >> counts;
		histo->SetBinContent(i,counts);
		//cout << i << "  " << counts << endl;
	}
	
	histo->Draw();
	histo->GetXaxis()->SetTitle("E [keV]");
	histo->GetYaxis()->SetTitle("counts");
	histo->SetTitle();

}
