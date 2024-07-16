#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


namespace Robot {

enum class Pravci {
  Sjever,
  Sjeveroistok,
  Istok,
  Jugoistok,
  Jug,
  Jugozapad,
  Zapad,
  Sjeverozapad
};

enum class KodoviGresaka {
  PogresnaKomanda,
  NedostajeParametar,
  SuvisanParametar,
  NeispravanParametar
};

enum class Komande { Idi, Rotiraj, Sakrij, Otkrij, PrikaziTeren, Kraj };

typedef std::vector<std::vector<char>> Matrica;

Matrica teren(23, std::vector<char>(23, ' '));

int minpox = -10, minpoy = -10;

bool vidljivost = true;

const std::vector<std::string> orijent = {
    "sjever", "sjeveroistok", "istok", "jugoistok",
    "jug",    "jugozapad",    "zapad", "sjeverozapad"};
const std::vector<std::string> greske = {
    "Nerazumljiva komanda!\n", "Komanda trazi parametar koji nije naveden!\n",
    "Zadan je suvisan parametar nakon komande!\n",
    "Parametar komande nije ispravan!\n"};

void OkreniUSmjeruKazaljke(Pravci &orijentacija) {
  orijentacija = static_cast<Pravci>((int(orijentacija) + 1) % 8);
}

void OkreniObrnutoKazaljke(Pravci &orijentacija) {
  int temp = int(orijentacija) - 1;
  if (temp < 0)
    temp = 7;
  orijentacija = static_cast<Pravci>(temp);
}

void OkvirTerena() {
  for (int i = 1; i < teren.size() - 1; i++) {
    teren.at(i).at(0) = '#';
    teren.at(i).at(teren.at(i).size() - 1) = '#';
  }
  for (int i = 0; i < teren.at(0).size(); i++) {
    teren.at(0).at(i) = '#';
    teren.at(teren.size() - 1).at(i) = '#';
  }
}

void KreirajTeren(int xmin, int xmax, int ymin, int ymax, int &x, int &y, Pravci &orijentacija) {
  if (xmin >= xmax || ymin >= ymax)
    throw std::range_error("Nelegalan opseg");
  minpox = xmin;
  minpoy = ymin;
  orijentacija = Pravci::Sjever;
  x = (xmax + xmin) / 2; 
  y = (ymax + ymin) / 2;
  Matrica m(ymax - ymin + 3, std::vector<char>(xmax - xmin + 3, ' '));
  teren = m;
  teren.at(y - minpoy + 1).at(x - minpox + 1) = '*';
  Robot::OkvirTerena();
}

bool Idi(int &x, int &y, Pravci orijentacija, int korak) {
  bool PokusaoIzaciPoY = false, PokusaoIzaciPoX = false;
   if(vidljivost) teren.at(y - minpoy + 1).at(x - minpox + 1) = '*';
 
  int temp = static_cast<int>(orijentacija);
  bool PlusPoX = (((temp == 1 || temp == 2 || temp == 3) && korak > 0) ||
                  (temp == 5 || temp == 6 || temp == 7) && korak < 0);
  bool PlusPoY = (((temp == 0 || temp == 1 || temp == 7) && korak > 0) ||
                  (temp == 3 || temp == 4 || temp == 5) && korak < 0);
  bool SamoX = (temp % 4 == 2);
  bool SamoY = (temp % 4 == 0);
  for (int i = 0; i < abs(korak); i++) {
    x += (!SamoY) * PlusPoX;
    x -= (!SamoY) * (!PlusPoX);
    y += (!SamoX) * PlusPoY;
    y -= (!SamoX) * (!PlusPoY);
    if ((y - minpoy < 0) || (y - minpoy + 2) >= teren.size()) {
      y = (teren.size() - 3) * (y - minpoy + 2 >= teren.size()) + minpoy;
      PokusaoIzaciPoY = true;
      break;
    }
    if ((x - minpox < 0) || (x - minpox + 2) >= teren.at(0).size()) {
      x = (teren.at(0).size() - 3) * (x - minpox + 2 >= teren.at(0).size()) +
          minpox;
      PokusaoIzaciPoX = true;
      break;
    }
    if (vidljivost)
      teren.at(y - minpoy + 1).at(x - minpox + 1) = '*';
  }
  x += PokusaoIzaciPoY * (!SamoY) * (!PlusPoX - PlusPoX);
  y += PokusaoIzaciPoX * (!SamoX) * (!PlusPoY - PlusPoY);
  return !(PokusaoIzaciPoY || PokusaoIzaciPoX);
}

void Rotiraj(Pravci &orijentacija, int ugao) {
  for (int i = 0; i < ugao; i++)
    OkreniObrnutoKazaljke(orijentacija);
  for (int i = 0; i > ugao; i--)
    OkreniUSmjeruKazaljke(orijentacija);
}

void Sakrij() { vidljivost = false; }

void Otkrij() { vidljivost = true; }

void IspisiPoziciju(int x, int y, Pravci orijentacija) {
  std::vector<std::string> s{"nevidljiv", "vidljiv"};
  std::cout << "Robot je " << s.at(int(vidljivost)) << ", nalazi se na poziciji (" << x << "," << y
            << ") i gleda na " << orijent.at(static_cast<int>(orijentacija))
            << "." << std::endl;
}

void PrikaziTeren() {
  for (int i = teren.size() - 1; i >= 0; i--) {
    for (int j = 0; j < teren.at(i).size(); j++) {
      std::cout << teren.at(i).at(j);
    }
    std::cout << std::endl;
  }
}

void PrijaviGresku(KodoviGresaka kod_greske) {
  std::cout << greske.at(static_cast<int>(kod_greske));
}

bool IzvrsiKomandu(Komande komanda, int parametar, int &x, int &y,
                   Pravci &orijentacija) {
  switch (komanda) {
  case Komande::Idi:
    return Idi(x, y, orijentacija, parametar);
    break;
  case Komande::Rotiraj:
    Rotiraj(orijentacija, parametar);
    break;
  case Komande::Sakrij:
    Sakrij();
    break;
  case Komande::Otkrij:
    Otkrij();
    teren.at(y - minpoy + 1).at(x - minpox + 1) = '*';
    break;
  case Komande::PrikaziTeren:
    teren.at(y - minpoy + 1).at(x - minpox + 1) = 'O';
    PrikaziTeren();
    teren.at(y - minpoy + 1).at(x - minpox + 1) = '*';
    break;
  case Komande::Kraj:
    break;
  } return true;
}

bool UnosParametra(int &parametar, bool &fali){
    char temp;
    if(std::cin.peek() == '\n'){
        fali = true;
        return false;
    }
    while(std::cin.peek() == ' ') std::cin.get(temp);
    if(temp == '\n') {
        fali = true;
        return false;
    }
    std::cin >> parametar;
    if(std::cin.peek() != ' ' && std::cin.peek() != '\n') return false;
    return bool(std::cin);
}

bool ImaLiSta(){
    char temp;
    if(std::cin.peek() != '\n'){
        std::cin.get(temp);
        while(temp == ' ') std::cin.get(temp);
    return temp != '\n';
    } return false;
}

void Ocisti(){
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}

bool UnosKomande(Komande &komanda, int &parametar, KodoviGresaka &kod_greske){
    char c;
    std::cin >> c;
    if(c == 'I' || c == 'R'){
        bool fali = false;
        bool temp = UnosParametra(parametar, fali);
        if(!temp && fali){
            kod_greske = KodoviGresaka::NedostajeParametar;
            return false;
        } else if(!temp && !fali){
            kod_greske = KodoviGresaka::NeispravanParametar;
            Ocisti();
            return false;
        } else if(ImaLiSta()){
            kod_greske = KodoviGresaka::SuvisanParametar;
            Ocisti();
            return false;
        } if(c == 'I') komanda = Komande::Idi;
        else komanda = Komande::Rotiraj;
        return true;
    } else if(c == 'S'){
        char znak;
        if(std::cin.peek() == '\n'){
            kod_greske = KodoviGresaka::NedostajeParametar;
            return false;
        }
        std::cin.get(znak);
        if(znak != '+' && znak != '-'){
            kod_greske = KodoviGresaka::PogresnaKomanda;
            Ocisti();
            return false;
        } else if(znak == '+' && !ImaLiSta()){
            komanda = Komande::Sakrij;
            return true;
        } else if(znak == '-' && !ImaLiSta()){
            komanda = Komande::Otkrij;
            return true;
        } else if(znak == '-' || znak == '+'){
            kod_greske = KodoviGresaka::SuvisanParametar;
            Ocisti();
            return false;
        }
            kod_greske = KodoviGresaka::PogresnaKomanda;
            Ocisti();
            return false; 
    } else if(c == 'T' || c == 'K'){
        if(ImaLiSta()){
            kod_greske = KodoviGresaka::SuvisanParametar;
            Ocisti();
            return false;
        } else if(c == 'T') komanda = Komande::PrikaziTeren;
        else komanda = Komande::Kraj;
        return true;
    } else{
        kod_greske = KodoviGresaka::PogresnaKomanda;
        Ocisti();
        return false;
    } return true;
}
}

