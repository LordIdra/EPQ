#pragma once

#include <iostream>
#include <parser/parser.hpp>
#include <util/errors.hpp>
#include <grammar/symbolNames.hpp>



auto TreesIdentical(const parser::TreeNode *node1, const parser::TreeNode *node2) -> bool;