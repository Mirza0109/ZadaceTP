#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <deque>
#include <iomanip>

template <typename NekiTip>
    int BrojElemenataKolekcije(const NekiTip &kolekcija){
        int brojac = 0;
        for(auto pocetak = std::begin(kolekcija); pocetak != std::end(kolekcija); ++pocetak) brojac++;
        return brojac;
    }

template <typename matrica1, typename matrica2, typename Funkcija>
    auto GeneraliziraniKroneckerovProizvod(const matrica1 &mat1, const matrica2 &mat2, Funkcija F){
        auto pocetak1 = std::begin(mat1);
        auto kraj1 = std::end(mat1);
        auto pocetak2 = std::begin(mat2);
        auto kraj2 = std::end(mat2);
        bool Ispravna1 = true, Ispravna2 = true;
        int br_redova_1{}, br_redova_2{};
        while(pocetak1 != kraj1){
            if(BrojElemenataKolekcije(*(std::begin(mat1))) != BrojElemenataKolekcije(*pocetak1++)){
                Ispravna1 = false;
                break;
            } br_redova_1++;
        }
        while(pocetak2 != kraj2){
            if(BrojElemenataKolekcije(*(std::begin(mat2))) != BrojElemenataKolekcije(*pocetak2++)){
                Ispravna2 = false;
                break;
            } br_redova_2++;
        }
        if(!Ispravna1 && Ispravna2) throw std::domain_error("Prvi parametar nema formu matrice");
        if(Ispravna1 && !Ispravna2) throw std::domain_error("Drugi parametar nema formu matrice");
        if(!Ispravna1 && !Ispravna2) throw std::domain_error("Parametri nemaju formu matrice");
        int br_kolona_1 = BrojElemenataKolekcije(*(std::begin(mat1))), br_kolona_2 = BrojElemenataKolekcije(*(std::begin(mat2)));
        auto a = *(std::begin(*(std::begin(mat1))));
        auto b = *(std::begin(*(std::begin(mat2))));
        try{
        std::vector<std::vector<decltype(F(a, b))>> GenerProizvod(br_redova_1 * br_redova_2, 
                                   std::vector<decltype(F(a, b))>(br_kolona_1 * br_kolona_2));
        int m = 0;
        for(auto p1 = std::begin(mat1); p1 != kraj1; p1++){
            int i = 0;
            for(auto p2 = std::begin(mat2); p2 != kraj2; p2++){
                int n = 0;
                for(auto i1 = std::begin(*p1); i1 != std::end(*p1); i1++){
                    int j = 0;
                    for(auto i2 = std::begin(*p2); i2 != std::end(*p2); i2++){
                        GenerProizvod.at(m * br_redova_2 + i).at(n * br_kolona_2 + j) = F(*i1, *i2);
                        j++;
                    } n++;
                } i++;
            } m++;
        } return GenerProizvod;
        }catch(...){
            throw std::runtime_error("Neocekivani problemi pri racunanju");
        }
    }

    template<typename kolekcija>
        void UnesiMatricu(kolekcija &M, int redovi, int kolone){
            for(auto &v:M){
                for(auto &x:v){
                    std::cin >> x;
                }
            }
        }

int main (){
    try{
    int red1, kol1, red2, kol2;
    std::cout << "Unesite dimenzije prve matrice: ";
    std::cin >> red1 >> kol1;
    std::vector<std::deque<double>> A(red1, std::deque<double>(kol1));
    std::cout << "Unesite elemente prve matrice: ";
    UnesiMatricu(A, red1, kol1);
    std::cout << "Unesite dimenzije druge matrice: ";
    std::cin >> red2 >> kol2;
    std::deque<std::vector<double>> B(red2, std::vector<double>(kol2));
    std::cout << "Unesite elemente druge matrice: ";
    UnesiMatricu(B, red2, kol2);
    auto M = GeneraliziraniKroneckerovProizvod(A, B, [](double x, double y){return x * y;});
    std::cout << "Njihov Kroneckerov proizvod glasi:\n";
    for(auto &v:M){
        for(auto &x:v){
            if(fabs(x - 0) < 1e-10) x = 0;
            std::cout << std::setw(4) << x;
        } std::cout << "\n";
    }
    std::cout << "\n";
    } catch(std::domain_error e){
        std::cout << e.what();
    } catch(std::runtime_error e){
        std::cout << e.what();
    }
	return 0;
}
