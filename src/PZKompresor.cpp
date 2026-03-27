#include <iostream>
#include <ostream>
#include <istream>
#include <vector>
#include <cstdint>

#include "PZKompresor.h"
#include "huffman.h"
#include "ThreadPool.h"

std::vector<uint8_t> PZKompresor::kompresujBlok(const std::vector<char>& blok)
{
	HuffmanEncoding h;

	return h.kompresujTekst(blok);
}

/*std::vector<uint8_t> PZKompresor::kompresujBlok(const std::vector<char>& blok)
{
	HuffmanEncoding h;

	std::vector<uint8_t> kompresovaniPodaci = h.kompresujTekst(blok);

	std::vector<uint8_t> rezultat;
	rezultat.reserve(kompresovaniPodaci.size() + 4);


	for (int i = 0; i < 4; i++)
	{
		uint8_t bajt = (kompresovaniPodaci.size() >> ((3 - i) * 8)) & 255;
		rezultat.push_back(bajt);
	}

	rezultat.insert(rezultat.end(), kompresovaniPodaci.begin(), kompresovaniPodaci.end());

	return rezultat;
}*/

std::vector<uint8_t> PZKompresor::dekompresujBlok(const std::vector<char>& blok)
{
	HuffmanEncoding h;

	return h.dekompresujTekst(blok);
}
void PZKompresor::kompresuj(std::istream& ulaz, std::ostream& izlaz)
{
	std::vector<char> buffer(VELICINA_BLOKA);
	ThreadPool pool(BROJ_NITI);
	std::vector<std::future<std::vector<uint8_t>>> rezultati;

	while (ulaz.good())
	{
		ulaz.read(buffer.data(), buffer.size());
		std::streamsize s = ulaz.gcount();

		if (buffer.size() != size_t(s))
		{
			buffer.resize(s);
		}

		rezultati.emplace_back(pool.enqueue(kompresujBlok, buffer));
	}

	if (ulaz.fail() && !ulaz.eof())
	{
		std::cout << "pz: Neuspešno kompresovan fajl!" << std::endl;
		exit(1);
	}

	for (auto&& rezultat: rezultati)
	{
		std::vector<uint8_t> blok = rezultat.get();

		uint32_t duzina = blok.size();
		for (int i = 0; i < 4; i++)
		{
			uint8_t bajt = (duzina >> ((3 - i) * 8)) & 255;
			izlaz.write(reinterpret_cast<const char*>(&bajt), 1);
		}

		izlaz.write(reinterpret_cast<const char*>(blok.data()), blok.size());
	}
}

void PZKompresor::dekompresuj(std::istream& ulaz, std::ostream& izlaz)
{
	ThreadPool pool(BROJ_NITI);
	std::vector<std::future<std::vector<uint8_t>>> rezultati;

	while (ulaz.good())
	{
		uint32_t duzina = 0;

		for (int i = 0; i < 4; i++)
		{
			uint8_t bajt;
			ulaz.read(reinterpret_cast<char*>(&bajt), 1);
			duzina = (duzina << 8) | bajt;
		}

		if (!ulaz.good())
			break;

		std::vector<char> buffer(duzina);
		ulaz.read(buffer.data(), duzina);

		rezultati.emplace_back(pool.enqueue(dekompresujBlok, buffer));
	}

	if (ulaz.fail() && !ulaz.eof())
	{
		std::cout << "pz: Neuspešno dekompresovan fajl!" << std::endl;
		exit(1);
	}

	for (auto&& rezultat: rezultati)
	{
		std::vector<uint8_t> blok = rezultat.get();
		izlaz.write(reinterpret_cast<const char*>(blok.data()), blok.size());
	}
}
