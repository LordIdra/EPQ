#include "treesIdentical.hpp"
#include "grammar/symbolNames.hpp"



auto TreesIdentical(const parser::TreeNode *node1, const parser::TreeNode *node2) -> bool {
    //std::cout << symbolNames.at(node1->token.type) << " " << symbolNames.at(node2->token.type) << "\n";
    if (node1->children.empty() && node2->children.empty()) {
        if (node1->token.type == NONE) {
            return true;
        }
        const bool contentsIdentical = (node1->token.type == node2->token.type)
            && (node1->token.text == node2->token.text);
        if (!contentsIdentical) {
            std::cout << colors::RED << "Mismatched tree node: " << 
                         colors::CYAN << node1->token.text << " " << node2->token.text << 
                         colors::WHITE << "\n";
        }
        return contentsIdentical;
    } else if (node1->children.size() == node2->children.size()) {
        bool childrenIdentical = true;
        for (int i = 0; i < node1->children.size(); i++) {
            if (!TreesIdentical(&(node1->children[i]), &(node2->children[i]))) {
                childrenIdentical = false;
            }
        }
        return childrenIdentical;
    }

    std::cout << colors::RED << "Number of children does not match " << 
                 colors::CYAN << symbolNames.at(node1->token.type) << "(" << node1->children.size() << " - ";
    
    for (const auto &child : node1->children) {
        std::cout << symbolNames.at(child.token.type) << " ";
    }
    
    std::cout << ") " << symbolNames.at(node2->token.type) << "(" << node2->children.size() << " - ";
    
    for (const auto &child : node2->children) {
        std::cout << symbolNames.at(child.token.type) << " ";
    }

    std::cout << ")" << colors::WHITE << "\n";;

    return false;
}