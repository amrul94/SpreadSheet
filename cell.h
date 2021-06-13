#pragma once

#include "common.h"
#include "formula.h"
#include "sheet.h"

#include <cassert>
#include <deque>
#include <functional>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_set>

class Sheet;

class Cell : public CellInterface {
public:
    using Positions = std::vector<Position>;

    explicit Cell(Sheet &sheet);
    ~Cell() override = default;

    void Set(std::string text);
    void Clear();
    Value GetValue() const override;
    std::string GetText() const override;

    Positions GetReferencedCells() const override;
    bool IsReferenced() const;

private:
    class Impl;
    class EmptyImpl;
    class TextImpl;
    class FormulaImpl;

    void CheckForCircularDependencies(const Positions& referenced_cells) const;

    void ClearCache();
    void ClearCaches();
    void ClearInfluence();

    Cell* ConvertPosToCell(Position pos);

    const CellInterface* CreateEmptyCell(const Position& pos) const;
    std::unique_ptr<Impl> CreateFormulaImpl(std::string text) const;
    std::vector<std::vector<uint8_t>> CreateVisitedCells() const;

    void RebuildGraph();
    void SetImpl(std::string text, std::unique_ptr<Impl>&& formula_impl);

    static void AddToStack(std::stack<Position>& destination, const Positions& source);
    static void AddToStack(std::stack<CellInterface*>& destination, const std::unordered_set<CellInterface*>& source);

    static std::stack<Position> CreateStack(const Positions& referenced_cells);
    static std::stack<CellInterface*> CreateStack(const std::unordered_set<CellInterface*>& influence);

    std::unique_ptr<Impl> impl_;
    const Sheet& sheet_;
    std::unordered_set<CellInterface*> influence_;
    mutable std::optional<Value> cache_;
};

class Cell::Impl {
public:
    virtual ~Impl() = default;
    [[nodiscard]] virtual Value GetValue(const Sheet& sheet) const = 0;
    [[nodiscard]] virtual std::string GetText() const = 0;
    [[nodiscard]] virtual Positions GetReferencedCells() const;

private:
    Positions empty_vector_;
};