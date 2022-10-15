namespace nmgr {

template <uint32_t Width>
inline uint32_t constexpr MemoryManager::CellDescription::
    getDescIdxBeginOfCertainLine() const {
  // Masked usefull bits in Desc responsible for LineIdx.
  constexpr uint32_t LineMask = (1u << I::NumOfLineWidths) - 1u;
  constexpr uint32_t ShiftMask = LineMask ^ -1;
  // Amount of bits to shift to get the Number, responsible for Description
  // begining for sertain Line<Width>.
  uint32_t const Shift = getPowOfTwo<Width>() - 1;
  uint32_t const DescIdx = (ShiftMask >> Shift) & LineMask;
  return DescIdx;
}

template <uint32_t Width>
inline MemoryManager::CellDescription::Cells::const_iterator
MemoryManager::CellDescription::getDescBeginOfCertainLine() const {
  return Desc.begin() + getDescIdxBeginOfCertainLine<Width>();
}
template <uint32_t Width>
inline MemoryManager::CellDescription::Cells::iterator
MemoryManager::CellDescription::getDescBeginOfCertainLine() {
  return Desc.begin() + getDescIdxBeginOfCertainLine<Width>();
}

template <uint32_t Width>
inline MemoryManager::CellDescription::Cells::const_iterator
MemoryManager::CellDescription::getDescEndOfCertainLine() const {
  uint32_t constexpr NextLine = Width << 1u;
  return Desc.begin() + getDescIdxBeginOfCertainLine<NextLine>();
}

template <uint32_t Width>
inline MemoryManager::CellDescription::Cells::iterator
MemoryManager::CellDescription::getDescEndOfCertainLine() {
  uint32_t constexpr NextLine = Width << 1u;
  return Desc.begin() + getDescIdxBeginOfCertainLine<NextLine>();
}

template <uint32_t Width>
inline MemoryManager::CellDescription::Cells::iterator
MemoryManager::CellDescription::getFirstFreeCell() {
  return std::find(getDescBeginOfCertainLine<Width>(),
                   getDescEndOfCertainLine<Width>(), Cell::FREE);
}

template <uint32_t Width>
inline std::optional<uint32_t>
MemoryManager::CellDescription::retrieveFirstFree() {
  auto const Begin = getDescBeginOfCertainLine<Width>();
  auto const End = getDescEndOfCertainLine<Width>();
  auto const FindIt = std::find(Begin, End, Cell::FREE);
  if (FindIt == End)
    return {};
  *FindIt = Cell::OCCUPIED;
  return getMemoryOffset(FindIt);
}

template <uint32_t Width> inline Line<Width> MemoryManager::createLine() {
  std::cerr << "Attempt to create Line<" << Width << ">." << std::endl;
  auto const IsOffset = CD.retrieveFirstFree<Width>();
  if (!IsOffset) {
    std::cerr << "No free cells for Line<" << Width << ">." << std::endl;
    return Line<Width>::createLine(nullptr);
  }
  uint32_t const Offset = IsOffset.value();
  // std::cerr << "Memory offset: " << Offset << std::endl;
  I::DummyLine *Start = static_cast<I::DummyLine *>(Buffer);
  Point *Ptr = reinterpret_cast<Point *>(Start + Offset);
  // CD.print(std::cerr) << std::endl;
  return Line<Width>::createLine(Ptr);
}

} // namespace nmgr