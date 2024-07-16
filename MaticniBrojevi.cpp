#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <iomanip>
#include <algorithm>

int BrojCifara(long long int JMBG) {
  int n = 0;
  while (JMBG != 0) {
    JMBG /= 10;
    n++;
  }
  return n;
}

bool RazloziMaticni(long long int JMBG, int &dan, int &mjesec, int &godina,
                    int &regija, int &kod, int &kontrola) {
  long long int pom = JMBG;
  kontrola = JMBG % 10;
  JMBG /= 10;
  kod = JMBG % 1000;
  JMBG /= 1000;
  regija = JMBG % 100;
  JMBG /= 100;
  godina = JMBG % 1000;
  if(godina >= 0 && godina <= 21) godina += 2000;
  else if(godina >= 921 && godina <= 999) godina += 1000;
  JMBG /= 1000;
  mjesec = JMBG % 100;
  JMBG /= 100;
  dan = JMBG % 100;
  int c[13];
  for (int i = 0; i < 13; i++) {
    c[12 - i] = pom % 10;
    pom /= 10;
  }
  int test =
      11 - (7 * (c[0] + c[6]) + 6 * (c[1] + c[7]) + 5 * (c[2] + c[8]) +
            4 * (c[3] + c[9]) + 3 * (c[4] + c[10]) + 2 * (c[5] + c[11])) %
               11;
  if (test == 11)
    test = 0;
  if (test == 10 || test != kontrola)
    return false;
  return true;
}

bool SloziMaticni(long long int &JMBG, int dan, int mjesec, int godina,
                  int regija, int kod) {
  JMBG = dan;
  JMBG *= 100;
  JMBG += mjesec;
  JMBG *= 1000;
  JMBG += godina;
  JMBG *= 100;
  JMBG += regija;
  JMBG *= 1000;
  JMBG += kod;
  int c[12];
  auto pom = JMBG;
  for (int i = 1; i <= 12; i++) {
    c[12 - i] = pom % 10;
    pom /= 10;
  }
  int kontrolna =
      11 - (7 * (c[0] + c[6]) + 6 * (c[1] + c[7]) + 5 * (c[2] + c[8]) +
            4 * (c[3] + c[9]) + 3 * (c[4] + c[10]) + 2 * (c[5] + c[11])) %
               11;
  if (kontrolna == 10)
    return false;
  if (kontrolna == 11)
    kontrolna = 0;
  JMBG *= 10;
  JMBG += kontrolna;
  return true;
}

