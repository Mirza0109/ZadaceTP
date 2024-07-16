#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <new>
#include <stdexcept>


enum class Smjer { Desno, Dolje, Dijagonalno };

typedef std::map<Smjer, std::function<int(int)>> Mapa;

template <typename Tip> struct Matrica {
  char ime_matrice;
  int br_redova, br_kolona;
  Tip **elementi;
};

template <typename Tip> void UnistiMatricu(Matrica<Tip> &mat) {
  if (!mat.elementi)
    return;
  for (int i = 0; i < mat.br_redova; i++) {
      delete[] mat.elementi[i];
      mat.elementi[i] = nullptr;
  }
  delete[] mat.elementi;
  mat.elementi = nullptr;
}

template <typename Tip>
Matrica<Tip> StvoriMatricu(int br_redova, int br_kolona, char ime = 0) {
  Matrica<Tip> mat;
  mat.br_redova = br_redova;
  mat.br_kolona = br_kolona;
  mat.ime_matrice = ime;
  mat.elementi = new Tip *[br_redova];
  for(int i = 0; i < br_redova; i++) mat.elementi[i] = nullptr;
  try {
    for (int i = 0; i < br_redova; i++)
      mat.elementi[i] = new Tip[br_kolona]{};
  }catch (...) {
    UnistiMatricu(mat);
    throw;
  }
  return mat;
}

template <typename Tip> void UnesiMatricu(Matrica<Tip> &mat) {
  for (int i = 0; i < mat.br_redova; i++)
    for (int j = 0; j < mat.br_kolona; j++) {
      std::cout << mat.ime_matrice << "M(" << i + 1 << "," << j + 1 << ") = \n";
      std::cin >> mat.elementi[i][j];
    }
}

template <typename Tip>
void IspisiMatricu(Matrica<Tip> mat, int sirina_ispisa = 4, int preciznost = 3,
                   bool treba_brisati = false) {
  for (int i = 0; i < mat.br_redova; i++) {
    for (int j = 0; j < mat.br_kolona; j++)
      std::cout << std::setw(sirina_ispisa) << std::setprecision(preciznost) << mat.elementi[i][j];
      std::cout << "\n";
  }
  if (treba_brisati)
    UnistiMatricu(mat);
}

template <typename Tip>
Matrica<Tip> ZbirMatrica(const Matrica<Tip> &m1, const Matrica<Tip> &m2) {
  if (m1.br_redova != m2.br_redova || m1.br_kolona != m2.br_kolona)
    throw std::domain_error("Matrice nemaju jednake dimenzije!");
  auto m3 = StvoriMatricu<Tip>(m1.br_redova, m1.br_kolona);
  for (int i = 0; i < m1.br_redova; i++)
    for (int j = 0; j < m1.br_kolona; j++)
      m3.elementi[i][j] = m1.elementi[i][j] + m2.elementi[i][j];
  return m3;
}

template <typename Tip>
void PrepisiElemente(const Matrica<Tip> &m1, Matrica<Tip> &m2){
    if(m1.br_kolona != m2.br_kolona || m1.br_redova != m2.br_redova) throw std::logic_error("Nije dobro");
    for(int i = 0; i < m1.br_redova; i++){
        for(int j = 0; j < m1.br_kolona; j++) m2.elementi[i][j] = m1.elementi[i][j];
    }
}

Matrica<int> ProsiriPremaFunkcijama(Matrica<int> &M, Mapa d, int n) {
  if (n < 1) throw std::logic_error("Besmislen parametar");
  else if(n == 1 || d.empty() || M.br_kolona == 0 || M.br_redova == 0){
      Matrica<int> vracena = StvoriMatricu<int>(M.br_redova, M.br_kolona);
      PrepisiElemente(M, vracena);
      return vracena;
  }
  else if (n != 1){
    
  if (d.find(Smjer::Dijagonalno) != d.end()) {
    if (d.find(Smjer::Desno) == d.end())
      d.insert(std::make_pair(Smjer::Desno, [](int x) { return x; }));
    if (d.find(Smjer::Dolje) == d.end())
      d.insert(std::make_pair(Smjer::Dolje, [](int x) { return x; }));
  } else if (d.find(Smjer::Desno) != d.end() && d.find(Smjer::Dolje) != d.end())
    d.insert(std::make_pair(Smjer::Dijagonalno, [](int x) { return x; }));

  for (int i = 1; i < n; i++) {
    int a = M.br_redova, b = M.br_kolona;
    int red = a, kol = b;
    if (d.find(Smjer::Desno) != d.end())
      b *= 2;
    if (d.find(Smjer::Dolje) != d.end())
      a *= 2;
    Matrica<int> T = StvoriMatricu<int>(a, b);
      
    std::function<int(int)> f_dijag{}, f_desno{}, f_dolje{};
    if (d.find(Smjer::Dijagonalno) != d.end())
      f_dijag = d.find(Smjer::Dijagonalno)->second;
    if (d.find(Smjer::Desno) != d.end())
      f_desno = d.find(Smjer::Desno)->second;
    if (d.find(Smjer::Dolje) != d.end())
      f_dolje = d.find(Smjer::Dolje)->second;
    for (int x = 0; x < a; x++) {
      for (int y = 0; y < b; y++) {
        if (x >= red && y >= kol)
          T.elementi[x][y] = f_dijag(M.elementi[x - red][y - kol]);
        else if (x >= red)
          T.elementi[x][y] = f_dolje(M.elementi[x - red][y]);
        else if (y >= kol)
          T.elementi[x][y] = f_desno(M.elementi[x][y - kol]);
        else
          T.elementi[x][y] = M.elementi[x][y];
      }
    }
    UnistiMatricu(M);
    M = StvoriMatricu<int>(a, b);
    PrepisiElemente(T, M);
    UnistiMatricu(T);
  }
  } Matrica<int> vracena = StvoriMatricu<int>(M.br_redova, M.br_kolona);
  PrepisiElemente(M, vracena);
  return vracena;
}

int main(){ 
    try{
        int a, b;
        std::cout << "Unesi broj redova i kolona matrice: \n";
        std::cin >> a >> b;
        Matrica<int> A, M = StvoriMatricu<int>(a, b);
        UnesiMatricu(M);
        std::map<Smjer, std::function<int(int)>> m{
            {Smjer::Desno, [] (int x) {return x + 1; }},
            {Smjer::Dolje, [] (int x) {return x + 2; }},
            {Smjer::Dijagonalno, [] (int x) {return x + 3; }}
        };
        int n;
        std::cout << "Unesite n: \n";
        std::cin >> n;
        A = ProsiriPremaFunkcijama(M, m, n);
        IspisiMatricu(M, 5, true);
        UnistiMatricu(M);
        UnistiMatricu(A);
        return 0; 
    } catch(...){
        std::cout << "Greska";
    } return 0;
}
