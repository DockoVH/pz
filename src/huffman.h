#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <memory>

struct Node
{
	char ch;
	int freq;
	std::shared_ptr<Node> left;
	std::shared_ptr<Node> right;

	Node(char ch, int freq, std::shared_ptr<Node> left = nullptr, std::shared_ptr<Node> right = nullptr)
	 	: ch(ch), freq(freq), left(left), right(right) {}
};

struct compare
{
	bool operator()(std::shared_ptr<Node> l, std::shared_ptr<Node> r)
	{
		return l->freq > r->freq;
	}
};

class HuffmanEncoding
{
public:
	std::unordered_map<char, int> odrediFrekvencije(std::vector<char> text);
	std::shared_ptr<Node> napraviHuffmanStablo(std::unordered_map<char, int>& frekvencije);
	void odrediKodove(const std::shared_ptr<Node>& koren, std::string kod, std::unordered_map<char, std::string>& kodovi);
	std::vector<uint8_t> kodirajTekst(const std::vector<char>& text, std::unordered_map<char, std::string>& kodovi, uint8_t& padding);
	void napraviStabloString(const std::shared_ptr<Node>& koren, std::string& stabloString);
	std::vector<uint8_t> kompresujTekst(const std::vector<char>& tekst);
	std::shared_ptr<Node> ucitajHuffmanStablo(const std::vector<char>& stabloString, int& idx);
	std::vector<uint8_t> dekompresujTekst(const std::vector<char>& ulazniBajtovi);
};

#endif