bool IspravanDatum(int dan, int mjesec, int godina) {
  if (godina > 2021 || godina < 1921)
    return false;
  int dani_u_mjesecima[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (godina % 4 == 0)
    dani_u_mjesecima[1]++;
  if (mjesec > 12 || mjesec < 1)
    return false;
  if (dan > dani_u_mjesecima[mjesec - 1])
    return false;
  return true;
}

enum Pol{Musko, Zensko};

class GradjaninBiH {
  std::string Ime;
  long long int JMBG;
  static GradjaninBiH *zadnji_kreiran;
  GradjaninBiH *prethodni;
  int dan, mjesec, godina, sifra_regije, kod;
  Pol pol;

public:
    GradjaninBiH(const GradjaninBiH&) = delete;
    GradjaninBiH operator=(const GradjaninBiH&) = delete;
  GradjaninBiH(std::string Ime_Prezime, long long int maticni_broj) {
    std::logic_error e("Neispravni podaci");
    int n = BrojCifara(maticni_broj);
    if(Ime_Prezime.empty()) throw std::logic_error("Ime ne moze biti prazno!");
    if (n > 13 || n < 12)
      throw e;
    int dan, mjesec, godina, regija, kod, kontrola;
    bool ispravan_maticni = RazloziMaticni(maticni_broj, dan, mjesec, godina,
                                           regija, kod, kontrola);
    bool datum = IspravanDatum(dan, mjesec, godina);
    if(!ispravan_maticni) throw std::logic_error("JMBG nije validan");
    if (!datum)
      throw e;
    GradjaninBiH *pok = zadnji_kreiran;
    while (pok) {
      if (maticni_broj == pok->JMBG)
        throw std::logic_error("Vec postoji gradjanin sa istim JMBG");
      pok = pok->prethodni;
    }
    this->Ime = Ime_Prezime;
    this->JMBG = maticni_broj;
    this->prethodni = zadnji_kreiran;
    this->dan = dan;
    this->mjesec = mjesec;
    this->godina = godina;
    this->sifra_regije = regija;
    this->kod = kod;
    if (kod < 500)
      this->pol = Pol::Musko;
    else
      this->pol = Pol::Zensko;
    zadnji_kreiran = this;
  }

  GradjaninBiH(std::string Ime_Prezime, int dan, int mjesec, int godina,
               int regija, Pol pol) {
    std::logic_error e("Neispravni podaci");
    if(Ime_Prezime.empty()) throw std::logic_error("Ime ne moze biti prazno!");
    if (!IspravanDatum(dan, mjesec, godina) || regija > 99 || regija < 0)
      throw e;
    int najmanji_kod;
    if (pol == Pol::Musko)
      najmanji_kod = 0;
    else
      najmanji_kod = 500;
    bool slobodan = false;
    long long int maticni;
    while (!slobodan) {
      slobodan = true;
      for (GradjaninBiH *pok = zadnji_kreiran; pok; pok = pok->prethodni) {
        if (pok->dan == dan && pok->mjesec == mjesec && pok->godina == godina &&
            pok->sifra_regije == regija && pok->kod == najmanji_kod) {
          slobodan = false;
          najmanji_kod++;
          break;
        }
      } if(!slobodan) continue;
      if ((!slobodan && najmanji_kod == 499) || (!slobodan && najmanji_kod == 999))
        throw e;
      if (slobodan && SloziMaticni(maticni, dan, mjesec, godina % 1000, regija, najmanji_kod))
        break;
      else if(!SloziMaticni(maticni, dan, mjesec, godina % 1000, regija, najmanji_kod)){
        slobodan = false;
        najmanji_kod++;
      }
    }
    this->JMBG = maticni;
    this->Ime = Ime_Prezime;
    this->prethodni = zadnji_kreiran;
    this->dan = dan;
    this->mjesec = mjesec;
    this->godina = godina;
    this->sifra_regije = regija;
    this->kod = najmanji_kod;
    this->pol = pol;
    zadnji_kreiran = this;
  }

  ~GradjaninBiH() {
    if (this == zadnji_kreiran)
      zadnji_kreiran = this->prethodni;
    else{
        GradjaninBiH *pok = zadnji_kreiran;
        while (pok && pok->prethodni != this) {
      pok = pok->prethodni;
    }
    if(pok && this->prethodni)pok->prethodni = this->prethodni;
    }
  }

  std::string DajImeIPrezime() const { return this->Ime; }
  long long int DajJMBG() const { return this->JMBG; }
  int DajDanRodjenja() const { return this->dan; }
  int DajMjesecRodjenja() const { return this->mjesec; }
  int DajGodinuRodjenja() const {return this->godina;}
  int DajSifruRegije() const { return this->sifra_regije; }
  Pol DajPol() const { return this->pol; }
  GradjaninBiH* DajPrethodnog() const{return prethodni;}
  void PostaviPrethodnog(GradjaninBiH *p){this->prethodni = p;}

  void PromijeniImeIPrezime(const std::string &novo_ime) {
      if(novo_ime.empty()) throw std::logic_error("Ime ne moze biti prazno!");
      this->Ime = novo_ime;}
};

GradjaninBiH *GradjaninBiH::zadnji_kreiran = nullptr;

int main() { 
    int n;
    std::cout << "Unesite broj gradjana: \n";
    std::cin >> n;
    GradjaninBiH **malogradjani = new GradjaninBiH*[n]{};
    try{
    for(int i = 0; i < n; i++){
        std::cout << "Unesite podatke za " << i+1 << ". gradjanina:\nIme i prezime: ";
        std::string ime{};
        if(std::cin.peek() != EOF) std::cin.ignore(10000, '\n');
        std::getline(std::cin, ime);
        std::cout << "Datum rodjenja: ";
        int dan, mjesec, godina;
        std::cin >> dan >> mjesec >> godina;
        std::cout << "Sifra regije: ";
        int sifra_regije;
        std::cin >> sifra_regije;
        std::cout << "Pol ( 1-Musko, 2-Zensko ): ";
        int p;
        std::cin >> p;
        if(!IspravanDatum(dan, mjesec, godina) || (sifra_regije < 0 || sifra_regije > 99) || !(p == 1 || p == 2)){
            std::cout << "Neispravni podaci. Molimo ponovite unos.\n";
            i--;
        } else{
            Pol pol = (p == 1) ? Pol::Musko : Pol::Zensko;
            malogradjani[i] = new GradjaninBiH(ime, dan, mjesec, godina, sifra_regije, pol);
        } std::cout << "\n";
    }
    std::cout << "Gradjani sortirani po starosti:\n";
    std::sort(malogradjani, malogradjani + n, [](GradjaninBiH *g1, GradjaninBiH *g2){
        if(g1->DajGodinuRodjenja() < g2->DajGodinuRodjenja()) return false;
        if(g1->DajMjesecRodjenja() < g2->DajMjesecRodjenja()) return false;
        return g1->DajDanRodjenja() > g2->DajDanRodjenja();
    });
    std::for_each(malogradjani, malogradjani + n, [](GradjaninBiH *g){
        std::cout << g->DajImeIPrezime() << " " << g->DajDanRodjenja() << "." << g->DajMjesecRodjenja() 
                  << "." << g->DajGodinuRodjenja() << " JMBG: ";
                  if(BrojCifara(g->DajJMBG()) == 12) std::cout << "0";
                  std::cout << g->DajJMBG() << "\n";
    });
    for(int i = 0; i < n; i++)delete malogradjani[i];
    delete[] malogradjani;
    }catch(std::bad_alloc){
        for(int i = 0; i < n; i++) delete malogradjani[i];
        delete[] malogradjani;
    }

    return 0; 
    }
