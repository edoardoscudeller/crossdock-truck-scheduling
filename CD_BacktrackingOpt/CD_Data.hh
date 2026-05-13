// File CD_Data.hh
#ifndef CD_DATA_HH
#define CD_DATA_HH
#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

// ---------------------------------------------------------------------------
// CD_Input: parametri dell'istanza di Cross-Docking Truck Scheduling
// ---------------------------------------------------------------------------
class CD_Input
{
  friend ostream& operator<<(ostream& os, const CD_Input& in);
public:
  CD_Input(string file_name);

  unsigned InboundTrucks()  const { return inbound_trucks; }
  unsigned OutboundTrucks() const { return outbound_trucks; }
  unsigned InboundDoors()   const { return inbound_doors; }
  unsigned OutboundDoors()  const { return outbound_doors; }

  unsigned ReleaseTime(unsigned i)    const { return release_time[i]; }
  unsigned UnloadingTime(unsigned i)  const { return unloading_time[i]; }
  unsigned LoadingTime(unsigned j)    const { return loading_time[j]; }
  unsigned TransferTime(unsigned i, unsigned j) const { return transfer_matrix[i][j]; }

private:
  unsigned inbound_trucks, outbound_trucks;
  unsigned inbound_doors, outbound_doors;
  vector<unsigned> release_time;
  vector<unsigned> unloading_time;
  vector<unsigned> loading_time;
  vector<vector<unsigned>> transfer_matrix;
};

// ---------------------------------------------------------------------------
// CD_Output: soluzione — sequenza dei camion inbound alle porte
// ---------------------------------------------------------------------------
class CD_Output
{
  friend ostream& operator<<(ostream& os, const CD_Output& out);
public:
  CD_Output(const CD_Input& i);
  CD_Output& operator=(const CD_Output& out);

  // Assegna il camion inbound t_idx alla posizione pos nella sequenza
  void AssignInbound(unsigned pos, int truck_idx);
  int  InboundAt(unsigned pos)    const { return inbound_sequence[pos]; }
  bool IsAssigned(unsigned truck) const { return assigned[truck]; }

  unsigned ComputeMakespan() const;
  unsigned ComputeViolations() const;

private:
  const CD_Input& in;
  vector<int>  inbound_sequence;  // sequenza posizione -> indice camion inbound
  vector<bool> assigned;          // truck inbound gia' in sequenza?
};

#endif
