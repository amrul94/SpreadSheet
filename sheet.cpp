#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <iostream>
#include <optional>

using namespace std::literals;

void Sheet::SetCell(Position pos, std::string text) {
    if (!pos.IsValid()) {
        throw InvalidPositionException{"InvalidPosition"};
    }
    IncreaseSize(pos);
    auto& cell_interface = cells_[pos.row][pos.col];
    if (cell_interface == nullptr) {
        cell_interface = std::make_unique<Cell>(*this);
    }
    Cell* cell = dynamic_cast<Cell*>(cell_interface.get());
    cell->Set(std::move(text));
}

const CellInterface* Sheet::GetCell(Position pos) const {
    return const_cast<Sheet*>(this)->GetCell(pos);
}

CellInterface* Sheet::GetCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException{"InvalidPosition"};
    }
    if (!IsPrintableSize(pos)) {
        return nullptr;
    }
    return cells_[pos.row][pos.col].get();
}

void Sheet::ClearCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException{"InvalidPosition"};
    }
    if (!IsPrintableSize(pos)) {
        return;
    }
    cells_[pos.row][pos.col].reset();
    ReduceSize(pos);
}

Size Sheet::GetPrintableSize() const {
    return size_;
}

void Sheet::PrintValues(std::ostream& output) const {
    auto lambda = [](const CellInterfacePtr& cell, std::ostream& output) {
        std::visit([&output](auto&& arg){output << arg;}, cell->GetValue());
    };

    PrintImpl(lambda, output);
}
void Sheet::PrintTexts(std::ostream& output) const {
    auto lambda = [](const CellInterfacePtr& cell, std::ostream& output) {
        output << cell->GetText();
    };

    PrintImpl(lambda, output);
}

bool Sheet::IsPrintableSize(Position pos) const {
    if (cells_.size() <= static_cast<size_t>(pos.row)) {
        return false;
    }
    if (cells_[pos.row].size() <= static_cast<size_t>(pos.col)) {
        return false;
    }
    return true;
}

void Sheet::IncreaseSize(Position pos) {
    if (size_.rows <= pos.row) {
        size_.rows = pos.row + 1;
        cells_.resize(size_.rows);
    }
    if (cells_[pos.row].size() <= static_cast<size_t>(pos.col)) {
        cells_[pos.row].resize(pos.col + 1);
    }
    if (size_.cols <= pos.col) {
        size_.cols = pos.col + 1;
    }
}

void Sheet::ReduceSize(Position pos) {
    if (cells_[pos.row].size() == static_cast<size_t>(pos.col) + 1) {
        cells_[pos.row].resize(pos.col);
        size_t new_size = cells_[pos.row].size();
        while (new_size != 0 && cells_[pos.row][new_size - 1] == nullptr) {
            --new_size;
        }
        if (new_size == 0 && cells_[pos.row][new_size] == nullptr) {
            cells_[pos.row].resize(new_size);
        }
    }

    if (cells_[pos.row].empty() && size_.rows == pos.row + 1) {
        --size_.rows;
        size_t new_size = size_.rows;
        while (new_size != 0 && cells_[new_size - 1].empty()) {
            --new_size;
        }
        size_.rows = static_cast<int>(new_size);
        cells_.resize(size_.rows);

    }

    if (size_.cols == pos.col + 1) {
        size_t max_size = 0;
        for (const auto& row : cells_) {
            if (max_size < row.size()) {
                max_size = row.size();
            }
        }
        size_.cols = static_cast<int>(max_size);
    }
}


std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}