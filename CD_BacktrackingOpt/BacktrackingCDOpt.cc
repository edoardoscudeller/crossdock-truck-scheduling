#include "BacktrackingCDOpt.hh"

// Al livello corrente, proviamo il primo camion inbound non ancora assegnato
void BacktrackingCDOpt::FirstOfLevel()
{
  for (unsigned t = 0; t < in.InboundTrucks(); t++)
  {
    if (!out.IsAssigned(t))
    {
      out.AssignInbound(level, t);
      return;
    }
  }
  out.AssignInbound(level, -1); // nessun camion disponibile (non dovrebbe accadere)
}

// Prova il successivo camion inbound non ancora assegnato
bool BacktrackingCDOpt::NextOfLevel()
{
  int current = out.InboundAt(level);
  // Rimuovi l'assegnazione corrente per poter cercare il successivo
  out.AssignInbound(level, -1);
  for (unsigned t = (unsigned)(current + 1); t < in.InboundTrucks(); t++)
  {
    if (!out.IsAssigned(t))
    {
      out.AssignInbound(level, t);
      return true;
    }
  }
  return false;
}

// Feasibility: al livello corrente, il camion appena inserito deve avere
// release time non negativo (sempre vero per costruzione) e non essere
// gia' in sequenza (garantito da IsAssigned). Sempre fattibile.
bool BacktrackingCDOpt::Feasible()
{
  if (level < 0) return true;
  return out.InboundAt(level) >= 0;
}

// Costo = makespan della soluzione completa (solo al livello foglia)
unsigned BacktrackingCDOpt::Cost()
{
  return out.ComputeMakespan();
}

// Bound: se il makespan parziale della sequenza fino al livello corrente
// supera gia' il miglior costo noto, potiamo il ramo
bool BacktrackingCDOpt::NonImprovingBranch()
{
  return out.ComputeMakespan() >= best_cost;
}

void BacktrackingCDOpt::GoUpOneLevel()
{
  out.AssignInbound(level, -1);
  level--;
}
