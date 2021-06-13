#pragma once

#include "cell.h"
#include "common.h"

#include <iostream>
#include <functional>

class Sheet : public SheetInterface {
public:
    using CellInterfacePtr = std::unique_ptr<CellInterface>;
    using Table = std::vector<std::vector<CellInterfacePtr>>;

    void SetCell(Position pos, std::string text) override;

    [[nodiscard]] const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    [[nodiscard]] Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

private:
    [[nodiscard]] bool IsPrintableSize(Position pos) const;
    void IncreaseSize(Position pos);
    void ReduceSize(Position pos);

    template<typename PrintContext>
    void PrintImpl(PrintContext print_context, std::ostream& output) const;

	Table cells_;
	Size size_;
};

template<typename PrintContext>
void Sheet::PrintImpl(PrintContext print_context, std::ostream& output) const {
    for (const auto& rows : cells_) {
        size_t col = 0;
        for (size_t min_bound = std::min(rows.size(), static_cast<size_t>(size_.cols)); col < min_bound; ++col) {
            if (rows[col]) {
                print_context(rows[col], output);
            }
            if (col < static_cast<size_t>(size_.cols) - 1) {
                output << '\t';
            }

        }
        for (;col < static_cast<size_t>(size_.cols) - 1; ++col) {
            output << '\t';
        }
        output << '\n';
    }
}