


drawLD(const char *name){
  system(Form("tail --line=84 stats/%s.stat >temp",name));
  system(Form("tail --line=84 stats/%s.status >temp2",name));
  FILE *f = fopen("temp","r");
  FILE *f2 = fopen("temp2","r");


  Int_t in,out,sec;
  Int_t n=0;
  Int_t n2=0;

  Float_t x[1000],x2[1000],y[1000],y2[1000],xmax;

  Int_t nld=0;

  while(fscanf(f,"%i %i %i",&sec,&in,&out)==3){
    x[n] = sec;
    y[n] = in/1024./1024;
    y2[n] = out/1024./1024;

    printf("%f %f %f\n",x[n],y[n],y2[n]);

    n++;
  }

  char status[100];
  while(fscanf(f2,"%i %s",&sec,status)==2){
    x2[n2] = sec;
    if(status[0]=='t') y2[n2]=1;
    else y2[n2]=0;

    n2++;
  }


  if(n > 480) nld = n - 480;


  TCanvas *c = new TCanvas("canvas","canvas",1200,900);
  c->Divide(1,2);
  c->cd(1);
  TGraph *g = new TGraph(n-nld,&(x[nld]),&(y[nld]));
  g->Draw("AL");
  g->SetTitle(Form("%s;;traffic IN (MB/s)",name));
  g->GetXaxis()->SetTimeFormat("%d/%b %H:%M");
  g->GetXaxis()->SetTimeDisplay(1);
  g->SetMinimum(0);
  c->cd(2);
  TGraph *g2 = new TGraph(n2,x2,y2);
  g2->Draw("AL");
  g2->SetTitle(Form("%s;;status",name));
  g2->GetXaxis()->SetTimeFormat("%d/%b %H:%M");
  g2->GetXaxis()->SetTimeDisplay(1);
  g2->SetMinimum(-0.1);
  g2->SetMaximum(1.1);

  c->Print(Form("%sTraffic.png",name));
}

