#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>


typedef std::map<std::string, std::vector<std::string>> Knjiga;
typedef std::map<std::string, std::set<std::tuple<std::string, int, int>>>
    IndeksP;

IndeksP KreirajIndeksPojmova(const Knjiga &bukvar) {
  IndeksP IndeksPojmova;
  for (auto poglavlje : bukvar) {
    std::set<std::tuple<std::string, int, int>> informacije;
    for (int stranica = 0; stranica < poglavlje.second.size(); stranica++) {
      for (int znak = 0; znak < poglavlje.second.at(stranica).size(); znak++) {
        std::string pojam;
        int pocetni = znak;
        while (std::isdigit(poglavlje.second.at(stranica).at(znak)) ||
               std::isalpha(poglavlje.second.at(stranica).at(znak))) {
          pojam.push_back(
              std::toupper(poglavlje.second.at(stranica).at(znak++)));
          if (znak == poglavlje.second.at(stranica).size())
            break;
        }
        if (!pojam.empty()) {
          std::tuple<std::string, int, int> info(poglavlje.first, stranica + 1,
                                                 pocetni + 1);
          auto it = IndeksPojmova.find(pojam);
          if (it == IndeksPojmova.end()) {
            std::set<std::tuple<std::string, int, int>> informacije;
            informacije.insert(info);
            IndeksPojmova.insert(std::make_pair(pojam, informacije));
          }
          it = IndeksPojmova.find(pojam);
          it->second.insert(info);
        }
      }
    }
  }
  return IndeksPojmova;
}

std::set<std::tuple<std::string, int, int>>
PretraziIndeksPojmova(std::string rijec, const IndeksP &IndeksPojmova) {
  for (char &c : rijec) {
    if (!isalpha(c) && !isdigit(c))
      throw std::domain_error("Neispravna rijec");
    c = std::toupper(c);
  }
  auto trazeno = IndeksPojmova.find(rijec);
  if (trazeno != IndeksPojmova.end())
    return trazeno->second;
  else
    return std::set<std::tuple<std::string, int, int>>{};
}

void IspisiIndeksPojmova(const IndeksP &IndeksPojmova) {
  for (const auto &pojam : IndeksPojmova) {
    std::cout << pojam.first << ": ";
    auto kraj = pojam.second.end();
    kraj--;
    for (auto it = pojam.second.begin(); it != pojam.second.end(); it++) {
      std::cout << std::get<0>(*it) << "/" << std::get<1>(*it) << "/"
                << std::get<2>(*it);
      if (it != kraj)
        std::cout << ", ";
    }
    std::cout << "\n";
  }
}

int main() {
  Knjiga bukvar;
  for (;;) {
    std::string poglavlje;
    std::cout << "Unesite naziv poglavlja:\n";
    std::getline(std::cin, poglavlje);
    if (poglavlje.empty())
      break;
    std::vector<std::string> stranice;
    for (int i = 1;; i++) {
      std::cout << "Unesite sadrzaj stranice " << i << ":\n";
      std::string s;
      std::getline(std::cin, s);
      if (s.empty())
        break;
      stranice.push_back(s);
    }
    bukvar.insert(std::make_pair(poglavlje, stranice));
  }
  IndeksP IndeksPojmova = KreirajIndeksPojmova(bukvar);
  std::cout << "\nKreirani indeks pojmova:\n";
  IspisiIndeksPojmova(IndeksPojmova);

  std::cout << "\n";

  for (;;) {
    std::string s;
    std::cout << "Unesite rijec: \n";
    std::getline(std::cin, s);
    if (s.empty())
      break;
    try {
      auto info = PretraziIndeksPojmova(s, IndeksPojmova);
      if (info.empty())
        std::cout << "Rijec nije nadjena!\n";
      else {
        std::cout << "Rijec nadjena na pozicijama:";
        for (const auto &skup : info) {
          std::cout << " " << std::get<0>(skup) << "/" << std::get<1>(skup)
                    << "/" << std::get<2>(skup);
        }
        std::cout << "\n";
      }
    } catch (std::domain_error e) {
      std::cout << e.what() << "!\n";
    }
  }
  std::cout << "Dovidjenja!";
  return 0;
}
