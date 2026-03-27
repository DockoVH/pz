#pragma once

#include <ostream>
#include <istream>
#include <vector>
#include <cstdint>

#define VELICINA_BLOKA 1048576 //Broj B u MB
#define BROJ_NITI 4

class PZKompresor
{
private:
	static std::vector<uint8_t> kompresujBlok(const std::vector<char>& blok);
	static std::vector<uint8_t> dekompresujBlok(const std::vector<char>& blok);

public:
	static void kompresuj(std::istream& ulaz, std::ostream& izlaz);
	static void dekompresuj(std::istream& ulaz, std::ostream& izlaz);
};
