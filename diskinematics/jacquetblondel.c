TGraph *funcpt(Double_t pt, Double_t Ee, Double_t Ep) {
    TGraph *frompt = new TGraph();
    frompt->SetLineColor(2);

    Double_t x, q2, q2_0;
    Double_t s = 4 * Ee * Ep;

    q2_0 = pt*pt;
    q2 = q2_0;

    for (int i = 0; i < 100000; i++) {
        q2 += 0.0001 * q2;
        x = (q2/s) / (1 - (pt*pt)/q2);

        frompt->SetPoint(i, x, q2);

        if (q2 > 1e+4) { break; }
    }

    return frompt;
}

Double_t funcEpz(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t Epz = par[0];

    Double_t Ee = par[1];
    Double_t Ep = par[2];
    Double_t s = 4 * Ee * Ep;

    Double_t y = Epz / (2 * Ee);

    return s * y * xx;
}

Double_t funcF(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t f = par[0];

    Double_t Ee = par[1];
    Double_t Ep = par[2];
    Double_t s = 4 * Ee * Ep;

    Double_t numer = s * xx * (1 - (4 * Ee * f) / (s * xx));
    Double_t denom = (4 * Ee * Ee) / (s * xx) - 1;

    return (numer / denom < s * 1 * xx) ? numer / denom : s * 1 * xx;
}

Double_t funcGamma(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t gamma = par[0] * 3.1415926 / 180;

    Double_t Ee = par[1];
    Double_t Ep = par[2];
    Double_t s = 4 * Ee * Ep;

    Double_t a = (4 * Ee * Ee) * (sin(gamma) * sin(gamma));
    Double_t b = 1 / (s * xx);
    Double_t c = (s * s * xx * xx) * (1 - cos(gamma)) * (1 - cos(gamma));
    Double_t q2 = 1 / ((a/c) + b);

    return (0 < q2 && q2 < s * 1 * xx) ? q2 : s * 1 * xx;
}

void genericJacquetBlondel(Double_t Ee, Double_t Ep) {
    Double_t pts[5] = {1, 2, 4, 10, 20};

    TF1 *fromEpz = new TF1("x vs Q^2 in variable E-pz", funcEpz,
        1e-5, 1, 3);
    fromEpz->SetParameter(1, Ee);
    fromEpz->SetParameter(2, Ep);
    fromEpz->SetLineColor(3);
    Double_t Epzs[6] = {0.01, 0.1, 1, 10, 2 * Ee, 20};
    if (2 * Ee < 20) { Epzs[5] = 0; }

    TF1 *fromF = new TF1("x vs Q^2 in variable F", funcF, 1e-5, 1, 3);
    fromF->SetParameter(1, Ee);
    fromF->SetParameter(2, Ep);
    fromF->SetLineColor(4);
    Double_t Fs[6] = {1, 5, 10, 20, 40, 100};

    TF1 *fromGamma = new TF1("x vs Q^2 in variable gamma", funcGamma,
        1e-5, 1, 3);
    fromGamma->SetParameter(1, Ee);
    fromGamma->SetParameter(2, Ep);
    fromGamma->SetFillStyle(1001);
    Double_t gammas[4] = {176, 147, 33, 4};
    int gammacolors[4] = {38, 41, 30, 40};

    auto name = "";
    if (Ee == 10 && Ep == 100) { name = "c10x100"; }
    else if (Ee == 5 && Ep == 41) { name = "c5x41"; }
    else if (Ee == 18 && Ep == 275) { name = "c18x275"; }
    TCanvas *c1 = new TCanvas(name, "The Jacquet-Blondel Method",
        200, 10, 700, 900);

    TPad *pad = new TPad("pad", "The pad with the function",
        0.05, 0.05, 0.95, 0.95);
    pad->Draw();

    pad->cd();
    pad->SetLogx();
    pad->SetLogy();

    TH1F *frame = pad->DrawFrame(1e-5, 1e-2, 1, 1e+4,
        "x vs Q^2 using Jacquet-Blondel method; x; Q^2");

    for (int i = 0; i < 4; i++) {
        fromGamma->SetParameter(0, gammas[i]);
        fromGamma->SetLineColor(gammacolors[i]);
        fromGamma->SetFillColor(gammacolors[i]);
        fromGamma->DrawCopy("SAME");
    }

    for (int i = 0; i < 6; i++) {
        fromF->SetParameter(0, Fs[i]);
        fromF->DrawCopy("SAME");
    }

    for (int i = 0; i < 5; i++) {
        auto graphType = "";
        if (i != 0) { graphType = "SAME"; }
        funcpt(pts[i], Ee, Ep)->Draw(graphType);
    }

    for (int i = 0; i < 6; i++) {
        fromEpz->SetParameter(0, Epzs[i]);
        fromEpz->DrawCopy("SAME");
    }

    c1->Update();
    if (Ee == 10 && Ep == 100) {
        c1->SaveAs("plots/jacquetblondel10x100.jpg");
    }
    else if (Ee == 5 && Ep == 41) {
        c1->SaveAs("plots/jacquetblondel5x41.jpg");
    }
    else if (Ee == 18 && Ep == 275) {
        c1->SaveAs("plots/jacquetblondel18x275.jpg");
    }
}

void jacquetblondel() {
    gBenchmark->Start("jacquetblondel");

    genericJacquetBlondel(10, 100);
    genericJacquetBlondel(5, 41);
    genericJacquetBlondel(18, 275);

    gBenchmark->Show("jacquetblondel");
}
