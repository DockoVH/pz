#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "huffman.h"
#include "PZKompresor.h"

enum class Operacija
{
	Kompresija,
	Dekompresija
};

void help()
{
	std::cout << "Upotreba: pz [OPCIJE] [FAJL]" << std::endl << std::endl
			  << "Opcije:" << std::endl
			  << "  -c <fajl>  - Kompresuj zadati fajl (podrazumevana opcija)" << std::endl
			  << "  -d <fajl>  - Dekompresuj zadati fajl" << std::endl
			  << "  -o <fajl>  - Izlazni fajl (podrazumevano: stdout)" << std::endl
			  << "  -h, --help - Prikaži ovu poruku" << std::endl << std::endl
			  << "Ukoliko ulazni fajl nije naveden podrazumevano je čitanje sa standardnog ulaza" << std::endl << std::endl
			  << "Primeri:" << std::endl
			  << "  pz ulaz.txt -o izlaz.pz" << std::endl
			  << "  pz -c ulaz.pdf -o izlaz.pz" << std::endl
			  << "  pz -d ulaz.pz -o izlaz.txt" << std::endl;
}

int main(int argc, char** argv)
{
	Operacija operacija = Operacija::Kompresija;
	std::string izlazniFajl;
	std::string ulazniFajl;

	for (int i = 0; i < argc; i++)
	{
		std::string arg = argv[i];

		if (arg == "--help" || arg == "-h")
		{
			help();
			return 0;
		}

		if (arg == "-o")
		{
			if (i + 1 < argc)
			{
				izlazniFajl = argv[++i];
			}
			else
			{
				std::cout << "pz: Unesite putanju izlaznog fajla!" << std::endl;
				exit(1);
			}
		}
		else if (arg == "-c")
		{
			if (i + 1 < argc)
			{
				operacija = Operacija::Kompresija;
				ulazniFajl = argv[++i];
			}
			else
			{
				std::cout << "pz: Unesite putanju ulaznog fajla!" << std::endl;
				exit(1);
			}
		}
		else if (arg == "-d")
		{
			if (i + 1 < argc)
			{
				operacija = Operacija::Dekompresija;
				ulazniFajl = argv[++i];
			}
			else
			{
				std::cout << "pz: Unesite putanju ulaznog fajla!" << std::endl;
				exit(1);
			}
		}
		else
		{
			ulazniFajl = arg;
		}
	}

	std::ifstream ulazniStream;
	std::ofstream izlazniStream;

	if (!ulazniFajl.empty())
	{
		ulazniStream.open(ulazniFajl, std::ios::binary);

		if (ulazniStream.fail())
		{
			std::cout << "Greška prilikom otvaranja ulaznog fajla!" << std::endl;
			exit(1);
		}
	}

	if (!izlazniFajl.empty())
	{
		izlazniStream.open(izlazniFajl, std::ios::binary);

		if (izlazniStream.fail())
		{
			std::cout << "Greška prilikom otvaranja izlaznog fajla!" << std::endl;
			exit(1);
		}
	}

	std::istream& ulaz = ulazniFajl.empty() ? std::cin : ulazniStream;
	std::ostream& izlaz = izlazniFajl.empty() ? std::cout : izlazniStream;

	if (operacija == Operacija::Kompresija)
		PZKompresor::kompresuj(ulaz, izlaz);
	else
		PZKompresor::dekompresuj(ulaz, izlaz);

	return 0;
}
