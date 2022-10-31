Double_t funcy(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t y = par[0];

    Double_t Ee = 10;
    Double_t Ep = 100;
    Double_t s = 4 * Ee * Ep;

    return s * y * xx;
}

Double_t funcEprime(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t Eprime = par[0];

    Double_t Ee = 10;
    Double_t Ep = 100;
    Double_t wheny1 = 4 * Ee * Ep * 1 * xx; // the 1 is there because of y

    Double_t res = (2 * Ee * (Ee - Eprime) * xx) / ((Ee/Ep) - xx);
    if (res > wheny1) { return wheny1; }
    return res;
}

void electronmethod() {
    gBenchmark->Start("electronmethod");

    // Creating the functions via TF1
    TF1 *fromy = new TF1("x vs Q^2 from y", funcy, 1e-4, 1, 1);
    fromy->SetLineColor(3);
    double ys[3] = {1, 0.1, 0.01};

    TF1 *fromEprime = new TF1("x vs Q^2 from E'", funcEprime, 1e-4, 1, 1);
    fromEprime->SetLineColor(2);
    double Eprimes[9] = {2, 4, 6, 8, 10, 12, 20, 40, 100};

    // Create the plot
    TCanvas *c1 = new TCanvas("c1", "x vs Q^2", 200, 10, 700, 900);

    auto pad1 = new TPad("pad1", "The pad with the x vs Q2 functions", 0.05, 0.05, 0.95, 0.95);
    pad1->Draw();

    pad1->cd();
    pad1->SetLogx();
    pad1->SetLogy();

    pad1->GetFrame()->SetBorderMode(-1);
    pad1->GetFrame()->SetBorderSize(5);

    // Draw the x vs Q^2 given y function
    for (int i = 0; i < 3; i++) {
        fromy->SetParameter(0, ys[i]);
        auto graphtype = "";
        if (i != 0) { graphtype = "SAME"; }
        fromy->DrawCopy(graphtype);
    }

    // Draw the x vs Q^2 given E prime function
    for (int i = 0; i < 9; i++) {
        fromEprime->SetParameter(0, Eprimes[i]);
        fromEprime->DrawCopy("SAME");
    }

    c1->Update();

    gBenchmark->Show("xvsq2");
}
