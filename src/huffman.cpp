#include "huffman.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <memory>

std::unordered_map<char, int> HuffmanEncoding::odrediFrekvencije(std::vector<char> text)
{
	std::unordered_map<char, int> freq;

	for (char ch : text)
	{
		freq[ch]++;
	}

	return freq;
}

std::shared_ptr<Node> HuffmanEncoding::napraviHuffmanStablo(std::unordered_map<char, int>& frekvencije)
{
	std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, compare> pq;

	for (auto& x : frekvencije)
	{
		pq.push(std::make_shared<Node>(x.first, x.second));
	}

	while (pq.size() != 1)
	{
		std::shared_ptr<Node> n1 = pq.top();
		pq.pop();
		std::shared_ptr<Node> n2 = pq.top();
		pq.pop();

		pq.push(std::make_shared<Node>('\0', n1->freq + n2->freq, n1, n2));
	}

	return pq.top();
}

void HuffmanEncoding::odrediKodove(const std::shared_ptr<Node>& koren, std::string kod, std::unordered_map<char, std::string>& kodovi)
{
	if (koren == nullptr)
		return;

	if (!(koren->left) && !(koren->right))
	{
		kodovi[koren->ch] = kod;
		return;
	}

	odrediKodove(koren->left, kod + '0', kodovi);
	odrediKodove(koren->right, kod + '1', kodovi);
}

std::vector<uint8_t> HuffmanEncoding::kodirajTekst(const std::vector<char>& text, std::unordered_map<char, std::string>& kodovi, uint8_t& padding)
{
	std::string bitString;

	for (const char& c : text)
	{
		bitString += kodovi[c];
	}

	padding = (8 - bitString.size() % 8) % 8;
	bitString.append(padding, '0');

	std::vector<uint8_t> kodiraniPodaci;
	kodiraniPodaci.reserve(bitString.size() / 8);

	for (size_t i = 0; i < bitString.size(); i += 8)
	{
		uint8_t bajt = 0;
		for (int j = 0; j < 8; j++)
		{
			if (bitString[i + j] == '1')
				bajt |= (1 << (7 - j));
		}

		kodiraniPodaci.push_back(bajt);
	}

	return kodiraniPodaci;
}

void HuffmanEncoding::napraviStabloString(const std::shared_ptr<Node>& koren, std::string& stabloString)
{
	if (koren == nullptr)
		return;

	if (!(koren->left) && !(koren->right))
	{
		stabloString += '1';
		stabloString += koren->ch;
	}
	else
	{
		stabloString += '0';
		napraviStabloString(koren->left, stabloString);
		napraviStabloString(koren->right, stabloString);
	}
}

std::vector<uint8_t> HuffmanEncoding::kompresujTekst(const std::vector<char>& tekst)
{
	auto frekvencije = odrediFrekvencije(tekst);
	auto korenStabla = napraviHuffmanStablo(frekvencije);
	std::string stabloString;
	napraviStabloString(korenStabla, stabloString);

	std::unordered_map<char, std::string> kodovi;
	odrediKodove(korenStabla, "", kodovi);
	uint8_t padding;
	auto kodiraniPodaci = kodirajTekst(tekst, kodovi, padding);

	std::vector<uint8_t> rezultat;
	rezultat.reserve(stabloString.size() + 1 + 1 + kodiraniPodaci.size()); //stablo + padding + kodiraniPodaci

	rezultat.insert(rezultat.end(), stabloString.begin(), stabloString.end());

	rezultat.push_back(padding);

	rezultat.insert(rezultat.end(), kodiraniPodaci.begin(), kodiraniPodaci.end());

	return rezultat;
}

std::shared_ptr<Node> HuffmanEncoding::ucitajHuffmanStablo(const std::vector<char>& stabloString, int& idx)
{
	if (stabloString[idx++] == '1')
	{
		return std::make_shared<Node>(stabloString[idx++], 0);
	}
	else
	{
		std::shared_ptr<Node> levoDete = ucitajHuffmanStablo(stabloString, idx);
		std::shared_ptr<Node> desnoDete = ucitajHuffmanStablo(stabloString, idx);
		return std::make_shared<Node>('\0', 0, levoDete, desnoDete);
	}
}

std::vector<uint8_t> HuffmanEncoding::dekompresujTekst(const std::vector<char>& ulazniBajtovi)
{
	int idx = 0;
	std::shared_ptr<Node> korenStabla = ucitajHuffmanStablo(ulazniBajtovi, idx);

	uint8_t padding = static_cast<uint8_t>(ulazniBajtovi[idx++]);

	std::vector<char> bitovi;

	for (size_t i = idx; i < ulazniBajtovi.size(); i++)
	{
		uint8_t bajt = static_cast<uint8_t>(ulazniBajtovi[i]);

		for (int j = 0; j < 8; j++)
		{
			if ((bajt >> (7 - j) & 1) == 0)
				bitovi.push_back('0');
			else
				bitovi.push_back('1');
		}
	}

	bitovi.resize(bitovi.size() - padding);

	std::vector<uint8_t> rezultat;
	std::shared_ptr<Node> cvor = korenStabla;

	for (char& ch : bitovi)
	{
		if (ch == '0')
			cvor = cvor->left;
		else
			cvor = cvor->right;

		if (!(cvor->left) && !(cvor->right))
		{
			rezultat.push_back(uint8_t(cvor->ch));
			cvor = korenStabla;
		}
	}

	return rezultat;
}
