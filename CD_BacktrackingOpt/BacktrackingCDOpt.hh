#ifndef CD_BACKTRACKING_HH
#define CD_BACKTRACKING_HH
#include "BacktrackingOpt.hh"
#include "CD_Data.hh"

class BacktrackingCDOpt : public BacktrackingOpt<CD_Input, CD_Output, unsigned>
{
public:
  BacktrackingCDOpt(const CD_Input& in, unsigned fl)
    : BacktrackingOpt(in, fl) {}
protected:
  // hot-spots
  void    FirstOfLevel() override;
  bool    NextOfLevel()  override;
  bool    Feasible()     override;
  unsigned Cost()        override;
  // cold-spots
  bool    NonImprovingBranch() override;
  void    GoUpOneLevel() override;
};
#endif
