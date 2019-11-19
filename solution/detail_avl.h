#ifndef DETAIL_AVL_H
#define DETAIL_AVL_H 1

#include "avl.h"
#include "string.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>

using TUll = unsigned long long;

class TDetailAvl : public TAvl<TString, TUll> {

	void Lower(TString &str) {
		int strSize = str.Size();
		for (int i = 0; i < strSize; i++) {
			str[i] = std::tolower(str[i]);
		}
	}

	void Save(std::ostream &os, const TAvlNode *node) {
		if (node == nullptr) {
			return;
		}
		int keySize = node->key.Size();
		os.write(reinterpret_cast<char *>(&keySize), sizeof(keySize));
		os.write(node->key.Cstr(), keySize);
		os.write((char *)&node->value, sizeof(node->value));

		bool hasLeft = node->left != nullptr;
		bool hasRight = node->right != nullptr;

		os.write(reinterpret_cast<char *>(&hasLeft), sizeof(hasLeft));
		os.write(reinterpret_cast<char *>(&hasRight), sizeof(hasRight));

		if (hasLeft) {
			Save(os, node->left);
		}
		if (hasRight) {
			Save(os, node->right);
		}
	}

	TAvlNode *Load(std::istream &is, const TAvlNode *node) {
		(void)(node);
		TAvlNode *root = nullptr;

		int keySize;
		is.read(reinterpret_cast<char *>(&keySize), sizeof(keySize));

		if (is.gcount() == 0) {
			return root;
		}

		char *key = new char[keySize + 1];
		key[keySize] = '\0';
		is.read(key, keySize);

		TUll value;
		is.read(reinterpret_cast<char *>(&value), sizeof(value));

		bool hasLeft = false;
		bool hasRight = false;
		is.read(reinterpret_cast<char *>(&hasLeft), sizeof(hasLeft));
		is.read(reinterpret_cast<char *>(&hasRight), sizeof(hasRight));

		root = new TAvlNode();
		root->key.CstrMove(key);
		root->value = value;

		if (hasLeft) {
			root->left = Load(is, root);
		}
		else {
			root->left = nullptr;
		}

		if (hasRight) {
			root->right = Load(is, root);
		}
		else {
			root->right = nullptr;
		}

		return root;
	}

	bool OpenFileSave(TString &fileName) {
		std::ofstream os{fileName.Cstr(), std::ios::binary | std::ios::out};
		if (os) {
			Save(os, root);
		} 
		else {
			return false;
		}
		os.close();
		return true;
	}

	bool OpenFileLoad(TString &fileName) {
		std::ifstream is{fileName.Cstr(), std::ios::binary | std::ios::in};
		if (is) {
			TreeDelete(root);
			root = Load(is, nullptr);
		}
		else {
			return false;
		}
		is.close();
		return true;
	}

public:
	void DetailInsert() {
		TString key;
		TUll value = 0;

		std::cin >> key >> value;
		Lower(key);

		AddPrint(std::move(key), value);
	}

	void DetailRemove() {
		TString key;

		std::cin >> key;
		Lower(key);

		DeletePrint(std::move(key));
	}

	void DetailFind(const TString &k) {
		TString key{k};

		Lower(key);

		TAvlNode *resFind = Find(std::move(key));
		if (resFind != nullptr) {
			std::cout << "OK: " << resFind->value << "\n"; 
		}
		else {
			std::cout << "NoSuchWord\n";
		}

	}

	void SaveLoad() {
		TString cmd;
		TString fileName;

		std::cin >> cmd >> fileName;
		if (cmd[0] == 'S') {
			if (!OpenFileSave(fileName)) {
				std::cout << "ERROR: can't open file\n";
				return;
			}
		}
		else {
			if (!OpenFileLoad(fileName)) {
				std::cout << "ERROR: can't open file\n";
				return;
			}
		}
		std::cout << "OK\n";
	}


};


#endif //DETAIL_AVL_H