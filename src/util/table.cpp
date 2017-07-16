#include "saiga/util/table.h"

namespace Saiga {

Table::Table(std::vector<int> colWidth)
{
    setColWidth(colWidth);
}

void Table::setColWidth(std::vector<int> colWidth)
{
    this->colWidth = colWidth;
    numCols = colWidth.size();
}

}
