#include <iostream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <vector>
#include <functional>
#include <stdexcept>

auto KubnaInterpolacija(std::vector<std::pair<double, double>> &cvorovi){
    if(cvorovi.size() < 4) throw std::domain_error("Nedovoljan broj cvorova");
    std::sort(cvorovi.begin(), cvorovi.end(), 
                [](const auto &a, const auto &b){
                    return a.first < b.first;
                });
    for(auto it = cvorovi.begin(); it != cvorovi.end(); it++){
        auto t = it;
        t++;
        if(t != cvorovi.end() && t->first == it->first) throw std::domain_error("Neispravni cvorovi");
    }
    return [=](double x){
        if(x < cvorovi.begin()->first || 
           x > cvorovi.rbegin()->first) throw std::range_error("Argument izvan opsega!\n");
        if(x == cvorovi.rbegin()->first) return cvorovi.rbegin()->second;
        auto p = std::lower_bound(cvorovi.begin(), cvorovi.end(), x, 
                                        [x](const auto &a, const auto &b){
                                            return a.first < x;
                                        });
        int i = std::distance(cvorovi.begin(), p);
        if(p->first == x) return p->second;
        if(i != 1)i--;
        if(i == (cvorovi.size() - 2)) i--;
        double suma{};
        for(int k = i - 1; k <= i + 2; k++){
            double proizvod = 1;
            for(int j = i - 1; j <= i + 2; j++){
                if(j == k) continue;
                proizvod *= (x - cvorovi.at(j).first)/(cvorovi.at(k).first - cvorovi.at(j).first);
            } suma += proizvod * cvorovi.at(k).second;
        } return suma;
    };
}

auto KubnaInterpolacija(std::function<double(double)> f, double xmin, double xmax, double delta){
    if(xmin > xmax || delta <= 0) throw std::domain_error("Nekorektni parametri");
    std::vector<std::pair<double, double>> cvorovi;
    while(xmax + 1e-10 > xmin){
        cvorovi.push_back(std::make_pair(xmin, f(xmin)));
        xmin += delta;
    }
    if(cvorovi.size() < 4) throw std::domain_error("Nedovoljan broj cvorova");
    return [cvorovi](double x){
        if(x < cvorovi.begin()->first || x > cvorovi.rbegin()->first) throw std::range_error("Argument izvan opsega!\n");
        if(x == cvorovi.rbegin()->first) return cvorovi.rbegin()->second;
        auto p = std::lower_bound(cvorovi.begin(), cvorovi.end(), x,
                                        [x](const auto &a, const auto &b){
                                            return a.first < x;
                                        });
        int i = std::distance(cvorovi.begin(), p);
        if(p->first == x) return p->second;
        if(i != 1) i--;
        if(i == cvorovi.size() - 2) i--;
        double suma{};
        for(int k = i - 1; k <= i + 2; k++){
            double proizvod = 1;
            for(int j = i - 1; j <= i + 2; j++){
                if(j == k) continue;
                proizvod *= (x - cvorovi.at(j).first) / (cvorovi.at(k).first - cvorovi.at(j).first);
            } suma += proizvod * cvorovi.at(k).second;
        } return suma;
    };
}

int main (){
    try{
        int opcija;
        std::cout << "Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): \n";
        std::cin >> opcija;
        if(opcija == 1){
            int br_cv;
            std::cout << "Unesite broj cvorova: \n";
            std::cin >> br_cv;
            std::vector<std::pair<double, double>> cvorovi(br_cv);
            std::cout << "Unesite cvorove kao parove x y: \n";
            for(int i = 0; i < br_cv; i++){
                double x, y;
                std::cin >> x >> y;
                cvorovi.at(i).first = x; cvorovi.at(i).second = y;
            }
            auto f = KubnaInterpolacija(cvorovi);
            while(std::cin){
                double a;
                std::cout << "Unesite argument (ili \"kraj\" za kraj): \n";
                std::cin >> a;
                if(std::cin){
                    try{
                        double t = f(a);
                        std::cout << "f(" << a << ") = " << t << "\n";  
                    } catch(std::range_error e){
                        std::cout << e.what();
                    }
                }
            }
        } else if(opcija == 2){
            double min, max, d;
            std::cout << "Unesite krajeve intervala i korak: \n";
            std::cin >> min >> max >> d;
            auto funkcija = [](double x){return (x*x + std::sin(x) + std::log(x + 1));};
            auto f = KubnaInterpolacija(funkcija, min, max, d);
            while(std::cin){
                double a;
                std::cout << "Unesite argument (ili \"kraj\" za kraj): \n";
                std::cin >> a;
                if(std::cin){
                    try{
                        f(a);
                        std::cout << "f(" << a << ") = " << funkcija(a) 
                                  << " fapprox(" << a << ") = " << f(a) << "\n";
                    } catch(std::range_error e){
                        std::cout << e.what();
                    }
                }
            }
        }
	return 0;
    } catch(std::domain_error e){
        std::cout << e.what();
    } return 0;
}
