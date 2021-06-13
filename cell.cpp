#include "cell.h"


#include <stack>
#include <string>
#include <optional>

// ---------- Cell::Impl ----------

class Cell::EmptyImpl : public Cell::Impl {
public:
    [[nodiscard]] Value GetValue(const Sheet& sheet) const override;
    [[nodiscard]] std::string GetText() const override;

private:
    std::string empty_text_;
};

class Cell::TextImpl : public Cell::Impl {
public:
    explicit TextImpl(std::string text);
    [[nodiscard]] Value GetValue(const Sheet& sheet) const override;
    [[nodiscard]] std::string GetText() const override;

private:
    std::string text_;
};

class Cell::FormulaImpl : public Cell::Impl {
public:
    explicit FormulaImpl(const std::string& text);
    [[nodiscard]] Value GetValue(const Sheet& sheet) const override;
    [[nodiscard]] std::string GetText() const override;
    [[nodiscard]] Positions GetReferencedCells() const override;

private:
    std::unique_ptr<FormulaInterface> formula_;
};

// ---------- Cell methods ----------

Cell::Cell(Sheet &sheet)
    : impl_(std::make_unique<EmptyImpl>())
    , sheet_(sheet) {
}

void Cell::Set(std::string text) {
    if (text == impl_->GetText()) {
        return;
    }

    std::unique_ptr<Impl> formula_impl = CreateFormulaImpl(text);

    ClearInfluence();
    SetImpl(std::move(text), std::move(formula_impl));
    ClearCaches();
    RebuildGraph();
}

void Cell::Clear() {
    impl_ = std::make_unique<EmptyImpl>();
}

Cell::Value Cell::GetValue() const {
    if (cache_ == std::nullopt) {
        cache_ = impl_->GetValue(sheet_);
    }
    return cache_.value();
}

std::string Cell::GetText() const {
    return impl_->GetText();
}

Cell::Positions Cell::GetReferencedCells() const {
    return impl_->GetReferencedCells();
}

bool Cell::IsReferenced() const {
    return !GetReferencedCells().empty();
}

void Cell::ClearCache() {
    cache_.reset();
}

void Cell::ClearCaches() {
    std::stack<CellInterface*> stack_positions = CreateStack(influence_);
    std::unordered_set<CellInterface*> visited_cells;

    while (!stack_positions.empty()) {
        CellInterface* now_cell = stack_positions.top();
        stack_positions.pop();
        auto is_visited = visited_cells.find(now_cell);
        if (is_visited != visited_cells.end()) {
            continue;
        }
        visited_cells.insert(now_cell);
        Cell* real_cell = dynamic_cast<Cell*>(now_cell);
        AddToStack(stack_positions, real_cell->influence_);
        real_cell->ClearCache();
    }
    ClearCache();
}

void Cell::ClearInfluence() {
    const Positions& references = impl_->GetReferencedCells();
    for (const Position& pos : references) {
        auto* cell = ConvertPosToCell(pos);
        if (cell != nullptr) {
            cell->influence_.erase(this);
        }
    }
}

Cell* Cell::ConvertPosToCell(Position pos) {
    const CellInterface* const_cell = sheet_.GetCell(pos);
    if (const_cell == nullptr) {
        return nullptr;
    }
    auto* non_const_cell = const_cast<CellInterface*>(const_cell);
    return dynamic_cast<Cell*>(non_const_cell);
}

const CellInterface* Cell::CreateEmptyCell(const Position& pos) const {
    auto& dirty_sheet = const_cast<Sheet&>(sheet_);
    dirty_sheet.SetCell(pos, "");
    return sheet_.GetCell(pos);
}

std::unique_ptr<Cell::Impl> Cell::CreateFormulaImpl(std::string text) const {
    std::unique_ptr<Impl> temp_impl;

    if (text.size() != 1 && text.front() == FORMULA_SIGN) {
        try {
            temp_impl = std::make_unique<FormulaImpl>(text.substr(1));
        } catch (std::exception& e) {
            throw FormulaException("Formula error");
        }
        auto referenced_cells = temp_impl->GetReferencedCells();
        CheckForCircularDependencies(referenced_cells);
    }

    return temp_impl;
}