int main() {
  int xmin, xmax, ymin, ymax;
  std::cout << "Unesite dimenzije terena (xmin xmax ymin ymax): ";
  std::cin >> xmin >> xmax >> ymin >> ymax;
  if (xmin >= xmax || ymin >= ymax) {
    std::cout << "Nije moguce kreirati takav teren!";
    return 0;
  }
  int x, y;
  Robot::Pravci orijentacija = Robot::Pravci::Sjever;
  Robot::KreirajTeren(xmin, xmax, ymin, ymax, x, y, orijentacija);
  Robot::Komande komanda;
  Robot::KodoviGresaka kod_greske;
  int parametar;
  bool greska = false;
  for (;;) {
    if(!greska && komanda != Robot::Komande::PrikaziTeren) Robot::IspisiPoziciju(x, y, orijentacija);
    greska = false;
    std::cout << "Unesi komandu: ";
    if (Robot::UnosKomande(komanda, parametar, kod_greske)) {
      if (komanda == Robot::Komande::Kraj)
        break;
      if(!Robot::IzvrsiKomandu(komanda, parametar, x, y, orijentacija)) std::cout << "Robot je pokusao napustiti teren!\n";
    } else {
      Robot::PrijaviGresku(kod_greske);
      greska = true;
    } 
  }
  std::cout << "Dovidjenja!";
  return 0;
}
