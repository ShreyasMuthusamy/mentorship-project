Double_t funcpt(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t pt = par[0];

    Double_t Ee = 10;
    Double_t Ep = 100;
    Double_t s = 4 * Ee * Ep;

    return (s * xx + TMath::Sqrt(s * s * xx * xx + 4 * s * xx * pt * pt)) / 2;
}

Double_t funcEpz(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t Epz = par[0];

    Double_t Ee = 10;
    Double_t Ep = 100;
    Double_t s = 4 * Ee * Ep;

    Double_t y = Epz / (2 * Ee);

    return s * y * xx;
}

void jacquetblondel() {
    gBenchmark->Start("jacquetblondel");

    TF1 *frompt = new TF1("x vs Q^2 in variable pt; x; Q^2", funcpt,
        1e-5, 1, 1);
    Double_t pts[5] = {1, 2, 4, 10, 20};

    TF1 *fromEpz = new TF1("x vs Q^2 in variable E-pz", funcEpz,
        1e-5, 1, 1);
    fromEpz->SetLineColor(3);
    Double_t Epzs[6] = {0.01, 0.1, 1, 10, 20};

    TCanvas *c1 = new TCanvas("c1", "The Jacquet-Blondel Method",
        200, 10, 700, 900);

    TPad *pad = new TPad("pad", "The pad with the function",
        0.05, 0.05, 0.95, 0.95);
    pad->Draw();

    pad->cd();
    pad->SetLogx();
    pad->SetLogy();

    TFrame *frame = pad->GetFrame();
    frame->SetBorderMode(-1);
    frame->SetBorderSize(5);

    for (int i = 0; i < 5; i++) {
        frompt->SetParameter(0, pts[i]);
        auto graphType = "";
        if (i != 0) { graphType = "SAME"; }
        frompt->DrawCopy(graphType);
    }

    for (int i = 0; i < 6; i++) {
        fromEpz->SetParameter(0, Epzs[i]);
        fromEpz->DrawCopy("SAME");
    }

    c1->Update();
}
