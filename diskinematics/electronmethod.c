Double_t funcy(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t y = par[0];

    Double_t Ee = par[1];
    Double_t Ep = par[2];
    Double_t s = 4 * Ee * Ep;

    return s * y * xx;
}

Double_t funcEprime(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t Eprime = par[0];

    Double_t Ee = par[1];
    Double_t Ep = par[2];
    Double_t wheny1 = 4 * Ee * Ep * 1 * xx; // the 1 is there because of y

    Double_t res = (2 * Ee * (Ee - Eprime) * xx) / ((Ee/Ep) - xx);
    if (res > wheny1) { return wheny1; }
    return res;
}

Double_t funcThetaprime(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t thetaprime = par[0] * 3.1415926535 / 180;

    Double_t Ee = par[1];
    Double_t Ep = par[2];
    Double_t s = 4 * Ee * Ep;

    return (s * xx) / (1 + ((Ep * xx) * (1 - TMath::Cos(thetaprime)) / (Ee * (1 + TMath::Cos(thetaprime)))));
}

void DrawText(Double_t pose[3], char *ctext,  char *cgraph) {
    string text = string(ctext);
    string graph = string(cgraph);

    int color = kBlack;
    if (graph == "fromy") {
        color = kGreen;
    } else if (graph == "fromEprime") {
        color = kRed;
    } else if (graph == "fromThetaprime") {
        color = kBlack;
    }

    TText *t1 = new TText(pose[0], pose[1], ctext);
    t1->SetTextColor(color);
    t1->SetTextFont(43);
    if (graph == "fromy" || graph == "fromEprime") {
        t1->SetTextAlign(22);
        t1->SetTextSize(10);
        t1->SetTextAngle(pose[2]);
    } else if (graph == "fromThetaprime") {
        t1->SetTextAlign(22);
        t1->SetTextSize(20);
        t1->SetTextAngle(pose[2]);
    }

    t1->Draw();
}

void genericElectronMethod(Double_t Ee, Double_t Ep) {
    // Creating the functions via TF1
    TF1 *fromy = new TF1("x vs Q^2 via the electron method", funcy, 1e-5, 1, 3);
    fromy->SetParameter(1, Ee);
    fromy->SetParameter(2, Ep);
    fromy->SetLineColor(3);
    fromy->SetLineStyle(9);
    double ys[3] = {1, 0.1, 0.01};

    TF1 *fromEprime = new TF1("x vs Q^2 via the electron method", funcEprime, 1e-5, 1, 3);
    fromEprime->SetParameter(1, Ee);
    fromEprime->SetParameter(2, Ep);
    fromEprime->SetLineColor(2);
    double Eprimes[9] = {2, 4, 6, 8, 10, 12, 20, 40, 100};

    TF1 *fromThetaprime = new TF1("x vs Q^2 via the electron method", funcThetaprime, 1e-5, 1, 3);
    fromThetaprime->SetParameter(1, Ee);
    fromThetaprime->SetParameter(2, Ep);
    fromThetaprime->SetFillStyle(1001);
    double thetaprimes[4] = {4, 33, 147, 176};
    int thetacolors[4] = {38, 41, 30, 40};

    // Text
    Double_t EprimePoses[3][3] = {
        {0.03, 10, 45},
        {0.12, 10, 90},
        {0.3, 40, -20}
    };

    Double_t YPoses[3][3] = {
        {0.01, 60, 35},
        {0.01, 3, 35},
        {0.07, 2, 35}
    };

    Double_t ThetaprimePoses[2][3] = {
        {0.6, 0.7, 0},
        {0.6, 45, 0}
    };

    // Create the plot
    auto name = "c10x100";
    if (Ee == 10 && Ep == 100) { name = "c10x100"; }
    else if (Ee == 5 && Ep == 41) { name = "c5x41"; }
    else if (Ee == 18 && Ep == 275) { name = "c18x275"; }
    TCanvas *c1 = new TCanvas(name, "x vs Q^2", 200, 10, 700, 900);

    auto pad1 = new TPad("pad1", "The pad with the x vs Q2 functions", 0.05, 0.05, 0.95, 0.95);
    pad1->Draw();

    pad1->cd();
    pad1->SetLogx();
    pad1->SetLogy();

    for (int i = 0; i < 4; i++) {
        fromThetaprime->SetParameter(0, thetaprimes[i]);
        fromThetaprime->SetLineColor(thetacolors[i]);
        fromThetaprime->SetFillColor(thetacolors[i]);
        auto graphtype = "";
        if (i != 0) { graphtype = "SAME"; }
        fromThetaprime->DrawCopy(graphtype);
    }

    // Draw the x vs Q^2 given E prime function
    for (int i = 0; i < 9; i++) {
        fromEprime->SetParameter(0, Eprimes[i]);
        fromEprime->DrawCopy("SAME");
    }

    // Draw the x vs Q^2 given y function
    for (int i = 0; i < 3; i++) {
        fromy->SetParameter(0, ys[i]);
        fromy->DrawCopy("SAME");
    }

    if (Ee == 10 && Ep == 100) {
        DrawText(EprimePoses[0], "E' = 8 GeV", "fromEprime");
        DrawText(EprimePoses[1], "E' = 10 GeV", "fromEprime");
        DrawText(EprimePoses[2], "E' = 12 GeV", "fromEprime");

        DrawText(YPoses[0], "y = 1", "fromy");
        DrawText(YPoses[1], "y = 0.1", "fromy");
        DrawText(YPoses[2], "y = 0.01", "fromy");

        DrawText(ThetaprimePoses[0], "176\370", "fromThetaprime");
        DrawText(ThetaprimePoses[1], "147\370", "fromThetaprime");
    }

    c1->Update();
    if (Ee == 10 && Ep == 100) {
        c1->SaveAs("plots/electronmethod10x100.jpg");
    } else if (Ee == 5 && Ep == 41) {
        c1->SaveAs("plots/electronmethod5x41.jpg");
    } else if (Ee == 18 && Ep == 275) {
        c1->SaveAs("plots/electronmethod18x275.jpg");
    }
}

void electronmethod() {
    gBenchmark->Start("electronmethod");

    genericElectronMethod(10, 100);
    genericElectronMethod(5, 41);
    genericElectronMethod(18, 275);

    gBenchmark->Show("electronmethod");
}