std::vector<std::vector<uint8_t>> Cell::CreateVisitedCells() const {
    Size sheet_printable_size = sheet_.GetPrintableSize();
    std::vector<uint8_t> inner_vec(sheet_printable_size.cols, 0);
    std::vector<std::vector<uint8_t>> visited_cells(sheet_printable_size.rows, inner_vec);
    return visited_cells;
}

void Cell::CheckForCircularDependencies(const Positions& referenced_cells) const {
    std::stack<Position> stack_positions = CreateStack(referenced_cells);
    std::vector<std::vector<uint8_t>> visited_cells = CreateVisitedCells();

    while (!stack_positions.empty()) {
        Position current_pos = stack_positions.top();
        stack_positions.pop();
        uint8_t& check_pos = visited_cells[current_pos.row][current_pos.col];
        if (check_pos == 1) {
            continue;
        }
        check_pos = 1;
        const CellInterface* current_cell = sheet_.GetCell(current_pos);
        if (current_cell == this) {
            throw CircularDependencyException{"Circular dependency"};
        }
        if (current_cell == nullptr) {
            current_cell = CreateEmptyCell(current_pos);
        }
        AddToStack(stack_positions, current_cell->GetReferencedCells());
    }
}

void Cell::SetImpl(std::string text, std::unique_ptr<Impl>&& formula_impl) {
    if (text.empty()) {
        impl_ = std::make_unique<EmptyImpl>();
    } else if (text.size() != 1 && text.front() == FORMULA_SIGN) {
        impl_ = std::move(formula_impl);
    } else {
        impl_ = std::make_unique<TextImpl>(std::move(text));
    }
}

void Cell::RebuildGraph() {
    for (Position pos : impl_->GetReferencedCells()) {
        Cell* cell = ConvertPosToCell(pos);
        if (cell == nullptr) {
            auto& dirty_sheet = const_cast<Sheet&>(sheet_);
            dirty_sheet.SetCell(pos, {});	/// старайтесть объявлять строковое константы ""s, в данном случае (пустой строки) можно просто {}
            cell = ConvertPosToCell(pos);
        }
        cell->influence_.insert(this);
    }
}

void Cell::AddToStack(std::stack<Position>& destination, const Positions& source) {
    for (Position pos : source) {
        destination.push(pos);
    }
}

void Cell::AddToStack(std::stack<CellInterface*>& destination, const std::unordered_set<CellInterface*>& source) {
    for (CellInterface* cell : source) {
        destination.push(cell);
    }
}

std::stack<Position> Cell::CreateStack(const Positions& referenced_cells) {
    std::stack<Position> stack_positions;
    AddToStack(stack_positions, referenced_cells);
    return stack_positions;
}

std::stack<CellInterface*> Cell::CreateStack(const std::unordered_set<CellInterface*>& influence) {
    std::stack<CellInterface*> stack_positions;
    AddToStack(stack_positions, influence);
    return stack_positions;
}


// ---------- Cell::impl methods ----------

Cell::Positions Cell::Impl::GetReferencedCells() const {
    return empty_vector_;
}

CellInterface::Value Cell::EmptyImpl::GetValue(const Sheet& sheet) const {
    return empty_text_;
}

std::string Cell::EmptyImpl::GetText() const {
    return empty_text_;
}

Cell::TextImpl::TextImpl(std::string text)
    : text_(std::move(text)) {
}

CellInterface::Value Cell::TextImpl::GetValue(const Sheet& sheet) const {
    if (!text_.empty() && text_.front() == ESCAPE_SIGN) {
        return text_.substr(1);
    } else {
        return text_;
    }
}

std::string Cell::TextImpl::GetText() const {
    return text_;
}

Cell::FormulaImpl::FormulaImpl(const std::string& text)
    : formula_(ParseFormula(text)) {
}

CellInterface::Value Cell::FormulaImpl::GetValue(const Sheet& sheet) const {
    auto formula_evaluate = formula_->Evaluate(sheet);
    if (std::holds_alternative<double>(formula_evaluate)) {
        return std::get<double>(formula_evaluate);
    } else {
        return std::get<FormulaError>(formula_evaluate);
    }
}

std::string Cell::FormulaImpl::GetText() const {
    return '=' + formula_->GetExpression();
}

Cell::Positions Cell::FormulaImpl::GetReferencedCells() const {
    return formula_->GetReferencedCells();
}