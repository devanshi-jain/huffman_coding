#include "HCTree.hpp"

/**
 * Destructor for HCTree.
 * Deletes all the nodes in the tree by recursively calling deleteAll() starting from the root.
 */
HCTree::~HCTree() {
    deleteAll(root);
}

/**
 * Recursively deletes all nodes in the tree starting from the given node.
 * @param node The node to start deleting from.
 */
void HCTree::deleteAll(HCNode* node) {
    if (node == nullptr) {
        return;
    }
    deleteAll(node->c0);
    deleteAll(node->c1);
    delete node;
}

/**
 * Builds the Huffman tree based on the provided frequencies.
 * @param freqs The vector of frequencies for each symbol.
 */
void HCTree::build(const vector<int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for (int i = 0; i < (int)freqs.size(); i++) {
        if (freqs[i] > 0) {
            leaves[i] = new HCNode(freqs[i], (unsigned char)i);
            pq.push(leaves[i]);
        }
    }

    while (pq.size() > 1) {
        HCNode* first = pq.top();
        pq.pop();
        HCNode* second = pq.top();
        pq.pop();

        HCNode* parent = new HCNode(first->count + second->count, '\0');
        parent->c0 = first;
        parent->c1 = second;
        first->p = parent;
        second->p = parent;

        pq.push(parent);
    }

    if (!pq.empty()) {
        root = pq.top();
    }
}

/**
 * Encodes the given symbol and writes the encoded bits to the output stream.
 * @param symbol The symbol to encode.
 * @param out The output stream to write the encoded bits to.
 */
void HCTree::encode(unsigned char symbol, FancyOutputStream& out) const {
    HCNode* node = leaves[symbol];

    string code;
    while (node->p != nullptr) {
        if (node->p->c0 == node) {
            code.push_back('0');
        } else {
            code.push_back('1');
        }
        node = node->p;
    }

    for (int i = (int)code.length() - 1; i >= 0; i--) {
        out.write_bit(code[i] - '0');
    }
}

/**
 * Decodes the next symbol from the input stream based on the Huffman tree and returns it.
 * @param in The input stream to read the encoded bits from.
 * @return The decoded symbol.
 */
unsigned char HCTree::decode(FancyInputStream& in) const {
    HCNode* node = root;

    while (node->c0 != nullptr || node->c1 != nullptr) {
        int bit = in.read_bit();
        if (bit == 0) {
            node = node->c0;
        } 
        else {
            node = node->c1;
        }
    }
    
    return node->symbol;
}

