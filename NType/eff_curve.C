void eff_curve(){
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	
	const int num_en = 22;
	double solidAngle[3] = {0.303283,.349979,.334437};
	int energy[num_en] = {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500,8000};
	double en_MeV[num_en];
	double en_err[num_en] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	TH1F *histo[3][num_en]; 
	double gross_hits[3][num_en];
	double hits_err[3][num_en];
	double eff[3][num_en];
	double eff_err[3][num_en];
	TGraphErrors *gEff[3];
	
	TH1F *histoPb[3][num_en]; 
	double gross_hitsPb[3][num_en];
	double hits_errPb[3][num_en];
	double effPb[3][num_en];
	double eff_errPb[3][num_en];
	TGraphErrors *gEffPb[3];

	TH1F *histoCu[3][num_en]; 
	double gross_hitsCu[3][num_en];
	double hits_errCu[3][num_en];
	double effCu[3][num_en];
	double eff_errCu[3][num_en];
	TGraphErrors *gEffCu[3];

	for (int j=0; j<3; j++){
		solidAngle[j] = solidAngle[j]*1./(4*3.141592);
		for (int i=0; i<num_en; i++){
			

			gross_hits[j][i] = 0;
			hits_err[j][i] = 0;
			eff[j][i] = 0;
			eff_err[j][i] = 0;
			gross_hitsPb[j][i] = 0;
			hits_errPb[j][i] = 0;
			effPb[j][i] = 0;
			eff_errPb[j][i] = 0;
			gross_hitsCu[j][i] = 0;
			hits_errCu[j][i] = 0;
			effCu[j][i] = 0;
			eff_errCu[j][i] = 0;

			en_MeV[i] = energy[i]/1000.;
			histo[j][i] = new TH1F(Form("histo%i%i",j,i),"histo",20000,0,10000);
			histoPb[j][i] = new TH1F(Form("histoPb%i%i",j,i),"histoPb",20000,0,10000);
			histoCu[j][i] = new TH1F(Form("histoCu%i%i",j,i),"histoCu",20000,0,10000);
			ifstream infile;
			ifstream infilePb;
			ifstream infileCu;
			if (j==0){
				infile.open(Form("D1_eff/eff%i.dat",energy[i]));
				infilePb.open(Form("D1_eff_Pb/eff%i.dat",energy[i]));
				infileCu.open(Form("D1_eff_Cu/eff%i.dat",energy[i]));
			}
			if (j==1){
				infile.open(Form("D3_eff/eff%i.dat",energy[i]));
				infilePb.open(Form("D3_eff_Pb/eff%i.dat",energy[i]));
				infileCu.open(Form("D3_eff_Cu/eff%i.dat",energy[i]));
			}
			if (j==2){
				infile.open(Form("D4_eff/eff%i.dat",energy[i]));
				infilePb.open(Form("D4_eff_Pb/eff%i.dat",energy[i]));
				infileCu.open(Form("D4_eff_Cu/eff%i.dat",energy[i]));
			}
		
			for (int k=0; k<20000; k++){
				int counts = 0;
				infile >> counts;
				histo[j][i]->SetBinContent(k,counts);
				counts = 0;
				infilePb >> counts;
				histoPb[j][i]->SetBinContent(k,counts);
				counts = 0;
				infileCu >> counts;
				histoCu[j][i]->SetBinContent(k,counts);
				//cout << i << "  " << counts << endl;
			}
			infile.close();
			infileCu.close();
			infilePb.close();

			//new TCanvas();
			//histo[j][i]->Draw();
			

			histo[j][i]->GetXaxis()->SetTitle("E [keV]");
			histo[j][i]->GetYaxis()->SetTitle("counts");
			histo[j][i]->SetTitle();
			
			gross_hits[j][i] = histo[j][i]->GetBinContent(2*energy[i]) + histo[j][i]->GetBinContent(2*energy[i]-1);
			hits_err[j][i] = sqrt(gross_hits[j][i]);
		
			gross_hitsPb[j][i] = histoPb[j][i]->GetBinContent(2*energy[i]) + histoPb[j][i]->GetBinContent(2*energy[i]-1);
			hits_errPb[j][i] = sqrt(gross_hitsPb[j][i]);
		
			gross_hitsCu[j][i] = histoCu[j][i]->GetBinContent(2*energy[i]) + histoCu[j][i]->GetBinContent(2*energy[i]-1);
			hits_errCu[j][i] = sqrt(gross_hitsCu[j][i]);
		
			//cout << en_MeV[i] << "  " << gross_hits[j][i] << "  " << hits_err[j][i] << endl;

		}
	}

	for (int j=0; j<3; j++){
		for (int i=0; i<num_en; i++){
			eff[j][i] = gross_hits[j][i]*solidAngle[j]/1000000.;
			eff_err[j][i] = hits_err[j][i]*solidAngle[j]/1000000.;
			effPb[j][i] = gross_hitsPb[j][i]*solidAngle[j]/1000000.;
			eff_errPb[j][i] = hits_errPb[j][i]*solidAngle[j]/1000000.;
			effCu[j][i] = gross_hitsCu[j][i]*solidAngle[j]/1000000.;
			eff_errCu[j][i] = hits_errCu[j][i]*solidAngle[j]/1000000.;
		}
	}

	new TCanvas();
	for (int j=0; j<3; j++){
		gEff[j] = new TGraphErrors(num_en,en_MeV,eff[j],en_err,eff_err[j]);
		gEff[j]->SetMarkerStyle(20);
		gEff[j]->SetTitle();
		gEff[j]->GetXaxis()->SetTitle("E [MeV]");
		gEff[j]->GetYaxis()->SetTitle("Eff");
		gEff[j]->SetMinimum(0);
	}

	gEff[0]->Draw("AP");
	gEff[1]->Draw("P");
	gEff[2]->Draw("P");
	gEff[0]->SetMarkerColor(8);
	gEff[1]->SetMarkerColor(2);
	gEff[2]->SetMarkerColor(9);

	TCanvas *cAbs = new TCanvas("cAbs","Absorbers",1000,500);
	cAbs->Divide(3);
	for (int j=0; j<3; j++){
		cAbs->cd(j+1);
		gEffPb[j] = new TGraphErrors(num_en,en_MeV,effPb[j],en_err,eff_errPb[j]);
		gEffPb[j]->SetMarkerStyle(20);
		gEffPb[j]->SetTitle();
		gEffPb[j]->GetXaxis()->SetTitle("E [MeV]");
		gEffPb[j]->GetYaxis()->SetTitle("Eff");
		gEffPb[j]->SetMinimum(0);

		gEffCu[j] = new TGraphErrors(num_en,en_MeV,effCu[j],en_err,eff_errCu[j]);
		gEffCu[j]->SetMarkerStyle(20);
		gEffCu[j]->SetTitle();
		gEffCu[j]->GetXaxis()->SetTitle("E [MeV]");
		gEffCu[j]->GetYaxis()->SetTitle("Eff");
		gEffCu[j]->SetMinimum(0);

		gEff[j]->SetMarkerColor(8);
		gEffPb[j]->SetMarkerColor(9);
		gEffCu[j]->SetMarkerColor(2);

		gEff[j]->Draw("AP");
		gEffPb[j]->Draw("P");
		gEffCu[j]->Draw("P");
	}


}
