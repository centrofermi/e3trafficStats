


Int_t confronta(const char *a,const char *b);

drawtest(const char *name="BOLO-04",const char *date="2017-01-01",Int_t timestart=-1){
  Int_t queued=0;
  Float_t sizequeued=0;
  FILE *fqueued = fopen(Form("stats/%s.queued",name),"r");
  if(fqueued){
     fscanf(fqueued,"%i",&queued);
     fscanf(fqueued,"%f",&sizequeued);
    fclose(fqueued);
  }

  system(Form("rm temp;ls /data/%s/data/%s/|grep bin|tail --line=1 >temp",name,date));
  FILE *flast = fopen("temp","r");
  char lastfile[100];
  sprintf(lastfile,"");
  if(flast) fscanf(flast,"%s",lastfile);
  if(flast) fclose(flast);

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
    printf("%i %f %f\n",n,time,timestart);
    if(time < timestart - 100000000) getchar();

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
return;
  Float_t x2[10000],y2[10000];

  printf("n=%i\n",n);

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
  c2->Print(Form("%sTrafficIN.png",name));

  TCanvas *c3 = new TCanvas();
  g = new TGraph(n,x,z);
  g->SetTitle(Form("%s;date;Traffic OUT [MB/s]",name));
  g->Draw("AL");
  g->GetXaxis()->SetTimeDisplay(1);
  g->GetXaxis()->SetTimeFormat("%d/%m/%yh%H%F1970-01-01 00:00:00s0");
  g->GetXaxis()->SetNdivisions(406);
  g->GetXaxis()->SetTitleSize(0.05);
  g->GetYaxis()->SetTitleSize(0.05);
  c3->Print(Form("%sTrafficOUT.png",name));

  char *color;
  char *color2;

  if(eff > 0.4) color="green";
  else if(eff > 0.1) color="orange";
  else color="red";

  if(megabyte > 5 || queued) color2="green";
  else if(megabyte > 2) color2="orange";
  else color2="red";

  system(Form("rm temp;cat /data/%s/data/%s/ErrorLog.txt|grep \"-\"|tail --line=1 >temp",name,date));
  FILE *ferr = fopen("temp","r");
  char mess[10][100];

  sprintf(mess[0],"");
  sprintf(mess[1],"");
  sprintf(mess[2],"");
  sprintf(mess[3],"");
  sprintf(mess[4],"");
  sprintf(mess[5],"");
  sprintf(mess[6],"");
  sprintf(mess[7],"");
  sprintf(mess[8],"");
  sprintf(mess[9],"");

  if(ferr) fscanf(ferr,"%s %s %s %s %s %s %s %s %s %s",mess[0],mess[1],mess[2],mess[3],mess[4],mess[5],mess[6],mess[7],mess[8]);
  if(ferr) fclose(ferr);

  if(!confronta(lastfile,mess[2])){
    sprintf(mess[0],"");
    sprintf(mess[1],"");
    sprintf(mess[2],"");
    sprintf(mess[3],"");
    sprintf(mess[4],"");
    sprintf(mess[5],"");
    sprintf(mess[6],"");
    sprintf(mess[7],"");
    sprintf(mess[8],"");
    sprintf(mess[9],"");
  }

  for(Int_t i=0;i<10;i++){
    Int_t j=0;
    while(mess[i][j] != '\0'){
       if(mess[i][j] == '<' || mess[i][j] == '(') mess[i][j] = '[';
       if(mess[i][j] == '>' || mess[i][j] == ')') mess[i][j] = ']';
       if(mess[i][j] == '/') mess[i][j] = '-';

       j++;
    }
  } 

  system(Form("echo \"%s %4.2f %s %.1f %s %i %.1f %s %s %s %s %s %s %s %s %s %s\" >>trafficSummary.txt",name,eff+0.00499,color,megabyte,color2,queued,sizequeued,mess[0],mess[1],mess[2],mess[3],mess[4],mess[5],mess[6],mess[7],mess[8],mess[9]));
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
