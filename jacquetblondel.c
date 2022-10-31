Double_t frompt(Double_t *x, Double_t *par) {
    Double_t xx = x[0];
    Double_t pt = par[0];

    Double_t Ee = 10;
    Double_t Ep = 100;
    Double_t s = 4 * Ee * Ep;

    return (s * xx + TMath::sqrt(s * s * xx * xx + 4 * s * xx * pt * pt)) / 2;
}

void jacquetblondel() {

}
