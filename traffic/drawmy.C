

Int_t confronta(const char *a,const char *b);

drawmy(const char *name="total",Int_t timestart=-864000){
  Int_t queued=0;
  FILE *fqueued = fopen(Form("stats/%s.queued",name),"r");
  if(fqueued){
     fscanf(fqueued,"%i",&queued);
    fclose(fqueued);
  }


  timestart -= 86400;
  // system(Form("cat %s.status |grep \'true\\|false\' >%sc.status",name,name));
  // system(Form("cat %s.stat |grep -v -E '[^ ]{10}$' >%sc.stat",name,name));
  FILE *f = fopen(Form("stats/%s.status",name),"r");
  FILE *f2 = fopen(Form("stats/%s.stat",name),"r");
  
  Float_t eff=0,megabyte=0;
  Int_t time,istatus;
  char status[100];
  Int_t n=0;
  Float_t x[1000000];
  Float_t y[1000000];
  Float_t z[1000000];
  while(fscanf(f,"%i %s",&time,status)==2){
    istatus=0;
    if(status[0]=='t') istatus=1;
    x[n] = time;
    y[n] = istatus;
    if(time > timestart+86400-3600*2){
       n++;
       if(istatus){
          eff+=1;
       }
    }
  }
  eff /= n;
  printf("n=%i\n",n);
  TCanvas *c1 = new TCanvas();
  TGraph *g = new TGraph(n,x,y);
  g->SetTitle(Form("%s (eff=%f);date;status",name,eff));
  g->Draw("AL");
  g->GetXaxis()->SetTimeDisplay(1);
  g->GetXaxis()->SetTimeFormat("%H:%M%F1970-01-01 00:00:00s0");
  g->GetXaxis()->SetNdivisions(408);
  g->GetXaxis()->SetTitleSize(0.05);
  g->GetYaxis()->SetTitleSize(0.05);
  g->SetMinimum(-0.05);
  g->SetMaximum(1.05);

  c1->Print(Form("%sStatus.png",name));

  TCanvas *c2 = new TCanvas();

  if(confronta(name,"total")) c2=new TCanvas("total","total",1200,400);
  n=0;
  Int_t in,out;

  Float_t dtime=300;
  while(fscanf(f2,"%i %i %i",&time,&in,&out)==3 && n < 200000){
    if(n) dtime = time - x[n-1];
    //printf("%i %i %i\n",dtime,in,out);
    x[n] = time;
    y[n] = in/dtime*1E-6;
    z[n] = out/dtime*1E-6;
    if(dtime > 0 && x[n] >=0 && y[n] >= 0&& z[n] >= 0 && time > timestart){
        n++;
        if(time > timestart+86400-5400) megabyte += in*1E-6;
    }
  }

  Float_t x2[10000],y2[10000];

  Int_t rebin=20;
  if(timestart > 0) rebin = 1;
  Int_t n2=n/rebin;
  for(Int_t i=0;i <n;i++){
    x2[i/rebin] = x[i];
    y2[i/rebin] += y[i]/rebin;
  }
  
  printf("n=%i\n",n);
  g = new TGraph(n2,x2,y2);
  g->SetTitle(Form("%s;date;Traffic IN [MB/s]",name));
  g->Draw("AL");
  g->GetXaxis()->SetTimeDisplay(1);
  g->GetXaxis()->SetTimeFormat("%d/%m/%yh%H%F1970-01-01 00:00:00s0");
  g->GetXaxis()->SetNdivisions(406);
  g->GetXaxis()->SetTitleSize(0.05);
  g->GetYaxis()->SetTitleSize(0.05);
  //c2->SetLogy();

  if(confronta(name,"total")) g->Fit("pol0");
  c2->Print(Form("%sTrafficINsum.png",name));

  TFile *ff = new TFile("out.root","RECREATE");
  c2->Write();
  ff->Close();

  TCanvas *c3 = new TCanvas();
  g = new TGraph(n,x,z);
  g->SetTitle(Form("%s;date;Traffic OUT [MB/s]",name));
  g->Draw("AL");
  g->GetXaxis()->SetTimeDisplay(1);
  g->GetXaxis()->SetTimeFormat("%d/%m/%yh%H%F1970-01-01 00:00:00s0");
  g->GetXaxis()->SetNdivisions(406);
  g->GetXaxis()->SetTitleSize(0.05);
  g->GetYaxis()->SetTitleSize(0.05);
  c3->Print(Form("%sTrafficOUTsum.png",name));

}

Int_t confronta(const char *a,const char *b){
  Int_t status=1,i=0;
  while(a[i]!='\0' && b[i] != '\0'){
    if(a[i] != b[i]) status=0;
    i++;
  }
  if(a[i] != b[i]) status=0;

  return status;
}
