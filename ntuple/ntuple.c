void ntuple() {
    gBenchmark->Start("ntuple");

    TFile *f = new TFile("pythia8NCDIS_10x100_minQ2=100_beamEffects_xAngle=-0.025_hiDiv_vtxfix_1_000.0001.eicrecon.tree.edm4eic.root");
    TTree *evTree = new TTree("events;12", "Events 12 Data");

    evTree->Print("toponly");

    gBenchmark->Show("ntuple");
}
