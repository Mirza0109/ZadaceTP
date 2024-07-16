#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <new>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

bool IspravanDatum(int dan, int mjesec, int godina) {
  if (dan < 1 || mjesec < 1 || mjesec > 12 || godina < 1)
    return false;
  int DaniUMjesecu[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (godina % 4 == 0)
    DaniUMjesecu[1]++;
  if (dan > DaniUMjesecu[mjesec - 1])
    return false;
  return true;
}

class Datum {
  int dan, mjesec, godina;

public:
  Datum(int dan, int mjesec, int godina) {
    std::domain_error e("Neispravan datum");
    if (!IspravanDatum(dan, mjesec, godina))
      throw e;
    else
      this->dan = dan;
    this->mjesec = mjesec;
    this->godina = godina;
  }
  void Postavi(int dan, int mjesec, int godina) {
    std::domain_error e("Neispravan datum");
    if (!IspravanDatum(dan, mjesec, godina))
      throw e;
    else
      this->dan = dan;
    this->mjesec = mjesec;
    this->godina = godina;
  }
  std::tuple<int, int, int> Ocitaj() {
    return std::make_tuple(this->dan, this->mjesec, this->godina);
  }
  void Ispisi() {
    std::cout << this->dan << "/" << this->mjesec << "/" << this->godina;
  }

  void PomjeriNaprijed();
  void PomjeriNazad();
  static bool UporediDatume(const Datum &d1, const Datum &d2);
  bool operator==(const Datum &d1) const;
};

void Datum::PomjeriNaprijed() {
  int DUM[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(this->godina % 4 == 0) DUM[1]++;
  if (this->dan == DUM[this->mjesec - 1]) {
    if (this->mjesec != 12)
      this->mjesec++;
    else{
      this->mjesec = 1;
      this->godina++;
    }
    this->dan = 1;
  } else
    this->dan++;
}

void Datum::PomjeriNazad() {
  int DUM[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(this->godina % 4 == 0) DUM[1]++; 
  if (this->dan == 1) {
    if (this->mjesec == 1){
      this->mjesec = 12;
      this->godina--;
    }
    else
      this->mjesec--;
    this->dan = DUM[this->mjesec - 1];
  } else
    this->dan--;
}

bool Datum::UporediDatume(const Datum &d1, const Datum &d2) {
  if (d1.godina < d2.godina)
    return true;
  if (d1.mjesec < d2.mjesec)
    return true;
  return d1.dan < d2.dan;
}

bool Datum::operator==(const Datum &d1) const {
  return (d1.dan == this->dan && d1.mjesec == this->mjesec &&
          d1.godina == this->godina);
}

class Vrijeme {
  int sati, minute;

public:
  Vrijeme(int h, int min) {
    if (h < 0 || min < 0 || h > 23 || min > 59)
      throw std::domain_error("Neispravno vrijeme");
    this->sati = h;
    this->minute = min;
  }
  void Postavi(int h, int min) {
    if (h < 0 || min < 0 || h > 23 || min > 59)
      throw std::domain_error("Neispravno vrijeme");
    this->sati = h;
    this->minute = min;
  }
  std::pair<int, int> Ocitaj() {
    return std::make_pair(this->sati, this->minute);
  }
  void Ispisi() {
    if (sati < 10)
      std::cout << 0;
    std::cout << this->sati << ":";
    if (minute < 10)
      std::cout << 0;
    std::cout << this->minute;
  }
  static bool UporediVremena(const Vrijeme &v1, const Vrijeme &v2);
};

bool Vrijeme::UporediVremena(const Vrijeme &v1, const Vrijeme &v2) {
  if (v1.sati < v2.sati)
    return true;
  return v1.minute < v2.minute;
}

class Pregled {
  std::string Ime_Pacijenta;
  Datum Datum_Pregleda;
  Vrijeme Vrijeme_Pregleda;

public:
  Pregled(const std::string &ime, const Datum &datum, const Vrijeme &vrijeme)
      : Ime_Pacijenta(ime), Datum_Pregleda(datum), Vrijeme_Pregleda(vrijeme) {}
  Pregled(const std::string &ime, int dan, int mjesec, int godina, int sati,
          int minute)
      : Ime_Pacijenta(ime), Datum_Pregleda(dan, mjesec, godina),
        Vrijeme_Pregleda(sati, minute) {}

  Pregled &PromijeniPacijenta(const std::string &s) {
    this->Ime_Pacijenta = s;
    return *this;
  }
  Pregled &PromijeniDatum(const Datum &datum) {
    this->Datum_Pregleda = datum;
    return *this;
  }
  Pregled &PromijeniVrijeme(const Vrijeme &vrijeme) {
    this->Vrijeme_Pregleda = vrijeme;
    return *this;
  }

  void PomjeriDanUnaprijed() { this->Datum_Pregleda.PomjeriNaprijed(); }
  void PomjeriDanUnazad() { this->Datum_Pregleda.PomjeriNazad(); }

  std::string DajImePacijenta() const { return this->Ime_Pacijenta; }
  Datum DajDatumPregleda() const { return this->Datum_Pregleda; }
  Vrijeme DajVrijemePregleda() const { return this->Vrijeme_Pregleda; }

  static bool DolaziPrije(const Pregled &p1, const Pregled &p2) {
    if (Datum::UporediDatume(p1.Datum_Pregleda, p2.Datum_Pregleda))
      return true;
    if (Datum::UporediDatume(p2.Datum_Pregleda, p1.Datum_Pregleda))
      return false;
    return Vrijeme::UporediVremena(p1.Vrijeme_Pregleda, p2.Vrijeme_Pregleda);
  }

  void Ispisi() {
    std::cout << std::setw(30) << std::left << this->Ime_Pacijenta;
    this->Datum_Pregleda.Ispisi();
    std::cout << " ";
    this->Vrijeme_Pregleda.Ispisi();
    std::cout << "\n";
  }
};

class Pregledi {
  std::vector<std::shared_ptr<Pregled>> pregledi;

public:
  explicit Pregledi() : pregledi() {}
  Pregledi(std::initializer_list<Pregled> lista) {
    this->pregledi.clear();
    for (auto it = lista.begin(); it != lista.end(); it++) {
      this->pregledi.push_back(std::make_shared<Pregled>(*it));
    }
  }
  ~Pregledi() { this->pregledi.clear(); }
  Pregledi(const Pregledi &neki) : pregledi(neki.pregledi) {}
  Pregledi(Pregledi &&neki){ std::swap(pregledi, neki.pregledi); }
  Pregledi &operator=(Pregledi &&neki) {
      if(&pregledi != &neki.pregledi)
    std::swap(pregledi, neki.pregledi);
    return *this;
  }
  Pregledi &operator=(const Pregledi &neki) {
    this->pregledi = neki.pregledi;
    return *this;
  }

  void RegistrirajPregled(const std::string &ime, const Datum &datum,
                          const Vrijeme &vrijeme);
  void RegistrirajPregled(const std::string &ime, int dan, int mjesec,
                          int godina, int sati, int minute);
  void RegistrirajPregled(std::shared_ptr<Pregled> neki);
  int DajBrojPregleda() const { return this->pregledi.size(); };
  int DajBrojPregledaNaDatum(const Datum &datum) const;
  Pregled DajNajranijiPregled() const;
  Pregled &DajNajranijiPregled();
  void IsprazniKolekciju() { this->pregledi.clear(); }
  void ObrisiNajranijiPregled();
  void ObrisiPregledePacijenta(const std::string &ime);
  void IspisiPregledeNaDatum(const Datum &datum) const;
  void IspisiSvePreglede() const {
      for (int i = 0; i < this->pregledi.size(); i++)
            this->pregledi[i]->Ispisi();
  }
  void SortirajPreglede();
};

void Pregledi::RegistrirajPregled(const std::string &ime, const Datum &datum,
                                  const Vrijeme &vrijeme) {
  this->pregledi.push_back(
      std::make_shared<Pregled>(Pregled(ime, datum, vrijeme)));
}

void Pregledi::RegistrirajPregled(const std::string &ime, int dan, int mjesec,
                                  int godina, int sati, int minute) {
  this->pregledi.push_back(std::make_shared<Pregled>(
      Pregled(ime, dan, mjesec, godina, sati, minute)));
}

void Pregledi::RegistrirajPregled(std::shared_ptr<Pregled> neki) {
  this->pregledi.push_back(neki);
}

int Pregledi::DajBrojPregledaNaDatum(const Datum &datum) const {
  return std::count_if(this->pregledi.begin(), this->pregledi.end(),
                       [datum](std::shared_ptr<Pregled> p) {
                         Datum d = p->DajDatumPregleda();
                         return (datum == d);
                       });
}

Pregled Pregledi::DajNajranijiPregled() const {
  if (this->pregledi.size() == 0)
    throw std::domain_error("Nema registriranih pregleda");
  return **std::min_element(
      this->pregledi.begin(), this->pregledi.end(),
      [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2) {
        return Pregled::DolaziPrije(*p1, *p2);
      });
}

Pregled &Pregledi::DajNajranijiPregled() {
  if (this->pregledi.size() == 0)
    throw std::domain_error("Nema registriranih pregleda");
  return **std::min_element(
      this->pregledi.begin(), this->pregledi.end(),
      [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2) {
        return Pregled::DolaziPrije(*p1, *p2);
      });
}

void Pregledi::ObrisiNajranijiPregled() {
  if (this->pregledi.size() == 0)
    throw std::range_error("Prazna kolekcija");
  else {
    std::shared_ptr<Pregled> d = *std::min_element(
        this->pregledi.begin(), this->pregledi.end(),
        [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2) {
          return Pregled::DolaziPrije(*p1, *p2);
        });
    Datum dat = d->DajDatumPregleda();
    for (int i = 0; i < this->pregledi.size(); i++) {
      if (this->pregledi.at(i)->DajDatumPregleda() == dat) {
        for (int j = i; j < this->pregledi.size() - 1; j++) {
          this->pregledi.at(j) = this->pregledi.at(j + 1);
        }
        this->pregledi.resize(this->pregledi.size() - 1);
        break;
      }
    }
  }
}

void Pregledi::ObrisiPregledePacijenta(const std::string &ime) {
  for (int i = 0; i < this->pregledi.size(); i++) {
    if (this->pregledi.at(i)->DajImePacijenta() == ime) {
      for (int j = i; j < this->pregledi.size() - 1; j++)
        this->pregledi.at(j) = this->pregledi.at(j + 1);
      this->pregledi.resize(this->pregledi.size() - 1);
      i--;
    }
  }
}

void Pregledi::IspisiPregledeNaDatum(const Datum &datum) const{
  for (int i = 0; i < this->pregledi.size(); i++)
    if (this->pregledi.at(i)->DajDatumPregleda() == datum)
      this->pregledi.at(i)->Ispisi();
}

void Pregledi::SortirajPreglede(){
    std::sort(this->pregledi.begin(), this->pregledi.end(), [](std::shared_ptr<Pregled> p1, std::shared_ptr<Pregled> p2){
        return Pregled::DolaziPrije(*p1, *p2);
    });
}

void UnesiPregled(Pregledi &p) {

  std::string ime;
  std::cout << "Unesite ime pacijenta: ";
  std::getline(std::cin, ime);
  int dan, mjesec, godina, sati, minute;
  std::cout
      << "Unesite datum i vrijeme pregleda (dan mjesec godina sati minute): ";
  std::cin >> dan >> mjesec >> godina >> sati >> minute;
  try {
    p.RegistrirajPregled(ime, dan, mjesec, godina, sati, minute);
  } catch (const std::range_error &e) {
    std::cout << e.what() << "\n";
  }
}

void Br_Pregleda_Na_Datum(Pregledi &p) {
  int dan, mjesec, godina;
  std::cout << "Unesite datum (dan mjesec godina): ";
  std::cin >> dan >> mjesec >> godina;
  std::cout << "Broj pregleda na datum: "
            << p.DajBrojPregledaNaDatum(Datum(dan, mjesec, godina)) << "\n";
}

void ObrisiZaPacijenta(Pregledi &p) {
  std::string Ime;
  std::cout << "Unesite ime pacijenta: ";
  std::getline(std::cin, Ime);
  p.ObrisiPregledePacijenta(Ime);
  std::cout << "Pregledi za pacijenta " << Ime << " su obrisani.\n";
}

void IspisPregledaNaDatum(Pregledi &p) {
  int dan, mjesec, godina;
  std::cout << "Unesite datum: ";
  std::cin >> dan >> mjesec >> godina;
  Datum d(dan, mjesec, godina);
  p.IspisiPregledeNaDatum(d);
}

int main() {
  Pregledi p;
  int izbor;
  while (izbor != 10) {
    std::cout << "1. Registriraj Pregled\n2. Daj Broj Pregleda\n3. Daj Broj "
                 "Pregleda Na Datum\n";
    std::cout << "4. Daj Najraniji Pregled\n5. Obrisi Najraniji Pregled\n6. "
                 "Obrisi Preglede Pacijenta\n";
    std::cout << "7. Ispisi Preglede Na Datum\n8. Ispisi Sve Preglede\n9. "
                 "Isprazni Kolekciju\n10. Izlaz\n";
    std::cout << "Unesite izbor: ";
    std::cin >> izbor;
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    switch (izbor) {
    case 1:
      UnesiPregled(p);
      break;
    case 2:
      std::cout << "Ukupan broj pregleda: " << p.DajBrojPregleda();
      std::cout << "\n";
      break;
    case 3:
      Br_Pregleda_Na_Datum(p);
      break;
    case 4:
      std::cout << "Najraniji pregled je:\n";
      p.DajNajranijiPregled().Ispisi();
      break;
    case 5:
      p.ObrisiNajranijiPregled();
      std::cout << "Najraniji pregled je obrisan.\n";
      break;
    case 6:
      ObrisiZaPacijenta(p);
      break;
    case 7:
      IspisPregledaNaDatum(p);
      break;
    case 8:
      std::cout << "Svi pregledi:\n";
      p.SortirajPreglede(); p.IspisiSvePreglede();
      break;
    case 9:
      p.IsprazniKolekciju();
      std::cout << "Kolekcija ispraznjena.\n";
    case 10:
      break;
    default:
      std::cout << "Neispravna komanda!";
      break;
    }
  }
  std::cout << "Kraj programa.";
  return 0;
}
