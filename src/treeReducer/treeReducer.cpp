#include "parser/parser.hpp"
#include "parser/sets/setUtil.hpp"
#include <treeReducer/treeReducer.hpp>



namespace treeReducer {
    namespace {
        auto Traverse(parser::TreeNode &node, bool &changeMade) -> void {
            // Traverse children
            for (int i = 0; i < node.children.size(); i++) {

                parser::TreeNode child = node.children.at(i);

                // If node has one non-terminal child remove it
                if (child.children.size() == 1) {
                    if (!setUtil::IsSymbolTerminal(child.token.type)) {
                        node.children.push_back(child.children.at(0));
                        node.children.erase(node.children.begin());
                        changeMade = true;
                        return;
                    }
                }

                // If node hs no children and is non-terminal remove it
                if (child.children.empty()) {
                    if (!setUtil::IsSymbolTerminal(child.token.type)) {
                        node.children.erase(node.children.begin() + i);
                        changeMade = true;
                        i--;
                        return;
                    }
                }

                // If child is epsilon, remove it
                if (child.token.type == NONE) {
                    node.children.erase(node.children.begin() + i);
                    changeMade = true;
                    i--;
                    return;
                }
                Traverse(node.children.at(i), changeMade);
            }
        }
    }

    auto Reduce(parser::TreeNode node) -> parser::TreeNode {
        bool changeMade = true;
        while (changeMade){
            changeMade = false;
            Traverse(node, changeMade);
        }
        return node;
    }
}